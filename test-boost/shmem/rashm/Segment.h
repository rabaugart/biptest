/*
 * Segment.h
 *
 *  Created on: 01.12.2017
 *      Author: netz
 */

#ifndef SHMEM_RASHM_SEGMENT_H_
#define SHMEM_RASHM_SEGMENT_H_

#include <memory>

#include <boost/interprocess/sync/interprocess_sharable_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition_any.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>

#include "rashm_traits.h"

namespace rashm {

struct DefaultId {
    static std::string name() {
        return "default";
    }
};

typedef boost::posix_time::ptime timestamp_t;

timestamp_t now();

/**
 * The header data of the frame with timestamp, validity,...
 */
struct Header {

    Header();

    void update(bool valid = true);

    //! The timestamp of the last write operation
    timestamp_t timestamp;

    //! The number of write operations
    size_t counter;

    //! A writer is connected to the shared memory segment
    bool writerIsPresent;

    //! The data is valid
    bool isValid;
};

/**
 * The frame stored in shared memory
 *
 * It consists of
 *  - the payload data
 *  - header of type Header
 *  - anonymous mutex
 *  - anonymous condition variable
 */
template<typename DATA, typename ID>
struct Frame {

    typedef DATA data_t;
    typedef boost::interprocess::interprocess_sharable_mutex mutex_t;
    typedef boost::interprocess::interprocess_condition_any condition_t;

    static std::string name() {
        return DataTraits<data_t>::SHM_NAME + "-" + ID::name();
    }

    void setIsValid(bool b = true) {
        head.isValid = b;
    }

    void setWriterIsPresent(bool b) {
        head.writerIsPresent = b;
    }

    void operator =(data_t const& d) {
        data = d;
        head.update();
    }

    Header head;
    data_t data;
    mutex_t mutex;
    condition_t condition;
};

template<typename DATA, typename ID>
class Segment {
protected:
    typedef Frame<DATA, ID> frame_t;
    Segment() :
            name(frame_t::name()), frame(nullptr) {
        try {
            // Trying to construct a new segment and a new frame object
            shm = std::move(
                    boost::interprocess::shared_memory_object(
                            boost::interprocess::create_only, name.c_str(),
                            boost::interprocess::read_write));

            shm.truncate(sizeof(frame_t));
            reg = std::move(
                    boost::interprocess::mapped_region(shm,
                            boost::interprocess::read_write
#if defined(FIXED_MAPPING_ADDRESS)
                            , 0, 0, (void*) 0x3f00000000
#endif
                            ));
            void * addr = reg.get_address();

            //Construct the shared structure in memory
            frame = new (addr) frame_t;

        } catch (boost::interprocess::interprocess_exception& ex) {
            // Creation failed, so try to open an existing one, without construction
            shm = std::move(
                    boost::interprocess::shared_memory_object(
                            boost::interprocess::open_only, name.c_str(),
                            boost::interprocess::read_write));
            reg = std::move(
                    boost::interprocess::mapped_region(shm,
                            boost::interprocess::read_write
#if defined(FIXED_MAPPING_ADDRESS)
                            , 0, 0, (void*) 0x3f00000000
#endif
                            ));

            frame = static_cast<frame_t*>(reg.get_address());
        }
    }

    virtual ~Segment() {
    }

public:
    bool writerIsPresent() const {
        return frame->head.writerIsPresent;
    }

    static std::string segmentName() {
        return frame_t::name();
    }

    static void removeSegment() {
        boost::interprocess::shared_memory_object::remove(
                segmentName().c_str());
    }

protected:

    typedef boost::interprocess::scoped_lock<typename frame_t::mutex_t> scoped_lock_t;

    std::string name;
    boost::interprocess::shared_memory_object shm;
    boost::interprocess::mapped_region reg;
    frame_t* frame;
};

} /* namespace rashm */

#endif /* SHMEM_RASHM_SEGMENT_H_ */
