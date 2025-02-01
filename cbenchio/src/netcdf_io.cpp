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

    if (mode == benchio::writeMode)
    {
        
        ret = nc_create_par(filename.c_str(), NC_NETCDF4 | NC_CLOBBER, data.getCartesianCommunicator(), info, &fileId);

        check(ret, "Create Parallel");

        ret = nc_def_dim(fileId,"x",data.getGlobalShape()[0],&dimIds[0]);
        check(ret, "Create Dim x");
        ret = nc_def_dim(fileId,"y",data.getGlobalShape()[1],&dimIds[1]);
        check(ret, "Create Dim y");
        ret = nc_def_dim(fileId,"z",data.getGlobalShape()[2],&dimIds[2]);
        check(ret, "Create Dim z");
        ret = nc_def_var(fileId, "data", NC_DOUBLE, 3, dimIds, &dataId);
        check(ret, "Create Data variable");

    }
    else
    {
        ret = nc_open_par(filename.c_str(), NC_NOWRITE, data.getCartesianCommunicator(), info, &fileId);
    }

    if (isCollective)
    {
        nc_var_par_access(fileId,NC_GLOBAL,NC_COLLECTIVE);
    }
    else 
    {
        nc_var_par_access(fileId,NC_GLOBAL,NC_INDEPENDENT);
    }

}   

void netcdf_io::write( distributedCartesianArray & data) 
{   
    ptrdiff_t stride[3] {1,1,1};
    ptrdiff_t imap[3] {1, (ptrdiff_t)(data.getLocalShape()[0]), (ptrdiff_t)(data.getLocalShape()[1] *data.getLocalShape()[0]) , };
        
    int ret = nc_put_varm_double( fileId, dataId, data.getLocalOffset().data(), data.getLocalShape().data(), stride,imap, data.getData().data() );
    check(ret,"Write netcdf variable");

}

void netcdf_io::read( distributedCartesianArray & data)
{   
    ptrdiff_t stride[3] {1,1,1};
    ptrdiff_t imap[3] {1, (ptrdiff_t)(data.getLocalShape()[0]), (ptrdiff_t)(data.getLocalShape()[1] *data.getLocalShape()[0]) , };
    
    int ret = nc_get_varm_double( fileId, dataId, data.getLocalOffset().data(), data.getLocalShape().data(), stride,imap, data.getData().data() );
    check(ret,"Read netcdf variable");

}

void netcdf_io::sync()
{
    nc_sync(fileId);
}