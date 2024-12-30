#include <iostream>
#include <vector>
#include <mpi.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <stdexcept>
#include "posix_io.h"
#include <errno.h>

namespace posix
{
#include <fcntl.h>
#include <unistd.h>
#include <sys/resource.h>

}

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
    if (chunkSize == 0) chunkSize=1;

}

size_t posix_io::getStride(distributedCartesianArray & data) const
{
    if (strided)
    {
        int nRanks;

        MPI_Comm_size( data.getCartesianCommunicator() , &nRanks );
        
        return chunkSize * sizeof(real_t) * (nRanks-1);

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

        return chunkSize*sizeof(real_t)*rank;
    };

}

void posix_io::open(std::string filename, distributedCartesianArray & data, benchio::openMode mode )
{
    auto posixMode = getPosixMode(mode);

    f = posix::open( filename.c_str(), posixMode,0666 );

    if ( f< 0 )
    {
        throw std::runtime_error("Error: Could not open POSIX file.");
    }

    off_t ret=posix::lseek( f, getInitialFileOffset(data), SEEK_SET );
    
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

            written_bytes= posix::write(f, offset , bytes_chunk_to_write );

            offset+= written_bytes;
            bytes_chunk_to_write-=written_bytes;
            
            if (written_bytes < 0 ) throw std::runtime_error("Error: POSIX write returned  error:  " + std::string(strerror(errno)) );

            if (counter > max_read_write_operations) throw std::runtime_error("Error: Maximum number of read attempts reached");
            counter++;
    
        }
        bytes_to_write-= currentChunkSize;
        
        if (strided )
        {
            off_t ret= posix::lseek( f, getStride(data) , SEEK_CUR );

            if (ret<0 )
            {
                throw std::runtime_error("Error: Could not seek to the next stride to write.");
            }
        }
    }

    
}

void posix_io::close()
{
    posix::close(f);

}

void posix_io::sync()
{

    auto ret = posix::syncfs(f);
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

            written_bytes= posix::read(f, offset , bytes_chunk_to_read );

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
            off_t ret= posix::lseek( f, getStride(data), SEEK_CUR );

            if (ret<0 )
            {
                throw std::runtime_error("Error: Could not seek to the next stride.");
            }
        }

    }


}