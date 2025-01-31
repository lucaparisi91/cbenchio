
#include <mpi.h>
#include "../src/dataGenerator.h"
#include <array>
#include <iostream>
#include "../src/mpi_io.h"
#include "tools.h"
#include "../src/netcdf_io.h"

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
    
    netcdf_io writer, reader;
    writer.open("data.nc",data1,benchio::writeMode);
    writer.write(data1);
    writer.close();


    reader.open("data.nc",data2,benchio::readMode);
        
    reader.read(data2);

    reader.close();


    auto diff = distance(data2.getData(),data1.getData() );  
   
    std::cout << diff << std::endl;

    testing::check_near( diff, 0 , "Distance btw. read and write " );

    MPI_Finalize();

}
