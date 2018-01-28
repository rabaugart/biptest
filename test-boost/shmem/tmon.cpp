/*
 * tmon.cpp
 *
 *  Created on: 26.01.2018
 *      Author: netz
 */

#include <iostream>

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

#include "rashm/FieldAdapterFactory.h"
#include "test/test_signal_values.h"

#include "test/TestDataA.h"

typedef rashm::FieldAdapterFactory<TestDataA, utest::signal_values> monitor_t;

struct Printer {

    typedef monitor_t::adapter_ptr_t adapter_ptr;

    Printer(adapter_ptr ad_) :
            ad(ad_) {
        ad->sigValue.connect(*this);
    }

    struct ToString: public boost::static_visitor<std::string> {

        ToString(adapter_ptr ad_) :
                ad(ad_) {
        }

        template<typename T>
        std::string operator()(T const& v) const {
            if (ad->descr.format == "") {
                return boost::lexical_cast<std::string>(v);
            } else {
                return (boost::format(ad->descr.format) % v).str();
            }
        }

        adapter_ptr ad;
    };

    void operator()(monitor_t::value_frame_t const &v) {
        if (v.valid) {
            std::cout << "Got " << ad->descr.label << " "
                    << boost::apply_visitor(ToString { ad }, v.value)
                    << std::endl;
        } else {
            std::cout << "Got " << ad->descr.label << " invalid" << std::endl;
        }
    }

    adapter_ptr ad;
};

int main() {

    monitor_t mon;

    Printer pr1 { mon.makeAdapter("a", "%7.3f") };
    Printer pr2 { mon.makeAdapter("b", "%7d") };

    TestDataA d;

    d.a = 1.23;
    d.b = 55;
    mon = d;

    d.a = -7.89;
    d.b = 77;
    mon = d;

    mon.setValid(false);

    d.a = 4.56;
    d.b = -55;
    mon = d;

    std::cout << "tmon ok" << std::endl;
    return 0;
}

