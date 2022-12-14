
project( CPP_20 )
cmake_minimum_required(VERSION 3.10)

set( CMAKE_CXX_STANDARD 17 )

include_directories( "$ENV{HOME}/dev/local/include" )

add_executable( a a.cpp )
add_custom_target( run a )

add_executable( tuple tuple.cpp )
add_custom_target( runt tuple )
