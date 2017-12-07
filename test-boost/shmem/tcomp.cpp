/*
 * tcomp.cpp
 *
 *  Created on: 05.12.2017
 *      Author: netz
 */

#include <map>
#include <memory>
#include <thread>
#include <chrono>

#include <boost/program_options.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/chrono.hpp>
#include <boost/program_options/options_description.hpp>

#include "rashm/Segment.h"
#include "rashm/SegmentWriter.h"
#include "rashm/SegmentReader.h"
#include "rashm/data_functors.h"

#include "test/all_data_types.h"
#include "test/TestGenerator.h"

template<typename COMP>
class CompMap: public std::map<std::string, std::shared_ptr<typename COMP::type>> {

};

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

        boost::chrono::time_point<boost::chrono::system_clock> start =
                boost::chrono::system_clock::now();

        while (boost::chrono::system_clock::now() - start
                < boost::chrono::seconds(cfg.duration)) {
            std::cout << "Writer " << boost::chrono::system_clock::now() << " "
                    << gen.next() << std::endl;
            sw = gen.current;
            std::this_thread::sleep_for(std::chrono::milliseconds(cfg.period));
        }

    }

    void join() {
        if (th.joinable()) {
            th.join();
        }
    }

    CompConfig cfg;
    std::thread th;
};

struct WriterComp {
    typedef CompBase type;

    template<typename DATA, typename ID>
    static std::shared_ptr<type> make(CompConfig const& cfg) {
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

        rashm::SegmentReader<DATA, ID> sr;

        boost::chrono::time_point<boost::chrono::system_clock> start =
                boost::chrono::system_clock::now();

        boost::posix_time::microseconds timeout(cfg.timeout * 1000);

        while (boost::chrono::system_clock::now() - start
                < boost::chrono::seconds(cfg.duration)) {

            try {
                DATA d = sr.timed_wait_for(timeout);
                std::cout << "Reader " << boost::chrono::system_clock::now()
                        << " " << d << std::endl;
            } catch (std::runtime_error const & e) {
                std::cout << "Timeout" << std::endl;
            }

        }

    }

    void join() {
        if (th.joinable()) {
            th.join();
        }
    }

    CompConfig cfg;
    std::thread th;
};

struct ReaderComp {
    typedef CompBase type;

    template<typename DATA, typename ID>
    static std::shared_ptr<type> make(CompConfig const& cfg) {
        return std::make_shared<Reader<DATA, ID>>(cfg);
    }
};

template<typename COMP>
struct Coll {
    Coll(CompMap<COMP> & v_, CompConfig cfg_) :
            v(v_), cfg(cfg_) {
    }

    template<typename U>
    void operator()(U x) {
        v[x.name()] = COMP::template make<typename U::data_t, typename U::id_t>(
                cfg);
    }
    CompMap<COMP>& v;
    CompConfig cfg;
};

template<typename COMP>
CompMap<COMP> makeMap(CompConfig const& cfg) {
    CompMap<COMP> ret;

    typedef rashm::apply_all_data_ids<data_vector_t, rashm::DataIdTraitsFunctor>::type all_packets_t;

    boost::mpl::for_each<all_packets_t>(Coll<COMP>(ret, cfg));

    return ret;
}

int main(int argc, char** argv) {

    CompConfig cfg;

    std::string compName { "" };
    namespace po = boost::program_options;

    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "produce help message")("duration,d",
            po::value<size_t>(&(cfg.duration))->default_value(5),
            "Duration in seconds")("timeout,t",
            po::value<size_t>(&(cfg.timeout))->default_value(500),
            "Timeout in milliseconds")("period,p",
                    po::value<size_t>(&(cfg.period))->default_value(500),
                    "Period in milliseconds")("writer,w",
            po::value<std::string>(&compName), "Start writer by name")("reader,r",
                    po::value<std::string>(&compName), "Start reader by name");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);


    if (vm.count("help")) {
        std::cout << desc << "\nComponents:\n";
        typedef WriterComp comp_t;

        CompMap<comp_t> const map = makeMap<comp_t>(cfg);

        for (auto const& i : map) {
            std::cout << i.first << std::endl;
        }
        return 1;
    }

    if (vm.count("writer")) {
        typedef WriterComp comp_t;

        CompMap<comp_t> const map = makeMap<comp_t>(cfg);

        map.at(compName)->start();
        map.at(compName)->join();
        return 0;
    }

    if (vm.count("reader")) {
        typedef ReaderComp comp_t;

        CompMap<comp_t> const map = makeMap<comp_t>(cfg);

        map.at(compName)->start();
        map.at(compName)->join();
        return 0;
    }

    std::cerr << "Command missing" << std::endl;

    return 0;
}
