#include <string>
#include "data.h"
#include "hdf5.h"
#include "ctl_io.h"


class hdf5_io : public ctl_io
{
    public:
    
    void write( distributedCartesianArray & data) const;

    void read( distributedCartesianArray & data) const;
    
    void open( std::string filename,  distributedCartesianArray & data, benchio::openMode mode) ;

    void close();

    void setIsCollective() { isCollective=true;}

    void unSetIsCollective(){ isCollective=false;}

    private:

    bool isCollective=true;
    hid_t fileId,dsetId,fileSpaceId,memSpaceId,pListTransfer;

    
};