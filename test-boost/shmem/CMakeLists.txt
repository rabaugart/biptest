
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if ( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux" )
  link_libraries( -lpthread -lrt -lncurses )
endif()

if ( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin" )
  add_definitions( -DONLY_ONE_READER -DFIXED_MAPPING_ADDRESS )
  link_libraries( -lncurses )
endif()

#
# Check the boost libraries required
#
foreach( check_target boost_chrono boost_date_time boost_filesystem boost_log boost_regex
    boost_thread boost_serialization boost_system )
  if ( NOT TARGET ${check_target} )
    message( FATAL_ERROR "rashm requires target ${check_target}" )
  endif()
endforeach()


add_library( lrashm STATIC rashm/Segment.cpp rashm/Header.cpp )

add_library( lratest STATIC  test/TestDataA.cpp test/TestDataAFieldAdapterFactory.cpp test/TestDataB.cpp 
    test/MsgCollector.cpp test/ComProc.cpp test/SubProc.cpp test/TestGenerator.cpp 
    test/unitconfig.cpp test/UnitTestData.cpp )

add_executable( txcomp test/txcomp.cpp )
target_link_libraries( txcomp lrashm lratest boost_chrono boost_serialization boost_date_time boost_system
    boost_program_options boost_log boost_thread )

if ( NOT ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows" )
    add_executable( txterm test/txterm.cpp rashm/Console.cpp )
    target_link_libraries( txterm lrashm lratest boost_log boost_thread boost_chrono boost_system )
endif()

add_executable( txnet test/txnet.cpp )
target_link_libraries( txnet lrashm lratest boost_chrono boost_serialization boost_date_time boost_system
    boost_program_options boost_log boost_thread )

add_executable( txdoppler test/txdoppler.cpp )
target_link_libraries( txdoppler lrashm lratest boost_chrono boost_serialization boost_date_time boost_system
    boost_program_options boost_log boost_thread )

add_executable( tmon tmon.cpp )
target_link_libraries( tmon lrashm lratest boost_chrono boost_serialization boost_date_time boost_system
    boost_program_options boost_log boost_thread )
    
add_executable( unitproc test/unitproc.cpp )
target_link_libraries( unitproc lratest lrashm boost_serialization boost_chrono boost_regex boost_filesystem boost_system )

add_executable( unitrunner test/unitrunner.cpp )
target_link_libraries( unitrunner lratest lrashm boost_serialization boost_chrono
    boost_filesystem boost_regex boost_system boost_date_time )

