#ifndef BENCHIO_ARRAY_H
#define BENCHIO_ARRAY_H

#include <array>
#include "definitions.h"
#include <cstdlib>


class benchioArray
{
    public:
    using index_t= size_t;

    benchioArray() : _shape{0,0,0},_size{0} {};
    benchioArray( index_t i, index_t j, index_t k,size_t alignment=0);
    
    benchioArray(std::array<index_t,3> shape,size_t alignment=0) : benchioArray(shape[0],shape[1],shape[2],alignment) {}
    
    const auto & getShape() const { return _shape;}

    inline real_t &  operator()(index_t i,index_t j,index_t k)
    {
        return _data[ i + j*_shape[0] + k*_shape[1]*_shape[0]  ];
    };
    
    inline real_t  operator()(index_t i,index_t j,index_t k) const
    {
        return _data[ i + j*_shape[0] + k*_shape[1]*_shape[0]  ];
    };


    ~benchioArray() { delete _data;}

    auto data(){return _data;}
    const auto data() const {return _data;}


    private:

    real_t * _data;
    std::array<index_t,3> _shape;
    index_t _size;

};

    real_t sum( const benchioArray & data2);

    real_t distance( const benchioArray & data1, const benchioArray & data2);
    


#endif