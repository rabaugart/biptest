/*
 * AbstractScaleSignal.h
 *
 *  Created on: Jan 31, 2018
 *      Author: ran
 */

#ifndef SSC_SHMEM_SSCSHM_ABSTRACTSCALESIGNAL_H_
#define SSC_SHMEM_SSCSHM_ABSTRACTSCALESIGNAL_H_

namespace ran {
namespace ssc {

/**
 * The signal to be used in connection with AbstractScaleWidgets
 */
struct AbstractScaleSignal {
	double currentValue;
	bool currentValueEnable;
};

}
}



#endif /* SSC_SHMEM_SSCSHM_ABSTRACTSCALESIGNAL_H_ */
