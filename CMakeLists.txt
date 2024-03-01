cmake_minimum_required(VERSION 3.20)

project( Biptest )

include(CTest)
include(CPack)

enable_testing()

set( CMAKE_CXX_STANDARD 20 )

#add_compile_options( -Wall )

find_package( Boost COMPONENTS date_time filesystem system unit_test_framework )

set( CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install" )

message( "Installing to ${CMAKE_INSTALL_PREFIX}" )

add_subdirectory( cpp20 )
#add_subdirectory( comp )
#add_subdirectory( test-boost )
add_subdirectory( test-other )
#add_subdirectory( thamessage )
