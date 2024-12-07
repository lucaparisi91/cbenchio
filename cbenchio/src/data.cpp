#include "data.h"
#include <iostream>

distributedVector::distributedVector( MPI_Comm comm_,size_t globalSize_ ) : comm(comm_),globalSize(globalSize_)
{
    int rank,nRanks;

    MPI_Comm_rank( comm, & rank);
    MPI_Comm_size( comm, & nRanks);

    localSize = globalSize / nRanks ;
    localOffset = localSize * rank;

    auto remainder = globalSize % nRanks;

    if (rank < remainder)
    {
        localSize+=1;
        localOffset+=rank;
    }
    else 
    {
        localOffset+=remainder;
    }

    localData.resize(localSize,0);
}



distributedCartesianArray::distributedCartesianArray( MPI_Comm comm_,std::array<index_t,3> globalShape_ ,std::array<int, 3> processorGrid ) : comm(comm_)
{

    globalShape=globalShape_;
    int rank=-1, nRanks=-1;

    nDimensions=3;
    std::array<int,3 > dims = processorGrid;


    if ( globalShape[2] == 1 )
    {
        nDimensions-=1;
        dims[2]=1;
        if (globalShape[1] == 1)
        {
            nDimensions-=1;
            dims[1]=1;
        }
    } 


    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &nRanks);


    MPI_Dims_create( nRanks, max_dims, dims.data() );

    //std::cout << dims[0] << " " << dims[1] << " "<< dims[2] << std::endl;


    std::array<int , 3> periodic {0,0,0};

    MPI_Cart_create( comm,
                    3,
                    dims.begin(),
                    periodic.begin(),
                    1,
                    &cartesian_comm);
    



    MPI_Cart_get( cartesian_comm, 3 , nRanksCartesian.begin() , period.begin() , rankCartesian.begin() );

    globalSize=1;
    for (int d=0;d<3;d++)
    {
        globalSize*=globalShape[d];

        if ( nRanksCartesian[d] != 0 )
        {

            localShape[d]= globalShape[d]/nRanksCartesian[d];
            localOffset[d]=localShape[d]*rankCartesian[d];

            auto remainder = globalShape[d] % nRanksCartesian[d];

            if (rankCartesian[d] < remainder)
            {
                localShape[d]+=1;
                localOffset[d]+=rankCartesian[d];    
            }
            else 
            {
                localOffset[d]+=remainder;
            }

        }

    }

    localSize=localShape[0] * localShape[1] * localShape[2] ;
    
    localData= std::make_unique<benchioArray>(localShape);

    
}

void distributedCartesianArray::print() const
{
    int rank,nRanks;

    MPI_Comm_rank( comm, & rank);
    MPI_Comm_size( comm, & nRanks);
  
    std::cout << "Array grid: " << globalShape[0] << " " << globalShape[1] << " " << globalShape[2] << std::endl;
    std::cout << "Processor grid: " << nRanksCartesian[0] << " " << nRanksCartesian[1] << " " << nRanksCartesian[2 ] << std::endl;


    // for (int i=0; i< nRanks;i++)
    // {

    //     if (i==rank)
    //     {
    //         std::cout << "Rank: " << rank ;
    //         std::cout << ", Local Offset: " << localOffset[0] << " " << localOffset[1] << " " << localOffset[2] ;
    //         std::cout << ", Local Shape: " << localShape[0] << " " << localShape[1] << " " << localShape[2] << std::endl;   
    //     }

    //     MPI_Barrier(comm);

    // }

    
    

}

