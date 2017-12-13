/*
 * Segment.h
 *
 *  Created on: 01.12.2017
 *      Author: netz
 */

#ifndef SHMEM_RASHM_SEGMENT_H_
#define SHMEM_RASHM_SEGMENT_H_

#include <memory>
#include <type_traits>

#include <boost/interprocess/sync/interprocess_sharable_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition_any.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/core/noncopyable.hpp>

#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/posix_time/time_serialize.hpp>

#include "rashm_traits.h"
#include "default_id.h"
#include "Header.h"

namespace rashm {

struct shm_error: public std::runtime_error {
    shm_error(std::string const& msg) :
            std::runtime_error(msg) {
    }
};

struct no_segment_error: public shm_error {
    no_segment_error(std::string const & name) :
            shm_error(name + " no segment") {
    }
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

    //static_assert( std::is_pod<DATA>::value, "POD expected for rashm");

    typedef DATA data_t;
    typedef ID id_t;
    typedef boost::interprocess::interprocess_sharable_mutex mutex_t;
    typedef boost::interprocess::interprocess_condition_any condition_t;

    static std::string name() {
        return DataIdTraits<data_t, id_t>::name();
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
class Segment: private boost::noncopyable {
protected:
    typedef Frame<DATA, ID> frame_t;

    Segment(boost::interprocess::open_only_t) :
            name(frame_t::name()), shm(), frame(nullptr) {
        try {
            // shm is initialized in the body in order
            // to be able to throw the no_segment error
            shm = std::move(
                    boost::interprocess::shared_memory_object(
                            boost::interprocess::open_only, name.c_str(),
                            boost::interprocess::read_write));
            shm.truncate(sizeof(frame_t));
            reg = std::move(
                    boost::interprocess::mapped_region(shm,
                            boost::interprocess::read_write
#if defined(FIXED_MAPPING_ADDRESS)
                            , 0, 0, DataIdTraits<DATA, ID>::fixedAddress()
#endif
                            ));
            frame = static_cast<frame_t*>(reg.get_address());
        } catch (boost::interprocess::interprocess_exception const & e) {
            if (e.get_error_code() == boost::interprocess::not_found_error) {
                throw no_segment_error(name);
            } else {
                throw;
            }
        }
    }

    Segment& operator =(Segment&& s) {
        shm = std::move(s.shm);
        frame = s.frame;
        reg = std::move(s.reg);
    }

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
                            , 0, 0, DataIdTraits<DATA, ID>::fixedAddress()
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
                            , 0, 0, DataIdTraits<DATA, ID>::fixedAddress()
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

    Header const& head() const {
        return frame->head;
    }

    timestamp_t headerTime() const {
        return frame->head.timestamp;
    }

    size_t counter() const {
        return frame->head.counter;
    }

    const void* address() const {
        return reg.get_address();
    }

protected:

    typedef boost::interprocess::scoped_lock<typename frame_t::mutex_t> scoped_lock_t;

    const std::string name;
    boost::interprocess::shared_memory_object shm;
    boost::interprocess::mapped_region reg;
    frame_t* frame;
};

} /* namespace rashm */

#endif /* SHMEM_RASHM_SEGMENT_H_ */
