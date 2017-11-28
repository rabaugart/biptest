/*
 * shmem.h
 *
 *  Created on: 24.10.2017
 *      Author: netz
 */

#ifndef SHMEM_SHMEM_SHMEM_H_
#define SHMEM_SHMEM_SHMEM_H_

#include <chrono>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_sharable_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/interprocess_condition_any.hpp>

#include "shmem_traits.h"

namespace rshm {

struct ShmDefaultId
{
	static std::string name() { return "default"; }
};

template<typename SDATA>
struct DataFrame
{
	typedef std::chrono::steady_clock clock_type;
	typedef std::chrono::time_point<clock_type> time_point_type;
	typedef SDATA data_type;
	boost::interprocess::interprocess_sharable_mutex mutex;
	boost::interprocess::interprocess_condition_any cond;
	unsigned long long counter;
	time_point_type timestamp;
	SDATA data;
};

class ShmSegment
{
public:
	struct shm_remove
	{
		shm_remove();
		~shm_remove();
	};

	template<typename FDATA>
	typename boost::interprocess::managed_shared_memory::segment_manager::construct_proxy<FDATA>::type
	  	  construct(const std::string& name);

	static constexpr char const* SHM_FRAME_NAME = "MySharedMemory";

	ShmSegment();

private:
	boost::interprocess::managed_shared_memory shm;
};

template<typename FDATA>
typename boost::interprocess::managed_shared_memory::segment_manager::construct_proxy<FDATA>::type
	  	 ShmSegment::construct(const std::string& name)
{
	return shm.find_or_construct<FDATA>(name.c_str());
}

template<typename SDATA,typename ID = ShmDefaultId>
class ShmAccessorBase
{
public:

	typedef typename DataFrame<SDATA>::time_point_type time_point_type;

	unsigned long long getCounter() const { return fptr->counter; }

	time_point_type getTimestamp() const { return fptr->timestamp; }

	static time_point_type now()  { return DataFrame<SDATA>::clock_type::now(); }

	static const std::string seg_name;

protected:
	template<typename OC>
	ShmAccessorBase( ShmSegment& seg, OC ooc );

	typedef typename boost::interprocess::managed_shared_memory::segment_manager::construct_proxy<DataFrame<SDATA>>::type proxy_type;
	proxy_type proxy;
	DataFrame<SDATA>* fptr;
	SDATA& data;
};

template<typename SDATA,typename ID>
const std::string ShmAccessorBase<SDATA,ID>::seg_name = DataTraits<SDATA>::SHM_NAME+"_" + ID::name();

template<typename SDATA,typename ID>
template<typename OC>
ShmAccessorBase<SDATA,ID>::ShmAccessorBase( ShmSegment& seg, OC ooc ) :
	proxy( seg.construct<DataFrame<SDATA>>(seg_name) ),
	fptr(proxy()), data(fptr->data)
{
}

} // end namespace rshm

#endif /* SHMEM_SHMEM_SHMEM_H_ */
