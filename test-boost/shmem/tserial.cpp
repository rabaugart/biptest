/*
 * tserial.cpp
 *
 *  Created on: 03.12.2017
 *      Author: netz
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include <boost/variant.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/variant.hpp>

#include <boost/mpl/for_each.hpp>

#include "rashm/SegmentWriter.h"
#include "rashm/data_functors.h"
#include "rashm/Packet.h"

#include "test/TestDataA.h"
#include "test/TestDataB.h"
#include "test/all_data_types.h"
#include "test/TestGenerator.h"


typedef rashm::apply_all_data_ids<data_vector_t, rashm::packet_functor_t>::type all_packets_t;

typedef boost::make_variant_over<all_packets_t>::type all_packet_variant_t;

struct c {
    c(std::vector<std::string> & v_) :
            v(v_) {
    }

    template<typename U>
    void operator()(U x) {
        v.push_back(x.name());
    }
    std::vector<std::string>& v;
};

std::vector<std::string> all_names() {

    std::vector<std::string> v;
    boost::mpl::for_each<all_packets_t>(c(v));
    return v;
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

    auto const v = all_names();
    for (auto const& i : v) {
        std::cout << i << ",";
    }
    std::cout << std::endl;
    all_packet_variant_t p;

    typedef TestDataB test_t;
    TestGenerator<test_t> gen;
    rashm::Packet<test_t,TIdB2> pa { rashm::Header(), gen.next() };
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
        all_packet_variant_t p2;
        ia >> p2;
        boost::apply_visitor(print_names_visitor(), p2);
    }

    std::cout << "tserial ok" << std::endl;
    return 0;
}

