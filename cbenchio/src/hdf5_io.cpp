#include "hdf5_io.h"
#include "hdf5.h"
#include <stdexcept>

template<class T>
auto to_hsize(std::array<T,3> dims)
{
    return std::array<hsize_t,3> { (hsize_t)dims[0] , (hsize_t)dims[1], (hsize_t) dims[2] };
}


void hdf5_io::close( )
{
    H5Sclose(fileSpaceId);
    H5Sclose(memSpaceId);
    H5Fclose(fileId);
    H5Pclose(pListTransfer);
    
}

void hdf5_io::open( std::string filename,  distributedCartesianArray & data, benchio::openMode mode)
{
    auto pAccessId = H5Pcreate(H5P_FILE_ACCESS);

    H5Pset_fapl_mpio(pAccessId, data.getCartesianCommunicator(),  MPI_INFO_NULL);
    if (mode == benchio::writeMode)
    {
        fileId = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC,  H5P_DEFAULT, pAccessId);
    }
    else if (mode == benchio::readMode)
    {
         fileId = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, pAccessId);
    }
    else 
    {
        throw std::invalid_argument("Unrecognized open mode");
    }
   
    
    H5Pclose(pAccessId);


    const auto & localShape = data.getLocalShape();

    auto dims = to_hsize(data.getGlobalShape());
    std::reverse( dims.begin(),dims.end() );

    fileSpaceId=H5Screate_simple(3, dims.begin(), NULL);
   
    
   
    auto offset = to_hsize(data.getLocalOffset());
    auto counts = to_hsize(data.getLocalShape());

    

    auto memShape=data.getLocalShape();
    std::reverse(memShape.begin(), memShape.end() );
    auto memHShape = to_hsize(memShape);

    memSpaceId  = H5Screate_simple(3, memHShape.begin() , NULL);

    std::reverse(offset.begin(),offset.end() );
    std::reverse(counts.begin(),counts.end() );


    auto localDataSelection=H5Sselect_hyperslab(fileSpaceId, H5S_SELECT_SET, offset.begin() , NULL, counts.begin(), NULL);

    pListTransfer = H5Pcreate(H5P_DATASET_XFER);
    if (isCollective) H5Pset_dxpl_mpio(pListTransfer, H5FD_MPIO_COLLECTIVE);
    
    
}

void hdf5_io::setChunking(std::vector<size_t> chunkDims_) { 

    chunkDimsCorder=chunkDims_; 
    std::reverse( chunkDimsCorder.begin(),chunkDimsCorder.end() ); 
};


void hdf5_io::write( distributedCartesianArray & data) 
{

    // The dataset creation property list is used to set chunking
    auto dpListId = H5Pcreate(H5P_DATASET_CREATE);
    if ( chunkDimsCorder.size() >0 )
    {
        assert (chunkDimsCorder.size() == data.getGlobalShape().size() ); // The number of dimensions in the chunk and the dataset must match

        auto status = H5Pset_chunk(dpListId, chunkDimsCorder.size(), chunkDimsCorder.data());
        
    }

    // Create the dataset
    auto dsetId = H5Dcreate(fileId, ("data" + std::to_string(currentField)).c_str(), H5T_NATIVE_DOUBLE, fileSpaceId, H5P_DEFAULT, dpListId, H5P_DEFAULT);
    H5Pclose(dpListId);

    // Write the data to the dataset
    auto status = H5Dwrite(dsetId, H5T_NATIVE_DOUBLE, memSpaceId, fileSpaceId, pListTransfer, data.getData().data() );
    H5Dclose(dsetId);
    currentField++;
}

void hdf5_io::read( distributedCartesianArray & data)
{
    auto dsetId = H5Dopen(fileId, ("data" + std::to_string(currentField)).c_str(), H5P_DEFAULT);
    auto status = H5Dread(dsetId, H5T_NATIVE_DOUBLE, memSpaceId, fileSpaceId, pListTransfer, data.getData().data() );
    H5Dclose(dsetId);
    currentField++;
}

void hdf5_io::sync()
{
    auto ret = H5Fflush (fileId, H5F_SCOPE_GLOBAL 	);
    if (ret!=0)
    {
        throw std::runtime_error("Could not sync file with HDF5.");
    };
}