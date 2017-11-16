
project( test-boost )

# Bauen Windows
# ..\cmake-3.9.4-win64-x64\bin\cmake -G Ninja -DCMAKE_MAKE_PROGRAM=c:/Users/de1-rbaumgarten/Desktop/tmp/ninjatest/ninja.exe ..\biptest-master\test-boost
# ..\cmake-3.9.4-win64-x64\bin\cmake -G  "Visual Studio 12 2013 Win64" -DCMAKE_MAKE_PROGRAM=c:/Users/de1-rbaumgarten/Desktop/tmp/ninjatest/ninja.exe ..\biptest-master\test-boost

# Office-Umgebung hat 2.8
cmake_minimum_required(VERSION 3.2)

set( CMAKE_CXX_STANDARD 14 )
add_compile_options( -std=c++0x )
set( CXX11_SUPPORT TRUE )

set( BOOST_ROOT "$ENV{HOME}/dev/devlibs/boost_1_64_0" )
set( BOOST_ROOT "$ENV{HOME}/Sub_SW_S54S51/trunk/ext/boost_1_55_0" )
set( BOOST_ROOT "D:/Raba/lib/boost_1_55_0" )

set( Boost_USE_STATIC_LIBS ON )

find_package(Boost COMPONENTS program_options system filesystem thread chrono )

message( STATUS "Boost ${Boost_PROGRAM_OPTIONS_LIBRARY}" )

include_directories( "${Boost_INCLUDE_DIR}" )

link_directories( "${Boost_LIBRARY_DIR}" )

add_library( boost_program_options STATIC IMPORTED )
set_target_properties( boost_program_options PROPERTIES IMPORTED_LOCATION ${Boost_PROGRAM_OPTIONS_LIBRARY} )

add_library( boost_system STATIC IMPORTED )
set_target_properties( boost_system PROPERTIES IMPORTED_LOCATION ${Boost_SYSTEM_LIBRARY} )

add_library( boost_filesystem STATIC IMPORTED )
set_target_properties( boost_filesystem PROPERTIES IMPORTED_LOCATION ${Boost_FILESYSTEM_LIBRARY} )

add_library( boost_thread STATIC IMPORTED )
set_target_properties( boost_thread PROPERTIES IMPORTED_LOCATION ${Boost_THREAD_LIBRARY} )

add_library( boost_chrono STATIC IMPORTED )
set_target_properties( boost_chrono PROPERTIES IMPORTED_LOCATION ${Boost_CHRONO_LIBRARY} )

add_subdirectory( tprocess )
add_subdirectory( shmem )

#add_custom_target( one tproc --svn ${CMAKE_SOURCE_DIR} )
add_custom_target( one tcshmem )
