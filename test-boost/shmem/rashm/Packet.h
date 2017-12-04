/*
 * Packet.h
 *
 *  Created on: 04.12.2017
 *      Author: netz
 */

#ifndef SHMEM_RASHM_PACKET_H_
#define SHMEM_RASHM_PACKET_H_

#include "Header.h"

namespace rashm {

template<typename DATA, typename ID = DefaultId>
struct Packet {
    typedef DATA data_t;
    typedef ID id_t;
    Header head;
    data_t data;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & head;
        ar & data;
    }

    static std::string name() {
        return Frame<DATA, ID>::name();
    }
};

struct packet_functor_t {
    template<typename DATA, typename ID>
    struct apply {
        typedef Packet<DATA, ID> type;
    };
};

}

#endif /* SHMEM_RASHM_PACKET_H_ */
