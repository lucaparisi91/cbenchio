#include "data.h"
#include "posix_io.h"
#include "timers.h"
#include <memory>
#include "yaml-cpp/yaml.h"

class benchmark
{
    public:
    using data_t = distributedCartesianArray;
    using ioCtl_t = ctl_io;

    benchmark(  std::string name_) : name(name_),sizeTransferred(0) {};

    

    void write( data_t & data, ioCtl_t & ctl );


    std::string report();

    YAML::Node report_yaml();

    private:

    std::shared_ptr<ioCtl_t> ioCtrl;
    timer benchmarkTimer;
    std::string name;
    real_t sizeTransferred;

};