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

void posix_io::write( distributedCartesianArray & data) const
{

   
    posix::write(f, (const char *)data.getData().data(), data.getLocalSize()*sizeof(real_t));
}

void posix_io::close()
{
    posix::close(f);

}

void posix_io::read( distributedCartesianArray & data) const
{
    
    //auto f = posix::open( filename.c_str(), O_RDONLY );

    //posix::lseek( f, data.getLocalOffset()[0]*sizeof(real_t), SEEK_SET );
    posix::read(f, (char *)data.getData().data(), data.getLocalSize()*sizeof(real_t));

    //posix::close(f);

   
}






