#include <string>

#include "TestDataA.h"
#include "rashm/rashm_traits.h"
#include "shmem/shmem_traits.h"

std::ostream& operator << ( std::ostream& os, const TestDataA& d )
{
	os << "a:" << d.a << " b:" << d.b;
	return os;
}

bool TestDataA::operator==( TestDataA const & d ) const {
    return a == d.a && b == d.b;
}

namespace rashm {

template<>
const std::string DataTraits<TestDataA>::SHM_NAME{"testda"};

template<>
const std::string DataTraits<TIdA>::SHM_NAME{"tida"};

}

namespace rshm {

template<>
const ipstring DataTraits<TestDataA>::SHM_NAME{"abc"};

} // end namespace rshm

