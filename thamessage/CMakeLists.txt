project(CMTest)

cmake_minimum_required(VERSION 3.10)

include(CTest)

#find_package( Boost COMPONENTS lexical_cast )
 
add_compile_options( -std=c++14 -Wall )

add_executable( a a.cpp message.cpp )
#target_link_libraries( a Boost::lexical_cast )

add_custom_target( run_a a )

add_test( testa a )