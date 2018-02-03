project( gmonitor )

add_executable( gmonitor gmonitor.cpp )
target_link_libraries( gmonitor  ssc_shm lrashm boost_program_options 
    boost_serialization boost_date_time boost_log boost_log_setup 
    boost_filesystem boost_thread boost_chrono boost_system Qt5::Widgets )