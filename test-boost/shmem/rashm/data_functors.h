/*
 * data_functors.h
 *
 *  Created on: 04.12.2017
 *      Author: netz
 */

#ifndef SHMEM_RASHM_DATA_FUNCTORS_H_
#define SHMEM_RASHM_DATA_FUNCTORS_H_

#include <boost/mpl/transform.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/back_inserter.hpp>

namespace rashm {

template<typename FUNC>
struct fold_functor_t {

    template<typename DATA>
    struct igenerator {
        template<typename ID>
        struct apply {
            typedef typename FUNC::template apply<DATA, ID>::type type;
        };
    };

    template<typename V, typename DATA>
    struct apply {
        typedef typename boost::mpl::transform<
                typename index_vector_t<DATA>::type, igenerator<DATA>>::type packets_t;
        typedef typename boost::mpl::copy<packets_t,
                boost::mpl::back_inserter<V> >::type type;
    };

};

template<typename DATA_VEC, typename FUNC>
struct apply_all_data_ids {
    typedef typename boost::mpl::fold<DATA_VEC, boost::mpl::vector<>,
            fold_functor_t<FUNC>>::type type;
};

}

#endif /* SHMEM_RASHM_DATA_FUNCTORS_H_ */
