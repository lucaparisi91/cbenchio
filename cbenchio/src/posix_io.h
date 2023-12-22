#ifndef POSIX_IO_H
#define POSIX_IO_H


#include "timers.h"
#include <string>
#include <vector>
#include "data.h"


class posix_io
{
    public:

    posix_io(std::string basename);

    void write( distributedCartesianArray & data) const;

    void read( distributedCartesianArray & data);
    

    private:

    std::string filename;
    int rank,nRanks;
};


#endif