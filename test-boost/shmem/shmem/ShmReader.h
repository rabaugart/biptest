/*
 * ShmReader.h
 *
 *  Created on: 24.10.2017
 *      Author: netz
 */

#ifndef SHMEM_SHMEM_SHMREADER_H_
#define SHMEM_SHMEM_SHMREADER_H_

#include "shmem.h"

namespace rshm {

template<typename SDATA,typename ID=ShmDefaultId>
class ShmReader : public ShmAccessorBase<SDATA,ID>
{
public:
	ShmReader( ShmSegment& seg ) : ShmAccessorBase<SDATA,ID>(seg,boost::interprocess::open_or_create) {}

	SDATA get()
	{
		boost::interprocess::scoped_lock<boost::interprocess::interprocess_sharable_mutex> lock(ShmAccessorBase<SDATA,ID>::fptr->mutex);
		return ShmAccessorBase<SDATA,ID>::data;
	}

protected:
	SDATA getNoLock()
	{
		return ShmAccessorBase<SDATA,ID>::data;
	}
};

} // end namespace rshm

#endif /* SHMEM_SHMEM_SHMREADER_H_ */
