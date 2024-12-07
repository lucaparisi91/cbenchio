

#include "../src/posix_io.h"
#include <mpi.h>
#include "../src/dataGenerator.h"
#include <iostream>

#include "tools.h"
#include <sstream>



int main(int argc, char ** argv)
{
    
    const size_t globalDataSize = 1e+2;

    int rank=-1, nRanks=-1;

    MPI_Init( &argc , & argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);
    
    {
    distributedCartesianArray data(MPI_COMM_WORLD, { 100, 100, 1  }) ;
    
    // std::cout << "size "<<rank << " "<< data.getLocalSize() << std::endl;
    // std::cout << "offset "<<rank << " "<< data.getLocalOffset() << std::endl;

    auto localShape = data.getLocalShape();
    auto localOffset = data.getLocalOffset();
    auto globalShape = data.getGlobalShape();
    auto localSize = data.getLocalSize();


    //testing::log(localOffset, "local offset");

    testing::check( globalShape[0]==100,"globalShape test");
    testing::check( globalShape[1]==100,"globalShape test");

    if (nRanks==1)
    {
        testing::check( localShape[0]==100,"localShape test");
        testing::check( localShape[1]==100,"localShape test");
        testing::check( localSize==100*100,"localSize test");

    }
    else if (nRanks==2)
    {
       
        testing::check_near( localSize,100*100/2, "localSize");

        testing::check_near(localShape, {50,100,1},"localShape");
        if (rank ==0 ) testing::check_near( localOffset, {0,0,0} , "localOffset" );
        if (rank ==1 ) testing::check_near( localOffset, {50,0,0} , "localOffset" );
        
    }

    indexDataGenerator gen;
    gen.generate( data);
    int N = 100;

    auto sum_expected = N * (N-1)*(2*N-1)/6. * N * 2;

    auto sum_local = sum(data.getData());


    real_t sum_global=0;
    MPI_Reduce( &sum_local, &sum_global , 1 , MPI_DOUBLE, MPI_SUM, 0 , MPI_COMM_WORLD );

    if (rank ==0)
    {
        testing::check_near( sum_global, sum_expected,"Data Sum");
    }

    testing::check_near( 2 , data.getNDimensions()  ,"nDimensions");

    }

    /*
    1D case
    */
    {
        distributedCartesianArray data(MPI_COMM_WORLD, { 100,1, 1  }) ;

        if (nRanks ==3)
        {
            if (rank == 0 )
                {
                testing::check_near( data.getLocalShape() , {34,1,1} ,"localShape");
                testing::check_near( data.getLocalOffset() , {0,0,0} ,"localShape");
                }
            else 
                {
                testing::check_near( data.getLocalShape() , {33,1,1} ,"localShape");

                if (rank==1) testing::check_near( data.getLocalOffset() , {34,0,0} ,"localShape");
                }
        }
        
    
    }
    
    //testing::check_near( )
    // if (nRanks == 2)
    // {
    //     if ( localSize != 50 ) MPI_Abort(MPI_COMM_WORLD,1);
    //     if ( globalSize != 100 ) MPI_Abort(MPI_COMM_WORLD,1);

    //     if (rank == 0 & localOffset!=0)  MPI_Abort(MPI_COMM_WORLD,1);

    //     if (rank == 1 & localOffset!=50)  MPI_Abort(MPI_COMM_WORLD,1);


         
         
    // }
    // else if (nRanks == 3)
    // {
    //     if ( globalSize != 100 ) MPI_Abort(MPI_COMM_WORLD,1);

    //     if (rank == 0 & (localOffset!=0) & (localSize!=34))  MPI_Abort(MPI_COMM_WORLD,1);

    //     if (rank == 1 & (localOffset!=34) & (localSize!=33))  MPI_Abort(MPI_COMM_WORLD,1);

    //     if (rank == 2 & (localOffset!=67) & (localSize!=33))  MPI_Abort(MPI_COMM_WORLD,1);
        
    // }
    // else 
    // {
    //     if (rank==0)
    //     {
    //         std::cout << "Unsupported number of ranks" << std::endl;
    //         MPI_Abort(MPI_COMM_WORLD,2);
    //     }

        
    // }

    

    MPI_Finalize();

}