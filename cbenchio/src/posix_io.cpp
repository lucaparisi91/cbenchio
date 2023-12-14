#include <iostream>
#include <vector>
#include <mpi.h>
#include <fstream>
#include <sstream>
#include <filesystem>


#include "posix_io.h"

namespace posix
{
#include <fcntl.h>
#include <unistd.h>

}

posix_io::posix_io(std::string basename)
{
    //MPI_Comm_rank(MPI_COMM_WORLD, & rank);
    //MPI_Comm_size(MPI_COMM_WORLD, & nRanks);
    
    std::stringstream s;
    s << basename << ".out"  ;

    filename = s.str();

}



void posix_io::write( distributedCartesianArray & data) const
{
    auto f = posix::open( filename.c_str(), O_WRONLY | O_CREAT );


    posix::lseek( f, data.getLocalOffset()[0]*sizeof(real_t), SEEK_SET );
    posix::write(f, (const char *)data.getData().data(), data.getLocalSize()*sizeof(real_t));
    posix::close(f);
}

void posix_io::read( distributedCartesianArray & data)
{
    
    auto f = posix::open( filename.c_str(), O_RDONLY );

    posix::lseek( f, data.getLocalOffset()[0]*sizeof(real_t), SEEK_SET );
    posix::read(f, (char *)data.getData().data(), data.getLocalSize()*sizeof(real_t));

    posix::close(f);

   
}






