

#include "posix_io.h"
#include <mpi.h>
#include "dataGenerator.h"
#include <iostream>
#include "benchmarks.h"
#include "yaml-cpp/yaml.h"
#include "mpi_io.h"
#include "hdf5_io.h"
#include <filesystem>

auto getName(const YAML::Node & benchmark)
{
    std::string default_name = benchmark["API"].as<std::string>();

    return benchmark["name"].as<std::string>(default_name);

}

auto createData( const YAML::Node & benchmark)
{
    auto comm = MPI_COMM_WORLD;

    int rank=-1, nRanks=-1;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    auto filePerProcess = benchmark["filePerProcess"].as<bool>(false);
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


    auto filePerProcess = benchmark["filePerProcess"].as<bool>(false);

    auto filePath=std::filesystem::path(benchmark["path"].as<std::string>("."));

    if ( filePerProcess)
    {
        filePath/= ("data" + std::to_string(rank) + ".out");
    }
    else 
    {
        filePath/= "data.out";
    }

    return filePath;
}

std::shared_ptr<ctl_io> createWriter(YAML::Node benchmark)
{
    auto api = benchmark["API"].as<std::string>();

    if ( api == "posix" )
    {
        return std::make_shared<posix_io>();
        
    }
    else  if (api == "mpi")
    {
        auto writer = std::make_shared<mpi_io>();

        if (benchmark["isCollective"].as<bool>(true) == false )
        {
            writer->unSetCollective();
        }    
        return writer;
    }
    else  if (api == "hdf5")
    {
        auto writer=std::make_shared<hdf5_io>();
        if (benchmark["isCollective"].as<bool>(true) == false )
        {
            writer->unSetCollective();
        }
        return writer;
    }
    else
    {
        throw std::invalid_argument(std::string("api not known: ") + api);
    }
}

int main(int argc, char ** argv)
{
    int rank=-1, nRanks=-1;

    MPI_Init( &argc , & argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    YAML::Node config = YAML::LoadFile("config.yaml");


    for ( const auto node : config)
    {
        auto benchmarkNode=node["benchmark"];


        std::string basename = createFileName(benchmarkNode);
        auto data= createData(benchmarkNode);
        auto name = getName(benchmarkNode);
        
        indexDataGenerator gen;
        if (rank==0)
        {
             std::cout << "Name: "<< name <<std::endl;

           
        }
        
        gen.generate(data); 

        auto writer = createWriter(benchmarkNode);

        writer->open(basename,data,benchio::writeMode);

        
        benchmark current_benchmark( name  );

        if (rank==0) {
            std::cout << "---------------------" <<std::endl;
        }

        current_benchmark.write(data, *writer);

        writer->close();

        current_benchmark.report();

    }
    
   
    

    
    

    


    MPI_Finalize();

}