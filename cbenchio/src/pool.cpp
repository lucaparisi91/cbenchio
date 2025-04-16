#include "pool.h"

pool::pool( int nproc, MPI_Comm comm,std::string basename)
{
    int nranks,rank;
    MPI_Comm_size(comm, &nranks);
    MPI_Comm_rank(comm, &rank);
    
    int colour = rank/nproc;
    MPI_Comm_split(comm,colour,rank,&pool_communicator);
    name=basename + "_" + std::to_string(colour);

}