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

template<typename DATA, typename ID=DefaultId>
class SegmentWriter : public Segment<DATA,ID> {

public:

    typedef Segment<DATA,ID> base_t;

    SegmentWriter() {
        typename base_t::scoped_lock_t lck(base_t::frame->mutex);
        base_t::frame->setWriterIsPresent(true);
    }

    ~SegmentWriter() {
        base_t::frame->setWriterIsPresent(false);
    }
};

}

#endif /* SHMEM_RASHM_SEGMENTWRITER_H_ */
