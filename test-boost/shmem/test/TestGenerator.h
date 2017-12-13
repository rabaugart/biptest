/*
 * TestGenerator.h
 *
 *  Created on: 05.12.2017
 *      Author: netz
 */

#ifndef SHMEM_TEST_TESTGENERATOR_H_
#define SHMEM_TEST_TESTGENERATOR_H_

#include <random>

#include <boost/optional.hpp>

struct TestGeneratorEngine {

    TestGeneratorEngine();

    template<typename T> T rand();

    template<typename T>
    boost::optional<T> randopt() {
        if ( std::uniform_int_distribution<int>{0,5}(e) > 1 ) {
            return boost::optional<T>(rand<T>());
        }
        return boost::optional<T>();
    }

    template<typename T,size_t N>
    std::array<T,N> randarray() {
        std::array<T,N> ret;
        for ( auto& i : ret ) {
            i = rand<T>();
        }
        return ret;
    }

    std::default_random_engine e;
};

template<typename DATA>
struct TestGenerator {

    TestGenerator();
    DATA next();

    template<typename T> T rand() {
        return e.rand<T>();
    }

    TestGeneratorEngine e;
    DATA current;
};



#endif /* SHMEM_TEST_TESTGENERATOR_H_ */
