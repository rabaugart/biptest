/*
 * ssc_signal_values.h
 *
 *  Created on: Jan 31, 2018
 *      Author: ran
 */

#ifndef SSC_SHMEM_SSCSHM_SSC_SIGNAL_VALUES_H_
#define SSC_SHMEM_SSCSHM_SSC_SIGNAL_VALUES_H_

#include <string>

#include "AbstractScaleSignal.h"

namespace ran {
namespace ssc {

typedef boost::variant<double,bool,int,std::string,AbstractScaleSignal> signal_values;

}
}



#endif /* SSC_SHMEM_SSCSHM_SSC_SIGNAL_VALUES_H_ */
