#ifndef TIMER_H
#define TIMER_H

#include "mpi.h"
#include "definitions.h"


class timer
{
    public:

    timer()
    {

    }


    public:
        void reset() { tStart=0; tEnd=0; tElapsed=0;tMaxElapsed=0;}
        void start() { tStart= MPI_Wtime(); }
        void end() {tEnd = MPI_Wtime(); tElapsed += tEnd - tStart;  }
        auto const elapsed() { return tElapsed; }

        auto const maxElapsed() 
        {          
            return tMaxElapsed;

        }

        auto computeMaxElapsed()
        {
            // only the master will get the maxed out value
            MPI_Reduce( &tElapsed, &tMaxElapsed, 1, MPI_DOUBLE, MPI_MAX, 0 , MPI_COMM_WORLD);

        }

    private:
    real_t tStart=0; 
    real_t tEnd=0;
    real_t tElapsed=0;

    real_t tMaxElapsed;

   
};

#endif