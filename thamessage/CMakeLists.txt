project(CMTest)

cmake_minimum_required(VERSION 3.10)

set( CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install" )

message( "Installing to ${CMAKE_INSTALL_PREFIX}" )

include(CTest)
include(CPack)

find_package( Boost COMPONENTS date_time filesystem system unit_test_framework )
 
add_compile_options( -std=c++17 -Wall )

add_executable( a a.cpp message.cpp )
target_link_libraries( a Boost::date_time Boost::filesystem Boost::system Boost::unit_test_framework )
install( TARGETS a DESTINATION bin )

add_custom_target( run_a a --log_level=message )

add_test( testa a --log_level=error )