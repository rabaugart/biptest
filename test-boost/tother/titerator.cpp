/*
 * titerator.cpp
 *
 *  Created on: 27.10.2018
 *      Author: netz
 */

//  (C) Copyright Jeremy Siek 1999-2004.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/config.hpp>
#include <algorithm>
#include <functional>
#include <sstream>
#include <set>

#include <boost/container/flat_set.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/mpl/list.hpp>

#include <boost/test/unit_test.hpp>

struct is_positive_number {
    bool operator()(int x) {
        return 0 < x;
    }
};

template<typename SET, typename PRED>
SET apply( const SET& s, PRED p )
{
    SET ret;
    std::copy_if(s.begin(),s.end(),std::inserter(ret,ret.begin()),p);
    return ret;
}

template<typename SET>
SET sunion( const SET& a, const SET& b )
{
    SET ret;
    std::set_union( a.begin(), a.end(), b.begin(), b.end(),
            std::inserter(ret,ret.begin()));
    return ret;
}

template<typename SET>
SET sintersection( const SET& a, const SET& b )
{
    SET ret;
    std::set_intersection( a.begin(), a.end(), b.begin(), b.end(),
            std::inserter(ret,ret.begin()));
    return ret;
}

template<typename T>
std::string str( const T& s )
{
    std::ostringstream os;
    std::copy( s.begin(), s.end(), std::ostream_iterator<typename T::value_type>(os, " "));
    return os.str();
}

template<typename T>
std::string str( T begin, T end )
{
    std::ostringstream os;
    std::copy( begin, end, std::ostream_iterator<typename T::value_type>(os, " "));
    return os.str();
}

BOOST_AUTO_TEST_CASE(titer) {
    int numbers_[] = { 0, -1, 4, -3, 5, 8, -2 };
    const int N = sizeof(numbers_) / sizeof(int);

    typedef int* base_iterator;
    base_iterator numbers(numbers_);

    {
        // Example using make_filter_iterator()
        const auto st = str(
                boost::make_filter_iterator<is_positive_number>(numbers,
                        numbers + N),
                boost::make_filter_iterator<is_positive_number>(numbers + N,
                        numbers + N));
        BOOST_TEST_MESSAGE(st);
    }

    // Example using filter_iterator
    typedef boost::filter_iterator<is_positive_number, base_iterator> FilterIter;

    is_positive_number predicate;
    FilterIter filter_iter_first(predicate, numbers, numbers + N);
    FilterIter filter_iter_last(predicate, numbers + N, numbers + N);

    {
        const auto st = str(filter_iter_first, filter_iter_last);
        BOOST_TEST_MESSAGE(st);
    }

    {
        // Another example using make_filter_iterator()
        const auto gtm2 = []( auto i ) { return i > -2; };
        const auto st = str(
                boost::make_filter_iterator(
                        gtm2, numbers, numbers + N),
                boost::make_filter_iterator(
                        gtm2, numbers + N,
                        numbers + N));
        BOOST_TEST_MESSAGE(st);
    }

    BOOST_CHECK(true);
}

typedef boost::mpl::list<boost::container::flat_set<int>,std::set<int>> test_types;

template<typename T>
struct test_traits
{
    static std::string name();
};

template<>
struct test_traits<boost::container::flat_set<int>>
{
    static std::string name() { return "flat_set"; }
};

template<>
struct test_traits<std::set<int>>
{
    static std::string name() { return "set"; }
};

BOOST_AUTO_TEST_CASE_TEMPLATE( iter2, set_t, test_types )
{
    BOOST_TEST_MESSAGE( "Testing " << test_traits<set_t>::name() );
    const set_t s { 5, 7, 9, 8, -2 };

    const auto is_even = [](int i) {return i%2==0;};
    const auto is_odd = [](int i) {return i%2==1;};
    const set_t fch { -2, 8 };
    const auto st = str(boost::make_filter_iterator(is_even, s.begin()),
            boost::make_filter_iterator(is_even, s.end()));
    BOOST_TEST_MESSAGE(st);

    {
        set_t fs;
        std::copy(boost::make_filter_iterator(is_even, s.begin()),
                boost::make_filter_iterator(is_even, s.end()),
                std::inserter(fs, fs.begin()));
        BOOST_CHECK(fs == fch);
    }

    {
        set_t fs;
        std::copy_if(s.begin(), s.end(),
                std::inserter(fs, fs.begin()), is_even);
        BOOST_CHECK(fs == fch);
    }

    {
        auto fil = []( auto set, auto pred ) {
            decltype(set) ret;
            std::copy_if(set.begin(),set.end(), std::inserter(ret,ret.begin()),pred);
            return ret;
        };

        BOOST_CHECK( fil(s,is_even) == fch );
    }

    BOOST_TEST_MESSAGE( str(apply(s,is_even)));
    BOOST_CHECK( apply(s,is_even) == fch );
    BOOST_CHECK( sunion(apply(s,is_even),apply(s,is_odd)) == s );
    BOOST_CHECK_EQUAL( sintersection(apply(s,is_even),apply(s,is_odd)).size(), 0 );

}


