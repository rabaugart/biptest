#include <string>

#include "TestDataA.h"
#include "rashm/rashm_traits.h"
#include "shmem/shmem_traits.h"

#include "TestGenerator.h"

std::ostream& operator <<(std::ostream& os, const TestDataA& d) {
    os << "TA a:" << d.a << " b:" << d.b;
    return os;
}

bool TestDataA::operator==(TestDataA const & d) const {
    return a == d.a && b == d.b;
}

template<>
TestGenerator<TestDataA>::TestGenerator() : current{ 5.6, 99 } {

}

template<>
TestDataA TestGenerator<TestDataA>::next() {
    return current ;
}

namespace rashm {

template<>
const std::string DataTraits<TestDataA>::SHM_NAME { "testda" };

template<>
const std::string DataTraits<TIdA>::SHM_NAME { "tida" };

}

namespace rshm {

template<>
const ipstring DataTraits<TestDataA>::SHM_NAME { "abc" };

} // end namespace rshm

