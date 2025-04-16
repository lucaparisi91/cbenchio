

add_library( benchio++ )
target_sources( benchio++ PRIVATE posix_io.cpp dataGenerator.cpp timers.cpp data.cpp benchmarks.cpp mpi_io.cpp hdf5_io.cpp netcdf_io.cpp std_io.cpp benchioArray.cpp pool.cpp )
configure_target(benchio++)

add_executable(benchio)
target_sources( benchio PRIVATE main.cpp)
target_link_libraries( benchio PRIVATE benchio++)
configure_target(benchio)

install( TARGETS benchio++ )
install( TARGETS benchio )


