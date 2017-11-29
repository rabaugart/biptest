
#if ! defined(SHMEM_TRAITS_HEADER_239102948)
#define SHMEM_TRAITS_HEADER_239102948

#include <string>

#include "shmem/shmem_base.h"

namespace rshm {

template<typename SDATA> struct DataTraits
{
	static const ipstring SHM_NAME;
};

} // end namespace rshm

#endif

