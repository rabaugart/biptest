#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/format.hpp>

#include "TestDataB.h"
#include "rashm/rashm_traits.h"

#include "TestGenerator.h"

std::ostream& operator <<(std::ostream& os, const TestDataB& d) {
    os
            << boost::format("l1:%ul l2:%ul l3:%ul ard: %.1f/%.1f/%.1f")
                    % d.l1 % d.l2 % d.l3 % d.ard[0] % d.ard[1] % d.ard[2];
    return os;
}

bool TestDataB::operator==(TestDataB const & d) const {
    return l1 == d.l1 && l2 == d.l2 && l3 == d.l3;
}

template<>
TestGenerator<TestDataB>::TestGenerator() :
        current { 789, 22, 5792 } {
    current.l2 = rand<long>();
    current.l3 = rand<long>();
}

template<>
TestDataB TestGenerator<TestDataB>::next() {
    current.l1 = rand<long>();
    current.l2++;
    current.l3--;
    typedef decltype(current.ard) atype;
    current.ard = e.randarray<atype::value_type, 3>();
    return current;
}

namespace rashm {

template<>
const std::string DataTraits<TestDataB>::SHM_NAME { "testdb" };

#if defined(FIXED_MAPPING_ADDRESS)
template<>
const long DataTraits<TestDataB>::BASE_ADDRESS { 0x4000000000 };
#endif

template<>
const std::string DataTraits<TIdB1>::SHM_NAME { "tidb1" };

template<>
const std::string DataTraits<TIdB2>::SHM_NAME { "tidb2" };
}


