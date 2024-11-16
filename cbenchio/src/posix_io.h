#ifndef POSIX_IO_H
#define POSIX_IO_H

#include "timers.h"
#include <string>
#include <vector>
#include "data.h"
#include "ctl_io.h"

class posix_io : public ctl_io
{
    public:


    virtual void open(std::string filename, distributedCartesianArray & data, benchio::openMode mode );


    virtual void write( distributedCartesianArray & data) ;

    virtual void read( distributedCartesianArray & data) ;
    
    virtual void close();

    virtual void sync();
    
    void setAligment(size_t aligment);


    private:

    int rank,nRanks;
    bool aligned=false;
    size_t alignment = 1048576 ;

    int f; // file descriptor
};


#endif