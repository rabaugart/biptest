/*
 * all_ssc_data_types.h
 *
 *  Created on: 29.01.2018
 *      Author: netz
 */

#ifndef SHMEM_SSC_ALL_SSC_DATA_TYPES_H_
#define SHMEM_SSC_ALL_SSC_DATA_TYPES_H_

#include "NdmcData.h"
#include "DepthData.h"

namespace ran {
namespace ssc {

typedef boost::mpl::vector<NdmcData, DepthData>::type data_vector_t;

}
}


#endif /* SHMEM_SSC_ALL_SSC_DATA_TYPES_H_ */
