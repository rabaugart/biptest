/*
 * tfuture.cpp
 *
 *  Created on: 01.02.2018
 *      Author: netz
 */

#include <chrono>
#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>

int f(int i) {
    std::this_thread::sleep_for( std::chrono::milliseconds{5000} );
    if (i==0)
        throw std::runtime_error("Blöder Fehler");
    return 5;
}

int main( int argc, char** argv ) {

    std::packaged_task<int(int)> p1{f};
    std::packaged_task<int(int)> p2{f};

    p1(1);
    p2(0);

    std::cout << "Started" << std::endl;

    auto fu = p1.get_future();
    auto fu2 = p2.get_future();

    std::cout << fu.get() << std::endl;
    std::cout << fu2.get() << std::endl;
    return 0;
}


