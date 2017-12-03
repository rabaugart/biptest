#include <string>

#include "TestDataB.h"
#include "rashm/rashm_traits.h"
#include "shmem/shmem_traits.h"

std::ostream& operator << ( std::ostream& os, const TestDataB& d )
{
	os << "a:" << d.a << " b:" << d.b;
	return os;
}

bool TestDataB::operator==( TestDataB const & d ) const {
    return a == d.a && b == d.b;
}

namespace rashm {

template<>
const std::string DataTraits<TestDataB>::SHM_NAME{"testdb"};

template<>
const std::string DataTraits<TIdB1>::SHM_NAME{"tidb1"};

template<>
const std::string DataTraits<TIdB2>::SHM_NAME{"tidb2"};
}

namespace rshm {

template<>
const ipstring DataTraits<TestDataB>::SHM_NAME{"abc"};

} // end namespace rshm

