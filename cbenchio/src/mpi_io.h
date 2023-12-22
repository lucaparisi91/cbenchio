#include <string>
#include "data.h"
class mpi_io
{
    public:


    void write( distributedCartesianArray & data) const;

    void read( distributedCartesianArray & data) const;
    
    void open(std::string filename, distributedCartesianArray & data, benchio::openMode mode );

    void close();

    private:

    std::string basename;
    void initFileDataType( distributedCartesianArray & data, MPI_Datatype & fileDataType ) const;
    void finalizeFileDataType( MPI_Datatype & fileDataType ) const;

    MPI_File fh;    
    MPI_Datatype subArrayDataType;


};