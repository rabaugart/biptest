#ifndef VISCONFIG_H
#define VISCONFIG_H

#if defined _WIN32
#define RA_DYN_EXPORT
#define RA_DYN_LOCAL
#else
#define RA_DYN_EXPORT __attribute__ ((visibility ("default")))
#define RA_DYN_LOCAL __attribute__ ((visibility ("hidden")))
#endif

#endif // VISCONFIG_H

