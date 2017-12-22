/*
 * unitrunner.cpp
 *
 *  Created on: 15.12.2017
 *      Author: netz
 */

#include <sstream>
#include <thread>
#include <chrono>

#include <boost/process.hpp>
#include <boost/filesystem.hpp>

#include "rashm/rashm_utils.h"

#include "all_data_types.h"
#include "UnitTestData.h"
#include "unitconfig.h"

#define BOOST_TEST_MODULE utest
#include <boost/test/included/unit_test.hpp>
#include <boost/test/framework.hpp>
#include <boost/algorithm/string/join.hpp>

namespace bp = boost::process;
namespace bfs = boost::filesystem;
namespace bt = boost::unit_test::framework;

BOOST_AUTO_TEST_CASE( writer1_reader2 ) {

    rashm::create_all_segments<test_data_vector_t>();

    bfs::path p =
            bfs::absolute(bfs::path(bt::master_test_suite().argv[0])).parent_path();

    BOOST_TEST_MESSAGE("Path " << p);
    BOOST_TEST_MESSAGE(
            "Segments " << boost::algorithm::join(rashm::all_segment_names<test_data_vector_t>(),";"));

    utest::unit_config cfg;
    cfg.comp_name = "utestda-100.default";
    cfg.bin_path = p.generic_string();
    cfg.niter = 2000;
    cfg.period = 1000;
    cfg.w_startup_delay = 100;
    cfg.w_shutdown_delay = 500;
    cfg.duration = (cfg.niter * cfg.period * 2) / 1000 + 2000;
    cfg.timeout = (3 * cfg.period) / 1000 + 2;

    utest::process_vec pv;

    for ( size_t i=0; i<3; ++i ) {
        pv.add(utest::COM::READER, cfg );
    }
    pv.add(utest::COM::WRITER, cfg);

    bool const res { pv.waitall() };
    BOOST_CHECK_MESSAGE(res, pv.message);
    for (auto const& pi : pv) {
        for (auto const& li : pi.output) {
            //BOOST_TEST_MESSAGE(li);
        }
        BOOST_TEST_MESSAGE("Result " << pi.id << " " << pi.result);
    }

    rashm::remove_all_segments<test_data_vector_t>();
}

