cmake_minimum_required( VERSION 3.2 )

project( hello_world LANGUAGES CXX )


find_package( MPI REQUIRED )

include( tools.cmake)

add_subdirectory( src )

enable_testing()
add_subdirectory( test )
