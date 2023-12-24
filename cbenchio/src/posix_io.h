#ifndef POSIX_IO_H
#define POSIX_IO_H


#include "timers.h"
#include <string>
#include <vector>
#include "data.h"




class posix_io
{
    public:


    void open(std::string filename, distributedCartesianArray & data, benchio::openMode mode );


    void write( distributedCartesianArray & data) const;

    void read( distributedCartesianArray & data);
    
    void close();


    private:

    int rank,nRanks;

    int f; // file descriptor
};


#endif