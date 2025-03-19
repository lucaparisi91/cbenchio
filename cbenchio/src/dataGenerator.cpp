#include "dataGenerator.h"
#include <iostream>

void indexDataGenerator::generate( distributedCartesianArray & disData,real_t offset)
{
    using index_t = distributedCartesianArray::index_t ;
    auto & localData = disData.getData();
    auto localShape = disData.getLocalShape();
    auto localOffset = disData.getLocalOffset();
    
    for( index_t i =0 ; i< localShape[0] ; i++  )
    {
        for( index_t j =0 ; j< localShape[1] ; j++  )
        {
            for( index_t k =0 ; k< localShape[2] ; k++  )
            {
                auto ig = i + localOffset[0];
                auto jg = j + localOffset[1];
                auto kg = k + localOffset[2];

                localData((signed long int)i,(signed long int)j,(signed long int)k)=ig*ig + jg*jg + kg*kg + offset;
            }   
        }


    }


}  