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

#include <boost/mpl/vector.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/back_inserter.hpp>

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

    static std::string name() {
        return Frame<DATA, ID>::name();
    }
};

template<typename T> struct index_list_t {
};

template<>
struct index_list_t<TestDataB> {
    typedef boost::mpl::vector<DefaultId, TIdB1, TIdB2>::type type;
};

template<>
struct index_list_t<TestDataA> {
    typedef boost::mpl::vector<DefaultId, TIdA>::type type;
};

typedef boost::mpl::vector<TestDataA, TestDataB>::type data_vector_t;

struct packet_functor_t {
    template<typename DATA, typename ID>
    struct apply {
        typedef Packet<DATA, ID> type;
    };
};

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
                typename index_list_t<DATA>::type, igenerator<DATA>>::type packets_t;
        typedef typename boost::mpl::copy<packets_t,
                boost::mpl::back_inserter<V> >::type type;
    };

};

template<typename DATA_VEC, typename FUNC>
struct apply_all_data_ids {
    typedef typename boost::mpl::fold<DATA_VEC, boost::mpl::vector<>,
            fold_functor_t<FUNC>>::type type;
};

typedef apply_all_data_ids<data_vector_t, packet_functor_t>::type all_packets_t;

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

    auto const v = rashm::all_names();
    for (auto const& i : v) {
        std::cout << i << ",";
    }
    std::cout << std::endl;
    rashm::all_packet_variant_t p;

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
        rashm::all_packet_variant_t p2;
        ia >> p2;
        boost::apply_visitor(print_names_visitor(), p2);
    }

    std::cout << "tserial ok" << std::endl;
    return 0;
}

