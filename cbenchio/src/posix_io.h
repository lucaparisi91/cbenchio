#ifndef POSIX_IO_H
#define POSIX_IO_H


#include "timers.h"
#include <string>
#include <vector>
#include "data.h"

class ctl_io
{
    public:

    virtual void open(std::string filename, distributedCartesianArray & data, benchio::openMode mode )=0;


    virtual void write( distributedCartesianArray & data) const = 0;

    virtual void read( distributedCartesianArray & data) const = 0;
    
    virtual void close()=0;

};


class posix_io : public ctl_io
{
    public:


    virtual void open(std::string filename, distributedCartesianArray & data, benchio::openMode mode );


    virtual void write( distributedCartesianArray & data) const;

    virtual void read( distributedCartesianArray & data) const;
    
    virtual void close();


    private:

    int rank,nRanks;

    int f; // file descriptor
};


#endif