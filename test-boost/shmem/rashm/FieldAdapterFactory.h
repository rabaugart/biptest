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

    /**
     * Constructs a field adapter by key
     */
    std::shared_ptr<FieldAdapter<SIGNAL_VALUES>> makeAdapter(
            std::string const & key, std::string const & format) {
        auto const& regval = factoryMap.at(key);
        typedef typename adapter_t::FieldDescriptor descriptor_t;
        auto ad = std::make_shared<MyAdapter>(
                descriptor_t { key, regval.descriptor.label,
                        regval.descriptor.description, format }, currentData,
                regval.fun);
        adapters.push_back(ad);
        return ad;
    }

    void operator =(SDATA const & d) {
        currentData.data = d;
        currentData.valid = true;
        fire();
    }

    typedef FieldAdapter<SIGNAL_VALUES> adapter_t;
    typedef std::shared_ptr<adapter_t> adapter_ptr_t;
    typedef typename adapter_t::ValueFrame value_frame_t;
    typedef typename adapter_t::FieldDescriptor descriptor_t;

    void listFields( std::ostream& os ) {
        for ( auto const& i : factoryMap ) {
            os << i.first << ": " << i.second.descriptor.label << "/" << i.second.descriptor.description << "\n";

        }
    }

protected:

    /**
     * Trigger all adapters to fire the signal
     */
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

    typedef std::function<void(VData const&, value_frame_t& )> access_fun;

    class MyAdapter: public adapter_t {
    public:

        MyAdapter(descriptor_t const & d, VData const& data_, access_fun f) :
                adapter_t(d), data(data_), fun(f) {

        }

    protected:
        void fire() {
            value_frame_t newValue;
            newValue.valid = data.valid;
            newValue.key = adapter_t::descr.key;
            newValue.format = adapter_t::descr.format;
            fun(data, newValue);
            adapter_t::sigValue(newValue);
        }

        VData const& data;
        value_frame_t lastValue;
        access_fun fun;
    };

    VData currentData;
    bool valid;

    struct RegistryValue {
        typename adapter_t::FieldDescriptor descriptor;
        access_fun fun;
    };

    void registerFactoryFun(std::string const & key, std::string const & label,
            std::string const& description, access_fun fun) {
        RegistryValue val { { key, label, description }, fun };
        factoryMap[key] = val;
    }

    std::map<std::string, RegistryValue> factoryMap;

    std::vector<adapter_ptr_t> adapters;
};

}

#endif /* SHMEM_RASHM_FIELDADAPTERFACTORY_H_ */
