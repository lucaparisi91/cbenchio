


add_executable(test_posix)
target_sources( test_posix PRIVATE test_posix_io.cpp  )
target_link_libraries( test_posix PUBLIC benchio++ )
configure_target( test_posix )
add_test( test_posix test_posix )

add_executable(test_mpiio)
target_sources( test_mpiio PRIVATE test_mpiio.cpp  )
target_link_libraries( test_mpiio PUBLIC benchio++ )
configure_target( test_mpiio )
add_test( test_mpiio test_mpiio )