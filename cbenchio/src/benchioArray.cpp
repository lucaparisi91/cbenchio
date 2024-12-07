#include <cstring>
#include "benchioArray.h"
#include <cstdlib>
#include <mpi.h>
#include <iostream>

benchioArray::benchioArray(index_t i,index_t j,index_t k) : 
_shape{i,j,k}, _size(i*j*k)
{
    if (_size > 0)
    {
        _data = new real_t[_size];
    }
    
}
benchioArray::benchioArray(index_t i,index_t j,index_t k,size_t alignment) : 
_shape{i,j,k}, _size(i*j*k)
{
    if (_size > 0)
    {
        size_t alloc_size= ( ( sizeof(real_t)*_size + alignment - 1)  / alignment )*alignment ;



        _data = (real_t *)(aligned_alloc( alignment , alloc_size ) );
                
        if(!_data)
        {
            std::cerr << "Error: Memory allocation of " << _size << "elements with aligment of " << alignment << "bytes failed.";
            MPI_Abort(MPI_COMM_WORLD,1);
        }
    }

}


real_t sum(const benchioArray & dataArray)
{    
    real_t sum=0;
    for (int i=0;i<dataArray.getShape()[0] ; i++)
        for (int j=0;j<dataArray.getShape()[1]; j++ )
            for (int k=0;k<dataArray.getShape()[2]; k++ )
                {
                    sum+=dataArray(i,j,k);
                }
    return sum;
}


real_t distance(const benchioArray & dataArray,const benchioArray & dataArray2)
{    
    real_t sum=0;
    for (int i=0;i<dataArray.getShape()[0] ; i++)
        for (int j=0;j<dataArray.getShape()[1]; j++ )
            for (int k=0;k<dataArray.getShape()[2]; k++ )
                {
                    sum+=std::abs( dataArray2(i,j,k) - dataArray(i,j,k) );
                }
    return sum;
}


