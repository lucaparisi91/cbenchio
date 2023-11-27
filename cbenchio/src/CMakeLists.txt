



add_library( benchio++ )
target_sources( benchio++ PRIVATE posix_io.cpp dataGenerator.cpp timers.cpp  )
configure_target(benchio++)


install( TARGETS benchio++ )