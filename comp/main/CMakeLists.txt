
add_executable( main main.cpp )
target_link_libraries( main lcomp Boost::filesystem Boost::log -lboost_log_setup dl )

add_custom_target( run_main main DEPENDS compa )

