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

BOOST_AUTO_TEST_CASE(titer) {
    int numbers_[] = { 0, -1, 4, -3, 5, 8, -2 };
    const int N = sizeof(numbers_) / sizeof(int);

    typedef int* base_iterator;
    base_iterator numbers(numbers_);

    {
        std::ostringstream os;
        // Example using make_filter_iterator()
        std::copy(
                boost::make_filter_iterator<is_positive_number>(numbers,
                        numbers + N),
                boost::make_filter_iterator<is_positive_number>(numbers + N,
                        numbers + N), std::ostream_iterator<int>(os, " "));
        BOOST_TEST_MESSAGE(os.str());
    }

    // Example using filter_iterator
    typedef boost::filter_iterator<is_positive_number, base_iterator> FilterIter;

    is_positive_number predicate;
    FilterIter filter_iter_first(predicate, numbers, numbers + N);
    FilterIter filter_iter_last(predicate, numbers + N, numbers + N);

    {
        std::ostringstream os;
        std::copy(filter_iter_first, filter_iter_last,
                std::ostream_iterator<int>(os, " "));
        BOOST_TEST_MESSAGE(os.str());
    }

    {
        std::ostringstream os;
        // Another example using make_filter_iterator()
        std::copy(
                boost::make_filter_iterator(
                        std::bind2nd(std::greater<int>(), -2), numbers,
                        numbers + N)

                        ,
                boost::make_filter_iterator(
                        std::bind2nd(std::greater<int>(), -2), numbers + N,
                        numbers + N)

                        , std::ostream_iterator<int>(os, " "));

        BOOST_TEST_MESSAGE(os.str());
    }

    {
        using set_t = std::set<int>;
        const set_t s { 5, 7, 9, 8, -2 };

        std::ostringstream os;
        const auto is_even = [](int i) {return i%2==0;};
        const auto is_odd = [](int i) {return i%2==1;};
        const set_t fch { -2, 8 };
        std::copy(boost::make_filter_iterator(is_even, s.begin()),
                boost::make_filter_iterator(is_even, s.end()),
                std::ostream_iterator<int>(os, " "));
        BOOST_TEST_MESSAGE(os.str());

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

        BOOST_CHECK( apply(s,is_even) == fch );
        BOOST_CHECK( sunion(apply(s,is_even),apply(s,is_odd)) == s );
        BOOST_CHECK_EQUAL( sintersection(apply(s,is_even),apply(s,is_odd)).size(), 0 );

    }
}

