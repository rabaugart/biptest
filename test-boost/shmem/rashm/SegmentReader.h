/*
 * SegmentReader.h
 *
 *  Created on: 02.12.2017
 *      Author: netz
 */

#ifndef SHMEM_RASHM_SEGMENTREADER_H_
#define SHMEM_RASHM_SEGMENTREADER_H_

#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>

#include "Segment.h"

namespace rashm {

struct timeout_error : public shm_error {
    timeout_error() : shm_error("Reader timeout") {}
};

template<typename DATA, typename ID = DefaultId>
class SegmentReader: public Segment<DATA, ID> {

public:

    typedef Segment<DATA, ID> base_t;
    typedef DATA data_t;

    SegmentReader() :
            lastRecvTime() {
    }

    SegmentReader(boost::interprocess::open_only_t) :
            Segment<DATA, ID>(boost::interprocess::open_only), lastRecvTime() {
    }

    data_t get() {
        auto l { sharable_lock() };
        return base_t::frame->data;
    }

    data_t wait() {
        auto l { sharable_lock() };
        base_t::frame->condition.wait(l);
        return base_t::frame->data;
    }

    data_t timed_wait_for(boost::posix_time::microseconds ms) {
        const boost::posix_time::ptime end = boost::date_time::microsec_clock<
                boost::posix_time::ptime>::universal_time() + ms;
        auto lck { sharable_lock() };
        if (base_t::frame->condition.timed_wait(lck, end)) {
            lastHead = base_t::frame->head;
            lastRecvTime = now();
            age = lastRecvTime-lastHead.timestamp;
            return base_t::frame->data;
        }
        throw timeout_error();
    }

    ~SegmentReader() {
    }

    Header const& lastReceivedHeader() const {
        return lastHead;
    }

    timestamp_t lastReceptionTime() const {
        return lastRecvTime;
    }

    duration_t lastAge() const {
        return age;
    }

protected:

    typedef boost::interprocess::sharable_lock<typename base_t::frame_t::mutex_t> sharable_lock_t;

    sharable_lock_t sharable_lock() {
        return sharable_lock_t(base_t::frame->mutex);
    }

    Header lastHead;
    timestamp_t lastRecvTime;
    duration_t age;
};

}

#endif /* SHMEM_RASHM_SEGMENTREADER_H_ */
