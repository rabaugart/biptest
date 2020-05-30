#ifndef LOGGING_H
#define LOGGING_H

#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/common.hpp>

enum class loglvl {
    trace,
    debug,
    info,
    warning,
    error
};

class Logger : public boost::log::sources::severity_logger<loglvl>
{
public:
    Logger( const std::string& compType="nn");
    Logger(const std::string& compType, const std::string& compName);
};

void init_logging();

#endif // LOGGING_H
