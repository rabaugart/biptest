/*
 * tcomp.cpp
 *
 *  Created on: 05.12.2017
 *      Author: netz
 */

#include <map>
#include <memory>
#include <thread>

#include <boost/program_options.hpp>
#include <boost/chrono.hpp>
#include <boost/program_options/options_description.hpp>

#include "rashm/Segment.h"
#include "rashm/SegmentWriter.h"
#include "rashm/SegmentReader.h"
#include "rashm/CompMap.h"

#include "test/all_data_types.h"
#include "test/TestGenerator.h"

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
// Writer
//
template<typename DATA, typename ID>
class Writer: public CompBase {
public:

    Writer(CompConfig cfg_) :
            cfg(cfg_) {
    }

    virtual ~Writer() {
        if (th.joinable()) {
            th.join();
        }
    }

    virtual void start() {
        std::cout << "Starting writer " << rashm::DataIdTraits<DATA, ID>::name()
                << std::endl;

        th = std::move(std::thread(std::ref(*this)));
    }

    void operator()() {
        TestGenerator<DATA> gen;

        rashm::SegmentWriter<DATA, ID> sw;

        std::cout << "Address " << sw.address()
#if defined(FIXED_MAPPING_ADDRESS)
                << "/" << rashm::DataIdTraits<DATA, ID>::fixedAddress()
#endif
                << std::endl;

        boost::chrono::time_point<boost::chrono::system_clock> start =
                boost::chrono::system_clock::now();

        while (boost::chrono::system_clock::now() - start
                < boost::chrono::seconds(cfg.duration)) {
            sw = gen.current;
            std::cout << sw.headerTime() << " written " << gen.next()
                    << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(cfg.period));
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

struct WriterFactory {
    typedef CompBase type;
    typedef std::shared_ptr<type> value_type;
    typedef CompConfig config_type;

    template<typename DATA, typename ID>
    static value_type make(config_type const& cfg) {
        return std::make_shared<Writer<DATA, ID>>(cfg);
    }
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
        std::cout << "Starting reader " << rashm::DataIdTraits<DATA, ID>::name()
                << std::endl;

        th = std::move(std::thread(std::ref(*this)));
    }

    void operator()() {
        TestGenerator<DATA> gen;

        boost::chrono::time_point<boost::chrono::system_clock> const start =
                boost::chrono::system_clock::now();

        while (boost::chrono::system_clock::now() - start
                < boost::chrono::seconds(cfg.duration)) {
            try {
                rashm::SegmentReader<DATA, ID> sr(
                        boost::interprocess::open_only);

                std::cout << "Address " << sr.address() << std::endl;

                boost::posix_time::microseconds timeout(cfg.timeout * 1000);

                while (boost::chrono::system_clock::now() - start
                        < boost::chrono::seconds(cfg.duration)) {

                    try {
                        DATA d = sr.timed_wait_for(timeout);
                        std::cout << sr.headerTime() << " read "
                                << sr.lastAge().total_microseconds() << "us "
                                << d << std::endl;
                    } catch (std::runtime_error const & e) {
                        std::cout << rashm::now() << " timeout (last "
                                << sr.lastReceptionTime() << "/"
                                << sr.headerTime() << ")" << std::endl;
                    }

                }
            } catch (boost::interprocess::interprocess_exception const & e) {
                if (e.get_error_code()
                        == boost::interprocess::not_found_error) {
                    std::cout << rashm::now() << " no segment" << std::endl;
                    std::this_thread::sleep_for(
                            std::chrono::milliseconds(1000));
                } else {
                    throw;
                }
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
            po::value<size_t>(&(cfg.timeout))->default_value(1000),
            "timeout in milliseconds")("period,p",
            po::value<size_t>(&(cfg.period))->default_value(500),
            "Period in milliseconds")("writer,w",
            po::value<std::string>(&compName), "start writer by name")(
            "reader,r", po::value<std::string>(&compName),
            "start reader by name")("clear,c", "remove all segments");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\nComponents:\n";
        typedef WriterFactory fac_t;

        rashm::CompMap<fac_t> const map = rashm::makeMap<data_vector_t, fac_t>(
                cfg);
        for (auto const& i : map) {
            std::cout << i.first << std::endl;
        }
        return 1;
    }

    if (vm.count("clear")) {
        typedef ReaderFactory fac_t;

        rashm::CompMap<fac_t> const map = rashm::makeMap<data_vector_t, fac_t>(
                cfg);

        for (auto const& i : map) {
            i.second->removeSegment();
        }
    }

    //boost::chrono::set_time_fmt(std::cout, std::string { "%T" });

    if (vm.count("writer")) {
        typedef WriterFactory fac_t;

        rashm::CompMap<fac_t> const map = rashm::makeMap<data_vector_t, fac_t>(
                cfg);

        map.at(compName)->start();
        map.at(compName)->join();
        return 0;
    }

    if (vm.count("reader")) {
        typedef ReaderFactory fac_t;

        rashm::CompMap<fac_t> const map = rashm::makeMap<data_vector_t, fac_t>(
                cfg);

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
