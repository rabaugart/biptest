#ifndef LOGGING_H
#define LOGGING_H

#include <boost/log/sources/severity_logger.hpp>

enum class loglvl {
    trace,
    debug,
    info,
    warning,
    error
};

typedef boost::log::sources::severity_logger<loglvl> Logger;

#endif // LOGGING_H
