
function ( configure_target target )


target_link_libraries( ${target} PUBLIC MPI::MPI_CXX )
target_link_libraries( ${target} PUBLIC Eigen3::Eigen )
target_link_libraries( ${target} PUBLIC yaml-cpp::yaml-cpp )
target_link_libraries( ${target} PUBLIC lustreapi )
target_link_libraries( ${target} PUBLIC hdf5::hdf5 )
target_link_libraries( ${target} PUBLIC NetCDF::NetCDF )

set_property ( TARGET ${target} PROPERTY CXX_STANDARD 17 )

endfunction()

