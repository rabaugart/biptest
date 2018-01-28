/*
 * FieldAdapter.h
 *
 *  Created on: 28.01.2018
 *      Author: netz
 */

#ifndef SHMEM_RASHM_FIELDADAPTER_H_
#define SHMEM_RASHM_FIELDADAPTER_H_

namespace rashm {

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
        bool valid;
    };

    boost::signals2::signal<void(ValueFrame const&)> sigValue;

    virtual void fire() = 0;

    struct FieldDescriptor {
        std::string label;
        std::string description;
        std::string format;
    };

    FieldDescriptor const descr;

protected:

    FieldAdapter(FieldDescriptor const& d) :
            descr(d) {
    }

};

}


#endif /* SHMEM_RASHM_FIELDADAPTER_H_ */
