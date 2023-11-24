

#include "posix_io.h"
#include <mpi.h>
#include "dataGenerator.h"

#include <iostream>


int main(int argc, char ** argv)
{
    
    const size_t globalDataSize = 1e+9;

    int rank=-1, nRanks=-1;


    MPI_Init( &argc , & argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);




    dataGenerator randVecGen(globalDataSize);

    auto data = randVecGen.generate();


    auto writer = posix_io("data");

    timer writeTime;
    timer readTime;

    writer.write(data,writeTime);


    writeTime.computeMaxElapsed();

    
    if (rank == 0)
    {
        auto globalDataSizeGB = randVecGen.getGlobalSize() / 1.e+9;
        std::cout << "Data Size: " << globalDataSizeGB << "GB" << std::endl;
        std::cout << "N. ranks: " << nRanks << std::endl;
        std::cout << "BW: " << globalDataSizeGB/writeTime.maxElapsed()  << " GB/s" << std::endl;

    }

    std::vector<real_t> data2;

    writer.read(data2,readTime);

    readTime.computeMaxElapsed();
    
    if (rank == 0)
    {
        auto globalDataSizeGB = data2.size()*nRanks*sizeof(real_t) / 1.e+9;
        std::cout << "Data Size: " << globalDataSizeGB << "GB" << std::endl;
        std::cout << "N. ranks: " << nRanks << std::endl;
        std::cout << "BW: " << globalDataSizeGB/readTime.maxElapsed()  << " GB/s" << std::endl;


    }

    if ( data.size() !=  data2.size() )
    {
        exit(1);
    }

    real_t diff=0;
    for(int i=0;i<data.size();i++)
    {
        diff+=std::abs(data[i] - data2[i]);
    }

    
    if (diff> 0 ) exit(1);


    MPI_Finalize();

}