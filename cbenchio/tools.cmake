
function ( configure_target target )

target_link_libraries( ${target} PUBLIC MPI::MPI_CXX )
target_link_libraries( ${target} PUBLIC Eigen3::Eigen )
target_link_libraries( ${target} PUBLIC yaml-cpp::yaml-cpp )
target_link_libraries( ${target} PUBLIC lustreapi )

set_property ( TARGET ${target} PROPERTY CXX_STANDARD 17 )


endfunction()

