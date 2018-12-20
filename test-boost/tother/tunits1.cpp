/*
 * tunits1.cpp
 *
 *  Created on: 20.12.2018
 *      Author: netz
 */


// Boost.Units - A C++ library for zero-overhead dimensional analysis and
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2008 Matthias Christian Schabel
// Copyright (C) 2008 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <boost/units/units_fwd.hpp>

#include <boost/units/base_dimension.hpp>
#include <boost/units/base_unit.hpp>
#include <boost/units/derived_dimension.hpp>
#include <boost/units/make_system.hpp>
#include <boost/units/io.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/static_constant.hpp>
#include <boost/units/unit.hpp>

#include <boost/test/unit_test.hpp>

namespace boost {

namespace units {

struct length_base_dimension : boost::units::base_dimension<length_base_dimension,1> { };       ///> base dimension of length
struct time_base_dimension : boost::units::base_dimension<time_base_dimension,3> { };           ///> base dimension of time

typedef length_base_dimension::dimension_type    length_dimension;
typedef time_base_dimension::dimension_type      time_dimension;

struct length1_base_unit : base_unit<length1_base_unit,length_dimension,1>
{
    static std::string name()               { return "length 1"; }
    static std::string symbol()             { return "l1"; }
};

struct length2_base_unit : base_unit<length2_base_unit,length_dimension,2>
{
    static std::string name()               { return "length2"; }
    static std::string symbol()             { return "l2"; }
};

struct time1_base_unit : base_unit<time1_base_unit,time_dimension,3>
{
    static std::string name()               { return "time1"; }
    static std::string symbol()             { return "t1"; }
};

struct time2_base_unit : base_unit<time2_base_unit,time_dimension,4>
{
    static std::string name()               { return "time2"; }
    static std::string symbol()             { return "t2"; }
};

namespace s1 {

typedef make_system<length1_base_unit,time1_base_unit>::type   system;

/// unit typedefs
typedef unit<dimensionless_type,system>     dimensionless;

typedef unit<length_dimension,system>       length;
typedef unit<time_dimension,system>         time;

/// unit constants
BOOST_UNITS_STATIC_CONSTANT(length1,length);
BOOST_UNITS_STATIC_CONSTANT(time1,time);

} // namespace s1

namespace s2 {

typedef make_system<length2_base_unit,time2_base_unit>::type   system;

/// unit typedefs
typedef unit<dimensionless_type,system>     dimensionless;

typedef unit<length_dimension,system>       length;
typedef unit<time_dimension,system>         time;

/// unit constants
BOOST_UNITS_STATIC_CONSTANT(length2,length);
BOOST_UNITS_STATIC_CONSTANT(time2,time);

} // namespace s2

template<class X,class Y>
struct conversion_helper< quantity<s1::length,X>,quantity<s2::length,Y> >
{
    static quantity<s2::length,Y> convert(const quantity<s1::length,X>& source)
    {
        return quantity<s2::length,Y>::from_value(2.5*source.value());
    }
};

template<class X,class Y>
struct conversion_helper< quantity<s2::length,X>,quantity<s1::length,Y> >
{
    static quantity<s1::length,Y> convert(const quantity<s2::length,X>& source)
    {
        return quantity<s1::length,Y>::from_value((1.0/2.5)*source.value());
    }
};

template<class X,class Y>
struct conversion_helper< quantity<s1::time,X>,quantity<s2::time,Y> >
{
    static quantity<s2::time,Y> convert(const quantity<s1::time,X>& source)
    {
        return quantity<s2::time,Y>::from_value(0.5*source.value());
    }
};

} // namespace units

} // namespace boost

BOOST_AUTO_TEST_CASE(tunit1)
{
    using namespace boost::units;

    quantity<s1::length,double>  l1(1.0*s1::length1);
    BOOST_TEST_MESSAGE(l1);

    quantity<s2::length,double> l2(1.5*l1);
    quantity<s1::length,double>  l3(2.0*l2/3.0);

    BOOST_CHECK_EQUAL(l3,l1);

    quantity<s1::time,double>    t1(1.0*s1::time1);
    quantity<s2::time,double>   t2(1.5*t1);

    using s2t = quantity<s2::time,double>;
    BOOST_CHECK_EQUAL(t2,1.5*s2t(t1));
}

