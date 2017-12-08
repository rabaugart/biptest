#if ! defined(RASHMEM_TRAITS_HEADER_239102948)
#define RASHMEM_TRAITS_HEADER_239102948

#include <string>

#if defined(FIXED_MAPPING_ADDRESS)
#include <boost/mpl/begin.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/interprocess/mapped_region.hpp>
#endif

#include "index_vector.h"

namespace rashm {

template<typename SDATA> struct DataTraits {
    static const std::string SHM_NAME;
#if defined(FIXED_MAPPING_ADDRESS)
    static const long BASE_ADDRESS;
#endif
};

template<typename SDATA, typename ID> struct DataIdTraits {
    typedef SDATA data_t;
    typedef ID id_t;
    static std::string name() {
        return DataTraits<data_t>::SHM_NAME + "." + DataTraits<id_t>::SHM_NAME;
    }

#if defined(FIXED_MAPPING_ADDRESS)
    static const void * fixedAddress() {
        typedef typename index_vector_t<SDATA>::type vector_t;
        typedef typename boost::mpl::begin<vector_t>::type first;
        typedef typename boost::mpl::find<vector_t, ID>::type ipos;
        static long const pos = boost::mpl::distance<first, ipos>::value;
        return reinterpret_cast<void*>(DataTraits<data_t>::BASE_ADDRESS
                + pos * boost::interprocess::mapped_region::get_page_size());
    }
#endif
};

struct DataIdTraitsFunctor {
    template<typename DATA, typename ID>
    struct apply {
        typedef DataIdTraits<DATA, ID> type;
    };
};

} // end namespace rshm

#endif

