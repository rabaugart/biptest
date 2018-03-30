
# Siehe
# https://gcc.gnu.org/wiki/Visibility
#

add_library( alib1 SHARED afunc1.cpp )
add_library( alib2 SHARED afunc2.cpp )

add_library( blib SHARED bfunc.cpp )
target_link_libraries( blib PRIVATE alib2 )

add_library( clib SHARED cfunc.cpp )
target_link_libraries( clib PRIVATE alib1 )

add_executable( texe texemain.cpp )
target_link_libraries( texe blib alib1 )

add_executable( texe2 texemain2.cpp )
target_link_libraries( texe2 blib clib )

add_custom_target( texe_start texe )
