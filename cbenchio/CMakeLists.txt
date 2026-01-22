cmake_minimum_required( VERSION 3.2 )

project( hello_world LANGUAGES CXX C )
cmake_policy(SET CMP0074 NEW)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmakeModules")

find_package( MPI REQUIRED )

find_package( HDF5 REQUIRED )
find_package( NetCDF REQUIRED )

include( tools.cmake)

add_subdirectory( src )
add_subdirectory(eigen)
add_subdirectory(yaml-cpp)

enable_testing()
add_subdirectory( test )