/*
 * SegmentReader.h
 *
 *  Created on: 02.12.2017
 *      Author: netz
 */

#ifndef SHMEM_RASHM_SEGMENTREADER_H_
#define SHMEM_RASHM_SEGMENTREADER_H_

#include <boost/interprocess/sync/sharable_lock.hpp>

#include "Segment.h"

namespace rashm {

template<typename DATA, typename ID=DefaultId>
class SegmentReader : public Segment<DATA,ID> {

public:

    typedef Segment<DATA,ID> base_t;
    typedef DATA data_t;

    SegmentReader() {
    }

    data_t get() {
        auto l{sharable_lock()};
        return base_t::frame->data;
    }

    data_t wait() {
        auto l{sharable_lock()};
        base_t::frame->condition.wait(l);
        return base_t::frame->data;
    }

    ~SegmentReader() {
    }

protected:

    typedef boost::interprocess::sharable_lock<typename base_t::frame_t::mutex_t> sharable_lock_t;

    sharable_lock_t sharable_lock() {
        return sharable_lock_t(base_t::frame->mutex);
    }
};

}

#endif /* SHMEM_RASHM_SEGMENTREADER_H_ */
