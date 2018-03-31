#ifndef VISCONFIG_H
#define VISCONFIG_H

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define FOX_HELPER_DLL_IMPORT __declspec(dllimport)
  #define FOX_HELPER_DLL_EXPORT __declspec(dllexport)
  #define FOX_HELPER_DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define FOX_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define FOX_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define FOX_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define FOX_HELPER_DLL_IMPORT
    #define FOX_HELPER_DLL_EXPORT
    #define FOX_HELPER_DLL_LOCAL
  #endif
#endif

// Now we use the generic helper definitions above to define FOX_API and FOX_LOCAL.
// FOX_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// FOX_LOCAL is used for non-api symbols.

#ifdef RA_BLIB_DLL // defined if FOX is compiled as a DLL
  #ifdef RA_BLIB_DLL_EXPORTS // defined if we are building the FOX DLL (instead of using it)
    #define RA_DYN_EXPORT FOX_HELPER_DLL_EXPORT
  #else
    #define RA_DYN_EXPORT FOX_HELPER_DLL_IMPORT
  #endif // RA_BLIB_DLL_EXPORTS
  #define RA_DYN_LOCAL FOX_HELPER_DLL_LOCAL
#else // RA_BLIB_DLL is not defined: this means FOX is a static lib.
  #define RA_DYN_EXPORT
  #define RA_DYN_LOCAL
#endif // RA_BLIB_DLL


#endif // VISCONFIG_H

