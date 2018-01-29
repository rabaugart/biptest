/*
 * DepthData.h
 *
 *  Created on: 29.01.2018
 *      Author: netz
 */

#ifndef SHMEM_SSC_DEPTHDATA_H_
#define SHMEM_SSC_DEPTHDATA_H_

#include <iostream>
#include <type_traits>

#include <boost/mpl/vector.hpp>
#include <boost/optional.hpp>

#include <rashm/index_vector.h>

#include "rashm/default_id.h"

namespace ran {
namespace ssc {

struct DepthData {
    bool operator==(DepthData const &) const;
    boost::optional<double> orderedDepth;
    boost::optional<double> actualDepth;
};

std::ostream& operator <<(std::ostream& os, const DepthData& d);

}
}

namespace rashm {

/// The vector of all Ids of TestDataA
template<>
struct index_vector_t<ran::ssc::DepthData> {
    typedef boost::mpl::vector<DefaultId>::type type;
};

}

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, ran::ssc::DepthData & d, const unsigned int version) {
    ar & d.orderedDepth & d.actualDepth;
}

} // namespace serialization
} // namespace boost





#endif /* SHMEM_SSC_DEPTHDATA_H_ */
