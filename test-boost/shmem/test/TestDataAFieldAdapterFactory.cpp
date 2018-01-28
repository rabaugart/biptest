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

    factoryMap["a"] = [this]( std::string const& format ) {
        auto ad = std::make_shared<MyAdapter>(
                descriptor_t {"FeldA", "Wert von Feld a", format},
                currentData, [format]( VData const& d ) {
                    value_frame_t v {d.data.a,d.valid};
                    return v;});
        return ad;
    };

    factoryMap["b"] = [this]( std::string const& format ) {
        auto ad = std::make_shared<MyAdapter>(
                descriptor_t {"FeldB", "Wert von Feld b", format},
                currentData, [format]( VData const& d ) {
                    value_frame_t v {d.data.b,d.valid};
                    return v;});
        return ad;
    };

#if 0
    if (key == "b") {
        auto ad = std::make_shared<MyAdapter>(
                descriptor_t {"FeldB", "Wert von Feld b", format},
                currentData, [format]( TestDataA const& d ) {
                    value_frame_t v {d.b};
                    return v;});
        adapters.push_back(ad);
        return ad;
    }
#endif
}

}

