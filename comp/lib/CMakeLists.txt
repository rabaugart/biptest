
add_compile_options( -fPIC )

add_library( lcomp STATIC
  lcomp.cpp lcomp.hpp
  FactoryRegistry.cpp FactoryRegistry.hpp
  FactoryBase.cpp FactoryBase.hpp
  Component.cpp Component.hpp
  Environment.cpp Environment.hpp
  ComponentInfo.cpp ComponentInfo.hpp
  )

target_include_directories(lcomp INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/mylib>  # <prefix>/include/mylib
)
