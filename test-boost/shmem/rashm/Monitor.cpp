/*
 * Monitor.cpp
 *
 *  Created on: 26.01.2018
 *      Author: netz
 */

#include "Monitor.h"

namespace rashm {

MonitorAdapter::MonitorAdapter(FieldDescriptor const &d) : descr(d) {

}

template<>
void MonitorAdapter::emit( std::string s ) {
    sigString(s);
}

}
