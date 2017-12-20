/*
 * UnitTestData.h
 *
 *  Created on: 20.12.2017
 *      Author: netz
 */

#ifndef SHMEM_TEST_UNITTESTDATA_H_
#define SHMEM_TEST_UNITTESTDATA_H_

#include <array>

#include <boost/mpl/vector.hpp>
#include <boost/format.hpp>

#include "rashm/rashm_traits.h"
#include <rashm/index_vector.h>

#include "rashm/default_id.h"

namespace utest {

template<size_t N>
struct UnitTestData {

    std::array<double,N> values;
};

typedef boost::mpl::vector<UnitTestData<10>, UnitTestData<100>>::type udata_vector_t;
}

namespace rashm {

/// The vector of all Ids of TestDataA
template<size_t N>
struct index_vector_t<utest::UnitTestData<N>> {
    typedef boost::mpl::vector<DefaultId>::type type;
};

}

typedef utest::udata_vector_t test_data_vector_t;

#endif /* SHMEM_TEST_UNITTESTDATA_H_ */
