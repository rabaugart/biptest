
project( CPP_20 )
cmake_minimum_required(VERSION 3.10)

include_directories( "$ENV{HOME}/dev/local/include" )

add_executable( a a.cpp )
add_custom_target( run a DEPENDS a )

add_executable( tuple tuple.cpp )
add_custom_target( runt tuple )

add_test( test_tuple tuple )

