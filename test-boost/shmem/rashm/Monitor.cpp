/*
 * Monitor.cpp
 *
 *  Created on: 26.01.2018
 *      Author: netz
 */

#include "Monitor.h"

namespace rashm {

FieldAdapter::FieldAdapter(FieldDescriptor const &d) :
        descr(d) {

}

template<>
void FieldAdapter::emit(std::string s) {
    sigString(s);
}

}
