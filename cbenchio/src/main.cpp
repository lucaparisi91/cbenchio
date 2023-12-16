

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

    YAML::Node config = YAML::LoadFile("config.yaml");

    auto api = config["benchmark"]["API"].as<std::string>();

    auto comm=MPI_COMM_WORLD;
    auto shape = config["benchmark"]["shape"].as<std::array<int,3> >();
    std::string basename = "data.out";

        
    if (api == "posix")
    {
        auto filePerProcess = config["benchmark"]["filePerProcess"].as<bool>();

        if (filePerProcess)
        {
            comm=MPI_COMM_SELF;
            shape[0]/=nRanks;
            basename="data" + std::to_string(rank) + ".out";
        }   
        

    }
    


    distributedCartesianArray data(comm, shape);

    indexDataGenerator gen;
    if (rank==0)
    {
        std::cout << "Start generating data..."<<std::endl;
    }
    
    gen.generate(data); 

    
    auto writer = posix_io( basename );

    benchmark current_benchmark("posix");

    if (rank==0) std::cout << "Start benchmarking..." << std::endl;
    current_benchmark.write(data, writer);

    current_benchmark.report();

    


    MPI_Finalize();

}