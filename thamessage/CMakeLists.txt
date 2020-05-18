project(CMTest)

cmake_minimum_required(VERSION 2.8)

add_compile_options( -std=c++11 -Wall )

add_executable( a a.cpp message.cpp )

add_custom_target( run_a a )

