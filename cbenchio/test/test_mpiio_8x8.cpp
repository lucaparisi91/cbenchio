
#include <mpi.h>
#include "../src/dataGenerator.h"
#include <array>
#include <iostream>
#include <unsupported/Eigen/CXX11/Tensor>
#include "../src/mpi_io.h"
#include "tools.h"

int main(int argc, char ** argv)
{
    const std::array<size_t,3> globalShape { 160000,160000, 1};
   
    int rank=-1, nRanks=-1;

    MPI_Init( &argc , & argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);
    
    distributedCartesianArray data1(MPI_COMM_WORLD,globalShape, {8,8,1});
    
    distributedCartesianArray data2(MPI_COMM_WORLD,globalShape, {8,8,1});
    
    indexDataGenerator gen;

    gen.generate(data1);
    
    
   mpi_io writer;
   writer.open("data.out",data1,benchio::writeMode);
   writer.setCollective();

   writer.write( data1);
   writer.close();


   mpi_io reader;
   reader.open("data.out",data2,benchio::readMode);
   reader.read( data2);
   reader.close();


    real_t diff = ((Eigen::Tensor<real_t,0>)((data1.getData() - data2.getData()).abs().sum() ))();

    testing::check_near( diff, 0, "Data distance");


    MPI_Finalize();

}