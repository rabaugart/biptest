
project( test-boost )

# Bauen Windows
# ..\cmake-3.9.4-win64-x64\bin\cmake -G Ninja -DCMAKE_MAKE_PROGRAM=c:/Users/de1-rbaumgarten/Desktop/tmp/ninjatest/ninja.exe ..\biptest-master\test-boost
# ..\cmake-3.9.4-win64-x64\bin\cmake -G  "Visual Studio 12 2013 Win64" -DCMAKE_MAKE_PROGRAM=c:/Users/de1-rbaumgarten/Desktop/tmp/ninjatest/ninja.exe ..\biptest-master\test-boost

# Office-Umgebung hat 2.8
cmake_minimum_required(VERSION 3.2)

set( CMAKE_BUILD_TYPE Debug )

if ( ${CMAKE_SYSTEM} MATCHES "Linux-5.4.0-[0-9]+-generic" )
  message( "Ubuntu 20.04" )
  set( CMAKE_CXX_STANDARD 17 )
  add_compile_options( -Wall -std=c++17 )
  # Options required on Ubuntu 20.04
  add_compile_options( -pthread )
  add_link_options( -pthread )
elseif ( IS_DIRECTORY "$ENV{HOME}/dev/libs/boost_1_74_0" )
  message( "Situx mit boost-1.74" )
  set( CMAKE_CXX_STANDARD 14 )
  add_compile_options( -Wall -std=c++14 -pthread )
  link_libraries( -pthread )
  set( BOOST_ROOT "$ENV{HOME}/dev/libs/boost_1_74_0" )
  #set( Qt_PREFIX_DIR "$ENV{HOME}/dev/Qt/5.8/5.8/clang_64/lib/cmake/Qt5" )
elseif ( IS_DIRECTORY "$ENV{HOME}/Sub_SW_S54S51/trunk/ext/boost_1_55_0" )
elseif ( ${CMAKE_SYSTEM} MATCHES "Linux-5.3.0-[0-9]+-generic" )
  message( "Ubuntu 18.04" )
  set( CMAKE_CXX_STANDARD 17 )
  add_compile_options( -Wall -std=c++17 )
  # Options required on Ubuntu 20.04
  add_compile_options( -pthread )
  link_libraries( -pthread )
elseif ( IS_DIRECTORY "$ENV{HOME}/dev/devlibs/boost_1_64_0xx" )
  message( "Mac oder neues Linux" )
  set( CMAKE_CXX_STANDARD 14 )
  add_compile_options( -Wall -std=c++14 )
  set( BOOST_ROOT "$ENV{HOME}/dev/devlibs/boost_1_64_0" )
  set( Qt_PREFIX_DIR "$ENV{HOME}/dev/Qt/5.8/5.8/clang_64/lib/cmake/Qt5" )
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
  message( FATAL_ERROR "BOOST_ROOT not found for ${CMAKE_SYSTEM}" )
endif()

set( Boost_USE_STATIC_LIBS ON )

find_package(Boost COMPONENTS program_options system filesystem
    thread chrono regex serialization date_time log log_setup
    math_tr1 unit_test_framework )

message( STATUS "Boost ${Boost_PROGRAM_OPTIONS_LIBRARY}" )

include_directories( "${Boost_INCLUDE_DIR}" )

link_directories( "${Boost_LIBRARY_DIR_RELEASE}" )

add_library( boost_program_options STATIC IMPORTED )
set_target_properties( boost_program_options PROPERTIES IMPORTED_LOCATION ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE} )

add_library( boost_system STATIC IMPORTED )
set_target_properties( boost_system PROPERTIES IMPORTED_LOCATION ${Boost_SYSTEM_LIBRARY_RELEASE} )

add_library( boost_filesystem STATIC IMPORTED )
set_target_properties( boost_filesystem PROPERTIES IMPORTED_LOCATION ${Boost_FILESYSTEM_LIBRARY_RELEASE} )

add_library( boost_math_tr1 STATIC IMPORTED )
set_target_properties( boost_math_tr1 PROPERTIES IMPORTED_LOCATION ${Boost_MATH_TR1_LIBRARY_RELEASE} )

add_library( boost_log STATIC IMPORTED )
set_target_properties( boost_log PROPERTIES IMPORTED_LOCATION ${Boost_LOG_LIBRARY_RELEASE} )

add_library( boost_log_setup STATIC IMPORTED )
set_target_properties( boost_log_setup PROPERTIES IMPORTED_LOCATION ${Boost_LOG_SETUP_LIBRARY_RELEASE} )

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

add_library( boost_unit_test_framework STATIC IMPORTED )
set_target_properties( boost_unit_test_framework PROPERTIES IMPORTED_LOCATION ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE} )

#
# QT
#
set( CMAKE_PREFIX_PATH "Qt_PREFIX_DIR" )

# Funktioniert nicht
#set( Qt5_ROOT "$ENV{HOME}/dev/Qt/5.8/5.8/clang_64" )

find_package( Qt5 COMPONENTS Widgets OPTIONAL )
if ( ${Qt5_FOUND} )
message( "Qt found ${Qt5_INCLUDE_DIRECTORIES}" )
else()
message( "Qt not found" )
endif()

#
# Subdirectories
#
add_subdirectory( tprocess )
#add_subdirectory( shmem )
add_subdirectory( tfuture )
add_subdirectory( tother )
add_subdirectory( tmath )

#add_custom_target( one txcomp -w testdb.tidb1 -d 3 )
add_custom_target( run_tfuture tfuture --log_level=message )
add_custom_target( run_tother tother --run_test=iter2* --log_level=message )
add_custom_target( utest unitrunner --log_level=message DEPENDS unitproc )

add_subdirectory( linktest )
