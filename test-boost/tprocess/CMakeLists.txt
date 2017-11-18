
if ( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux" )
  link_libraries( -lpthread -lrt )
endif()

add_executable( tproc tprocess.cpp )
target_link_libraries( tproc boost_program_options boost_system boost_filesystem )
