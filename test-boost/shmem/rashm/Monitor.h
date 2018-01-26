/*
 * Monitor.h
 *
 *  Created on: 26.01.2018
 *      Author: netz
 */

#ifndef SHMEM_RASHM_MONITOR_H_
#define SHMEM_RASHM_MONITOR_H_

#include <string>
#include <memory>
#include <vector>

#include <boost/signals2.hpp>

namespace rashm {

struct MonitorAdapter {
public:
    boost::signals2::signal<void(double)> sigDouble;
    boost::signals2::signal<void(std::string const&)> sigString;
    boost::signals2::signal<void(bool)> sigBool;

    virtual void fire() = 0;

protected:
    MonitorAdapter() {

    }

    template<typename T>
    void emit( T );

};

template<typename SDATA>
class Monitor {
public:

    std::shared_ptr<MonitorAdapter> makeAdapter( std::string const & key, std::string const & format );

    void operator = ( SDATA const & d ) {
        currentData = d;
        for ( auto& i : adapters ) {
            i->fire();
        }
    }

protected:

    template<typename VALUE>
    class MyAdapter : public MonitorAdapter {
    public:
        typedef std::function<VALUE(SDATA const&)> access_fun;

        MyAdapter( SDATA const& data_, access_fun f ) : data(data_), fun(f) {

        }

        void fire() {
            VALUE newValue = fun(data);
            emit( newValue );
        }

    protected:
        SDATA const& data;
        VALUE lastValue;
        access_fun fun;
    };

    SDATA currentData;
    std::vector<std::shared_ptr<MonitorAdapter>> adapters;
};

}

#endif /* SHMEM_RASHM_MONITOR_H_ */
