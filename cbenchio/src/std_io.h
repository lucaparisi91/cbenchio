#ifndef STD_IO_H
#define STD_IO_H

#include "timers.h"
#include <string>
#include <vector>
#include "data.h"
#include "ctl_io.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

class std_io : public ctl_io
{
    public:


    virtual void open(std::string filename, distributedCartesianArray & data, benchio::openMode mode );


    virtual void write( distributedCartesianArray & data) ;

    virtual void read( distributedCartesianArray & data) ;
    
    virtual void close();


    private:

    int rank,nRanks;

    FILE * fp ;
};


#endif