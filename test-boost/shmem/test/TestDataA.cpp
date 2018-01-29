#include <string>

#include "TestDataA.h"
#include "rashm/rashm_traits.h"

#include "TestGenerator.h"

std::ostream& operator <<(std::ostream& os, const TestDataA& d) {
    os << "a:" << d.a << " b:" << d.b << " o:";
    if (d.opd) {
        os << *d.opd;
    } else {
        os << "--";
    }
    return os;
}

bool TestDataA::operator==(TestDataA const & d) const {
    return a == d.a && b == d.b;
}

template<>
TestGenerator<TestDataA>::TestGenerator() : current{ e.rand<double>(), 99 } {

}

template<>
TestDataA TestGenerator<TestDataA>::next() {
    current.a = e.rand<double>();
    current.b++;
    current.opd = e.randopt<double>();
    return current ;
}

namespace rashm {

template<>
const std::string DataTraits<TestDataA>::SHM_NAME { "testda" };

#if defined(FIXED_MAPPING_ADDRESS)
template<>
const long DataTraits<TestDataA>::BASE_ADDRESS {0x3f00000000};
#endif

template<>
const std::string DataTraits<TIdA>::SHM_NAME { "tida" };

}

