#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>
#include "boost/date_time/gregorian/gregorian_types.hpp"

#include "TestDataB.h"
#include "rashm/rashm_traits.h"
#include "shmem/shmem_traits.h"

#include "TestGenerator.h"

std::ostream& operator << ( std::ostream& os, const TestDataB& d )
{
	os << "TB l1:" << d.l1 << " l2:" << d.l2 << " l3:" << d.l3;
	return os;
}

bool TestDataB::operator==( TestDataB const & d ) const {
    return l1 == d.l1 && l2 == d.l2 && l3 == d.l3;
}

template<>
TestGenerator<TestDataB>::TestGenerator() : current { 789, 22, 5792 } {
    e.seed((boost::posix_time::microsec_clock::universal_time()-boost::posix_time::ptime(boost::gregorian::date(1970,1,1))).total_nanoseconds());
    current.l2 = randlong();
    current.l3 = randlong();
}

template<>
TestDataB TestGenerator<TestDataB>::next() {
    current.l1 = randlong();
    current.l2++;
    current.l3--;
    return current;
}


namespace rashm {

template<>
const std::string DataTraits<TestDataB>::SHM_NAME{"testdb"};

#if defined(FIXED_MAPPING_ADDRESS)
template<>
const long DataTraits<TestDataB>::BASE_ADDRESS {0x4000000000};
#endif

template<>
const std::string DataTraits<TIdB1>::SHM_NAME{"tidb1"};

template<>
const std::string DataTraits<TIdB2>::SHM_NAME{"tidb2"};
}

namespace rshm {

template<>
const ipstring DataTraits<TestDataB>::SHM_NAME{"abc"};

} // end namespace rshm

