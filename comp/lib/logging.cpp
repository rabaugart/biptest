#include <iostream>
#include <string>

#include <boost/log/core.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

#include "logging.hpp"


Logger::Logger(const std::string& compType) : Logger(compType,compType)
{
}

Logger::Logger(const std::string& compType, const std::string& compName)
{
    add_attribute("comptype", boost::log::attributes::constant< std::string >(compType));
    add_attribute("compname", boost::log::attributes::constant< std::string >(compName));
}

void init_logging()
{
    boost::shared_ptr< boost::log::core > core = boost::log::core::get();
    core->add_global_attribute("TimeStamp", boost::log::attributes::local_clock());
    boost::log::add_console_log( std::cout, boost::log::keywords::format = "%TimeStamp% %comptype%/%compname%: %Message%");
}
