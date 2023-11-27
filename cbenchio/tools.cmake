
function ( configure_target target )

target_link_libraries( ${target} PUBLIC MPI::MPI_CXX )
set_property ( TARGET ${target} PROPERTY CXX_STANDARD 17 )

endfunction()

