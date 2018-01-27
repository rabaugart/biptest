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
std::shared_ptr<FieldAdapter<utest::signal_values>> Monitor<TestDataA,utest::signal_values>::makeAdapter(
        std::string const & key, std::string const & format) {

    if (key == "a") {
        auto ad = std::make_shared<MyAdapter>(
                descriptor_t{ "FeldA", "Wert von Feld a" },
                currentData, [format]( TestDataA const& d ) {
                    value_frame_t v{d.a};
                    return v;});
        adapters.push_back(ad);
        return ad;
    }

    if (key == "b") {
        auto ad = std::make_shared<MyAdapter>(
                descriptor_t { "FeldB", "Wert von Feld b" },
                currentData, [format]( TestDataA const& d ) {
                    value_frame_t v{d.b};
                    return v;});
        adapters.push_back(ad);
        return ad;
    }

    throw std::runtime_error("Invalid key " + key);
}

}

