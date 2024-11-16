#ifndef CTL_IO_H
#define CTL_IO_H

class ctl_io
{
    public:

    virtual void open(std::string filename, distributedCartesianArray & data, benchio::openMode mode )=0;


    virtual void write( distributedCartesianArray & data)  = 0;

    virtual void read( distributedCartesianArray & data) = 0;
    
    virtual void close()=0;

    virtual void sync() {

        throw std::runtime_error("Sync operation is not implemented");
    };

};

#endif
