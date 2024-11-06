#include <string>
#include "data.h"
#include "hdf5.h"
#include "ctl_io.h"


class hdf5_io : public ctl_io
{
    public:
    
    void write( distributedCartesianArray & data) ;

    void read( distributedCartesianArray & data) ;
    
    void open( std::string filename,  distributedCartesianArray & data, benchio::openMode mode) ;

    void close();

    void setCollective() { isCollective=true;}

    void unSetCollective(){ isCollective=false;}

    private:

    bool isCollective=true;
    hid_t fileId,dsetId,fileSpaceId,memSpaceId,pListTransfer;

    
};