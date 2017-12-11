/*
 * TestGenerator.h
 *
 *  Created on: 05.12.2017
 *      Author: netz
 */

#ifndef SHMEM_TEST_TESTGENERATOR_H_
#define SHMEM_TEST_TESTGENERATOR_H_

#include <random>

template<typename DATA>
struct TestGenerator {
    TestGenerator();
    DATA next();
    DATA current;

    long randlong() {
        std::uniform_int_distribution<long> d{100,1000};
        return d(e);
    }

    std::default_random_engine e;
};



#endif /* SHMEM_TEST_TESTGENERATOR_H_ */
