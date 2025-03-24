#include "benchmarks.h"
#include <iostream>
#include <sstream>


void benchmark::write(data_t & data, ioCtl_t & ctl)
{
    int rank,nRanks;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);
    
    benchmarkTimer.start();

    for(int i=0;i<nFields;i++)
    {        
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
    }
    
    benchmarkTimer.end();

    real_t localSizeTransferred=data.getLocalSize()*nFields;
    sizeTransferred=0;
    MPI_Reduce( &localSizeTransferred, &sizeTransferred , 1 , MPI_DOUBLE, MPI_SUM, 0 , MPI_COMM_WORLD );
    
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
        report["nFields"]=nFields;
    }
    return report;
}
