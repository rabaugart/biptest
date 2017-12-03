/*
 * tserial.cpp
 *
 *  Created on: 03.12.2017
 *      Author: netz
 */

#include <iostream>
#include <sstream>

#include <boost/variant.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/variant.hpp>

#include "rashm/SegmentWriter.h"

#include "test/TestDataA.h"
#include "test/TestDataB.h"

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
};

typedef boost::variant<Packet<TestDataA, DefaultId>, Packet<TestDataA, TIdA>,
        Packet<TestDataB, DefaultId>, Packet<TestDataB, TIdB1>> all_packet_t;

}

class print_names_visitor: public boost::static_visitor<> {
public:

    template<typename T>
    void operator()(T const & v) const {
        typedef T packet_t;
        std::cout << rashm::DataTraits<typename packet_t::data_t>::SHM_NAME
                << "/" << rashm::DataTraits<typename packet_t::id_t>::SHM_NAME
                << " " << v.head.timestamp << " " << v.data << std::endl;
    }

    void operator()(rashm::Packet<TestDataB, TIdB1> const & v) const {
        std::cout << "tidb1 " << v.head.timestamp << " " << v.data << std::endl;
    }

};

int main(int argc, char** argv) {

    rashm::all_packet_t p;

    typedef TestDataA test_t;
    rashm::Packet<test_t> pa { rashm::Header(), test_t(987.6, 55) };
    p = pa;

    boost::apply_visitor(print_names_visitor(), p);

    std::ostringstream os;

    {
        boost::archive::text_oarchive oa(os);
        // write class instance to archive
        oa << p;
    }

    {
        std::istringstream is(os.str());
        boost::archive::text_iarchive ia(is);
        rashm::all_packet_t p2;
        ia >> p2;
        boost::apply_visitor(print_names_visitor(), p2);
    }

    std::cout << "tserial ok" << std::endl;
    return 0;
}

