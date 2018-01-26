/*
 * Monitor.cpp
 *
 *  Created on: 26.01.2018
 *      Author: netz
 */

#include "Monitor.h"

namespace rashm {

template<>
void MonitorAdapter::emit( std::string s ) {
    sigString(s);
}

}
