#ifndef TOOLS_TEST_H
#define TOOLS_TEST_H

#include <iostream>
#include <sstream>
#include "../src/definitions.h"
#include <array>
#include "../src/data.h"

namespace testing
{

    void check( bool condition, const std::string & msg)
    {
        if (!condition)
        {   
            
            std::cerr << "Check Failed : " ;
            std::cerr << msg << std::endl;

            MPI_Abort(MPI_COMM_WORLD,1);
        }
        else 
        {
            if (msg != "")
            {
                std::cerr << "Check Succeeded : " ;
                std::cerr << msg << std::endl;
            }
            
        }
    };

    template<class T1,class T2>
    void check_near( T1 a, T2 b   , std::string usrMsg = "",real_t tol=1e-7)
    {
        auto cond = std::abs( (double)(a -b) ) < tol;
        std::stringstream ss;
        ss << usrMsg;
        if (!cond) ss << a << " differ from " << b << std::endl; 
        check( std::abs( (double)(a -b) ) < tol ,ss.str() );
    }

    

    template<class T>
    void check_near( std::array<T,3> a, std::array<T,3> b   , std::string usrMsg = "",real_t tol=1e-7)
    {
        

        for(int d=0;d<3;d++)
        {
            std::stringstream ss;
            ss << usrMsg << " d=" << d;
            check_near( a[d],b[d],ss.str(),tol);
        }
    }

    template<class T>
    void log( std::array<T,3> arr, const std::string & msg="" )
    {
        int rank=-1, nRanks=-1;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &nRanks);
        std::cout << msg <<  " Rank "<<rank<< ": " <<arr[0] << " " << arr[1] << " " << arr[2] << std::endl;
    }


    void check_near(const distributedCartesianArray & data,const distributedCartesianArray & data2,const std::string & msg )
    {
        int rank;
        real_t diff_local = distance( data.getData(),data2.getData() );  
        real_t diff_global;
        MPI_Reduce( &diff_local, &diff_global , 1 , MPI_DOUBLE, MPI_SUM, 0 , MPI_COMM_WORLD );
        
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        if (rank ==0)
        {
            check_near( diff_global,0 , msg) ;
        }

    }
    

}



#endif