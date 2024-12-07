
#include "benchioArray.h"


benchioArray::benchioArray(index_t i,index_t j,index_t k) : 
_shape{i,j,k}
{
    _data = new real_t[i*j*k];

}
