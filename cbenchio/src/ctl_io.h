#ifndef CTL_IO_H
#define CTL_IO_H

class ctl_io
{
    public:

    virtual void open(std::string filename, distributedCartesianArray & data, benchio::openMode mode )=0;


    virtual void write( distributedCartesianArray & data) const = 0;

    virtual void read( distributedCartesianArray & data) const = 0;
    
    virtual void close()=0;

};

#endif
