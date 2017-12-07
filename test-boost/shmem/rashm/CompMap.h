/*
 * CompMap.h
 *
 *  Created on: 07.12.2017
 *      Author: netz
 */

#ifndef SHMEM_RASHM_COMPMAP_H_
#define SHMEM_RASHM_COMPMAP_H_

#include <string>
#include <map>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>

#include "rashm/data_functors.h"
#include "rashm/rashm_traits.h"

namespace rashm {

template<typename CFACTORY>
class CompMap: public std::map<std::string, typename CFACTORY::value_type> {

};

namespace {

template<typename CFACTORY>
struct CompFactoryFunctor {
    typedef typename CFACTORY::config_type config_type;

    CompFactoryFunctor(CompMap<CFACTORY> & v_, config_type cfg_) :
            v(v_), cfg(cfg_) {
    }

    template<typename U>
    void operator()(U x) {
        v[x.name()] = CFACTORY::template make<typename U::data_t,
                typename U::id_t>(cfg);
    }
    CompMap<CFACTORY>& v;
    config_type cfg;
};

}

template<typename DATAVEC, typename CFACTORY>
CompMap<CFACTORY> makeMap(typename CFACTORY::config_type const& cfg) {
    CompMap<CFACTORY> ret;

    typedef typename rashm::apply_all_data_ids<DATAVEC, rashm::DataIdTraitsFunctor>::type all_packets_t;

    boost::mpl::for_each<all_packets_t>(CompFactoryFunctor<CFACTORY>(ret, cfg));

    return ret;
}

}


#endif /* SHMEM_RASHM_COMPMAP_H_ */
