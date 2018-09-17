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

using namespace std::literals::chrono_literals;

template<typename T>
class FutureAdapter
{
public:
    template<typename... Args>
    FutureAdapter( Args... args ) : fun(args...) {}

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
        fun.stop();
        if (fut.valid())
        {
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
            std::this_thread::sleep_for(100ms);
            std::cout << "run " << counter++ << std::endl;
            if (counter>max_counter)
            {
                throw std::runtime_error("Max error");
            }
        }
    }

    void stop()
    {
        running = false;
    }

protected:
    bool running;
    size_t counter;
    size_t max_counter;
};

int f(int i) {
    std::this_thread::sleep_for( std::chrono::milliseconds{5000} );
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

    fa.configure(10);

    fa.start();
    fa.check();
    std::this_thread::sleep_for(500ms);
    fa.check();
    fa.stop();
    fa.check();

    return 0;
}


