
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

	std::cout << boost::format("Today: %1%\n") % today;

	std::cout << m.pack(1, 2) << std::endl;
	BOOST_TEST_MESSAGE( "Pack " << m.pack(1, 2, std::string { "abc" }, A(), "def", 3) );

	std::cout << "M1: " << m << std::endl;

	Message m2 { std::move(m) };

	std::cout << "M2: " << m2 << std::endl;

	Message m3;
	m3 = std::move(m2);

	std::cout << "M3: " << m3 << std::endl;

	m3 = testfun();
	std::cout << "M3b: " << m3 << std::endl;

	const Message m4 = testfun();
	std::cout << "M4: " << m4 << std::endl;

	const std::vector<int> v(3, 3);
	m3.pack(v);
	BOOST_TEST_MESSAGE( "M3c: " << m3 );
	BOOST_CHECK( boost::lexical_cast<std::string>(m3) == "int:3;int:3;int:3" );

	BOOST_TEST_MESSAGE("Path: " << boost::filesystem::current_path());
}

