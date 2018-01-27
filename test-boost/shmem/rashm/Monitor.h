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

template<typename SIGNAL_VALUES>
struct FieldAdapter {
public:
    typedef SIGNAL_VALUES signal_values;

    struct Value {

        template<typename T>
        Value(T const& v) : value(v), valid(true) {}
        Value() : valid(false) {}
        signal_values value;
        bool valid;
    };

    boost::signals2::signal<void( Value const& )> sigValue;

    virtual void fire() = 0;

    struct FieldDescriptor {
        std::string label;
        std::string description;
    };

    FieldDescriptor const descr;

protected:

    FieldAdapter(FieldDescriptor const& d) : descr(d) {}

};

template<typename SDATA,typename SIGNAL_VALUES>
class Monitor {
public:

    std::shared_ptr<FieldAdapter<SIGNAL_VALUES>> makeAdapter(std::string const & key,
            std::string const & format);

    void operator =(SDATA const & d) {
        currentData = d;
        for (auto& i : adapters) {
            i->fire();
        }
    }

    typedef FieldAdapter<SIGNAL_VALUES> adapter_t;
    typedef typename adapter_t::Value signal_value_t;
    typedef typename adapter_t::FieldDescriptor descriptor_t;

protected:


    class MyAdapter: public adapter_t {
    public:
        typedef std::function<signal_value_t(SDATA const&)> access_fun;

        MyAdapter(descriptor_t const & d, SDATA const& data_, access_fun f) :
            adapter_t(d), data(data_), fun(f) {

        }

        void fire() {
            signal_value_t newValue = fun(data);
            adapter_t::sigValue(newValue);
        }

    protected:
        SDATA const& data;
        signal_value_t lastValue;
        access_fun fun;
    };

    SDATA currentData;
    std::vector<std::shared_ptr<FieldAdapter<SIGNAL_VALUES>>> adapters;
};

}

#endif /* SHMEM_RASHM_MONITOR_H_ */
