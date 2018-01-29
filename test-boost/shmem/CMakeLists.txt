
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if ( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux" )
  link_libraries( -lpthread -lrt -lncurses )
endif()

if ( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin" )
  add_definitions( -DONLY_ONE_READER -DFIXED_MAPPING_ADDRESS )
  link_libraries( -lncurses )
endif()

add_library( lrashm STATIC rashm/Segment.cpp rashm/Header.cpp )

add_library( lratest STATIC  test/TestDataA.cpp test/TestDataB.cpp 
    test/MsgCollector.cpp test/ComProc.cpp test/SubProc.cpp test/TestGenerator.cpp 
    test/unitconfig.cpp test/UnitTestData.cpp )
add_executable( teshmem teshmem.cpp )
target_link_libraries( teshmem lrashm lratest )

add_executable( tfshmem tserial.cpp )
target_link_libraries( tfshmem lrashm lratest boost_serialization boost_date_time )

add_executable( tcomp tcomp.cpp )
target_link_libraries( tcomp lrashm lratest boost_chrono boost_serialization boost_date_time boost_system
    boost_program_options boost_log boost_thread )

if ( NOT ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows" )
    add_executable( tterm tterm.cpp rashm/Console.cpp )
    target_link_libraries( tterm lrashm lratest boost_log boost_thread boost_chrono boost_system )
endif()

add_executable( tfnet tfnet.cpp )
target_link_libraries( tfnet lrashm lratest boost_chrono boost_serialization boost_date_time boost_system
    boost_program_options boost_log boost_thread )

add_executable( tdoppler tdoppler.cpp )
target_link_libraries( tdoppler lrashm lratest boost_chrono boost_serialization boost_date_time boost_system
    boost_program_options boost_log boost_thread )

add_executable( unitproc test/unitproc.cpp )
target_link_libraries( unitproc lratest lrashm boost_serialization boost_chrono boost_regex boost_filesystem boost_system )

add_executable( unitrunner test/unitrunner.cpp )
target_link_libraries( unitrunner lratest lrashm boost_serialization boost_chrono
    boost_filesystem boost_regex boost_system )

