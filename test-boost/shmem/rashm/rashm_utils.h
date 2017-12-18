/*
 * rashm_utils.h
 *
 *  Created on: 18.12.2017
 *      Author: netz
 */

#ifndef SHMEM_RASHM_RASHM_UTILS_H_
#define SHMEM_RASHM_RASHM_UTILS_H_

#include <memory>

#include "SegmentReader.h"
#include "CompMap.h"

namespace rashm {

namespace util_details {

struct Base {
    struct config {
    };

    virtual void create() = 0;
    virtual void remove() = 0;
};

template<typename SDATA, typename ID>
struct Impl: public Base {

    Impl( Base::config const & ) {}

    void create() {
        SegmentReader<SDATA,ID> sr;
    }

    void remove() {
        Segment<SDATA,ID>::removeSegment();
    }
};

struct ImplFactory {
    typedef Base type;
    typedef std::shared_ptr<type> value_type;
    typedef Base::config config_type;

    template<typename DATA, typename ID>
    static value_type make(config_type const& cfg) {
        return std::make_shared<Impl<DATA, ID>>(cfg);
    }
};

template<typename DATA_VEC>
CompMap<ImplFactory> make_map() {
    typedef ImplFactory fac_t;
    Base::config cfg;
    return rashm::makeMap<DATA_VEC, fac_t>(cfg);
}

}

/**
 * Creates all shared mem segments in the OS
 *
 * No writing takes place except the default ctor of the Frame
 */
template<typename DATA_VEC>
void create_all_segments() {
    auto const map{util_details::make_map<DATA_VEC>()};
    for ( auto& i : map ) {
        i.second->create();
    }
}

/**
 * Removes all shared mem segments from the OS
 */
template<typename DATA_VEC>
void remove_all_segments() {
    auto const map{util_details::make_map<DATA_VEC>()};
    for ( auto& i : map ) {
        i.second->remove();
    }
}

}

#endif /* SHMEM_RASHM_RASHM_UTILS_H_ */
