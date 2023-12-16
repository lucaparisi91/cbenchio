

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
    gen.generate(data);


    // vectorDataGenerator randVecGen(MPI_COMM_WORLD,globalDataSize);

    // auto data = randVecGen.generate();

    auto writer = posix_io("data.out" );

    timer writeTime;
    timer readTime;

    writer.write(data);


    //writeTime.computeMaxElapsed();

    //std::cout << data.getGlobalSize()
    
    // if (rank == 0)
    // {
    //     auto globalDataSizeGB = randVecGen.getGlobalSize() / 1.e+9;
    //     std::cout << "Data Size: " << globalDataSizeGB << "GB" << std::endl;
    //     std::cout << "N. ranks: " << nRanks << std::endl;
    //     std::cout << "BW: " << globalDataSizeGB/writeTime.maxElapsed()  << " GB/s" << std::endl;

    // }

    // std::vector<real_t> data2;

    distributedCartesianArray data2(MPI_COMM_WORLD, { 100, 1, 1  });

    MPI_Barrier(MPI_COMM_WORLD);
    writer.read(data2);
    

    real_t diff_local = (Eigen::Tensor<real_t,0>((data2.getData() - data.getData() ).abs().sum()))();  
    real_t diff_global;
    MPI_Reduce( &diff_local, &diff_global , 1 , MPI_DOUBLE, MPI_SUM, 0 , MPI_COMM_WORLD );

    if (rank ==0)
    {
        testing::check_near( diff_global,0 , "write/read distance null") ;
    }

    // readTime.computeMaxElapsed();
    
    // if (rank == 0)
    // {
    //     auto globalDataSizeGB = data2.size()*nRanks*sizeof(real_t) / 1.e+9;
    //     std::cout << "Data Size: " << globalDataSizeGB << "GB" << std::endl;
    //     std::cout << "N. ranks: " << nRanks << std::endl;
    //     std::cout << "BW: " << globalDataSizeGB/readTime.maxElapsed()  << " GB/s" << std::endl;


    // }

    // if ( data.size() !=  data2.size() )
    // {
    //     exit(1);
    // }

    // real_t diff=0;
    // for(int i=0;i<data.size();i++)
    // {
    //     diff+=std::abs(data[i] - data2[i]);
    // }

    
    // if (diff> 0 ) exit(1);

    MPI_Finalize();

}