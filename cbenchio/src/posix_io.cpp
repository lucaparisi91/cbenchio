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
    else 
    {
        throw std::invalid_argument("mode not supported by POSIX");
    }
}

void posix_io::open(std::string filename, distributedCartesianArray & data, benchio::openMode mode )
{
    auto posixMode = getPosixMode(mode);

    f = posix::open( filename.c_str(), posixMode,0666 );


    posix::lseek( f, data.getLocalOffset()[0]*sizeof(real_t), SEEK_SET );

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

       if (written_bytes < 0 ) throw std::runtime_error("Not all bytes were written succesfully");
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
        throw std::runtime_error("Could not sync posix file");
    };
    
}


void posix_io::read( distributedCartesianArray & data) 
{
    posix::read(f, (char *)data.getData().data(), data.getLocalSize()*sizeof(real_t));
}

