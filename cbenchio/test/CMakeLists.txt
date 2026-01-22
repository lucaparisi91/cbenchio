
function ( add_test_executable name_test )

    add_executable( ${name_test})
    target_sources( ${name_test} PRIVATE ${name_test}.cpp  )
    target_link_libraries( ${name_test} PUBLIC benchio++ )
    configure_target( ${name_test} )

endfunction()

function ( add_parallel_test name_exec nRanks )
    add_test( ${name_exec}_n${nRanks} srun --cpus-per-task=1 --ntasks=${nRanks} ${name_exec} )
endfunction()

# add_executable(test_posix)
# target_sources( test_posix PRIVATE test_posix_io.cpp  )
# target_link_libraries( test_posix PUBLIC benchio++ )
# configure_target( test_posix )
# add_test( test_posix test_posix )

# add_executable(test_mpiio)
# target_sources( test_mpiio PRIVATE test_mpiio.cpp  )
# target_link_libraries( test_mpiio PUBLIC benchio++ )
# configure_target( test_mpiio )
# add_test( test_mpiio test_mpiio )

add_test_executable( test_distributedCartesianArray )
add_parallel_test( test_distributedCartesianArray 1)
add_parallel_test( test_distributedCartesianArray 2)

add_test_executable( test_posix_io )
add_parallel_test( test_posix_io 1)
add_parallel_test( test_posix_io 13)
add_parallel_test( test_posix_io 2)

add_test_executable( test_mpiio)
add_parallel_test( test_mpiio 1)
add_parallel_test( test_mpiio 2)
add_parallel_test( test_mpiio 4)

add_test_executable( test_hdf5)
add_parallel_test( test_hdf5 1)
add_parallel_test( test_hdf5 2)
add_parallel_test( test_hdf5 4)

add_test_executable( test_netcdf)
add_parallel_test( test_netcdf 1)
add_parallel_test( test_netcdf 2)
add_parallel_test( test_netcdf 4)

add_test_executable( test_std_io )
add_parallel_test( test_std_io 1)

add_test_executable( test_mpiio_8x8 )
add_parallel_test( test_mpiio_8x8 64)

add_test_executable( test_benchio_array )
add_parallel_test( test_benchio_array 1)


add_test_executable( test_benchio_array_aligned )
add_parallel_test( test_benchio_array_aligned 1)

add_test_executable( test_pool )
add_parallel_test( test_pool 5)