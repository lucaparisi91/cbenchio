#include "benchmarks.h"
#include <iostream>

void benchmark::write(data_t & data, ioCtl_t & ctl)
{
    int rank,nRanks;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    benchmarkTimer.start();
    ctl.write(data);
    benchmarkTimer.end();

    real_t localSizeTransferred=data.getLocalSize();
    sizeTransferred=0;
    MPI_Reduce( &localSizeTransferred, &sizeTransferred , 1 , MPI_DOUBLE, MPI_SUM, 0 , MPI_COMM_WORLD );
    
}

void benchmark::report()
{
    int rank,nRanks;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    benchmarkTimer.computeMaxElapsed();

    if (rank == 0)
    {
        auto globalDataSizeGB = sizeTransferred*sizeof(real_t) / 1.e+9;
        std::cout << "Data Size: " << globalDataSizeGB << "GB" << std::endl;
        std::cout << "N. ranks: " << nRanks << std::endl;
        std::cout << "Time: " << benchmarkTimer.maxElapsed()  << " s" << std::endl;
        std::cout << "BW: " << globalDataSizeGB/benchmarkTimer.maxElapsed()  << " GB/s" << std::endl;

     }

    
}