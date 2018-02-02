/*
 * nettrans.cpp
 *
 *  Created on: Feb 1, 2018
 *      Author: ran
 */

/*
 * tcomp.cpp
 *
 *  Created on: 05.12.2017
 *      Author: netz
 */

#include <map>
#include <memory>
#include <thread>
#include <sstream>

#include <boost/program_options.hpp>
#include <boost/chrono.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/variant.hpp>
#include <boost/serialization/variant.hpp>

#include <boost/bind.hpp>

#include "rashm/Segment.h"
#include "rashm/SegmentWriter.h"
#include "rashm/SegmentReader.h"
#include "rashm/CompMap.h"
#include "rashm/data_functors.h"
#include "rashm/Packet.h"
#include "rashm/shm_net.h"

#include "all_ssc_data_types.h"

int main(int argc, char** argv) {

    using namespace rashm;

    NetConfig cfg;

    std::string compName { "" };
    namespace po = boost::program_options;

    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "produce help message")("duration,d",
            po::value<size_t>(&(cfg.duration))->default_value(5),
            "duration in seconds")("timeout,t",
            po::value<size_t>(&(cfg.timeout))->default_value(1000),
            "timeout in milliseconds")("port,p",
            po::value<unsigned short>(&(cfg.port))->default_value(9090), "Port")(
            "address,a",
            po::value<std::string>(&(cfg.address))->default_value("127.0.0.1"),
            "ip address")("sender,s", po::value<std::string>(&compName),
            "start sender by name")("sall", "start all senders")("rall",
            "receive all messages")("quiet,q", "show only errors");

    using ran::ssc::data_vector_t;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\nComponents:\n";
        typedef SenderFactory<data_vector_t> fac_t;

        rashm::CompMap<fac_t> const map = rashm::makeMap<data_vector_t, fac_t>(
                cfg);
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

    if (vm.count("sall")) {
        BOOST_LOG_TRIVIAL(info)<< "Starting all senders";
        typedef SenderFactory<data_vector_t> fac_t;

        rashm::CompMap<fac_t> const map = rashm::makeMap<data_vector_t, fac_t>(
                cfg);

        for (auto& i : map) {
            i.second->start();
        }

        for (auto& i : map) {
            i.second->join();
        }
        return 0;
    }

    if (vm.count("sender")) {
        typedef SenderFactory<data_vector_t> fac_t;

        rashm::CompMap<fac_t> const map = rashm::makeMap<data_vector_t, fac_t>(
                cfg);

        map.at(compName)->start();
        map.at(compName)->join();
        return 0;
    }

    if (vm.count("rall")) {

        receive<data_vector_t, IdentityVisitor>(cfg);
        return 0;
    }

    return 0;
}

