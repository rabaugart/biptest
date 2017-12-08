#if ! defined(RASHMEM_TRAITS_HEADER_239102948)
#define RASHMEM_TRAITS_HEADER_239102948

#include <string>

namespace rashm {

template<typename SDATA> struct DataTraits {
    static const std::string SHM_NAME;
#if defined(FIXED_MAPPING_ADDRESS)
    static const void * BASE_ADDRESS;
#endif
};

template<typename SDATA, typename ID> struct DataIdTraits {
    typedef SDATA data_t;
    typedef ID id_t;
    static std::string name() {
        return DataTraits<data_t>::SHM_NAME + "." + DataTraits<id_t>::SHM_NAME;
    }

    static const void * address() {
        return DataTraits<data_t>::BASE_ADDRESS;
    }
};

struct DataIdTraitsFunctor {
    template<typename DATA, typename ID>
    struct apply {
        typedef DataIdTraits<DATA, ID> type;
    };
};


} // end namespace rshm

#endif

