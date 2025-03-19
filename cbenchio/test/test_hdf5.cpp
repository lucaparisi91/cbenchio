
#include <mpi.h>
#include "../src/dataGenerator.h"
#include <array>
#include <iostream>
#include <unsupported/Eigen/CXX11/Tensor>
#include "../src/mpi_io.h"
#include "tools.h"
#include "../src/hdf5_io.h"



int main(int argc, char ** argv)
{
    const std::array<size_t,3> globalShape { 10,5, 1};

    int rank=-1, nRanks=-1;

    MPI_Init( &argc , & argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    distributedCartesianArray data1(MPI_COMM_WORLD,globalShape);
    distributedCartesianArray data2(MPI_COMM_WORLD,globalShape);
    
    indexDataGenerator gen;

    gen.generate(data1);
    
    hdf5_io writer, reader;
    writer.open("data.hdf5",data1,benchio::writeMode);
    writer.write(data1);
    gen.generate(data1,1);
    writer.write(data1);
    
    writer.close();


    reader.open("data.hdf5",data2,benchio::readMode);
    
    gen.generate(data1,0);
    reader.read(data2);
    auto diff = distance(data2.getData(),data1.getData() );  
    testing::check_near( diff, 0 , "Distance btw. read and write 1 on rank " + std::to_string(rank) );

    gen.generate(data1,1);
    reader.read(data2);
    diff = distance(data2.getData(),data1.getData() );  
    testing::check_near( diff, 0 , "Distance btw. read and write 2 on rank " + std::to_string(rank) );
    

    reader.close();


    
   


   MPI_Finalize();

}
