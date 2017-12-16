/*
 * unitconfig.cpp
 *
 *  Created on: 15.12.2017
 *      Author: netz
 */

#include <iostream>
#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <test/unitconfig.h>

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, unit_config & d, const unsigned int version) {
    ar & d.comp_name;
    ar & d.niter;
    ar & d.duration;
    ar & d.timeout;
    ar & d.period;
}

} // namespace serialization
} // namespace boost

unit_config::unit_config() :
        niter(100), duration(5), timeout(1000), period(50000) {
}

std::string unit_config::toString() const {
    std::ostringstream os;
    boost::archive::text_oarchive oa(os);

    oa << *this;

    return os.str();
}

unit_config unit_config::fromString(std::string const& cfgstr) {
    unit_config ret;

    std::istringstream is(cfgstr);
    boost::archive::text_iarchive ia(is);
    ia >> ret;
    return ret;
}
