
#include <iostream>

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/format.hpp"
#include "boost/filesystem.hpp"

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

BOOST_AUTO_TEST_CASE(future)
{
	BOOST_CHECK_EQUAL(1,1);
}
#if 0
boost::unit_test::test_suite* init_unit_test( int argc, char* argv[] )
{
	return 0;
}

int main(int argc, char**argv)
{
  try
  {
    Message m;

    using namespace boost::gregorian;

    date today = day_clock::local_day();

    std::cout << boost::format("Today: %1%\n") % today;

    std::cout << m.pack(1, 2) << std::endl;
    std::cout << "Pack " << m.pack(1, 2, std::string{ "abc" }, A(), "def", 3) << std::endl;

    std::cout << "M1: " << m << std::endl;

    Message m2{ std::move(m) };

    std::cout << "M2: " << m2 << std::endl;

    Message m3;
    m3 = std::move(m2);

    std::cout << "M3: " << m3 << std::endl;

    m3 = testfun();
    std::cout << "M3b: " << m3 << std::endl;

    const Message m4 = testfun();
    std::cout << "M4: " << m4 << std::endl;

    const std::vector<int> v(3,3);
    m3.pack(v);
    std::cout << "M3c: " << m3 << std::endl;

    ::boost::unit_test::unit_test_main( &init_unit_test, argc, argv );

    std::cout << "Path: " << boost::filesystem::current_path() << std::endl;

  } catch (const std::exception& e)
  {
    std::cout << "===== err =====: " << e.what() << std::endl;
    return 1;
  }
  std::cout << "Ok" << std::endl;
  return 0;
}
#endif
