#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include "definitions.h"
#include "mpi.h"
#include <random>



class dataGenerator
{
    public:

    dataGenerator(size_t globalSize) : real_distribution(0,1)
    {
        seed = 15686;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &nRanks);
        generator.seed( seed + rank);

        nDataLocal=globalSize/(nRanks*sizeof(real_t));


    }

    auto getGlobalSize()
    {
        return nDataLocal*sizeof(real_t)*nRanks;
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


    private:
    int rank,nRanks;
    int seed;
    int nDataLocal;
    std::default_random_engine generator;
    std::uniform_real_distribution<real_t> real_distribution;

};


#endif