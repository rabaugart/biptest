/*
 * Monitor.h
 *
 *  Created on: 26.01.2018
 *      Author: netz
 */

#ifndef SHMEM_RASHM_FIELDADAPTERFACTORY_H_
#define SHMEM_RASHM_FIELDADAPTERFACTORY_H_

#include <string>
#include <memory>
#include <vector>
#include <map>

#include <boost/signals2.hpp>

#include "FieldAdapter.h"

namespace rashm {

template<typename SDATA, typename SIGNAL_VALUES>
class FieldAdapterFactory {
public:

    FieldAdapterFactory() {
        init();
    }

    void init();

    void setValid(bool b) {
        currentData.valid = b;
        fire();
    }

    std::shared_ptr<FieldAdapter<SIGNAL_VALUES>> makeAdapter(
            std::string const & key, std::string const & format) {
        auto ad = factoryMap[key](format);
        adapters.push_back(ad);
        return ad;
    }

    void operator =(SDATA const & d) {
        currentData.data = d;
        currentData.valid = true;
        fire();
    }

    typedef FieldAdapter<SIGNAL_VALUES> adapter_t;
    typedef typename adapter_t::ValueFrame value_frame_t;
    typedef typename adapter_t::FieldDescriptor descriptor_t;

protected:

    void fire() {
        for (auto& i : adapters) {
            i->fire();
        }
    }

    struct VData {
        VData() :
                valid(false) {
        }
        SDATA data;
        bool valid;
    };

    class MyAdapter: public adapter_t {
    public:
        typedef std::function<value_frame_t(VData const&)> access_fun;

        MyAdapter(descriptor_t const & d, VData const& data_, access_fun f) :
                adapter_t(d), data(data_), fun(f) {

        }

    protected:
        void fire() {
            value_frame_t newValue = fun(data);
            adapter_t::sigValue(newValue);
        }

        VData const& data;
        value_frame_t lastValue;
        access_fun fun;
    };

    VData currentData;
    bool valid;

    typedef std::function<
            std::shared_ptr<FieldAdapter<SIGNAL_VALUES>>(
                    std::string const& format)> adapter_factory_fun;

    std::map<std::string, adapter_factory_fun> factoryMap;

    std::vector<std::shared_ptr<FieldAdapter<SIGNAL_VALUES>>>adapters;
};

}

#endif /* SHMEM_RASHM_FIELDADAPTERFACTORY_H_ */
