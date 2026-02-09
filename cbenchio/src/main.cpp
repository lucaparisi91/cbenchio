

#include "posix_io.h"
#include <mpi.h>
#include "dataGenerator.h"
#include <iostream>
#include "benchmarks.h"
#include "yaml-cpp/yaml.h"
#include "mpi_io.h"
#include "std_io.h"
#include "hdf5_io.h"
#include "netcdf_io.h"
#include "pool.h"
#include <filesystem>
#include <fstream>

auto getName(const YAML::Node & benchmark)
{
    std::string default_name = benchmark["API"].as<std::string>();

    return benchmark["name"].as<std::string>(default_name);

}

auto createData( const MPI_Comm & comm, const YAML::Node & benchmark)
{

    int rank=-1, nRanks=-1;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    

    auto shape = benchmark["shape"].as<std::array<size_t,3> >();
    auto processorGrid = benchmark["processorGrid"].as<std::array<int,3> >(std::array<int,3>{0,0,0});

    auto alignment = benchmark["alignment"].as<size_t>(0);

    return std::make_shared<distributedCartesianArray>(comm,shape,processorGrid,alignment);

};

bool containsKey(const YAML::Node & benchmark,const std::string & key)
{
    for (auto node : benchmark)
    {
        if (node.first.as<std::string>() == key ) return true;
    }
    
    return false;
}

auto createPool(const YAML::Node & benchmark)
{
    int rank=-1, nRanks=-1;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);
    int nProcessPerPool=-1;


    if (containsKey(benchmark,"poolSize") )
    {
        nProcessPerPool=benchmark["poolSize"].as<int>(nRanks);
    }
    else
    {
        bool isFilePerProcess=benchmark["filePerProcess"].as<bool>(false);
        if (isFilePerProcess)
        {
            nProcessPerPool=1;
        }
        else 
        {
            nProcessPerPool=nRanks;
        }
    }

    if (containsKey(benchmark,"filePerProcess") && containsKey(benchmark,"poolSize") )
    {
        throw std::runtime_error("FilePerProcess and poolSize cannot be both defined");
    }

    
    return pool(nProcessPerPool,MPI_COMM_WORLD,"pool");
}

std::vector<std::string> createFileNames( const pool & pool,std::vector<std::string> basePaths)
{
    int rank=-1, nRanks=-1;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    //pathItem.as<std::string>(".")
    //auto filePerProcess = benchmark["filePerProcess"].as<bool>(false);
    std::vector<std::string> filenames;

    for ( auto basePath : basePaths )
    {
        auto filePath=std::filesystem::path();
        filePath/= basePath;
        filePath/= ("data_" + pool.getName() + ".out");

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
        auto chunkSize = benchmark["chunkSize"].as<size_t>( 0 );
        writer->setChunkSize(chunkSize);
        auto strided = benchmark["strided"].as<bool>( false );
        if (strided) writer->setStride();
        auto lockAhead = benchmark["lockAhead"].as<bool>( false );
        auto waitLockAhead = benchmark["waitLockAhead"].as<bool>( false );
        auto lockNoExpand = benchmark["lockNoExpand"].as<bool>( false );

        if (lockAhead) writer->setLockAhead();
        if (waitLockAhead) writer->setWaitLockAhead();
        if (lockNoExpand) writer->setLockNoExpand();
        

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

        std::vector<size_t> defaultChunks;
        auto chunks = benchmark["chunks"].as<std::vector<size_t> >(defaultChunks);
        if (chunks.size() >0)
        {
            writer->setChunking( chunks );
        }
       

        return writer;
    }
    else  if (api == "netcdf")
    {
        auto writer=std::make_shared<netcdf_io>();
        if (benchmark["isCollective"].as<bool>(true) == false )
        {
            writer->unSetCollective();
        }
        

        std::vector<size_t> defaultChunks; // By default do not chunk the dataset.
        auto chunkDims = benchmark["chunks"].as<std::vector<size_t> >(defaultChunks);
        if (chunkDims.size() >0)
        {
            writer->setChunking( chunkDims );
        }
        
        auto time_stepping = benchmark["timeStepping"].as<bool>(false);
        writer->setTimeStepping(time_stepping);
        
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
    
    // Read the name of the configuration yaml file from the command line arguments
    std::string config_file = "config.yaml"; // default value
    if (argc > 1) {
        config_file = argv[1];
    }
    
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    const real_t tol = 1e-7;


    YAML::Node config = YAML::LoadFile(config_file);

    YAML::Node configOut;

    for ( const auto node : config["benchmarks"])
    {
        auto benchmarkNode=node;
        auto benchmarkPool=createPool( benchmarkNode );

        auto basenames = createFileNames(benchmarkPool,benchmarkNode["paths"].as<std::vector<std::string> >());
        
        std::shared_ptr<distributedCartesianArray> data;
        data= createData(benchmarkPool.getCommunicator(),benchmarkNode);


  

        if (rank ==0 )
        {
            data->print();
        }

        auto name = getName(benchmarkNode);
        auto repeat = benchmarkNode["repeat"].as<int>(1);
        auto sync = benchmarkNode["sync"].as<bool>(false);  
        auto operation = benchmarkNode["operation"].as<std::string>("write");
        auto nFields = benchmarkNode["fields"].as<size_t>(1);
        

        if (rank==0)
        {
            std::cout << "Name: "<< name <<std::endl;

        }        
        
        std::shared_ptr<distributedCartesianArray> valid_data;
        if (operation == "read")
        {
            valid_data = createData(benchmarkPool.getCommunicator(),benchmarkNode);
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
        
        auto mode = benchio::writeMode;
        bool isIdirect = benchmarkNode["direct"].as<bool>(false);

        if (operation == "write")
        {
            if (isIdirect) 
            {
                mode = benchio::writeDirectMode;

            }
            else
            {
                mode = benchio::writeMode;
            }

        }


        if (operation == "read")
        {
            if (isIdirect) 
            {
                mode = benchio::readDirectMode;

            }
            else
            {
                mode = benchio::readMode;
            }
            
        }

        for (int i=0;i<repeat;i++)
        {
            for ( auto basename : basenames)
                {
                    basename=basename + "T"+std::to_string(i);
                    auto writer = createWriter(benchmarkNode);
                    
                    writer->open(basename,*data,mode);
                    
                    benchmark current_benchmark( name  );
                    current_benchmark.setFileSync(sync);
                    current_benchmark.setOperation(operation);
                    current_benchmark.setNFields(nFields);
                    if (rank==0) {
                        std::cout << "---------------------" <<std::endl;
                    }
                    
                    current_benchmark.write(*data, *writer);
                    
                    
                    writer->close();

                    auto response = current_benchmark.report_yaml() ;
                    
                    response["file_rank_0"]=basename;
            

                    if( operation=="read" )
                    {
                        bool isAlmostEqual= ( distance(data->getData(),valid_data->getData()) <= tol);
                        
                        response["valid"]=isAlmostEqual;
                        
                        if (not isAlmostEqual)
                        {
                            std::cerr << "Error: read data is invalid." << std::endl; 
                            MPI_Abort(MPI_COMM_WORLD,1);
                        }
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