
#include <mpi.h>
#include "../src/dataGenerator.h"
#include <array>
#include <iostream>
#include <unsupported/Eigen/CXX11/Tensor>
#include "../src/mpi_io.h"
#include "tools.h"
#include "hdf5.h"


int main(int argc, char ** argv)
{
    const std::array<int,3> globalShape { 100,100, 1};

    

   
    int rank=-1, nRanks=-1;

    MPI_Init( &argc , & argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    distributedCartesianArray data1(MPI_COMM_WORLD,globalShape);
    distributedCartesianArray data2(MPI_COMM_WORLD,globalShape);
    
    indexDataGenerator gen;

    gen.generate(data1);

    file_id = H5Fcreate("data.out", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);


    MPI_Finalize();

}