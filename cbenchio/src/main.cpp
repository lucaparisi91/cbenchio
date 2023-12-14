

#include "posix_io.h"
#include <mpi.h>
#include "dataGenerator.h"
#include <iostream>
#include "benchmarks.h"
#include "yaml-cpp/yaml.h"


int main(int argc, char ** argv)
{
    int rank=-1, nRanks=-1;

    MPI_Init( &argc , & argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    //YAML::Node config = YAML::LoadFile("config.yaml");

    
    distributedCartesianArray data(MPI_COMM_WORLD, { 1000000000, 1, 1  });

    indexDataGenerator gen;
    if (rank==0)
    {
        std::cout << "Start generating data..."<<std::endl;
    }
    
    gen.generate(data); 

  
    auto writer = posix_io("data" + std::to_string(rank) );

    benchmark current_benchmark("posix");

    if (rank==0) std::cout << "Start benchmarking..." << std::endl;
    current_benchmark.write(data, writer);

    current_benchmark.report();

    


    MPI_Finalize();

}