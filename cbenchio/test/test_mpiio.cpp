
#include <mpi.h>
#include "../src/dataGenerator.h"
#include <array>
#include <iostream>
#include <unsupported/Eigen/CXX11/Tensor>

struct grid_shape
{
    std::array<int,3 > shape{1,1,1} ;
    std::array<int, 3> offset{0,0,0};
    int size {0};
};


auto getLocalGridShape( const std::array<int,3> & global_dims, const MPI_Comm & comm )
{
    std::array<int,3> rank,period,nRanks;
    grid_shape local_shape;

    MPI_Cart_get( comm, 3 , nRanks.begin() , period.begin() , rank.begin() );

    for (int d=0;d<3;d++)
    {
        if ( nRanks[d] != 0 )
        {

            local_shape.shape[d]= global_dims[d]/nRanks[d];
            local_shape.offset[d]=local_shape.shape[d]*rank[d];

            auto remainder = global_dims[d] % nRanks[d];

            if (rank[d] < remainder)
            {
                local_shape.shape[d]+=1;
                local_shape.offset[d]+=rank[d];    
            }
            else 
            {
                local_shape.offset[d]+=remainder;
            }

        }

    }

    local_shape.size=local_shape.shape[0] * local_shape.shape[1] * local_shape.shape[2] ;

    return local_shape;

}


int main(int argc, char ** argv)
{
    const std::array<int,3> globalSize { 100,100, 1};

    constexpr int ndims = 3 ;
    MPI_Comm cartesian_comm;

    std::array<int,3 > dims{0,0,1};

    int rank=-1, nRanks=-1;

    MPI_Init( &argc , & argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    MPI_Dims_create( nRanks, ndims, dims.data() );

    std::cout << dims[0] << " " << dims[1] << " "<< dims[2] << std::endl;
    

    std::array<int , 3> periodic {0,0,0};

    MPI_Cart_create( MPI_COMM_WORLD,
                    3,
                    dims.begin(),
                    periodic.begin(),
                    1,
                    &cartesian_comm);
    
    auto local_shape = getLocalGridShape( globalSize , cartesian_comm);

    std::cout << "Shape: " << rank << " " << local_shape.shape[0] << " " << local_shape.shape[1] << " " << local_shape.shape[2] << std::endl;
    std::cout << "Offset: " << rank << " " << local_shape.offset[0] << " " << local_shape.offset[1] << " " << local_shape.offset[2] << std::endl;
     
    Eigen::Tensor<real_t, 3 > data_local( local_shape.shape[0], local_shape.shape[1],local_shape.shape[2]);
    Eigen::Tensor<real_t, 3> data_local2(data_local.dimensions());

    for(int i=0;i<local_shape.shape[0];i++)
        for(int j=0;j<local_shape.shape[1];j++)
            for(int k=0;k<local_shape.shape[2];k++)
            {
                auto ig = i + local_shape.offset[0];
                auto jg = j + local_shape.offset[1];
                auto kg = k + local_shape.offset[2];

                data_local(i,j,k)= std::sqrt(ig*ig + jg*jg + kg*kg);
            }
    
    MPI_Datatype subArrayDataType ;
    MPI_Type_create_subarray( 3, globalSize.begin(), local_shape.shape.begin() , local_shape.offset.begin(), MPI_ORDER_FORTRAN ,MPI_DOUBLE, &subArrayDataType );
    MPI_Type_commit( &subArrayDataType);


    MPI_File fh;
    MPI_File_open( MPI_COMM_WORLD, "mpiio.dat",MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh );
    MPI_Offset disp=0;

    MPI_File_set_view( fh, disp, MPI_DOUBLE, subArrayDataType, "native" , MPI_INFO_NULL );

    MPI_File_write(fh, data_local.data(), local_shape.size, MPI_DOUBLE, MPI_STATUS_IGNORE);
    MPI_File_close(&fh );


    data_local2.setZero();

    MPI_File_open( MPI_COMM_WORLD, "mpiio.dat", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh );
    MPI_File_set_view( fh, disp, MPI_DOUBLE, subArrayDataType, "native" , MPI_INFO_NULL );

    MPI_File_read(fh, data_local2.data(), local_shape.size, MPI_DOUBLE, MPI_STATUS_IGNORE);

    std::cout << "diff: " << (data_local2 - data_local).abs().sum() << std::endl;
    MPI_File_close(&fh );






    MPI_Type_free( &subArrayDataType);






    MPI_Finalize();

}