#include <iostream>
#include <vector>
#include <random>
#include <mpi.h>
#include <fstream>
#include <sstream>

using real_t = double ;

class timer
{
    public:

    timer()
    {

    }

    public:
        void reset() { tStart=0; tEnd=0; tElapsed=0;tMaxElapsed=0;}
        void start() { tStart= MPI_Wtime(); }
        void end() {tEnd = MPI_Wtime(); tElapsed += tEnd - tStart;  }
        auto const elapsed() { return tElapsed; }

        auto const maxElapsed() 
        {
            
            return tMaxElapsed;

        }

        auto computeMaxElapsed()
        {
            // only the master will get the maxed out value
            MPI_Reduce( &tElapsed, &tMaxElapsed, 1, MPI_DOUBLE, MPI_MAX, 0 , MPI_COMM_WORLD);

        }

    private:
    real_t tStart=0; 
    real_t tEnd=0;
    real_t tElapsed=0;

    real_t tMaxElapsed;

   
};


class posix_write_file_per_process
{
    public:
    posix_write_file_per_process(std::string basename)
    {
        MPI_Comm_rank(MPI_COMM_WORLD, & rank);
        MPI_Comm_size(MPI_COMM_WORLD, & nRanks);
        
        std::stringstream s;
        s << basename << "_rank" << rank << ".out"  ;

        filename = s.str();

    }

    void write( const std::vector<real_t> & data, timer & timer)
    {
        std::fstream f( filename, std::ios::out|std::ios::binary);
        f.seekp(std::ios::beg);

        timer.start();
        f.write( (char*) (&data[0]) , data.size()*sizeof(real_t));
        f.flush();
        timer.end();

        f.close();
    }


    private:

    std::string filename;
    int rank,nRanks;

};

int main(int argc, char ** argv)
{
    int rank=-1, nRanks=-1;
    

    MPI_Init( &argc , & argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);
    

    size_t globalDataSize = 1e+8;
    size_t nData = globalDataSize/(sizeof(real_t)*nRanks);

    double tStart,tEnd;
    
   

    std::vector<real_t > data;
    data.resize(nData);
    size_t dataSize = data.size()* sizeof(real_t) ;



    std::default_random_engine generator;
    generator.seed(15686 + rank);
    std::uniform_real_distribution<real_t> distribution(0.0,1.0);

    for (int i=0;i< nData; i++)
    {
        data[i] = distribution(generator) ;
    }

    auto writer = posix_write_file_per_process("data");

    timer writeTime;

    writer.write(data,writeTime);

    writeTime.computeMaxElapsed();

    
    if (rank == 0)
    {
        auto globalDataSizeGB = data.size()*nRanks*sizeof(real_t) / 1.e+9;
        std::cout << "Data Size: " << globalDataSizeGB << "GB" << std::endl;
        std::cout << "N. ranks: " << nRanks << std::endl;
        std::cout << "BW: " << globalDataSizeGB/writeTime.maxElapsed()  << " GB/s" << std::endl;

    }

    
    MPI_Finalize();

}