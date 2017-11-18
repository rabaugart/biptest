/*
 * ShmReader.h
 *
 *  Created on: 24.10.2017
 *      Author: netz
 */

#ifndef SHMEM_SHMEM_SHMREADER_H_
#define SHMEM_SHMEM_SHMREADER_H_

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/microsec_time_clock.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
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

	SDATA wait()
	{
		shared_scoped_lock lck(ShmAccessorBase<SDATA,ID>::fptr->mutex);
		ShmAccessorBase<SDATA,ID>::fptr->cond.wait(lck);
		const auto data = getNoLock();
		return data;
	}

	SDATA timed_wait_for(boost::posix_time::microseconds ms)
	{
		const boost::posix_time::ptime end = boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time() + ms;
		shared_scoped_lock lck(ShmAccessorBase<SDATA,ID>::fptr->mutex);
		if ( ShmAccessorBase<SDATA,ID>::fptr->cond.timed_wait(lck,end) ) {
			return getNoLock();
		}
		throw std::runtime_error("Timeout");
	}

protected:
	struct shared_scoped_lock : boost::interprocess::sharable_lock<boost::interprocess::interprocess_sharable_mutex> {
		shared_scoped_lock( boost::interprocess::interprocess_sharable_mutex& mtx )
		: boost::interprocess::sharable_lock<boost::interprocess::interprocess_sharable_mutex>(mtx) {
		}
		~shared_scoped_lock() {
			unlock();
		}
	};

	SDATA getNoLock()
	{
		return ShmAccessorBase<SDATA,ID>::data;
	}
};

} // end namespace rshm

#endif /* SHMEM_SHMEM_SHMREADER_H_ */
