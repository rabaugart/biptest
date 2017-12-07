
project( test-boost )

# Bauen Windows
# ..\cmake-3.9.4-win64-x64\bin\cmake -G Ninja -DCMAKE_MAKE_PROGRAM=c:/Users/de1-rbaumgarten/Desktop/tmp/ninjatest/ninja.exe ..\biptest-master\test-boost
# ..\cmake-3.9.4-win64-x64\bin\cmake -G  "Visual Studio 12 2013 Win64" -DCMAKE_MAKE_PROGRAM=c:/Users/de1-rbaumgarten/Desktop/tmp/ninjatest/ninja.exe ..\biptest-master\test-boost

# Office-Umgebung hat 2.8
cmake_minimum_required(VERSION 3.2)


if ( IS_DIRECTORY "$ENV{HOME}/dev/devlibs/boost_1_64_0" )
  message( "Mac oder neues Linux" )
  set( CMAKE_CXX_STANDARD 14 )
  set( BOOST_ROOT "$ENV{HOME}/dev/devlibs/boost_1_64_0" )
elseif ( IS_DIRECTORY "$ENV{HOME}/Sub_SW_S54S51/trunk/ext/boost_1_55_0" )
  message( "Altes Linux" )
  add_compile_options( -std=c++0x )
  set( BOOST_ROOT "$ENV{HOME}/Sub_SW_S54S51/trunk/ext/boost_1_55_0" )
elseif ( IS_DIRECTORY "C:/dev/libs/boost_1_65_1" )
  message( "Windows" )
  set( CXX11_SUPPORT TRUE )
  set( BOOST_ROOT "C:/dev/libs/boost_1_65_1" )
  add_definitions( -D_WIN32_WINNT=0x0602 )
else()
  message( FATAL_ERROR "BOOST_ROOT not found" )
endif()

set( Boost_USE_STATIC_LIBS ON )

find_package(Boost COMPONENTS program_options system filesystem thread chrono regex serialization date_time )

message( STATUS "Boost ${Boost_PROGRAM_OPTIONS_LIBRARY}" )

include_directories( "${Boost_INCLUDE_DIR}" )

link_directories( "${Boost_LIBRARY_DIR_RELEASE}" )

add_library( boost_program_options STATIC IMPORTED )
set_target_properties( boost_program_options PROPERTIES IMPORTED_LOCATION ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE} )

add_library( boost_system STATIC IMPORTED )
set_target_properties( boost_system PROPERTIES IMPORTED_LOCATION ${Boost_SYSTEM_LIBRARY_RELEASE} )

add_library( boost_filesystem STATIC IMPORTED )
set_target_properties( boost_filesystem PROPERTIES IMPORTED_LOCATION ${Boost_FILESYSTEM_LIBRARY_RELEASE} )

add_library( boost_thread STATIC IMPORTED )
set_target_properties( boost_thread PROPERTIES IMPORTED_LOCATION ${Boost_THREAD_LIBRARY_RELEASE} )

add_library( boost_chrono STATIC IMPORTED )
set_target_properties( boost_chrono PROPERTIES IMPORTED_LOCATION ${Boost_CHRONO_LIBRARY_RELEASE} )

add_library( boost_regex STATIC IMPORTED )
set_target_properties( boost_regex PROPERTIES IMPORTED_LOCATION ${Boost_REGEX_LIBRARY_RELEASE} )

add_library( boost_serialization STATIC IMPORTED )
set_target_properties( boost_serialization PROPERTIES IMPORTED_LOCATION ${Boost_SERIALIZATION_LIBRARY_RELEASE} )

add_library( boost_date_time STATIC IMPORTED )
set_target_properties( boost_date_time PROPERTIES IMPORTED_LOCATION ${Boost_DATE_TIME_LIBRARY_RELEASE} )

add_subdirectory( tprocess )
add_subdirectory( shmem )

#add_custom_target( one tproc --svn ${CMAKE_SOURCE_DIR} )
add_custom_target( one tcomp -r testdb.default -d 5 )
