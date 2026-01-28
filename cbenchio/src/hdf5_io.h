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
    
    virtual void sync();
    
    /*! \brief Set HDF5 chunking
     * 
     * @param chunkDims vector with chunk dimensions. Same size as number of dimensions of the dataset.
     */
    void setChunking(std::vector<hsize_t> chunkDims_) { chunkDims=chunkDims_; };

    private:


    bool isCollective=true;
    hid_t fileId,fileSpaceId,memSpaceId,pListTransfer;
    int currentField=0;
    std::vector<hsize_t> chunkDims;
    
};