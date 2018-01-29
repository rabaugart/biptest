/*
 * DepthData.cpp
 *
 *  Created on: 29.01.2018
 *      Author: netz
 */

#include "rashm/rashm_traits.h"
#include "DepthData.h"

namespace ran {
namespace ssc {

std::ostream& operator <<(std::ostream& os, const DepthData& d) {
    if ( d.orderedDepth ) {
        os << *d.orderedDepth;
    } else {
        os << "---";
    }
    os << "/";
    if ( d.actualDepth ) {
        os << *d.actualDepth;
    } else {
        os << "---";
    }
    return os;
}

}
}

namespace rashm {

template<>
const std::string DataTraits<ran::ssc::DepthData>::SHM_NAME { "depth" };

#if defined(FIXED_MAPPING_ADDRESS)
template<>
const long DataTraits<ran::ssc::DepthData>::BASE_ADDRESS {0x4000000000};
#endif

}
