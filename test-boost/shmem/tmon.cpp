/*
 * tmon.cpp
 *
 *  Created on: 26.01.2018
 *      Author: netz
 */

#include <iostream>

#include <boost/lexical_cast.hpp>

#include "rashm/Monitor.h"
#include "test/TestDataA.h"
#include "test/TestTypes.h"

typedef rashm::Monitor<TestDataA,utest::signal_values> monitor_t;

struct Printer {

    typedef monitor_t::adapter_t field_adapter_t;

    Printer(std::shared_ptr<field_adapter_t> ad_) :
            ad(ad_) {
        ad->sigValue.connect(*this);
    }

    struct ToString : public boost::static_visitor<std::string> {
        template<typename T>
        std::string operator()( T const& v  ) const {
            return boost::lexical_cast<std::string>(v);
        }
    };

    void operator()(monitor_t::signal_value_t const &v ) {
        std::cout << "Got " << ad->descr.label << " " << boost::apply_visitor(ToString(),v.value) << std::endl;
    }

    std::shared_ptr<field_adapter_t> ad;
};

int main() {

    monitor_t mon;

    Printer pr1 { mon.makeAdapter("a", "%5.3f") };
    Printer pr2 { mon.makeAdapter("b", "%3d") };

    TestDataA d;

    d.a = 1.23;
    d.b = 55;
    mon = d;

    d.a = -7.89;
    d.b = 77;
    mon = d;

    std::cout << "tmon ok" << std::endl;
    return 0;
}

