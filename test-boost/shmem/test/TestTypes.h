/*
 * TestTypes.h
 *
 *  Created on: 27.01.2018
 *      Author: netz
 */

#ifndef SHMEM_TEST_TESTTYPES_H_
#define SHMEM_TEST_TESTTYPES_H_

#include <string>

#include <boost/variant/variant.hpp>

namespace utest {

typedef boost::variant<double,bool,int,std::string> signal_values;

}



#endif /* SHMEM_TEST_TESTTYPES_H_ */
