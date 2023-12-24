

#include "posix_io.h"
#include <mpi.h>
#include "dataGenerator.h"
#include <iostream>
#include "benchmarks.h"
#include "yaml-cpp/yaml.h"

auto createData( const YAML::Node & benchmark)
{
    auto comm = MPI_COMM_WORLD;

    int rank=-1, nRanks=-1;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    auto filePerProcess = benchmark["filePerProcess"].as<bool>();
    if ( filePerProcess)
    {
        comm=MPI_COMM_SELF;
    }


    auto shape = benchmark["shape"].as<std::array<int,3> >();

    if (filePerProcess)
    {
        comm=MPI_COMM_SELF;
    }


    distributedCartesianArray data(comm, shape);
    return data;

};

std::string createFileName( const YAML::Node & benchmark)
{
    int rank=-1, nRanks=-1;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);


    auto filePerProcess = benchmark["filePerProcess"].as<bool>();
    if ( filePerProcess)
    {
        return "data" + std::to_string(rank) + ".out";
    }
    else 
    {
        return "data.out";
    }
}


int main(int argc, char ** argv)
{
    int rank=-1, nRanks=-1;

    MPI_Init( &argc , & argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    YAML::Node config = YAML::LoadFile("config.yaml");


    auto api = config["benchmark"]["API"].as<std::string>();

   
    std::string basename = getFileName(config["benchmark"]);


    auto data= createData(config["benchmark"]);



    indexDataGenerator gen;
    if (rank==0)
    {
        std::cout << "Start generating data..."<<std::endl;
    }
    
    gen.generate(data); 


    posix_io writer;


    writer.open(basename,data,benchio::writeMode);
    benchmark current_benchmark("posix");


    if (rank==0) std::cout << "Start benchmarking..." << std::endl;
    
    current_benchmark.write(data, writer);

    writer.close();



    current_benchmark.report();

    


    MPI_Finalize();

}