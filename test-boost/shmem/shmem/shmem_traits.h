
#if ! defined(SHMEM_TRAITS_HEADER_239102948)
#define SHMEM_TRAITS_HEADER_239102948

#include <string>

namespace rshm {

template<typename SDATA> struct DataTraits
{
	static const std::string SHM_NAME;
};

} // end namespace rshm

#endif

