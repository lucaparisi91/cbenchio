#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include "definitions.h"
#include "mpi.h"
#include <random>



class dataGenerator
{
    public:




    dataGenerator( MPI_Comm comm_, size_t globalSize) : real_distribution(0,1) , comm(comm_)
    {
        seed = 15686;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &nRanks);
        generator.seed( seed + rank);



    }

  

    

    protected:

    int rank,nRanks;
    int seed;


    std::default_random_engine generator;
    std::uniform_real_distribution<real_t> real_distribution;
    MPI_Comm comm;
};


class vectorDataGenerator : public dataGenerator
{

    public:
    vectorDataGenerator(MPI_Comm comm_,size_t globalSize) : dataGenerator(comm_,globalSize)    {
        
        nDataLocal=globalSize/(nRanks*sizeof(real_t));



    }


    auto generate( )
    {        

        std::vector<real_t> data(nDataLocal);
        for (int i=0;i< nDataLocal; i++)
        {
            data[i] = real_distribution(generator) ;
        }

        return data;
    }


    auto getGlobalSize()
    {
        return nDataLocal*sizeof(real_t)*nRanks;
    }

    private:
        int nDataLocal;

};



#endif