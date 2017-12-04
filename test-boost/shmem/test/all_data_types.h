/*
 * all_data_types.h
 *
 *  Created on: 04.12.2017
 *      Author: netz
 */

#ifndef SHMEM_TEST_ALL_DATA_TYPES_H_
#define SHMEM_TEST_ALL_DATA_TYPES_H_

#include "TestDataA.h"
#include "TestDataB.h"

typedef boost::mpl::vector<TestDataA, TestDataB>::type data_vector_t;


#endif /* SHMEM_TEST_ALL_DATA_TYPES_H_ */
