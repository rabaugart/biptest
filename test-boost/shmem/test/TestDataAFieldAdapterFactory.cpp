/*
 * TestDataAMonitor.cpp
 *
 *  Created on: 26.01.2018
 *      Author: netz
 */

#include <boost/format.hpp>
#include <rashm/FieldAdapterFactory.h>
#include <test/test_signal_values.h>

#include "TestDataA.h"

namespace rashm {

template<>
void FieldAdapterFactory<TestDataA, utest::signal_values>::init() {

    registerFactoryFun("a", "FeldA", "Wert von Feld a (double)", []( VData const& d, value_frame_t& f ) {
        f.value = d.data.a;});
    registerFactoryFun("b", "FeldB", "Wert von Feld b (int)", []( VData const& d, value_frame_t& f ) {
        f.value = d.data.b;});

}

}

