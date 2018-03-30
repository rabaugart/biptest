
# Siehe
# https://gcc.gnu.org/wiki/Visibility
#

set( CMAKE_POSITION_INDEPENDENT_CODE True )

add_library( alib1 STATIC afunc1.cpp )
add_library( alib2 STATIC afunc2.cpp )

add_library( blib SHARED bfunc.cpp )
target_link_libraries( blib PRIVATE alib2 )
set_target_properties( blib PROPERTIES SOVERSION 2 )

add_library( clib SHARED cfunc.cpp )
target_link_libraries( clib PRIVATE alib1 )

add_executable( texe texemain.cpp )
target_link_libraries( texe blib alib1 )

add_executable( texe2 texemain2.cpp )
target_link_libraries( texe2 blib clib )

add_custom_target( texe_start texe )
