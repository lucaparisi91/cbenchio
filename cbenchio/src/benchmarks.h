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

    benchmark(  std::string name_) : name(name_),sizeTransferred(0), sync(false),operation("write") {};

    void write( data_t & data, ioCtl_t & ctl );    


    YAML::Node report_yaml();

    void setFileSync( bool _sync) { sync = _sync; }
    void setOperation(std::string op) { operation=op;   }
    void setNFields(size_t n ){nFields=n;} ;

    private:

    std::shared_ptr<ioCtl_t> ioCtrl;
    timer benchmarkTimer;
    std::string name;
    real_t sizeTransferred;
    bool sync;
    std::string operation;
    int nFields=1;

};