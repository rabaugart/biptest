/*
 * Segment.cpp
 *
 *  Created on: 02.12.2017
 *      Author: netz
 */

#include "segment.h"

namespace rashm {

Header::Header() :
        timestamp(now()), counter(0), writerIsPresent(false), isValid(false) {
}

timestamp_t now() {
    return boost::posix_time::microsec_clock::universal_time();
}

}

