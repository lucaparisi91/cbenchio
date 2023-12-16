#include "mpi_io.h"


void mpi_io::initFileDataType( distributedCartesianArray & data, MPI_Datatype & subArrayDataType ) const
{
    MPI_Type_create_subarray( 3, data.getGlobalShape().begin(), data.getLocalShape().begin() , data.getLocalOffset().begin(), MPI_ORDER_FORTRAN ,MPI_DOUBLE, &subArrayDataType );
    MPI_Type_commit( &subArrayDataType);
}

void mpi_io::finalizeFileDataType(  MPI_Datatype & subArrayDataType ) const
{
    
    MPI_Type_free( &subArrayDataType);

}


void mpi_io::read( distributedCartesianArray & data) const
{
    MPI_Datatype subArrayDataType;
    MPI_File fh;
    MPI_Offset disp=0;

    initFileDataType(data,subArrayDataType);
    
    MPI_File_open( MPI_COMM_WORLD, basename.c_str(), MPI_MODE_RDONLY, MPI_INFO_NULL, &fh );
    MPI_File_set_view( fh, disp, MPI_DOUBLE, subArrayDataType, "native" , MPI_INFO_NULL );

    MPI_File_read(fh, data.getData().data(), data.getLocalSize(), MPI_DOUBLE, MPI_STATUS_IGNORE);
    MPI_File_close(&fh );

    finalizeFileDataType(subArrayDataType);

}

void mpi_io::write( distributedCartesianArray & data) const
{
    MPI_Datatype subArrayDataType;
    MPI_File fh;
    MPI_Offset disp=0;

    initFileDataType(data,subArrayDataType);
    
    MPI_File_open( MPI_COMM_WORLD, basename.c_str(),MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh );
    MPI_File_set_view( fh, disp, MPI_DOUBLE, subArrayDataType, "native" , MPI_INFO_NULL );

    MPI_File_write(fh, data.getData().data(), data.getLocalSize(), MPI_DOUBLE, MPI_STATUS_IGNORE);
    
    MPI_File_close(&fh );
    finalizeFileDataType(subArrayDataType);

}