/*
 * TestGenerator.cpp
 *
 *  Created on: 13.12.2017
 *      Author: netz
 */

#include <boost/date_time.hpp>

#include "TestGenerator.h"

TestGeneratorEngine::TestGeneratorEngine() {
e.seed(static_cast<unsigned int>((boost::posix_time::microsec_clock::universal_time()-
        boost::posix_time::ptime(boost::gregorian::date(1970,1,1))).total_nanoseconds()));
}

template<>
long TestGeneratorEngine::rand<long>() {
std::uniform_int_distribution<long> d { 100, 1000 };
return d(e);
}

template<>
double TestGeneratorEngine::rand<double>() {
std::normal_distribution<double> d { 3.0, 10.0 };
return d(e);
}

