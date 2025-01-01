#include <iostream>
#include <vector>
#include <mpi.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <stdexcept>
#include "posix_io.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/resource.h>

#include <lustre/lustre_user.h>
#include <lustre/lustreapi.h>


auto getPosixMode( benchio::openMode mode)
{

    if ( mode == benchio::writeMode)
    {
        return O_WRONLY | O_CREAT;
    }
    else if (mode == benchio::readMode )
    {
        return O_RDONLY;
    }
    else if (mode == benchio::readDirectMode)
    {
        return O_RDONLY | O_DIRECT ;
    }
    else if (mode == benchio::writeDirectMode)
    {
        return O_WRONLY | O_CREAT | O_DIRECT ;
    }
    else 
    {
        throw std::invalid_argument("mode not supported by POSIX");
    }
}


void posix_io::setStride()
{
    strided=true;
    if (chunkSize == 0) chunkSize=sizeof(real_t);

}

void posix_io::setLockNoExpand(distributedCartesianArray & data)
{
    struct llapi_lu_ladvise advise;
    advise.lla_start = 0;
    advise.lla_end = data.getGlobalSize()*sizeof(real_t);
    advise.lla_advice = LU_LADVISE_LOCKNOEXPAND;
    advise.lla_value1=0;
    advise.lla_value2=0;
    advise.lla_value3=0;
    advise.lla_value4=0;
    
    auto rc = llapi_ladvise(f, 0, 1, &advise);

    if (rc != 0)
    {
        throw std::runtime_error("Error: Could set lock to not expand:  " + std::string(strerror(errno))  );
    }

}

void posix_io::setLockAhead(distributedCartesianArray & data)
{
    const int max_attempts_wait_lock=100;

    struct llapi_lu_ladvise * advises;

    size_t nAdvises=data.getLocalSize()*sizeof(real_t)/chunkSize;
    advises = new llapi_lu_ladvise[nAdvises];
    size_t offset = getInitialFileOffset(data);

    for( size_t i=0;i< nAdvises; i++)
    {
        advises[i].lla_start = offset;
        advises[i].lla_start=offset;
        advises[i].lla_end=offset + chunkSize;

        if (mode == benchio::readMode)
        {
            advises[i].lla_lockahead_mode=MODE_READ_USER;
        }
        else if (mode == benchio::writeMode)
        {
            advises[i].lla_lockahead_mode=MODE_WRITE_USER;
        }
        else
        {
            throw std::runtime_error("Mode " + std::to_string(mode) + "is not supported for lockahead" );

        }

        advises[i].lla_advice = LU_LADVISE_LOCKAHEAD;
        advises[i].lla_value4= 0;
        advises[i].lla_lockahead_result = 545785; // set to an arbitrary non zero number, to check the result of ladvise;
        advises[i].lla_peradvice_flags= LF_ASYNC;

        offset+= chunkSize + getStride(data);
        

    }


    size_t nLocksGranted=0;
    size_t nLocksGrantedSame=0;
    size_t nLocksGrantedDiff=0;

     
    auto rc = llapi_ladvise(f, 0, nAdvises, advises);

    if (rc != 0)
    {
        throw std::runtime_error("Error: Could not lock ahead:  " + std::string(strerror(errno))  );

    }
    if (waitLockAhead)
    {
        MPI_Barrier(MPI_COMM_WORLD);

        for (int i=0; i< max_attempts_wait_lock ;i++)
        {
        usleep(100000);

            auto rc = llapi_ladvise(f, 0, nAdvises, advises);

            if (rc != 0)
            {
                throw std::runtime_error("Error: Could not lock ahead:  " + std::string(strerror(errno))  );

            }



            nLocksGrantedSame=0;
            nLocksGrantedDiff=0;
            for(int ii=0;ii<nAdvises;ii++)
            {
                if (advises[ii].lla_lockahead_result == LLA_RESULT_DIFFERENT)
                {
                    nLocksGrantedDiff+=1;
                } 
                else if (advises[ii].lla_lockahead_result == LLA_RESULT_SAME)
                {
                    nLocksGrantedSame+=1;
                }
                else if (advises[ii].lla_lockahead_result != 0)
                {
                    throw std::runtime_error("Error: Unexpected lockahead error " + std::to_string(advises[ii].lla_lockahead_result) + " : " + std::string(strerror(errno)) );
                }

            }
            nLocksGranted = nLocksGrantedSame + nLocksGrantedDiff;
            if (nLocksGranted >= nAdvises) break;

        }
        std::cout << "Locks granted:" << nLocksGranted << "/" << nAdvises << ", different= " << nLocksGrantedDiff << ", same=" << nLocksGrantedSame << std::endl;

    }    
    
    //std::cout << "Lock ahead" << std::endl;

    // for(int i=0;i<nAdvises;i++)
    // {
    //     std::cout << i << " " << " " << advises[i].lla_start << " " << advises[i].lla_end << " " << advises[i].lla_lockahead_result << std::endl;

    // }

}

size_t posix_io::getStride(distributedCartesianArray & data) const
{
    if (strided)
    {
        int nRanks;

        MPI_Comm_size( data.getCartesianCommunicator() , &nRanks );

        return chunkSize * (nRanks-1);

    }
    else 
    {
        return 0;
    }

}

size_t posix_io::getInitialFileOffset(distributedCartesianArray & data) const
{
    if ( not strided) {
        return data.getLocalOffset()[0]*sizeof(real_t);
    }
    else
    {
        int rank;

        MPI_Comm_rank( data.getCartesianCommunicator() , &rank );

        return chunkSize*rank;
    };

}

void posix_io::open(std::string filename, distributedCartesianArray & data, benchio::openMode mode_ )
{
    mode=mode_;
    auto posixMode = getPosixMode(mode);

    f = ::open( filename.c_str(), posixMode,0666 );

    if ( f< 0 )
    {
        throw std::runtime_error("Error: Could not open POSIX file.");
    }

    off_t ret=lseek( f, getInitialFileOffset(data), SEEK_SET );
    
    if (ret<0 )
    {
        throw std::runtime_error("Error: Could not seek to the file offset");
    }

};

void posix_io::setChunkSize( size_t chunkSize_)
{
    chunkSize = chunkSize_;
} 


void posix_io::write( distributedCartesianArray & data) 
{  

    size_t bytes_to_write=data.getLocalSize()*sizeof(real_t);
    long long int written_bytes=0;
    auto  offset= (char * )data.getData().data();
    size_t current_bytes_write = 0;
    size_t counter=0;
    size_t currentChunkSize=0;
    

    if (lockNoExpand)
    {
        setLockNoExpand(data);
    }

    if (lockAhead)
    {
        setLockAhead(data);
    }



    if (chunkSize > 0) 
    {
        currentChunkSize=chunkSize;
    }
    else 
    {
        currentChunkSize=bytes_to_write;
    }

    while (bytes_to_write >= currentChunkSize)
    {
        size_t bytes_chunk_to_write=std::min(bytes_to_write,currentChunkSize);
        
        counter=0;
        while (bytes_chunk_to_write != 0 )
        {

            written_bytes= ::write(f, offset , bytes_chunk_to_write );

            offset+= written_bytes;
            bytes_chunk_to_write-=written_bytes;
            
            if (written_bytes < 0 ) throw std::runtime_error("Error: POSIX write returned  error:  " + std::string(strerror(errno)) );

            if (counter > max_read_write_operations) throw std::runtime_error("Error: Maximum number of read attempts reached");
            counter++;
    
        }
        bytes_to_write-= currentChunkSize;
        
        if (strided )
        {
            off_t ret= lseek( f, getStride(data) , SEEK_CUR );
            if (ret<0 )
            {
                throw std::runtime_error("Error: Could not seek to the next stride to write.");
            }
        }
    }

    
}

void posix_io::close()
{
    ::close(f);

}

void posix_io::sync()
{

    auto ret = ::syncfs(f);
    if (ret != 0)
    {
        throw std::runtime_error("Error: Not all bytes were written succesfully");
    }; 

}

void posix_io::read( distributedCartesianArray & data) 
{  
    size_t bytes_to_read=data.getLocalSize()*sizeof(real_t);
    long long int written_bytes=0;
    auto  offset= (char * )data.getData().data();
    size_t current_bytes_read = 0;
    size_t counter=0;
    size_t currentChunkSize=0;

    if (chunkSize > 0) 
    {
        currentChunkSize=chunkSize;
    }
    else 
    {
        currentChunkSize=bytes_to_read;
    }

    while (bytes_to_read >= currentChunkSize)
    {
        size_t bytes_chunk_to_read=std::min(bytes_to_read,currentChunkSize);
        
        counter=0;
        while (bytes_chunk_to_read != 0 )
        {

            written_bytes= ::read(f, offset , bytes_chunk_to_read );

            // DEBUG // std::cout << written_bytes << " " << bytes_chunk_to_read << " " << bytes_to_read  << " " << chunkSize << std::endl;


            offset+= written_bytes;
            bytes_chunk_to_read-=written_bytes;
            
            if (written_bytes < 0 ) throw std::runtime_error("Error: POSIX read returned  error:  " + std::string(strerror(errno)) );

            if (counter > max_read_write_operations) throw std::runtime_error("Error: Maximum number of read attempts reached");
            counter++;
    
        }
        
        bytes_to_read-= currentChunkSize;

        if (strided )
        {
            off_t ret= lseek( f, getStride(data), SEEK_CUR );

            if (ret<0 )
            {
                throw std::runtime_error("Error: Could not seek to the next stride.");
            }
        }

    }


}