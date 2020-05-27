project( comp )

cmake_minimum_required(VERSION 3.10)

# Tests of components, 27.5.20

find_package( Boost COMPONENTS filesystem )

add_subdirectory( main )
add_subdirectory( compa )
add_subdirectory( lib )

