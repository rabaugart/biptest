/*
 * TestDataAMonitor.cpp
 *
 *  Created on: 26.01.2018
 *      Author: netz
 */

#include <boost/format.hpp>

#include "rashm/Monitor.h"

#include "TestDataA.h"
#include "TestTypes.h"

namespace rashm {

template<>
void Monitor<TestDataA,utest::signal_values>::init() {

    factoryMap["a"] = [this]( std::string const& format ) {
        auto ad = std::make_shared<MyAdapter>(
                descriptor_t{ "FeldA", "Wert von Feld a", format },
                currentData, [format]( TestDataA const& d ) {
                    value_frame_t v{d.a};
                    return v;});
        return ad;
    };

    factoryMap["b"] = [this]( std::string const& format ) {
        auto ad = std::make_shared<MyAdapter>(
                descriptor_t{ "FeldB", "Wert von Feld b", format },
                currentData, [format]( TestDataA const& d ) {
                    value_frame_t v{d.b};
                    return v;});
        return ad;
    };

#if 0
    if (key == "b") {
        auto ad = std::make_shared<MyAdapter>(
                descriptor_t { "FeldB", "Wert von Feld b", format },
                currentData, [format]( TestDataA const& d ) {
                    value_frame_t v{d.b};
                    return v;});
        adapters.push_back(ad);
        return ad;
    }
#endif
}

}

