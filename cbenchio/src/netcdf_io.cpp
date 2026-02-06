#include "netcdf_io.h"
#include "netcdf_par.h"
#include <iostream>
#include <stdexcept>

void netcdf_io::close( )
{

    nc_close(fileId);
    
}

void check(int ret, std::string msg)
{
    if (ret!=0)
    {
        throw std::runtime_error("Error: " + std::to_string(ret) + ", " + msg);
    }
}

void netcdf_io::open( std::string filename,  distributedCartesianArray & data, benchio::openMode mode)
{
    int ret;

    MPI_Info info = MPI_INFO_NULL;

    ret = nc_set_chunk_cache(0,0,0);
    check(ret,"Disable chunk cache");
    auto spatial_dims = data.getGlobalShape().size();
    auto dims = spatial_dims;
    if (time_stepping)
    {
        dims += 1; // Add an extra dimension for time if time stepping is enabled
    }
    dimIds.resize(dims,0);
    offset.resize(dims,0);
    shape.resize(dims,0);


    int first_spatial_dimension=0;
    if (mode == benchio::writeMode)
    {

        ret = nc_create_par(filename.c_str(), NC_NETCDF4 | NC_CLOBBER, data.getCartesianCommunicator(), info, &fileId);
        check(ret, "Create Parallel");

        // Setup dimensions. If timestepping is enabled, the first dimension is unlimited and used for time, otherwise all dimensions are defined as the global shape of the data.
        if (time_stepping)
        {
            ret = nc_def_dim(fileId,"time",NC_UNLIMITED,&(dimIds[0]));
            check(ret, "Create time Dim");
            offset[0] = 0; // Start at the first time step
            shape[0] = 1; // Access one element at a time along the time dimension
            first_spatial_dimension=1;
        }
        for (int d=0;d<spatial_dims;d++)
        {
            offset[d+first_spatial_dimension] = data.getLocalOffset()[spatial_dims-1-d];
            shape[d+first_spatial_dimension] = data.getLocalShape()[spatial_dims-1-d];

            ret = nc_def_dim(fileId, ("Dim" + std::to_string(d)).c_str(),data.getGlobalShape()[d],&(dimIds[dims-1-d]));
            check(ret, "Create Dim " + std::to_string(d) );
            
        }

        set_variable(); // Set variable for the first field
    }
    else
    {
        ret = nc_open_par(filename.c_str(), NC_NOWRITE | NC_NETCDF4, data.getCartesianCommunicator(), info, &fileId);
    }


}

void netcdf_io::setChunking(std::vector<size_t> chunkDims_) { 

    chunkDimsCorder.resize(dimIds.size());
    auto first_spatial_dimension = chunkDimsCorder.begin();

    if (time_stepping)
    {
        chunkDimsCorder[0] = 1; // Chunk only on spatial dimensions
        first_spatial_dimension += 1; // If time stepping is enabled, the first dimension is reserved for time and chunking starts from the second dimension
    }

    std::reverse_copy(chunkDims_.begin(), chunkDims_.end(), first_spatial_dimension);
};


void netcdf_io::set_variable()
{
    int ret;

    ret = nc_def_var(fileId, ("data" + std::to_string(currentField)).c_str() , NC_DOUBLE, dimIds.size(), dimIds.data(), &dataId);
    check(ret, "Create data variable");

    if ( chunkDimsCorder.size() > 0 )
    {

        ret= nc_def_var_chunking( fileId, dataId, NC_CHUNKED, chunkDimsCorder.data() );
        check(ret,"Define chunking for data variable");
    }

    if (isCollective)
    {
        ret=nc_var_par_access(fileId,dataId,NC_COLLECTIVE);
    }
    else 
    {
        ret=nc_var_par_access(fileId,dataId,NC_INDEPENDENT);
    }    

    check(ret,"Set access mode for the variable");

}

void netcdf_io::write( distributedCartesianArray & data) 
{   
    int ret;

    if (! time_stepping && currentField > 0)
    {
        set_variable(); // Define a new variable for the next field if time stepping is not enabled
    }

    ret = nc_put_vara_double( fileId, dataId, offset.data(),shape.data(), data.getData().data() );
    check(ret,"Write netcdf variable");
    
    if (time_stepping)
    {
            offset[0] += 1; // Move to the next time step (assuming time is the slowest changing dimension)
    }
    else
    {
            currentField+=1; // Move to the next field
    }

}

void netcdf_io::read( distributedCartesianArray & data)
{   
    int ret;
    
    ptrdiff_t stride[3] {1,1,1};
    ptrdiff_t imap[3] {(ptrdiff_t)(data.getLocalShape()[1] *data.getLocalShape()[0]), (ptrdiff_t)(data.getLocalShape()[0]), 1 };

    size_t offset[3] { data.getLocalOffset()[2],data.getLocalOffset()[1],data.getLocalOffset()[0] } ;
    size_t shape[3] { data.getLocalShape()[2],data.getLocalShape()[1],data.getLocalShape()[0] } ;
    nc_inq_varid(fileId,("data" + std::to_string(currentField)).c_str() ,&dataId );

    if (isCollective)
    {
        nc_var_par_access(fileId,dataId,NC_COLLECTIVE);
    }
    else 
    {
        nc_var_par_access(fileId,dataId,NC_INDEPENDENT);
    }


    ret = nc_get_varm_double( fileId, dataId, offset,shape, stride,imap, data.getData().data() );
    check(ret,"Read netcdf variable");
    currentField+=1;
    
}

void netcdf_io::sync()
{
    nc_sync(fileId);
}