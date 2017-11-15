#include "TestDataA.h"
#include "shmem/shmem_traits.h"

std::ostream& operator << ( std::ostream& os, const TestDataA& d )
{
	os << "a:" << d.a << " b:" << d.b;
	return os;
}

namespace rshm {

template<>
const std::string DataTraits<TestDataA>::SHM_NAME{"abc"};

} // end namespace rshm

