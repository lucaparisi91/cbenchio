#include "../src/pool.h"
#include "tools.h"
/*
Run with 5 ranks
*/

int main(int argc,char** argv)
{
    int rank=-1, ranks=-1;
    MPI_Init( &argc , & argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ranks);

    
    int poolRanks=0;
    int poolSize=2;
    
    pool test_pool(poolSize,MPI_COMM_WORLD,"test_pool");
    auto comm_pool= test_pool.getCommunicator();

    MPI_Comm_size(comm_pool,&poolRanks);

    if (rank<(ranks/poolRanks) * poolRanks )
    {
        testing::check(poolRanks == poolSize,"Ranks in the pool");

    }
    else 
    {
        testing::check(poolRanks == ranks%poolSize,"Ranks in the pool");
    }  

};