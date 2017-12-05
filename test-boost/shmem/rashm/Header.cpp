/*
 * Header.cpp
 *
 *  Created on: 04.12.2017
 *      Author: netz
 */

#include "Header.h"

namespace rashm {

Header::Header() :
        timestamp(now()), counter(0), writerIsPresent(false), isValid(false) {
}

void Header::update(bool valid) {
    timestamp = now();
    isValid = valid;
    counter++;
}

timestamp_t now() {
    return boost::posix_time::microsec_clock::universal_time();
}

}