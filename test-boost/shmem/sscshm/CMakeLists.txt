
project( ssc_shm )

add_library( ssc_shm STATIC NdmcData.cpp DepthData.cpp NdmcDataFieldAdapterFactory.cpp DepthDataFieldAdapterFactory.cpp
    SscFieldAdapterFactory.cpp )

add_executable( ndmcsim NdmcSim.cpp )
target_link_libraries( ndmcsim ssc_shm lrashm boost_date_time boost_serialization )

add_executable( watcher watcher.cpp )
target_link_libraries( watcher ssc_shm lrashm boost_program_options boost_date_time boost_log boost_log_setup boost_serialization boost_filesystem boost_thread boost_chrono boost_system )

add_executable( depth_proc depth_proc.cpp )
target_link_libraries( depth_proc ssc_shm lrashm boost_program_options boost_serialization boost_date_time boost_log boost_log_setup boost_filesystem boost_thread boost_chrono boost_system )

add_executable( shmonitor shmonitor.cpp )
target_link_libraries( shmonitor ssc_shm lrashm boost_program_options boost_serialization boost_date_time boost_log boost_log_setup boost_filesystem boost_thread boost_chrono boost_system )

add_executable( nettrans nettrans.cpp )
target_link_libraries( nettrans ssc_shm lrashm boost_program_options boost_serialization boost_date_time boost_regex boost_log boost_log_setup boost_filesystem boost_thread boost_chrono boost_system )

install( TARGETS ndmcsim watcher depth_proc shmonitor nettrans DESTINATION bin )