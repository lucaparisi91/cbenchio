#include <iostream>
#include <vector>
#include <mpi.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <stdexcept>
#include "std_io.h"

#include <iostream>

std::string getStdioMode( benchio::openMode mode)
{
    if ( mode == benchio::writeMode)
    {
        return "w" ;
    }
    else if (mode == benchio::readMode )
    {
        return "r";
    }
    else 
    {
        throw std::invalid_argument("mode not supported by STDIO");
    }
}

void std_io::open(std::string filename, distributedCartesianArray & data, benchio::openMode mode )
{
    auto stdioMode = getStdioMode(mode);

    
    fp=fopen( filename.c_str() , stdioMode.c_str() );

    if ( data.getLocalOffset()[0] != 0  )
    {
        throw std::invalid_argument( "io does not support writing to a shared file" );
    }


};

void std_io::write( distributedCartesianArray & data) 
{  
    
    auto  offset= (char * )data.getData().data();
    

    fwrite( offset , sizeof(real_t),  data.getLocalSize() , fp );

}

void std_io::close()
{
    fclose(fp);

}


void std_io::read( distributedCartesianArray & data) 
{
    auto  offset= (char * )data.getData().data();
    

    fread( offset , sizeof(real_t),  data.getLocalSize() , fp );

}






