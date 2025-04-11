#include "mpi_io.h"
#include "tools.h"
#include <iostream>


void mpi_io::initFileDataType( distributedCartesianArray & data, MPI_Datatype & subArrayDataType ) const
{
    auto globalShapeInt= toInt(data.getGlobalShape());
    auto localShapeInt = toInt(data.getLocalShape());
    auto localOffsetInt= toInt(data.getLocalOffset());

    MPI_Type_create_subarray( 3, globalShapeInt.begin(), localShapeInt.begin() , localOffsetInt.begin(), MPI_ORDER_FORTRAN ,MPI_DOUBLE, &subArrayDataType );
    MPI_Type_commit( &subArrayDataType);
}

void mpi_io::finalizeFileDataType(  MPI_Datatype & subArrayDataType ) const 
{
    
    MPI_Type_free( &subArrayDataType);

}

void mpi_io::open(std::string filename, distributedCartesianArray & data, benchio::openMode mode )
{
    MPI_Offset disp=0;

    auto comm = data.getCartesianCommunicator();


    initFileDataType(data,subArrayDataType);

    auto modeMPI = MPI_MODE_WRONLY;

    if (mode == benchio::readMode) 
    {
        modeMPI = MPI_MODE_RDONLY;

    }
    else if ( mode == benchio::writeMode )
    {
        modeMPI = MPI_MODE_CREATE | MPI_MODE_WRONLY;
    }
    
    int ret = MPI_File_open( comm, filename.c_str(), modeMPI, MPI_INFO_NULL, &fh );
    
    if (ret != MPI_SUCCESS )
    {
        throw std::runtime_error("Could not open file: " + filename);
    }

    MPI_File_set_view( fh, disp, MPI_DOUBLE, subArrayDataType, "native" , MPI_INFO_NULL );

}

void mpi_io::read( distributedCartesianArray & data) 
{
    int ret;


    if (isCollective)
        ret=MPI_File_read_at_all(fh, offset, data.getData().data(), data.getLocalSize(), MPI_DOUBLE, MPI_STATUS_IGNORE);   
    else 
        ret=MPI_File_read_at(fh, offset, data.getData().data(), data.getLocalSize(), MPI_DOUBLE, MPI_STATUS_IGNORE);

    if (ret!=MPI_SUCCESS)
    {
        throw std::runtime_error("MPI file read did not succeed." );
    }

    offset+= sizeof(real_t) * data.getGlobalSize();

}




void mpi_io::close()
{

    MPI_File_close(&fh );
    finalizeFileDataType(subArrayDataType);
}

void mpi_io::write( distributedCartesianArray & data) 
{

    int ret;
    if (isCollective)
    {
        ret=MPI_File_write_at_all(fh,offset, data.getData().data(), data.getLocalSize(), MPI_DOUBLE, MPI_STATUS_IGNORE);
    }
    else 
    {
        ret= MPI_File_write_at(fh, offset, data.getData().data(), data.getLocalSize(), MPI_DOUBLE, MPI_STATUS_IGNORE);
    }
    
    if (ret!=MPI_SUCCESS)
    {
        throw std::runtime_error("MPI file write did not succeed." );
    }

   offset+= sizeof(real_t) * data.getGlobalSize();
}

void mpi_io::sync()
{
    auto ret = MPI_File_sync(fh);
    if (ret!=MPI_SUCCESS)
    {
        char msg[MPI_MAX_ERROR_STRING];
        int resultlen;
        MPI_Error_string(ret, msg, &resultlen);
        throw std::runtime_error("Could not sync file with MPI: " + std::string(msg));
    };
    
}