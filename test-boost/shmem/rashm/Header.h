/*
 * Header.h
 *
 *  Created on: 04.12.2017
 *      Author: netz
 */

#ifndef SHMEM_RASHM_HEADER_H_
#define SHMEM_RASHM_HEADER_H_

#include <boost/date_time/posix_time/posix_time.hpp>

namespace rashm {

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

}

#endif /* SHMEM_RASHM_HEADER_H_ */
