/*
 * TestGenerator.h
 *
 *  Created on: 05.12.2017
 *      Author: netz
 */

#ifndef SHMEM_TEST_TESTGENERATOR_H_
#define SHMEM_TEST_TESTGENERATOR_H_

template<typename DATA>
struct TestGenerator {
    TestGenerator();
    DATA next();
    DATA current;
};



#endif /* SHMEM_TEST_TESTGENERATOR_H_ */
