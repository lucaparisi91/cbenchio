#include <string>
#include "data.h"
#include "ctl_io.h"


class mpi_io : public ctl_io
{
    public:
    

    void write( distributedCartesianArray & data) const;

    void read( distributedCartesianArray & data) const;
    
    void open(std::string filename, distributedCartesianArray & data, benchio::openMode mode );

    void close();

    void setCollective() { isCollective=true;}
    void unSetCollective(){isCollective=false;}

    private:

    std::string basename;
    void initFileDataType( distributedCartesianArray & data, MPI_Datatype & fileDataType ) const;
    void finalizeFileDataType( MPI_Datatype & fileDataType ) const;

    MPI_File fh;    
    MPI_Datatype subArrayDataType;
    bool isCollective=true;

};