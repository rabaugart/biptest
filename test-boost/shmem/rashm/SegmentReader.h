/*
 * SegmentReader.h
 *
 *  Created on: 02.12.2017
 *      Author: netz
 */

#ifndef SHMEM_RASHM_SEGMENTREADER_H_
#define SHMEM_RASHM_SEGMENTREADER_H_

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
        typename base_t::scoped_lock_t lck(base_t::frame->mutex);
        return base_t::frame->data;
    }

    ~SegmentReader() {
    }
};

}

#endif /* SHMEM_RASHM_SEGMENTREADER_H_ */
