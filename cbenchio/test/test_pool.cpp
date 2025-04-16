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

    
    int poolSize=2;
    
    pool test_pool(poolSize,MPI_COMM_WORLD,"test_pool");
    auto comm_pool= test_pool.getCommunicator();
    
    int poolRank=-1;
    int poolRanks=-1;

    MPI_Comm_size(comm_pool,&poolRanks);
    MPI_Comm_rank(comm_pool,&poolRank);    

    if (rank<(ranks/poolSize) * poolSize )
    {
        testing::check_near(poolRanks , poolSize,"Ranks in the pool");
    }
    else 
    {
        testing::check_near(poolRanks ,poolSize - 1,"Ranks in the pool");
    }  

    switch(rank)
    {
        case 0: 
            testing::check_near(poolRank,0,"pool rank");
            break;
        case 1:
            testing::check_near(poolRank,1,"pool rank");
            break;
        case 2:
            testing::check_near(poolRank,0,"pool rank");
            break;
        case 3:
            testing::check_near(poolRank,1,"pool rank");
            break;
        case 4:
            testing::check_near(poolRank,0, "pool rank");
            break;
    }

};