/*
 * NdmcData.h
 *
 *  Created on: 29.01.2018
 *      Author: netz
 */

#ifndef SHMEM_SSC_NDMCDATA_H_
#define SHMEM_SSC_NDMCDATA_H_

#include <iostream>
#include <type_traits>

#include <boost/mpl/vector.hpp>
#include <boost/optional.hpp>
#include <boost/serialization/optional.hpp>

#include <rashm/index_vector.h>

#include "rashm/default_id.h"

namespace ran {
namespace ssc {

struct NdmcData {
    bool operator==(NdmcData const &) const;
    boost::optional<double> depth;
    boost::optional<double> dbk;
};

std::ostream& operator <<(std::ostream& os, const NdmcData& d);

}
}

namespace rashm {

/// The vector of all Ids of TestDataA
template<>
struct index_vector_t<ran::ssc::NdmcData> {
    typedef boost::mpl::vector<DefaultId>::type type;
};

}

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, ran::ssc::NdmcData & d, const unsigned int version) {
    ar & d.depth & d.dbk;
}

} // namespace serialization
} // namespace boost


#endif /* SHMEM_SSC_NDMCDATA_H_ */
