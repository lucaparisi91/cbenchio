#include <string>
#include "data.h"
#include "netcdf.h"
#include "ctl_io.h"

class netcdf_io : public ctl_io
{
    public:
    
    void write( distributedCartesianArray & data) ;

    void read( distributedCartesianArray & data) ;
    
    void open( std::string filename,  distributedCartesianArray & data, benchio::openMode mode) ;

    void close();

    void setCollective() { isCollective=true;}

    void unSetCollective(){ isCollective=false;}
    
    virtual void sync();

    private:

    bool isCollective=true;
    int fileId= 0;
    int dimIds[3] ;

    int currentField=0;
    
};