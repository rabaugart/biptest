
project( ssc_shm )

add_library( ssc_shm STATIC NdmcData.cpp DepthData.cpp )

add_executable( ndmcsim NdmcSim.cpp )
target_link_libraries( ndmcsim ssc_shm lrashm )

add_executable( watcher watcher.cpp )
target_link_libraries( watcher ssc_shm lrashm boost_program_options boost_log boost_thread boost_chrono boost_system )
