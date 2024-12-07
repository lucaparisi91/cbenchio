

#include "../src/posix_io.h"
#include <mpi.h>
#include "../src/dataGenerator.h"
#include <iostream>

#include "tools.h"
#include "../src/benchioArray.h"

using index_t = benchioArray::index_t;

int main(int argc, char ** argv)
{

    int rank=-1, nRanks=-1;

    MPI_Init( &argc , & argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);
    std::array<index_t,3> shape { 30,40,53 };
    size_t alignment=8;

    benchioArray arr(shape[0],shape[1],shape[2],alignment);
    
    testing::check_near( (size_t)arr.data() % alignment, 0 );


    for( index_t i=0;i<arr.getShape()[0];i++ )
        for( index_t j=0;j<arr.getShape()[1];j++ )
            for( index_t k=0;k<arr.getShape()[2];k++ )
            {
                arr(i,j,k)=i*i + j*j + k*k;
            }

    testing::check_near( shape, arr.getShape(), "benchio array shape" );


    for( index_t i=0;i<shape[0];i++ )
        for( index_t j=0;j<shape[1];j++ )
            for( index_t k=0;k<shape[2];k++ )
            {
                testing::check_near( arr(i,j,k), i*i + j*j + k*k);
    
            }
    


    
    MPI_Finalize();

}