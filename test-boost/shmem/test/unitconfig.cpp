/*
 * unitconfig.cpp
 *
 *  Created on: 15.12.2017
 *      Author: netz
 */

#include <iostream>
#include <sstream>

#include <boost/format.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <test/unitconfig.h>

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, utest::unit_config & d,
        const unsigned int version) {
    ar & d.comp_name;
    ar & d.niter;
    ar & d.duration;
    ar & d.timeout;
    ar & d.period;
}

} // namespace serialization
} // namespace boost

namespace utest {

unit_config::unit_config() :
        niter(100), duration(5000), timeout(1000), period(50000) {
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

std::string const process::runner { "unitproc" };
std::string const process::runnerpath { "shmem/" + process::runner };

namespace bp = boost::process;

process::process(std::string const& com, unit_config const& cfg,
        std::string const& id_) :
        command(com), config(cfg), id(id_), ch(process::runnerpath, bp::args= {command,cfg.toString(),id}) {
        }

void process_vec::add(std::string const& com, unit_config const& cfg,
        std::string const& id) {
    emplace_back(com, cfg, id);
}

bool process_vec::waitall() {

    bool ret=true;

    for (auto& pi : *this) {
        std::error_code ec;
        pi.ch.wait(ec);
        if (ec) {
            ret = false;
            message += (boost::format("Failure1(%s,%s,%d) ") % pi.command % pi.id
                    % ec.value()).str();
        } else {
            int const exc = pi.ch.exit_code();
            if (exc) {
                ret = false;
                message += (boost::format("Failure2(%s,%s,%d) ") % pi.command
                        % pi.id % exc).str();
            }
        }
    }

    return ret;
}

}
