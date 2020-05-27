
add_library( lcomp STATIC lcomp.cpp lcomp.hpp )

target_include_directories(lcomp INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/mylib>  # <prefix>/include/mylib
)
