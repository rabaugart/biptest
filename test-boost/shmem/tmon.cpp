/*
 * tmon.cpp
 *
 *  Created on: 26.01.2018
 *      Author: netz
 */

#include <iostream>

#include "rashm/Monitor.h"
#include "test/TestDataA.h"

struct Printer {

    Printer(std::shared_ptr<rashm::FieldAdapter> ad_) :
            ad(ad_) {
        ad->sigString.connect(*this);
    }

    void operator()(std::string const & s) {
        std::cout << "Got " << ad->descr.label << " " << s << std::endl;
    }

    std::shared_ptr<rashm::FieldAdapter> ad;
};

int main() {

    rashm::Monitor<TestDataA> mon;

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

