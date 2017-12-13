/*
 * tdoppler.cpp
 *
 *  Created on: 13.12.2017
 *      Author: netz
 */

#include <boost/chrono.hpp>
#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>

#include "rashm/SegmentReader.h"
#include "rashm/SegmentWriter.h"
#include "test/TestDataB.h"

int main(int argc, char** argv) {

    size_t duration;
    size_t timeout;

    namespace po = boost::program_options;

    po::options_description desc(
            "Reading testdb.tidb1, doubling l2 and writing testdb.tidb2.\n"
                    "Allowed options");
    desc.add_options()("help,h", "produce help message")("duration,d",
            po::value<size_t>(&duration)->default_value(5),
            "duration in seconds")("timeout,t",
            po::value<size_t>(&timeout)->default_value(1000),
            "timeout in milliseconds");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    boost::chrono::time_point<boost::chrono::system_clock> const start =
            boost::chrono::system_clock::now();

    rashm::SegmentReader<TestDataB, TIdB1> sr;
    rashm::SegmentWriter<TestDataB, TIdB2> sw;

    while (boost::chrono::system_clock::now() - start
            < boost::chrono::seconds(duration)) {
        try {
            TestDataB data = sr.timed_wait_for(
                    boost::posix_time::microseconds(timeout * 1000));
            BOOST_LOG_TRIVIAL(debug)<< "read " << data;
            data.l2 *= 2;
            sw = data;
        } catch (rashm::timeout_error const &) {
            BOOST_LOG_TRIVIAL(debug)<< "timeout";
        }
    }

    return 0;
}
