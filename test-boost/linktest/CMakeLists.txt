
add_library( alib1 SHARED afunc1.cpp )
add_library( alib2 SHARED afunc2.cpp )
add_library( blib SHARED bfunc.cpp )
target_link_libraries( blib alib2 )

add_executable( texe texemain.cpp )

target_link_libraries( texe alib1 blib )

add_custom_target( texe_start texe )
