/*
 * tflat_map.cpp
 *
 *  Created on: 26.10.2018
 *      Author: netz
 */

#include <algorithm>

#include <boost/container/flat_set.hpp>
#include <boost/mpl/list.hpp>

#define BOOST_TEST_MODULE tflat

#include <boost/test/unit_test.hpp>

using namespace boost::container;

using testset_t = flat_set<int>;

static const testset_t TSET{1,3,5};

BOOST_AUTO_TEST_CASE(tflat)
{
    BOOST_CHECK_EQUAL(TSET.size(),3);
    BOOST_CHECK_EQUAL( TSET.count(5), 1 );
    BOOST_CHECK( std::find(TSET.begin(),TSET.end(),5) != TSET.end() );
    BOOST_CHECK_EQUAL( TSET.count(7), 0 );
    BOOST_CHECK( std::find(TSET.begin(),TSET.end(),7) == TSET.end() );
}

typedef boost::mpl::list<int,unsigned int> test_types;

BOOST_AUTO_TEST_CASE_TEMPLATE( my_test, T, test_types )
{
  BOOST_TEST( sizeof(T) == (unsigned)4 );
}

