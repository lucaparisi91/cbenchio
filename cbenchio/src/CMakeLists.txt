

function ( configure_target target )

target_link_libraries( ${target} PUBLIC MPI::MPI_CXX )
set_property ( TARGET ${target} PROPERTY CXX_STANDARD 17 )

endfunction()

add_library( benchio++ )
target_sources( benchio++ PRIVATE posix_io.cpp dataGenerator.cpp timers.cpp  )
configure_target(benchio++)

add_executable(test_posix)
target_sources( test_posix PRIVATE test_posix_io.cpp  )
target_link_libraries( test_posix PUBLIC benchio++ )
configure_target( test_posix )



install( TARGETS benchio++ )