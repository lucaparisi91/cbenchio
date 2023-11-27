


add_executable(test_posix)
target_sources( test_posix PRIVATE test_posix_io.cpp  )
target_link_libraries( test_posix PUBLIC benchio++ )
configure_target( test_posix )
add_test( test_posix test_posix )

