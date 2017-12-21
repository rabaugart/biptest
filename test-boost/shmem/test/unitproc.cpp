/**
 * A subprocess of unitrunner
 *
 * The main expects a particular set of command line args. It should not be called manually
 *
 *  Created on: 15.12.2017
 *      Author: netz
 */

#include <iostream>
#include <thread>
#include <numeric>

#include <boost/chrono.hpp>
#include <boost/log/trivial.hpp>

#include "rashm/Segment.h"
#include "rashm/SegmentWriter.h"
#include "rashm/SegmentReader.h"
#include "rashm/CompMap.h"

#include "unitconfig.h"
#include "TestGenerator.h"
#include "all_data_types.h"
#include "UnitTestData.h"

using utest::unit_config;

/// The base for readers and writers keeps the test result
class TestBase {
public:
    virtual ~TestBase() {
    }
    virtual void start() = 0;
    virtual void join() = 0;
    virtual void removeSegment() = 0;

    utest::test_result result;
};

/**
 * The Writer periodically updates the segment using a TestGenerator
 */
template<typename DATA, typename ID>
class Writer: public TestBase {
public:

    Writer(unit_config cfg_) :
            cfg(cfg_) {
        result.name = "writer";
    }

    virtual ~Writer() {
        if (th.joinable()) {
            th.join();
        }
    }

    virtual void start() {
        th = std::move(std::thread(std::ref(*this)));
    }

    void operator()() {
        TestGenerator<DATA> gen;

        rashm::SegmentWriter<DATA, ID> sw;

        boost::chrono::time_point<boost::chrono::system_clock> start =
                boost::chrono::system_clock::now();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        while (boost::chrono::system_clock::now() - start
                < boost::chrono::milliseconds(cfg.duration)
                && (cfg.niter == 0 || result.n_loop < cfg.niter)) {
            sw = gen.current;
            std::this_thread::sleep_for(std::chrono::microseconds(cfg.period));
            if (result.n_loop == 0)
                result.first_counter = sw.head().counter;
            result.n_loop++;
        }
        result.last_counter = sw.head().counter;
    }

    void join() {
        if (th.joinable()) {
            th.join();
        }
    }

    virtual void removeSegment() {
        rashm::Segment<DATA, ID>::removeSegment();
    }

    unit_config cfg;
    std::thread th;
};

struct WriterFactory {
    typedef TestBase type;
    typedef std::shared_ptr<type> value_type;
    typedef unit_config config_type;

    template<typename DATA, typename ID>
    static value_type make(config_type const& cfg) {
        return std::make_shared<Writer<DATA, ID>>(cfg);
    }
};

//
// Reader
//
template<typename DATA, typename ID>
class Reader: public TestBase {
public:

    Reader(unit_config cfg_) :
            cfg(cfg_) {
        result.name = "reader";
    }

    virtual ~Reader() {
        if (th.joinable()) {
            th.join();
        }
    }

    virtual void start() {
        th = std::move(std::thread(std::ref(*this)));
    }

    void operator()() {

        size_t current_counter = std::numeric_limits<size_t>::max();

        boost::chrono::time_point<boost::chrono::system_clock> const start =
                boost::chrono::system_clock::now();

        while (boost::chrono::system_clock::now() - start
                < boost::chrono::milliseconds(cfg.duration)
                && (cfg.niter == 0 || result.n_loop < cfg.niter)) {
            try {
                rashm::SegmentReader<DATA, ID> sr;

                boost::posix_time::microseconds timeout(cfg.timeout * 1000);

                while (boost::chrono::system_clock::now() - start
                        < boost::chrono::milliseconds(cfg.duration)
                        && (cfg.niter == 0 || result.n_loop < cfg.niter)) {

                    try {
                        sr.timed_wait_for(timeout);
                        result.last_counter = sr.head().counter;
                        if (result.n_loop == 0)
                            result.first_counter = result.last_counter;
                        if (current_counter
                                != std::numeric_limits<size_t>::max()
                                && result.last_counter != current_counter + 1) {
                            result.counter_error_pos = result.last_counter;
                            result.n_counter_errors++;
                        }
                        current_counter = result.last_counter;
                        result.n_loop++;
                    } catch (rashm::timeout_error const & e) {
                        result.n_timeouts++;
                    }

                }
            } catch (rashm::no_segment_error const & e) {
                result.n_no_segment++;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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

    unit_config cfg;
    std::thread th;
};

struct ReaderFactory {
    typedef TestBase type;
    typedef std::shared_ptr<type> value_type;
    typedef unit_config config_type;

    template<typename DATA, typename ID>
    static value_type make(config_type const& cfg) {
        return std::make_shared<Reader<DATA, ID>>(cfg);
    }
};


int main(int argc, char** argv) {

    if (argc < 3)
        return 1;

    // The command line args
    std::string const command = argv[1];
    unit_config cfg = unit_config::fromString(argv[2]);
    std::string const id = argc >= 4 ? argv[3] : "nn";

    std::cout << "arg " << command << " " << id << " " << cfg.niter
            << std::endl;

    if (command == utest::COM::WRITER) {

        //
        // Instantiating the writer
        //

        typedef WriterFactory fac_t;

        rashm::CompMap<fac_t> const map = rashm::makeMap<test_data_vector_t, fac_t>(
                cfg);

        auto comp = map.at(cfg.comp_name);
        comp->start();
        comp->join();
        std::cout << "Final " << id << " " << command << " "
                << comp->result << std::endl;
        std::cout << comp->result.toString();
        return comp->result.n_loop == cfg.niter ? 0 : 1;
    } else if (command == utest::COM::READER) {
        //
        // Instantiating the reader
        //

        typedef ReaderFactory fac_t;

        rashm::CompMap<fac_t> const map = rashm::makeMap<test_data_vector_t, fac_t>(
                cfg);

        auto comp = map.at(cfg.comp_name);
        map.at(cfg.comp_name)->start();
        map.at(cfg.comp_name)->join();
        std::cout << "Final " << id << " " << command << " "
                << comp->result << std::endl;
        std::cout << comp->result.toString();
        return comp->result.n_loop == cfg.niter ? 0 : 1;
    }

    // Wrong arguments
    return 1;
}

