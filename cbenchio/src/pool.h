#include "mpi.h"
#include <string>

class pool
{
    public:

    pool(int nproc , MPI_Comm parent_comm,std::string basename="pool");

    auto getName(){return name;};
    auto getCommunicator(){return pool_communicator;}

    private:

    std::string name;
    MPI_Comm pool_communicator;

};