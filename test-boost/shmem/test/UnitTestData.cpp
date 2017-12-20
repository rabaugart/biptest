/*
 * UnitTestData.cpp
 *
 *  Created on: 20.12.2017
 *      Author: netz
 */

#include <test/TestGenerator.h>
#include <test/UnitTestData.h>

namespace utest {

}

namespace rashm {

template<>
const std::string DataTraits<::utest::UnitTestData<10>>::SHM_NAME { "utestda-10" };
template<>
const std::string DataTraits<::utest::UnitTestData<100>>::SHM_NAME {
        "utestda-100" };

#if defined(FIXED_MAPPING_ADDRESS)
template<>
const long DataTraits<::utest::UnitTestData<10>>::BASE_ADDRESS { 0x5f00000000 };
#endif

#if defined(FIXED_MAPPING_ADDRESS)
template<>
const long DataTraits<::utest::UnitTestData<100>>::BASE_ADDRESS { 0x6f00000000 };
#endif

}

template<>
TestGenerator<::utest::UnitTestData<10>>::TestGenerator() :
        current { e.randarray<double, 10>() } {
}
template<>
TestGenerator<::utest::UnitTestData<100>>::TestGenerator() :
        current { e.randarray<double, 100>() } {

}

template<>
::utest::UnitTestData<10> TestGenerator<::utest::UnitTestData<10>>::next() {
    current.values = e.randarray<double, 10>();
    return current;
}

template<>
::utest::UnitTestData<100> TestGenerator<::utest::UnitTestData<100>>::next() {
    current.values = e.randarray<double, 100>();
    return current;
}

