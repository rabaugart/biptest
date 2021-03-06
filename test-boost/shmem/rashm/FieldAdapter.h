/*
 * FieldAdapter.h
 *
 *  Created on: 28.01.2018
 *      Author: netz
 */

#ifndef SHMEM_RASHM_FIELDADAPTER_H_
#define SHMEM_RASHM_FIELDADAPTER_H_

#include <string>
#include <boost/signals2.hpp>

namespace rashm {

template<typename SDATA, typename SIGNAL_VALUES>
class FieldAdapterFactory;

/**
 * Sends a signal with a ValueFrame, when the source is updated
 */
template<typename SIGNAL_VALUES>
struct FieldAdapter {
public:
    typedef SIGNAL_VALUES signal_values;

    struct ValueFrame {

        template<typename T>
        ValueFrame(T const& v, bool val) :
                value(v), valid(val) {
        }
        ValueFrame() :
                valid(false) {
        }
        signal_values value;
        std::string key;
        std::string format;
        bool valid;
    };

    boost::signals2::signal<void(ValueFrame const&)> sigValue;

    struct FieldDescriptor {
        std::string key;
        std::string label;
        std::string description;
        std::string format;
    };

    FieldDescriptor const descr;

protected:

    template<typename T,typename S>
    friend class FieldAdapterFactory;

    FieldAdapter(FieldDescriptor const& d) :
            descr(d) {
    }

    virtual void fire() = 0;

};

}


#endif /* SHMEM_RASHM_FIELDADAPTER_H_ */
