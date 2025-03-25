

#include "../src/posix_io.h"
#include <mpi.h>
#include "../src/dataGenerator.h"
#include <iostream>

#include "tools.h"

int main(int argc, char ** argv)
{
    
    const size_t nGlobalElements = 1e+9;

    int rank=-1, nRanks=-1;

    MPI_Init( &argc , & argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    
    distributedCartesianArray data(MPI_COMM_WORLD, { 100, 1, 1  });

    indexDataGenerator gen;
    posix_io ioCtl;

    timer writeTime;
    timer readTime;
    ioCtl.open("data.out",data,benchio::writeMode);
    
    gen.generate(data);
    ioCtl.write(data);
    gen.generate(data,1);
    ioCtl.write(data);

    distributedCartesianArray data2(MPI_COMM_WORLD, { 100, 1, 1  });

    ioCtl.close();
    ioCtl.open("data.out",data,benchio::readMode);
    
    ioCtl.read(data2);
    gen.generate(data);
    testing::check_near(data,data2,"write/read data distance 1");
    ioCtl.read(data2);
    gen.generate(data,1);
    testing::check_near(data,data2,"write/read data distance 2");

    ioCtl.close();

    
    
    MPI_Finalize();

}