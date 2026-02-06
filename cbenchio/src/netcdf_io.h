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
    
    void setChunking(std::vector<size_t> chunkDims_);

    virtual void sync();

    void setTimestepping(bool time_stepping_) { time_stepping = time_stepping_; }

    private:
    
    void set_variable();

    bool isCollective=true;
    int fileId= 0;
    std::vector<int> dimIds;

    int currentField=0;
    int dataId=0;

   std::vector<size_t> offset;
   std::vector<size_t> shape;
   
    bool time_stepping=true;
    std::vector<size_t> chunkDimsCorder;
};