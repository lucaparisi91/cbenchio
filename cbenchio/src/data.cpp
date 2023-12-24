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



distributedCartesianArray::distributedCartesianArray( MPI_Comm comm,std::array<index_t,3> globalShape_ )
{

    globalShape=globalShape_;
    int rank=-1, nRanks=-1;

    nDimensions=3;
    std::array<int,3 > dims{0,0,0};


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
    


    std::array<int,3> rankCartesian,period,nRanksCartesian;

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

    localData.resize( { localShape[0] , localShape[1],localShape[2] });

}
