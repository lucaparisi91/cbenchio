

#include "posix_io.h"
#include <mpi.h>
#include "dataGenerator.h"
#include <iostream>
#include "benchmarks.h"
#include "yaml-cpp/yaml.h"
#include "mpi_io.h"
#include "std_io.h"
#include "hdf5_io.h"
#include <filesystem>
#include <fstream>

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


    auto shape = benchmark["shape"].as<std::array<size_t,3> >();
    auto processorGrid = benchmark["processorGrid"].as<std::array<int,3> >(std::array<int,3>{0,0,0});
    
    if (filePerProcess)
    {
        comm=MPI_COMM_SELF;
    }

    return std::make_shared<distributedCartesianArray>(comm,shape,processorGrid);

};

std::vector<std::string> createFileNames( const YAML::Node & benchmark)
{
    int rank=-1, nRanks=-1;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);


    auto filePerProcess = benchmark["filePerProcess"].as<bool>(false);
    std::vector<std::string> filenames;

    for ( auto pathItem : benchmark["paths"] )
    {
        auto filePath=std::filesystem::path(pathItem.as<std::string>("."));

        if ( filePerProcess)
        {
            filePath/= ("data" + std::to_string(rank) + ".out");
        }
        else 
        {
            filePath/= "data.out";
        }

        filenames.push_back(filePath);
    }

    return filenames;
    
}

std::shared_ptr<ctl_io> createWriter(YAML::Node benchmark)
{
    auto api = benchmark["API"].as<std::string>();

    
    if ( api == "posix" )
    {
        auto writer=  std::make_shared<posix_io>();
        auto aligment = benchmark["aligment"].as<size_t>( 0 );
        writer->setAligment(aligment);

        return writer;

    }
    else if ( api == "stdio" )
    {
        auto writer=  std::make_shared<std_io>();
        

        return writer;

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

    YAML::Node configOut;

    for ( const auto node : config["benchmarks"])
    {
        auto benchmarkNode=node;
        

        auto basenames = createFileNames(benchmarkNode);
        
        
        std::shared_ptr<distributedCartesianArray> data;
        data= createData(benchmarkNode);


  

        if (rank ==0 )
        {
            data->print();
        }

        auto name = getName(benchmarkNode);
        auto repeat = benchmarkNode["repeat"].as<int>(1);
        auto sync = benchmarkNode["sync"].as<bool>(false);  
        auto operation = benchmarkNode["operation"].as<std::string>("write");
        

        if (rank==0)
        {
            std::cout << "Name: "<< name <<std::endl;

        }        
        
        std::shared_ptr<distributedCartesianArray> valid_data;
        if (operation == "read")
        {
            valid_data = createData(benchmarkNode);
        }

        indexDataGenerator gen;
        if (operation == "read")
        {
            gen.generate(*valid_data);
        }
        else 
        {
            gen.generate(*data);
        }
        

        for (int i=0;i<repeat;i++)
        {
            for ( auto basename : basenames)
                {

                                    
                    auto writer = createWriter(benchmarkNode);

                    if (operation == "write")
                    {
                        writer->open(basename,*data,benchio::writeMode);
                    }
                    else 
                    {
                        writer->open(basename,*data,benchio::readMode);
                    }
                        
                    benchmark current_benchmark( name  );
                    current_benchmark.setFileSync(sync);
                    current_benchmark.setOperation(operation);

                    if (rank==0) {
                        std::cout << "---------------------" <<std::endl;
                    }

                    current_benchmark.write(*data, *writer);

                    writer->close();

                    auto response = current_benchmark.report_yaml() ;
                    
                    response["file_rank_0"]=basename;
                    

                    if( operation=="read" )
                    {
                        //if (rank==0) std::cout << "Checking read data..." << std::endl;
                        bool isAlmostEqual=data->checkAlmostEqual(*valid_data);

                        response["checked"]=true;
                        
                    }

                    if ( rank ==0)
                    {
                        std::cout << response << std::endl;
                        benchmarkNode["results"].push_back(response);
                        
                    }
                    
            }
            
        }

        configOut["benchmarks"].push_back(benchmarkNode);

    }

    if ( rank ==0 )
    {
        std::ofstream f;
        f.open("report.yaml");
        f << configOut;
        f.close();
        std::cout << "Done" << std::endl;
    }

    MPI_Finalize();

}