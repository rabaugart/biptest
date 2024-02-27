project(CMTest)

add_executable( tha a.cpp message.cpp )
target_link_libraries( tha Boost::date_time Boost::filesystem Boost::system Boost::unit_test_framework )

install( TARGETS tha DESTINATION bin )

add_custom_target( run_tha tha --log_level=message )

add_test( testa tha --log_level=error )
