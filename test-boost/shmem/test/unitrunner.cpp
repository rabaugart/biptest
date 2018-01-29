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

#include <boost/variant.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/variant.hpp>

#include "rashm/rashm_utils.h"

#include "all_data_types.h"
#include "rashm/data_functors.h"
#include "rashm/Packet.h"

#include "UnitTestData.h"
#include "unitconfig.h"
#include "test/TestGenerator.h"

#define BOOST_TEST_MODULE utest
#include <boost/test/included/unit_test.hpp>
#include <boost/test/framework.hpp>
#include <boost/algorithm/string/join.hpp>

namespace bp = boost::process;
namespace bfs = boost::filesystem;
namespace bt = boost::unit_test::framework;

BOOST_AUTO_TEST_CASE( writer1_reader2_medium ) {

	rashm::create_all_segments<test_data_vector_t>();

	bfs::path p =
			bfs::absolute(bfs::path(bt::master_test_suite().argv[0])).parent_path();

	utest::unit_config cfg;
	cfg.comp_name = "utestda-100.default";
	cfg.bin_path = p.generic_string();
	cfg.niter = 2000;
	cfg.period = 1000;
	cfg.w_startup_delay = 100;
	cfg.w_shutdown_delay = 500;
	cfg.duration = (cfg.niter * cfg.period * 2) / 1000 + 2000;
	cfg.timeout = (3 * cfg.period) / 1000 + 2;
	BOOST_TEST_MESSAGE("=== Starting writer1_reader2_medium " << cfg);

	utest::process_vec pv;

	for (size_t i = 0; i < 3; ++i) {
		pv.add(utest::COM::READER, cfg);
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

BOOST_AUTO_TEST_CASE( writer1_reader2_fast ) {

	rashm::create_all_segments<test_data_vector_t>();

	bfs::path p =
			bfs::absolute(bfs::path(bt::master_test_suite().argv[0])).parent_path();

	utest::unit_config cfg;
	cfg.comp_name = "utestda-100.default";
	cfg.bin_path = p.generic_string();
	cfg.niter = 5000;
	cfg.period = 200;
	cfg.w_startup_delay = 100;
	cfg.w_shutdown_delay = 500;
	cfg.duration = (cfg.niter * cfg.period * 2) / 1000 + 2000;
	cfg.timeout = (3 * cfg.period) / 1000 + 2;
	BOOST_TEST_MESSAGE("=== Starting writer1_reader2_fast " << cfg);

	utest::process_vec pv;

	for (size_t i = 0; i < 3; ++i) {
		pv.add(utest::COM::READER, cfg);
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

BOOST_AUTO_TEST_CASE( writer1_reader2_slow ) {

	rashm::create_all_segments<test_data_vector_t>();

	bfs::path p =
			bfs::absolute(bfs::path(bt::master_test_suite().argv[0])).parent_path();

	utest::unit_config cfg;
	cfg.comp_name = "utestda-100.default";
	cfg.bin_path = p.generic_string();
	cfg.niter = 200;
	cfg.period = 5000;
	cfg.w_startup_delay = 100;
	cfg.w_shutdown_delay = 500;
	cfg.duration = (cfg.niter * cfg.period * 2) / 1000 + 2000;
	cfg.timeout = (3 * cfg.period) / 1000 + 2;
	BOOST_TEST_MESSAGE("=== Starting writer1_reader2_slow " << cfg);

	utest::process_vec pv;

	for (size_t i = 0; i < 3; ++i) {
		pv.add(utest::COM::READER, cfg);
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

BOOST_AUTO_TEST_CASE( serialization ) {

	typedef rashm::apply_all_data_ids<data_vector_t, rashm::packet_functor_t>::type all_packets_t;

	typedef boost::make_variant_over<all_packets_t>::type all_packet_variant_t;

	all_packet_variant_t p;

	typedef TestDataB test_t;
	TestGenerator<test_t> gen;
	typedef rashm::Packet<test_t, TIdB2> test_packet_t;
	test_packet_t pa { rashm::Header(), gen.next() };
	p = pa;

	std::ostringstream os;

	{
		boost::archive::text_oarchive oa(os);
		// write class instance to archive
		oa << p;
	}

	{
		std::istringstream is(os.str());
		boost::archive::text_iarchive ia(is);
		all_packet_variant_t p2;
		ia >> p2;
		BOOST_CHECK(p == p2);
	}

}
