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

using namespace std::chrono_literals;
using std::this_thread::sleep_for;

template<typename T>
class FutureAdapter
{
public:
    template<typename... Args>
    FutureAdapter( Args... args ) : fun(args...) {}

    ~FutureAdapter()
    {
        stop();
    }

    void start()
    {
        fut = std::move(std::async(std::launch::async,[this]() { fun(); }));
    }

    template<typename... Args>
    void configure(Args... args)
    {
        fun.configure(args...);
    }

    void stop()
    {
        if (fut.valid())
        {
            fun.stop();
            fut.get();
        }
    }

    void check()
    {
        if (fut.valid())
        {
            switch(fut.wait_for(0ms))
            {
            case std::future_status::ready:
                fut.get();
                break;
            case std::future_status::deferred:
                throw std::runtime_error("deferred");
                break;
            case std::future_status::timeout:
                break;
            }
        }
    }

private:
    T fun;
    std::future<void> fut;
};

class A
{
public:
    A(size_t c_) : running(false), counter(c_),
        max_counter(std::numeric_limits<size_t>::max())
    {

    }

    void configure( size_t max )
    {
        max_counter = max;
    }
    void operator()()
    {
        running = true;
        while(running)
        {
            sleep_for(100ms);
            std::cout << "run " << counter++ << std::endl;
            if (counter>max_counter)
            {
                throw std::runtime_error("Max error");
            }
        }
    }

    void stop()
    {
        std::cout << "Stopping A" << std::endl;
        running = false;
    }

protected:
    bool running;
    size_t counter;
    size_t max_counter;
};

int f(int i) {
    sleep_for( 5000ms );
    if (i==0)
        throw std::runtime_error("Blöder Fehler");
    return 5;
}

int main( int argc, char** argv ) {
#if 0
    std::packaged_task<int(int)> p1{f};
    std::packaged_task<int(int)> p2{f};

    p1(1);
    p2(0);

    std::cout << "Started" << std::endl;

    auto fu = p1.get_future();
    auto fu2 = p2.get_future();

    std::cout << fu.get() << std::endl;
    std::cout << fu2.get() << std::endl;
#endif
    FutureAdapter<A> fa(7);

    fa.configure(20);

    fa.start();
    fa.check();
    sleep_for(500ms);
    fa.check();
    fa.stop();
    fa.check();

    std::cout << "main return" << std::endl;
    return 0;
}


