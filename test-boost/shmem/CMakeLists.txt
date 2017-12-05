
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if ( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux" )
  link_libraries( -lpthread -lrt )
endif()

if ( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin" )
  add_definitions( -DONLY_ONE_READER -DFIXED_MAPPING_ADDRESS )
endif()

add_executable( tashmem tashmem.cpp shmem/shmem.cpp test/TestDataA.cpp )

add_executable( tbshmem tbshmem.cpp shmem/shmem.cpp )
target_link_libraries( tbshmem lratest boost_thread boost_system boost_chrono boost_regex )

add_executable( tcshmem tcshmem.cpp )
target_link_libraries( tcshmem lratest boost_thread boost_system boost_chrono boost_regex )

if ( NOT ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows" )
  add_executable( tdshmem tdshmem.cpp )
endif()

add_library( lrashm STATIC rashm/Segment.cpp rashm/Header.cpp )

add_library( lratest STATIC  test/TestDataA.cpp test/TestDataB.cpp 
    test/MsgCollector.cpp test/ComProc.cpp test/SubProc.cpp)
add_executable( teshmem teshmem.cpp )
target_link_libraries( teshmem lrashm lratest )

add_executable( tfshmem tserial.cpp )
target_link_libraries( tfshmem lrashm lratest boost_serialization boost_date_time )

add_executable( tcomp tcomp.cpp )
target_link_libraries( tcomp lrashm lratest )
