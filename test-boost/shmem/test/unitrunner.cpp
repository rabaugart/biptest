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

BOOST_AUTO_TEST_CASE( writer1_reader2 )
{
    unit_config cfg;
    cfg.comp_name = "testda.default";
    cfg.duration = 2;
    cfg.niter = 5000;
    cfg.period = 100;

    bp::child cr(runnerpath,bp::args={"rs",cfg.toString(),"r1"});
    BOOST_TEST_MESSAGE( "Reader 1 started " << cr.id() );

    bp::child cr2(runnerpath,bp::args={"rs",cfg.toString(),"r2"});
    BOOST_TEST_MESSAGE( "Reader 2 started " << cr.id() );

    // Give the readers some time for startup
    std::this_thread::sleep_for( std::chrono::milliseconds(50) );

    bp::child cw(runnerpath,bp::args={"as",cfg.toString()});

    cw.wait(); //wait for the process to exit
    BOOST_CHECK_EQUAL(cw.exit_code(), 0);
    cr.wait();
    BOOST_CHECK_EQUAL(cr.exit_code(), 0);
    cr2.wait();
    BOOST_CHECK_EQUAL(cr2.exit_code(), 0);
}

