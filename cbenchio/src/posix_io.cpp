#include <iostream>
#include <vector>
#include <mpi.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <stdexcept>
#include "posix_io.h"

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

void posix_io::open(std::string filename, distributedCartesianArray & data, benchio::openMode mode )
{
    auto posixMode = getPosixMode(mode);

    f = posix::open( filename.c_str(), posixMode,0666 );

    if ( f< 0 )
    {
        throw std::runtime_error("Error: Could not open POSIX file.");
    }


    off_t ret=posix::lseek( f, data.getLocalOffset()[0]*sizeof(real_t), SEEK_SET );
    if (ret<0 )
    {
        throw std::runtime_error("Error: Could not seek to the file offset");
    }
};

void posix_io::setAligment( size_t alignment_)
{
    if (alignment_ > 0 )
    {
        aligned = true;
        alignment=alignment_;
    }
    else 
    {
        aligned = false;

    }
} 
void posix_io::write( distributedCartesianArray & data) 
{  
    
    auto bytes_to_write=data.getLocalSize()*sizeof(real_t);
    size_t written_bytes=0;
    auto  offset= (char * )data.getData().data();
    size_t current_bytes_write = 0;
    size_t counter=0;

    while (bytes_to_write != 0 )
    {
        if (aligned)
            {   
                current_bytes_write=std::min( bytes_to_write , alignment);
            }
            else 
            {
                current_bytes_write=bytes_to_write;
            }
            
        written_bytes= posix::write(f, offset , current_bytes_write );

        offset+= written_bytes;
        bytes_to_write-=written_bytes;
        
       if (written_bytes < 0 ) throw std::runtime_error("Error: Not all bytes were written succesfully");

       if (counter > max_read_write_operations) throw std::runtime_error("Error: Maximum number of read attempts reached");
       counter++;
   
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
    
    auto bytes_to_read=data.getLocalSize()*sizeof(real_t);
    size_t read_bytes=0;
    auto  offset= (char * )data.getData().data();
    size_t current_bytes_read = 0;
    size_t counter=0;

    while (bytes_to_read != 0  )
    {
        if (aligned)
            {   
                current_bytes_read=std::min( bytes_to_read , alignment);
            }
            else 
            {
                current_bytes_read=bytes_to_read;
            }
        
        read_bytes= posix::read(f, offset , current_bytes_read );
        //std::cout << "Bytes read: " << read_bytes << std::endl;
        //std::cout << "Bytes to read: " << current_bytes_read << std::endl;
        

        offset+= read_bytes;
        bytes_to_read-=read_bytes;
        if (read_bytes < 0 ) throw std::runtime_error("Error: Not all bytes were read succesfully");

        if (counter > max_read_write_operations) throw std::runtime_error("Error: Maximum number of read attempts reached");
        counter++;
    }

}