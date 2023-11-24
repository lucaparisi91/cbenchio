#include <iostream>
#include <vector>
#include <mpi.h>
#include <fstream>
#include <sstream>
#include <filesystem>


#include "posix_io.h"

posix_io::posix_io(std::string basename)
{
MPI_Comm_rank(MPI_COMM_WORLD, & rank);
MPI_Comm_size(MPI_COMM_WORLD, & nRanks);
    
    std::stringstream s;
    s << basename << "_rank" << rank << ".out"  ;

    filename = s.str();

}



void posix_io::write( const std::vector<real_t> & data, timer & timer) const
{
    std::fstream f( filename, std::ios::out|std::ios::binary);
    f.seekp(std::ios::beg);

    timer.start();
    f.write( (char*) (&data[0]) , data.size()*sizeof(real_t));
    f.flush();
    timer.end();

    f.close();
}

void posix_io::read( std::vector<real_t> & data, timer & timer)
{
    auto read_size = std::filesystem::file_size(filename);

    std::fstream f( filename, std::ios::in|std::ios::binary);
    f.seekp(std::ios::beg);

    auto nData=read_size/sizeof(real_t);

    data.resize(nData);


    timer.start();
    f.read( (char*) (&data[0]) , read_size);
    f.flush();
    timer.end();

    f.close();
}






