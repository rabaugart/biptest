/*
 * TestDataAMonitor.cpp
 *
 *  Created on: 26.01.2018
 *      Author: netz
 */

#include <boost/format.hpp>

#include "rashm/Monitor.h"

#include "TestDataA.h"

namespace rashm {

template<>
std::shared_ptr<FieldAdapter> Monitor<TestDataA>::makeAdapter(
        std::string const & key, std::string const & format) {

    if (key == "a") {
        auto ad = std::make_shared<MyAdapter<std::string>>(
                FieldAdapter::FieldDescriptor { "FeldA", "Wert von Feld a" },
                currentData, [format]( TestDataA const& d ) {
                    return (boost::format(format) % d.a).str();});
        adapters.push_back(ad);
        return ad;
    }

    if (key == "b") {
        auto ad = std::make_shared<MyAdapter<std::string>>(
                FieldAdapter::FieldDescriptor { "FeldB", "Wert von Feld b" },
                currentData, [format]( TestDataA const& d ) {
                    return (boost::format(format) % d.b).str();});
        adapters.push_back(ad);
        return ad;
    }

    throw std::runtime_error("Invalid key " + key);
}

}

