/*
 * Header.cpp
 *
 *  Created on: 04.12.2017
 *      Author: netz
 */

#include <limits>

#include "Header.h"

namespace rashm {

static constexpr size_t INITIAL_COUNTER {std::numeric_limits<size_t>::max()};

Header::Header() :
        timestamp(now()), counter(INITIAL_COUNTER), writerIsPresent(false), isValid(
                false) {
}

void Header::update(bool valid) {
    timestamp = now();
    isValid = valid;
    counter++;
}

bool Header::isNewerAs(Header const& other) {
    if (other.counter == INITIAL_COUNTER)
        return true;
    return counter > other.counter;
}

timestamp_t now() {
    return boost::posix_time::microsec_clock::universal_time();
}

}
