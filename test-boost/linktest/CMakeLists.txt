
# Siehe
# https://gcc.gnu.org/wiki/Visibility
# https://linux.die.net/man/1/ld
# man ld
#
# The diamond problem
# https://blog.habets.se/2012/05/Shared-libraries-diamond-problem.html
#
# Symbol versioning
# https://guru.multimedia.cx/ld-so-gnu-linkerloader/
#
# General information about libraries
# http://www.yolinux.com/TUTORIALS/LibraryArchives-StaticAndDynamic.html
#

set( CMAKE_POSITION_INDEPENDENT_CODE True )

add_library( alib1 SHARED afunc1.cpp )
set_target_properties( alib1 PROPERTIES 
   SOVERSION 1
   LINK_FLAGS "-Wl,--default-symver"
)

add_library( alib2 SHARED afunc2.cpp )
set_target_properties( alib2 PROPERTIES 
   SOVERSION 2
   LINK_FLAGS "-Wl,--default-symver"
)

add_library( blib SHARED bfunc.cpp )
target_link_libraries( blib PRIVATE alib2 )
set_target_properties( blib PROPERTIES 
   SOVERSION 1
   LINK_FLAGS "-Wl,--default-symver"
)
# Tested
#   LINK_FLAGS "-Wl,--exclude-libs linktest/libalib2.a"
#   INTERFACE_LINK_LIBRARIES ""

add_library( clib SHARED cfunc.cpp )
target_link_libraries( clib PRIVATE alib1 )
set_target_properties( clib PROPERTIES 
   SOVERSION 2
   LINK_FLAGS "-Wl,--default-symver"
)

add_executable( texe texemain.cpp )
target_link_libraries( texe blib alib1 )

add_executable( texe2 texemain2.cpp )
target_link_libraries( texe2 blib clib )

add_custom_target( texe_start texe )
