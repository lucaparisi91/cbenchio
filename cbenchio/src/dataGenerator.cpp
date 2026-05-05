#include "dataGenerator.h"
#include <iostream>


void indexDataGenerator::generate( distributedCartesianArray & disData,real_t offset)
{
    using index_t = distributedCartesianArray::index_t ;
    auto & localData = disData.getData();
    auto localShape = disData.getLocalShape();
    auto localOffset = disData.getLocalOffset();
    auto globalShape = disData.getGlobalShape();

    

    for( index_t i =0 ; i< localShape[0] ; i++  )
    {
        for( index_t j =0 ; j< localShape[1] ; j++  )
        {
            for( index_t k =0 ; k< localShape[2] ; k++  )
            {
                auto ig = i + localOffset[0];
                auto jg = j + localOffset[1];
                auto kg = k + localOffset[2];
                
                localData((signed long int)i,(signed long int)j,(signed long int)k)= offset + ig + globalShape[0]*jg + globalShape[0]*globalShape[1]*kg ;

            }   
        }


    }


}  


void randomDataGenerator::generate( distributedCartesianArray & disData,real_t offset)
{
    using index_t = distributedCartesianArray::index_t ;
    auto & localData = disData.getData();
    auto localShape = disData.getLocalShape();
    auto localOffset = disData.getLocalOffset();
    auto globalShape = disData.getGlobalShape();

    // Sets a different for each process by adding the rank to the seed. This ensures that different processes generate different random numbers, while still allowing for reproducibility, assuming the application uses the same number of processes
    auto comm = disData.getCartesianCommunicator();
    int rank;
    MPI_Comm_rank(comm, &rank);
    std::mt19937 gen(seed + rank);

    // Fill the local array with generated random numbers between 0 and 1
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for( index_t i =0 ; i< localShape[0] ; i++  )
    {
        for( index_t j =0 ; j< localShape[1] ; j++  )
        {
            for( index_t k =0 ; k< localShape[2] ; k++  )
            {
                auto ig = i + localOffset[0];
                auto jg = j + localOffset[1];
                auto kg = k + localOffset[2];
                
                localData((signed long int)i,(signed long int)j,(signed long int)k )= offset + dis(gen) ;
                
            }   
        }


    }


}  