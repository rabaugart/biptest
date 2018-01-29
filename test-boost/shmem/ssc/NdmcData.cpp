/*
 * NdmcData.cpp
 *
 *  Created on: 29.01.2018
 *      Author: netz
 */

#include "rashm/rashm_traits.h"
#include "NdmcData.h"

namespace ran {
namespace ssc {

std::ostream& operator <<(std::ostream& os, const NdmcData& d) {
    if ( d.depth ) {
        os << *d.depth;
    } else {
        os << "---";
    }
    return os;
}

}
}

namespace rashm {

template<>
const std::string DataTraits<ran::ssc::NdmcData>::SHM_NAME { "ndmc" };

#if defined(FIXED_MAPPING_ADDRESS)
template<>
const long DataTraits<ran::ssc::NdmcData>::BASE_ADDRESS {0x3f00000000};
#endif

}
