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
    /**
     * Set the chunking dimensions for the NetCDF file. If chunking is enabled, the data will be written in chunks of the specified dimensions. By default, chunking is disabled.
      * @param chunkDims_ A vector specifying the chunking size for each spatial dimension of the data. If a dimension is set to 0, it will not be chunked.
     */
    void setChunking(std::vector<size_t> chunkDims_){ spatialChunkDims = chunkDims_; std::reverse(spatialChunkDims.begin(), spatialChunkDims.end()); }

    virtual void sync();

    /**
     * Enable or disable time stepping. If time stepping is enabled, an extra unlimited dimension is added to the NetCDF file to represent time, and each call to write() will write to a new time step. If time stepping is disabled, each call to write() will create a new variable. By default, time stepping is disabled.
     * @param time_stepping_ Set to true to enable time stepping, false to disable it.
     */
    void setTimeStepping(bool time_stepping_) { time_stepping = time_stepping_; }


    private:


    void set_variable();

    /**
     * Initialize the chunking parameters from the chunking in the spatial dimensions.
     * This handles adding an extra dimension for time stepping if enabled.
     */
    void init_chunking();

    bool isCollective=true;
    int fileId= 0;
    std::vector<int> dimIds;

    int currentField=0;
    int dataId=0;
    
    std::vector<size_t> offset;
    std::vector<size_t> shape;

    bool time_stepping=false;
    std::vector<size_t> spatialChunkDims; // Spatial chunk dimensions in C ordering
    std::vector<size_t> chunkDimsCorder;
};