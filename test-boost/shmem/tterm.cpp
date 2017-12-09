/*
 * tterm.cpp
 *
 *  Created on: 09.12.2017
 *      Author: rainer
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>

#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/chrono.hpp>

#include "rashm/SegmentReader.h"
#include "rashm/CompMap.h"

#include "test/all_data_types.h"

#include "rashm/Console.h"

struct CompConfig {
    size_t duration;
    size_t timeout;
    size_t period;
};

class CompBase {
public:
    CompBase() : running(false) {}

    virtual ~CompBase() {
    }
    virtual void start() = 0;
    virtual void stop() {
        running = false;
    }

    std::string getText() {
        return text;
    }

    virtual void join() = 0;
    virtual void removeSegment() = 0;
protected:
    bool running;
    std::string text;
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
        BOOST_LOG_TRIVIAL(info) << "starting reader " << rashm::DataIdTraits<DATA, ID>::name();

        th = std::move(std::thread(std::ref(*this)));
        running = true;
    }

    void operator()() {

        while (running) {
            try {
                rashm::SegmentReader<DATA, ID> sr(
                        boost::interprocess::open_only);

                BOOST_LOG_TRIVIAL(info) << "address " << sr.address();

                boost::posix_time::microseconds timeout(cfg.timeout * 1000);

                while (running) {

                    try {
                        DATA d = sr.timed_wait_for(timeout);
                        BOOST_LOG_TRIVIAL(debug) << "read " << sr.headerTime()
                                << sr.lastAge().total_microseconds() << "us "
                                << d;
                        std::ostringstream os;
                        os << d;
                        text = os.str();
                    } catch (std::runtime_error const & e) {
                        BOOST_LOG_TRIVIAL(info) << "timeout (last "
                                << sr.headerTime() << ")";
                        std::ostringstream os;
                        os << "timeout (last "
                           << sr.headerTime() << ")";
                        text = os.str();
                    }

                }
            } catch (boost::interprocess::interprocess_exception const & e) {
                if (e.get_error_code()
                        == boost::interprocess::not_found_error) {
                    BOOST_LOG_TRIVIAL(info) << "no segment";
                    std::this_thread::sleep_for(
                            std::chrono::milliseconds(1000));
                    text = "no segment";
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

int main( int argc, char** argv ) {
    {
        CompConfig cfg;
        cfg.timeout = 1000;
        rashm::Console con;

        boost::log::core::get()->set_filter
           (
               boost::log::trivial::severity > boost::log::trivial::info
           );

        {
            typedef ReaderFactory fac_t;

            rashm::CompMap<fac_t> const map = rashm::makeMap<data_vector_t, fac_t>(
                        cfg);

            for ( auto& i : map ) {
                i.second->start();
            }

            do {
                int row = 3;
                for ( auto const& i : map ) {
                    con.show( row++, 2, (boost::format("%15s : %-50s" ) % i.first % i.second->getText()).str() );
                }
            } while ( !con.key() );

            con.show( 1, 2, "Stopping");

            for ( auto& i : map ) {
                i.second->stop();
            }
            for ( auto& i : map ) {
                i.second->join();
            }
        }
    }
    std::cout << "Ok" << std::endl;
}
