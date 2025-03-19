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

        ret = nc_def_dim(fileId,"x",data.getGlobalShape()[0],&dimIds[2]);
        check(ret, "Create Dim x");
        ret = nc_def_dim(fileId,"y",data.getGlobalShape()[1],&dimIds[1]);
        check(ret, "Create Dim y");
        ret = nc_def_dim(fileId,"z",data.getGlobalShape()[2],&dimIds[0]);
        check(ret, "Create Dim z");
     

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
    int ret;
    int dataId;
    ptrdiff_t stride[3] {1,1,1};
    ptrdiff_t imap[3] {(ptrdiff_t)(data.getLocalShape()[1] *data.getLocalShape()[0]), (ptrdiff_t)(data.getLocalShape()[0]), 1 };

    size_t offset[3] { data.getLocalOffset()[2],data.getLocalOffset()[1],data.getLocalOffset()[0] } ;
    size_t shape[3] { data.getLocalShape()[2],data.getLocalShape()[1],data.getLocalShape()[0] } ;

    ret = nc_def_var(fileId, ("data" + std::to_string(currentField)).c_str() , NC_DOUBLE, 3, dimIds, &dataId);
    check(ret, "Create Data variable");
    ret = nc_put_varm_double( fileId, dataId, offset,shape, stride,imap, data.getData().data() );
    check(ret,"Write netcdf variable");
    currentField++;

}

void netcdf_io::read( distributedCartesianArray & data)
{   
    int ret;
    int dataId;
    
    ptrdiff_t stride[3] {1,1,1};
    ptrdiff_t imap[3] {(ptrdiff_t)(data.getLocalShape()[1] *data.getLocalShape()[0]), (ptrdiff_t)(data.getLocalShape()[0]), 1 };

    size_t offset[3] { data.getLocalOffset()[2],data.getLocalOffset()[1],data.getLocalOffset()[0] } ;
    size_t shape[3] { data.getLocalShape()[2],data.getLocalShape()[1],data.getLocalShape()[0] } ;
    nc_inq_varid(fileId,("data" + std::to_string(currentField)).c_str() ,&dataId );

    ret = nc_get_varm_double( fileId, dataId, offset,shape, stride,imap, data.getData().data() );
    check(ret,"Read netcdf variable");
    currentField+=1;

}

void netcdf_io::sync()
{
    nc_sync(fileId);
}