#ifndef POSIX_IO_H
#define POSIX_IO_H


#include "timers.h"
#include <string>
#include <vector>


class posix_io
{
    public:

    posix_io(std::string basename);

    void write( const std::vector<real_t> & data, timer & timer) const;
    
    void read( std::vector<real_t> & data, timer & timer);


    private:

    std::string filename;
    int rank,nRanks;
};


#endif