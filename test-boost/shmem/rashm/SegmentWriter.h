/*
 * SegmentWriter.h
 *
 *  Created on: 02.12.2017
 *      Author: netz
 */

#ifndef SHMEM_RASHM_SEGMENTWRITER_H_
#define SHMEM_RASHM_SEGMENTWRITER_H_

#include "Segment.h"

namespace rashm {

template<typename DATA, typename ID = DefaultId>
class SegmentWriter: public Segment<DATA, ID> {

public:

    typedef Segment<DATA, ID> base_t;

    SegmentWriter() {
        auto l{lock()};
        base_t::frame->setWriterIsPresent(true);
    }

    void operator = ( DATA const & d ) {
        auto l = lock();
        *base_t::frame = d;
        base_t::frame->condition.notify_all();
    }

    ~SegmentWriter() {
        base_t::frame->setWriterIsPresent(false);
    }

protected:
    typename base_t::scoped_lock_t lock() {
        return typename base_t::scoped_lock_t(base_t::frame->mutex);
    }

};

}

#endif /* SHMEM_RASHM_SEGMENTWRITER_H_ */
