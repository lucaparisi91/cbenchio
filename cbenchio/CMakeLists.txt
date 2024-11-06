cmake_minimum_required( VERSION 3.2 )

project( hello_world LANGUAGES CXX C )
cmake_policy(SET CMP0074 NEW)


find_package( MPI REQUIRED )

find_package( HDF5 REQUIRED )

include( tools.cmake)

add_subdirectory( src )
add_subdirectory(eigen)
add_subdirectory(yaml-cpp)

enable_testing()
add_subdirectory( test )



