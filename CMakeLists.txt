project( Biptest )

cmake_minimum_required(VERSION 3.10)

enable_testing()

set( CMAKE_CXX_STANDARD 20 )


add_subdirectory( cpp20 )
#add_subdirectory( comp )
#add_subdirectory( test-boost )
add_subdirectory( test-other )
#add_subdirectory( thamessage )
