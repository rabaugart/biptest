/*
 * shmonitor.cpp
 *
 *  Created on: Jan 31, 2018
 *      Author: ran
 */

#include <string>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#include <csignal>

#include <boost/program_options.hpp>

#include "SscFieldAdapterFactory.h"

namespace ran {
namespace ssc {

class PrintReceiver {
public:
	PrintReceiver( SscFieldAdapterFactory::adapter_ptr ad_ ) : ad(ad_) {
		ad->sigValue.connect(*this);
	}

	void operator()( SscFieldAdapterFactory::signal_frame_t const& sig ) {
		if ( sig.valid ) {
		    std::cout << "Got signal " << sig.key << " " << sig.valid << " ";
		    boost::apply_visitor(visitor(),sig.value);
		    std::cout << " " << ad->descr.label << std::endl;
		} else {
			std::cout << "Got invalid " << sig.key << std::endl;
		}
	}

	struct visitor : public boost::static_visitor<void> {
		template<typename T>
		void operator()( T ) const {}
		void operator()( double d ) const {
			std::cout << d;
		}
	};

protected:
	SscFieldAdapterFactory::adapter_ptr ad;
};

}
}

static bool RUNNING = true;
void signalHandler(int) {
	RUNNING = false;
}

int main( int argc, char** argv ) {

    namespace po = boost::program_options;

    struct {
    	size_t duration; /// seconds
    } cfg;

    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "produce help message")("duration,d",
            po::value<size_t>(&(cfg.duration))->default_value(5),
            "duration in seconds")("field", po::value< std::vector<std::string> >(), "Field");

    po::positional_options_description p;
    p.add("field", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\nComponents:\n";
    	ran::ssc::SscFieldAdapterFactory fac;
    	fac.showFields( std::cout );
        return 1;
    }

    if (vm.count("field")<1) {
    	std::cout << "Field missing\n";
    	return 1;
    }

	ran::ssc::SscFieldAdapterFactory fac;
	auto ad = fac.makeAdapter( vm["field"].as<std::vector<std::string>>()[0] );

	ran::ssc::PrintReceiver pr1(ad);

	signal(SIGINT, signalHandler);

	size_t counter = cfg.duration;
	while(RUNNING && counter >= 1) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		counter--;
	}

	fac.stop();
}
