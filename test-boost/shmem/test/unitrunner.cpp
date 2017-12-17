/*
 * unitrunner.cpp
 *
 *  Created on: 15.12.2017
 *      Author: netz
 */

#include <strstream>
#include <thread>
#include <chrono>

#include <boost/process.hpp>

#include "unitconfig.h"

#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

namespace bp = boost::process;

std::string const runner { "unitproc" };
std::string const runnerpath { "shmem/" + runner };

BOOST_AUTO_TEST_CASE( writer1_reader2 ) {
    utest::unit_config cfg;
    cfg.comp_name = "testda.default";
    cfg.niter = 2000;
    cfg.period = 1000;
    cfg.duration = (cfg.niter * cfg.period * 2) / 1000 + 2000;
    cfg.timeout = (3 * cfg.period) / 1000 + 2;

    utest::process_vec pv;

    pv.add( "rs", cfg, "r1" );
    pv.add( "rs", cfg, "r2" );
    pv.add( "as", cfg);

    bool const res{pv.waitall()};
    BOOST_CHECK_MESSAGE(res,pv.message);

}

