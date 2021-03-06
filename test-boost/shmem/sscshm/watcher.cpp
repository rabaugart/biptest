/*
 * watcher.cpp
 *
 *  Created on: 29.01.2018
 *      Author: netz
 */

#include <map>
#include <memory>
#include <thread>

#include <boost/program_options.hpp>
#include <boost/chrono.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>

#include "rashm/Segment.h"
#include "rashm/SegmentWriter.h"
#include "rashm/SegmentReader.h"
#include "rashm/CompMap.h"

#include "all_ssc_data_types.h"

struct CompConfig {
    size_t duration;
    size_t timeout;
    size_t period;
};

class CompBase {
public:
    virtual ~CompBase() {
    }
    virtual void start() = 0;
    virtual void join() = 0;
    virtual void removeSegment() = 0;
};

//
// Reader
//
template<typename DATA, typename ID>
class Reader: public CompBase {
public:

    Reader(CompConfig cfg_) :
            cfg(cfg_) {
    }

    virtual ~Reader() {
        if (th.joinable()) {
            th.join();
        }
    }

    virtual void start() {
        BOOST_LOG_TRIVIAL(info)<< "starting reader " << rashm::DataIdTraits<DATA, ID>::name();

        th = std::move(std::thread(std::ref(*this)));
    }

    void operator()() {

        boost::chrono::time_point<boost::chrono::system_clock> const start =
        boost::chrono::system_clock::now();

        while (boost::chrono::system_clock::now() - start
                < boost::chrono::seconds(cfg.duration)) {
            try {
                rashm::SegmentReader<DATA, ID> sr(
                        boost::interprocess::open_only);

                BOOST_LOG_TRIVIAL(info) << "address " << sr.address();

                boost::posix_time::microseconds timeout(cfg.timeout * 1000);

                while (boost::chrono::system_clock::now() - start
                        < boost::chrono::seconds(cfg.duration)) {

                    try {
                        DATA d = sr.timed_wait_for(timeout);
                        BOOST_LOG_TRIVIAL(debug) << sr.segmentName() << " read " << sr.headerTime() << " "
                        << sr.lastAge().total_microseconds() << "us "
                        << d;
                    } catch ( rashm::timeout_error const & e) {
                        BOOST_LOG_TRIVIAL(info) << sr.segmentName() << " timeout (last "
                        << sr.lastReceptionTime() << "/"
                        << sr.headerTime() << ")";
                    }

                }
            } catch (rashm::no_segment_error const & e) {
                BOOST_LOG_TRIVIAL(info) << rashm::SegmentReader<DATA, ID>::segmentName() << " no segment";
                std::this_thread::sleep_for(
                        std::chrono::milliseconds(1000));
            }
        }
    }

    void join() {
        if (th.joinable()) {
            th.join();
        }
    }

    virtual void removeSegment() {
        rashm::Segment<DATA, ID>::removeSegment();
    }

    CompConfig cfg;
    std::thread th;
};

struct ReaderFactory {
    typedef CompBase type;
    typedef std::shared_ptr<type> value_type;
    typedef CompConfig config_type;

    template<typename DATA, typename ID>
    static value_type make(config_type const& cfg) {
        return std::make_shared<Reader<DATA, ID>>(cfg);
    }
};

int main(int argc, char** argv) {

    CompConfig cfg;

    std::string compName { "" };
    namespace po = boost::program_options;

    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "produce help message")("duration,d",
            po::value<size_t>(&(cfg.duration))->default_value(5),
            "duration in seconds")("timeout,t",
            po::value<size_t>(&(cfg.timeout))->default_value(3000),
            "timeout in milliseconds")("period,p",
            po::value<size_t>(&(cfg.period))->default_value(500),
            "Period in milliseconds")("writer,w",
            po::value<std::string>(&compName), "start writer by name")("wall",
            "start all writers")("reader,r", po::value<std::string>(&compName),
            "start reader by name")("clear,c", "remove all segments")("quiet,q",
            "Show only errors");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\nComponents:\n";
        typedef ReaderFactory fac_t;

        rashm::CompMap<fac_t> const map = rashm::makeMap<
                ran::ssc::data_vector_t, fac_t>(cfg);
        for (auto const& i : map) {
            std::cout << i.first << std::endl;
        }
        return 1;
    }

    //boost::log::add_console_log(std::cout,boost::log::keywords::format = "[%TimeStamp%]: %Message%");
    if (vm.count("quiet")) {
        boost::log::core::get()->set_filter(
                boost::log::trivial::severity > boost::log::trivial::info);
    }

    if (vm.count("clear")) {
        typedef ReaderFactory fac_t;

        rashm::CompMap<fac_t> const map = rashm::makeMap<
                ran::ssc::data_vector_t, fac_t>(cfg);

        for (auto const& i : map) {
            i.second->removeSegment();
        }
    }

    if (vm.count("reader")) {
        typedef ReaderFactory fac_t;

        rashm::CompMap<fac_t> const map = rashm::makeMap<
                ran::ssc::data_vector_t, fac_t>(cfg);

        map.at(compName)->start();
        map.at(compName)->join();
        return 0;
    }

    if (!vm.count("clear")) {
        std::cerr << "Command missing" << std::endl;
        return 1;
    }

    return 0;
}

