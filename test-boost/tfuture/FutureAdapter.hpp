/*
 * FutureAdapter.hpp
 *
 *  Created on: 09.11.2018
 *      Author: netz
 */

#ifndef TFUTURE_FUTUREADAPTER_HPP_
#define TFUTURE_FUTUREADAPTER_HPP_

#include <future>
#include <chrono>

namespace rashm
{

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
            switch(fut.wait_for(std::chrono::milliseconds{0}))
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

}


#endif /* TFUTURE_FUTUREADAPTER_HPP_ */
