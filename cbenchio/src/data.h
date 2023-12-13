#include <vector>
#include <mpi.h>
#include "definitions.h"
#include <array>
#include <unsupported/Eigen/CXX11/Tensor>


// struct grid_shape
// {
//     std::array<int,3 > shape{1,1,1} ;
//     std::array<int, 3> offset{0,0,0};
//     int size {0};
// };


// struct distributedCartesianArray
// {


//     grid_shape localShape;
//     grid_shape globalShape;

//     Eigen::Tensor<real_t,3> local_data;

// };


class distributedVector
{
    public:

    distributedVector( MPI_Comm comm,size_t globalSize );

    const auto & getLocalOffset() const  { return localOffset;}
    const auto & getLocalSize()  const { return localSize;}
    const auto & getGlobalSize() const { return globalSize;}

    const auto & getData() const { return localData;}
    auto & getData() { return localData;}
        
    private:

    size_t localOffset;
    size_t localSize;
    size_t globalSize;

    std::vector<real_t> localData;
    MPI_Comm comm;
    
};




class distributedCartesianArray
{
    public:

    using index_t = int;
    static constexpr int max_dims = 3;

    distributedCartesianArray( MPI_Comm comm,std::array<index_t,3> globalShape_  );

    const auto & getLocalShape() const { return localShape;}
    const auto & getGlobalShape() const { return globalShape;}
    const auto & getLocalOffset() const { return localOffset;}
    const auto & getLocalSize() const { return localSize;}

    
    const auto & getData() const { return localData; }
    auto & getData() { return localData; }

    auto getCartesianCommunicator() {return cartesian_comm;}

    private:

    
    Eigen::Tensor<real_t,3> localData;
    
    std::array<index_t,max_dims> localShape;
    std::array<index_t,max_dims> globalShape;
    
    std::array<index_t,max_dims> localOffset;
    MPI_Comm cartesian_comm;

    index_t localSize;



 };