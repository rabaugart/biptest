
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if ( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux" )
  link_libraries( -lpthread -lrt )
endif()

add_executable( tashmem tashmem.cpp shmem/shmem.cpp test/TestDataA.cpp )

add_executable( tbshmem tbshmem.cpp shmem/shmem.cpp test/TestDataA.cpp test/MsgCollector.cpp )
target_link_libraries( tbshmem boost_thread boost_system boost_chrono )

add_executable( tcshmem tcshmem.cpp  test/MsgCollector.cpp )
target_link_libraries( tcshmem boost_thread boost_system boost_chrono boost_regex )

if ( NOT ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows" )
  add_executable( tdshmem tdshmem.cpp )
endif()


