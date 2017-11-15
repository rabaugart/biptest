/*
 * shmem.cpp
 *
 *  Created on: 24.10.2017
 *      Author: netz
 */

#include "shmem.h"

namespace rshm {

namespace bip = boost::interprocess;

ShmSegment::ShmSegment() :
		shm(bip::open_or_create, SHM_FRAME_NAME, 5000) {
}

ShmSegment::shm_remove::shm_remove() {
	bip::shared_memory_object::remove(ShmSegment::SHM_FRAME_NAME);
}
ShmSegment::shm_remove::~shm_remove() {
	bip::shared_memory_object::remove(ShmSegment::SHM_FRAME_NAME);
}

} // end namespace rshm
