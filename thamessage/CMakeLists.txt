project(CMTest)

cmake_minimum_required(VERSION 3.6)

include(CTest)

add_compile_options( -std=c++11 -Wall )

add_executable( a a.cpp message.cpp )

add_custom_target( run_a a )

add_test( testa a )