project( comp )

cmake_minimum_required(VERSION 3.10)

# Tests of components, 27.5.20

set( CMAKE_BUILD_TYPE debug )

set( CXX_STANDARD 14 )

find_package( Boost COMPONENTS filesystem )

add_subdirectory( main )
add_subdirectory( compa )
add_subdirectory( lib )

