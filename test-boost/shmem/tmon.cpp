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
    void operator()( std::string const & s ) {
        std::cout << "Got " << s << std::endl;
    }
};
int main() {

    rashm::Monitor<TestDataA> mon;

    auto ad = mon.makeAdapter("a","%5.3f");
    auto ad2 = mon.makeAdapter("b","Valb %3d");
    Printer pr;
    ad->sigString.connect(pr);
    ad2->sigString.connect(pr);

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


