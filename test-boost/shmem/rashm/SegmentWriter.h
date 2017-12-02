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

    typename base_t::scoped_lock_t lock() {
        return std::move(typename base_t::scoped_lock_t(base_t::frame->mutex));
    }

    SegmentWriter() {
        auto l = lock();
        base_t::frame->setWriterIsPresent(true);
    }

    void operator = ( DATA const & d ) {
        auto l = lock();
        *base_t::frame = d;
    }

    ~SegmentWriter() {
        base_t::frame->setWriterIsPresent(false);
    }
};

}

#endif /* SHMEM_RASHM_SEGMENTWRITER_H_ */
