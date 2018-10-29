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
#include <utility>

#define BOOST_TEST_MODULE tfuture

#include <boost/test/unit_test.hpp>

using namespace std::chrono_literals;
using std::this_thread::sleep_for;

template<typename T>
class FutureAdapter
{
public:
    template<typename... Args>
    FutureAdapter( Args&&... args ) : fun(std::forward<Args...>(args...)) {}

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
        else
        {
            throw std::runtime_error("not started");
        }
    }

private:
    T fun;
    std::future<void> fut;
};

struct AData
{
    AData() : counter(0) {}
    size_t counter;
};

class A
{
public:
    A(AData& ad) : running(false), data(ad),
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
            BOOST_TEST_MESSAGE("run " << data.counter++);
            if (data.counter>max_counter)
            {
                throw std::runtime_error("Max error");
            }
        }
    }

    void stop()
    {
        BOOST_TEST_MESSAGE("Stopping A");
        running = false;
    }

protected:
    bool running;
    AData& data;
    size_t max_counter;
};

int f(int i) {
    sleep_for( 5000ms );
    if (i==0)
        throw std::runtime_error("Blöder Fehler");
    return 5;
}

BOOST_AUTO_TEST_CASE(future)
{
    BOOST_TEST_MESSAGE("===== future");
    AData ad;
    BOOST_REQUIRE(ad.counter==0);
    FutureAdapter<A> fa(ad);

    fa.configure(20);

    fa.start();
    fa.check();
    sleep_for(500ms);
    fa.check();
    fa.stop();
    BOOST_CHECK_THROW(fa.check(),std::runtime_error);

    BOOST_CHECK_EQUAL( ad.counter, 5 );

    BOOST_TEST_MESSAGE("main return");
}


BOOST_AUTO_TEST_CASE(future_error)
{
    BOOST_TEST_MESSAGE("===== future_error");
    AData ad;
    BOOST_REQUIRE(ad.counter==0);
    FutureAdapter<A> fa(ad);

    fa.configure(3);

    fa.start();
    fa.check();
    sleep_for(500ms);
    BOOST_CHECK_THROW(fa.check(),std::runtime_error);
    fa.stop();
    BOOST_CHECK_THROW(fa.check(),std::runtime_error);

    BOOST_CHECK_EQUAL( ad.counter, 4 );
}

BOOST_AUTO_TEST_CASE(future_not_started)
{
    BOOST_TEST_MESSAGE("===== future_not_started");
    AData ad;
    BOOST_REQUIRE(ad.counter==0);
    FutureAdapter<A> fa(ad);

    BOOST_CHECK_THROW(fa.check(),std::runtime_error);
}

BOOST_AUTO_TEST_CASE(future_not_started_stop)
{
    BOOST_TEST_MESSAGE("===== future_not_started_stop");
    AData ad;
    BOOST_REQUIRE(ad.counter==0);
    FutureAdapter<A> fa(ad);

    BOOST_CHECK_NO_THROW(fa.stop());
}

BOOST_AUTO_TEST_CASE(future_nothing) {
    BOOST_TEST_MESSAGE("===== future_nothing");
    AData ad;
    BOOST_REQUIRE(ad.counter == 0);
    {
        FutureAdapter<A> fa(ad);

        BOOST_CHECK_NO_THROW(fa.stop());
    }
}
