/*
 * unitconfig.cpp
 *
 *  Created on: 15.12.2017
 *      Author: netz
 */

#include <iostream>
#include <sstream>

#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>

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
    ar & d.bin_path;
}

template<class Archive>
void serialize(Archive & ar, utest::test_result & d,
        const unsigned int version) {
    ar & d.name;
    ar & d.n_loop;
    ar & d.n_timeouts;
    ar & d.n_no_segment;
    ar & d.n_counter_errors;
    ar & d.first_counter;
    ar & d.last_counter;
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

std::string test_result::toString() const {
    std::ostringstream os;
    boost::archive::text_oarchive oa(os);

    oa << *this;

    return os.str();
}

test_result test_result::fromString(std::string const& cfgstr) {
    test_result ret;

    std::istringstream is(cfgstr);
    boost::archive::text_iarchive ia(is);
    ia >> ret;
    return ret;
}

std::ostream& operator <<(std::ostream& os, test_result const& tr) {
    os << tr.name << " " << tr.n_loop << " " << tr.first_counter << "/"
            << tr.last_counter << " to " << tr.n_timeouts << "/"
            << tr.n_no_segment << "/" << tr.n_counter_errors << "/"
            << tr.counter_error_pos;
    return os;
}

std::string const process::runner { "unitproc" };

namespace bp = boost::process;
namespace bfs = boost::filesystem;

process::process(std::string const& com, unit_config const& cfg,
        std::string const& id_, boost::asio::io_service& ios) :
        command(com), config(cfg), id(id_), ch(
                bfs::path(cfg.bin_path) / process::runner, bp::args= {command,cfg.toString(),id},bp::std_in.close(), bp::std_out > data,
        bp::std_err > bp::null, ios) {
        }

bool process::wait() {
    std::error_code ec;
    ch.wait(ec);
    bool ret = true;
    if (ec) {
        message = (boost::format("Failure1(%s,%s,%d) ") % command % id
                % ec.value()).str();
        return false;
    } else {
        int const exc = ch.exit_code();
        if (exc) {
            message =
                    (boost::format("Failure2(%s,%s,%d) ") % command % id % exc).str();
            ret = false;
        } else {
            ret = true;
        }
        std::vector<std::string> rr;
        const boost::regex rx { "\n" };
        boost::algorithm::split_regex(rr, data.get(), rx);
        std::for_each(rr.begin(), rr.end(),
                [this]( const std::string &i) {output.push_back(i);});

        result = test_result::fromString(output[output.size() - 1]);
    }
    return ret;
}

void process_vec::add(std::string const& com, unit_config const& cfg) {
    add(com, cfg, (boost::format("%s-%03d") % com % size()).str());
}

void process_vec::add(std::string const& com, unit_config const& cfg,
        name_func f) {
    add(com, cfg, f(size()));
}

void process_vec::add(std::string const& com, unit_config const& cfg,
        std::string const& id) {
    emplace_back(com, cfg, id, ios);
}

bool process_vec::waitall() {

    ios.run();

    bool ret = true;

    for (auto& pi : *this) {
        if (!pi.wait()) {
            ret = false;
            message += pi.message;
        }
    }

    return ret;
}

}
