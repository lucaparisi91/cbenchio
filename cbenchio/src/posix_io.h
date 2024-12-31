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

    void setChunkSize(size_t aligment);
    void setStride();
    void setLockAhead(){lockAhead=true;}
    void setWaitLockAhead(){waitLockAhead=true;}

    void setLockAhead(distributedCartesianArray & data);


    private:

    size_t getInitialFileOffset(distributedCartesianArray & data) const ;

    size_t getStride(distributedCartesianArray & data) const;
    int rank,nRanks;
    size_t chunkSize = 0;
    int f; // file descriptor
    size_t max_read_write_operations=100000;
    bool strided = false;
    bool lockAhead = false;
    bool waitLockAhead=false;

    benchio::openMode mode = benchio::unknown;

};


#endif