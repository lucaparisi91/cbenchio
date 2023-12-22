#include "hdf5_io.h"
#include "hdf5.h"
#include <stdexcept>

auto to_hsize(std::array<int,3> dims)
{
    return std::array<hsize_t,3> { (hsize_t)dims[0] , (hsize_t)dims[1], (hsize_t) dims[2] };
}

void hdf5_io::close( )
{
    H5Sclose(fileSpaceId);
    H5Dclose(dsetId);
    H5Sclose(memSpaceId);
    H5Fclose(fileId);
    H5Pclose(pListId);

}

void hdf5_io::open( std::string filename,  distributedCartesianArray & data, benchio::openMode mode)
{
    auto hOpenMode = H5F_ACC_TRUNC;
    
    if (mode == benchio::writeMode)
    {
        hOpenMode = H5F_ACC_EXCL;
    }
    else if (mode == benchio::readMode)
    {
        hOpenMode=H5F_ACC_EXCL;
    }
    else 
    {
        throw std::invalid_argument("Unrecognized open mode");
    }
    auto pAccessId = H5Pcreate(H5P_FILE_ACCESS);

    H5Pset_fapl_mpio(pAccessId, MPI_COMM_WORLD,  MPI_INFO_NULL);

    fileId = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, pAccessId);

    const auto & localShape = data.getLocalShape();

    auto dims = to_hsize(data.getGlobalShape());
    std::reverse( dims.begin(),dims.end() );

    fileSpaceId=H5Screate_simple(3, dims.begin(), NULL);

    dsetId = H5Dcreate(fileId, "data", H5T_NATIVE_DOUBLE, fileSpaceId, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

   
    auto offset = to_hsize(data.getLocalOffset());
    auto counts = to_hsize(data.getLocalShape());

    

    auto memShape=data.getLocalShape();
    std::reverse(memShape.begin(), memShape.end() );
    auto memHShape = to_hsize(memShape);

    

    memSpaceId  = H5Screate_simple(3, memHShape.begin() , NULL);

    std::reverse(offset.begin(),offset.end() );
    std::reverse(counts.begin(),counts.end() );
    


    auto localDataSelection=H5Sselect_hyperslab(fileSpaceId, H5S_SELECT_SET, offset.begin() , NULL, counts.begin(), NULL);

    pListId = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(pListId, H5FD_MPIO_COLLECTIVE);

}

void hdf5_io::write( distributedCartesianArray & data) const
{   
    auto status = H5Dwrite(dsetId, H5T_NATIVE_DOUBLE, memSpaceId, fileSpaceId, pListId, data.getData().data() );

}

void hdf5_io::read( distributedCartesianArray & data) const
{   
    auto status = H5Dread(dsetId, H5T_NATIVE_DOUBLE, memSpaceId, fileSpaceId, pListId, data.getData().data() );

}