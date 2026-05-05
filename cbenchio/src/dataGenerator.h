#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include "definitions.h"
#include "mpi.h"
#include <random>
#include "data.h"

/*************************
 * This files defines Data Generators. Data generators are responsible to initialise the data to be written.
 * Given a dat structure as input, they fille the data structure with values/
 */


 /** indexDataGenerator
  * Fills the data with values that are a function of the global index of the data.
  *  
******************************************************/
class dataGenerator
{
    public:
    
    virtual void generate( distributedCartesianArray & disData, double offset=0) = 0;

};
 
class indexDataGenerator : public dataGenerator
{
    public:

    void generate( distributedCartesianArray & disData,double offset=0);

};

/** randomDataGenerator
 *  Fills the data with random values.
 *  The seed is used to ensure reproducibility of the generated data.
 */
class randomDataGenerator : public dataGenerator
{
    public:
    
    /**
     * Constructor for randomDataGenerator.
     * @param seed The seed for the random number generator. This ensures that the generated data is reproducible.
    */
    randomDataGenerator( int seed_) : seed(seed_) {};

    void generate( distributedCartesianArray & disData, double offset=0);
    
    private:

    int seed;
};

#endif