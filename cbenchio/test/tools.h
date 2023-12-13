
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
    };

    template<class T1,class T2>
    void check_near( T1 a, T2 b   , std::string usrMsg = "",real_t tol=1e-7)
    {
        auto diff = std::abs( a -b );
        std::stringstream ss;
        ss << usrMsg << std::endl;
        ss << a << " differ from " << b << std::endl; 
        check( std::abs( a -b ) < tol ,ss.str() );
    }

    template<class T>
    void check_near( std::array<T,3> a, std::array<T,3> b   , std::string usrMsg = "",real_t tol=1e-7)
    {
        for(int d=0;d<3;d++)
        {
            check_near( a[d],b[d],usrMsg,tol);
        }
    }

    template<class T>
    void log( std::array<T,3> arr, const std::string & msg )
    {
        int rank=-1, nRanks=-1;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &nRanks);
        std::cout << msg <<  " Rank "<<rank<< ": " <<arr[0] << " " << arr[1] << " " << arr[2] << std::endl;
    }

}