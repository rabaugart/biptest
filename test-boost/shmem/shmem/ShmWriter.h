/*
 * ShmWriter.h
 *
 *  Created on: 24.10.2017
 *      Author: netz
 */

#ifndef SHMEM_SHMEM_SHMWRITER_H_
#define SHMEM_SHMEM_SHMWRITER_H_

#include "shmem.h"

namespace rshm {

template<typename SDATA, typename ID = ShmDefaultId>
class ShmWriter: public ShmAccessorBase<SDATA, ID> {
public:
	ShmWriter(ShmSegment& seg);

	ShmWriter& operator =(SDATA const & d) {
		boost::interprocess::scoped_lock<boost::interprocess::interprocess_sharable_mutex> lock(
				ShmAccessorBase<SDATA, ID>::fptr->mutex);
		ShmAccessorBase<SDATA, ID>::data = d;
		ShmAccessorBase<SDATA, ID>::fptr->counter++;
		ShmAccessorBase<SDATA, ID>::fptr->timestamp =
				ShmAccessorBase<SDATA, ID>::now();
		ShmAccessorBase<SDATA,ID>::fptr->cond.notify_all();
		return *this;
	}

	void set(const SDATA& d) {
		*this = d;
	}

protected:
};

template<typename SDATA, typename ID>
ShmWriter<SDATA, ID>::ShmWriter(ShmSegment& seg) :
		ShmAccessorBase<SDATA, ID>(seg,boost::interprocess::open_only) {
	ShmAccessorBase<SDATA, ID>::fptr->counter = 0;
}

} // end namespace rshm

#endif /* SHMEM_SHMEM_SHMWRITER_H_ */
