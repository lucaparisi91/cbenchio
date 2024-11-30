#include "benchmarks.h"
#include <iostream>
#include <sstream>


void benchmark::write(data_t & data, ioCtl_t & ctl)
{
    int rank,nRanks;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);
    
    benchmarkTimer.start();

    if (operation == "write" )
    {
        ctl.write(data);
    }
    else if ( operation == "read")
    {
        ctl.read(data);
    }
    else
    {
        throw std::runtime_error( "Error at"  __FILE__ ": operation " + operation + " not recognised.");
    }
    if (sync)
    {
        ctl.sync();
    }
    
    benchmarkTimer.end();

    real_t localSizeTransferred=data.getLocalSize();
    sizeTransferred=0;
    MPI_Reduce( &localSizeTransferred, &sizeTransferred , 1 , MPI_DOUBLE, MPI_SUM, 0 , MPI_COMM_WORLD );
    
}

std::string benchmark::report()
{
    int rank,nRanks;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    benchmarkTimer.computeMaxElapsed();
    std::stringstream ss;

    if (rank == 0)
    {
        auto globalDataSizeGB = sizeTransferred*sizeof(real_t) / 1.e+9;
        ss  << "Data Size: " << globalDataSizeGB << "GB" << std::endl;
        ss << "N. ranks: " << nRanks << std::endl;
        ss << "Time: " << benchmarkTimer.maxElapsed()  << " s" << std::endl;
        ss << "BW: " << globalDataSizeGB/benchmarkTimer.maxElapsed()  << " GB/s" << std::endl;

    }

    return ss.str();
    
}

YAML::Node benchmark::report_yaml()
{
    YAML::Node report;

    int rank,nRanks;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    benchmarkTimer.computeMaxElapsed();
    std::stringstream ss;

    if (rank == 0)
    {
        auto globalDataSizeGB = sizeTransferred*sizeof(real_t) / 1.e+9;
        report["dataSize"] =globalDataSizeGB ;
        report["time"]=benchmarkTimer.maxElapsed();
        report["bandwidth"]=globalDataSizeGB/benchmarkTimer.maxElapsed();
        report["nRanks"]=nRanks;

    }
    return report;
}
