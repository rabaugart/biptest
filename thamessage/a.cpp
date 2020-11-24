
#include <iostream>

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/format.hpp"
#include "boost/filesystem.hpp"
#include "boost/lexical_cast.hpp"

#define BOOST_TEST_MODULE test_mod_1
#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "message.hpp"

struct A
{
  A() : a(55), s("sta") {}
  int a;
  std::string s;
};

template<>
void Message::serialize( std::ostream& os, const A& a )
{
  os << "structa:";
  serialize(os,a.a,a.s);
}

Message testfun()
{
  Message m;
  m.pack(55,66);
  return m;
}

BOOST_AUTO_TEST_CASE(dummy)
{
	BOOST_CHECK_EQUAL(1,1);
}

BOOST_AUTO_TEST_CASE(message) {

	Message m;

	using namespace boost::gregorian;

	date today = day_clock::local_day();

	BOOST_TEST_MESSAGE( boost::format("Today: %1%\n") % today );

	BOOST_TEST_MESSAGE(  m.pack(1, 2) );
	BOOST_TEST_MESSAGE( "Pack " << m.pack(1, 2, std::string { "abc" }, A(), "def", 3) );

	BOOST_TEST_MESSAGE( "M1: " << m );

	Message m2 { std::move(m) };

	BOOST_TEST_MESSAGE( "M2: " << m2 );

	Message m3;
	m3 = std::move(m2);

	BOOST_TEST_MESSAGE( "M3: " << m3 );

	m3 = testfun();
	BOOST_TEST_MESSAGE( "M3b: " << m3 );

	const Message m4 = testfun();
	BOOST_TEST_MESSAGE( "M4: " << m4 );

	const std::vector<int> v(3, 3);
	m3.pack(v);
	BOOST_TEST_MESSAGE( "M3c: " << m3 );
	BOOST_CHECK( boost::lexical_cast<std::string>(m3) == "int:3;int:3;int:3" );

	BOOST_TEST_MESSAGE("Path: " << boost::filesystem::current_path());
}

