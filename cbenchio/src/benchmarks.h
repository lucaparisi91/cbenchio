#include "data.h"
#include "posix_io.h"
#include "timers.h"
#include <memory>

class benchmark
{
    public:
    using data_t = distributedCartesianArray;
    using ioCtl_t = posix_io;

    benchmark(  std::string name_) : name(name_),sizeTransferred(0) {};

    

    void write( data_t & data, ioCtl_t & ctl );


    void report();


    private:

    std::shared_ptr<ioCtl_t> ioCtrl;
    timer benchmarkTimer;
    std::string name;
    real_t sizeTransferred;

};