#if ! defined(RASHMEM_TRAITS_HEADER_239102948)
#define RASHMEM_TRAITS_HEADER_239102948

#include <string>

namespace rashm {

template<typename SDATA> struct DataTraits {
    static const std::string SHM_NAME;
};

template<typename SDATA, typename ID> struct DataIdTraits {
    typedef SDATA data_t;
    typedef ID id_t;
    static std::string name() {
        return DataTraits<data_t>::SHM_NAME + "." + DataTraits<id_t>::SHM_NAME;
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

