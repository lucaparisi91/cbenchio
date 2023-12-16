#include <string>
#include "data.h"
class mpi_io
{
    public:

    mpi_io(std::string basename_) : basename(basename_){};


    void write( distributedCartesianArray & data) const;

    void read( distributedCartesianArray & data) const;
    
    private:

    std::string basename;
    void initFileDataType( distributedCartesianArray & data, MPI_Datatype & fileDataType ) const;
    void finalizeFileDataType( MPI_Datatype & fileDataType ) const;


};