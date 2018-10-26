/*
 * thana.cpp
 *
 *  Created on: 26.10.2018
 *      Author: netz
 */

#include <string>
#include <vector>

#include <boost/hana.hpp>
namespace ha = boost::hana;

#include <boost/test/unit_test.hpp>

struct Fish { std::string name; };
struct Cat  { std::string name; };
struct Dog  { std::string name; };

BOOST_AUTO_TEST_CASE( hana )
{
    auto animals = ha::make_tuple(Fish{"Nemo"}, Cat{"Garfield"}, Dog{"Snoopy"});
    auto names = ha::transform(animals, [](auto a) {
      return a.name;
    });
    BOOST_CHECK(ha::reverse(names) == ha::make_tuple("Snoopy", "Garfield", "Nemo"));

    std::vector<std::string> vnames;
    ha::for_each( animals, [&vnames]( const auto& i ) { vnames.push_back(i.name); });
    BOOST_CHECK_EQUAL( vnames[0], "Nemo" );
}



