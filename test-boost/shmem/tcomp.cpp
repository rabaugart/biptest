/*
 * tcomp.cpp
 *
 *  Created on: 05.12.2017
 *      Author: netz
 */


#include <map>
#include <memory>

#include <boost/program_options.hpp>
#include <boost/mpl/for_each.hpp>

#include "rashm/Segment.h"
#include "rashm/data_functors.h"
#include "rashm/Packet.h"

#include "test/all_data_types.h"

template<typename COMP>
class CompMap : public std::map<std::string,std::shared_ptr<typename COMP::type>> {

};

class WriterBase {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
};

template<typename DATA, typename ID>
class Writer : public WriterBase {
public:
    virtual void start() {

    }
    virtual void stop() {

    }
};

template<typename COMP>
struct Coll {
    Coll(CompMap<COMP> & v_) :
            v(v_) {
    }

    template<typename U>
    void operator()(U x) {
        v[x.name()] = COMP::template make<typename U::data_t,typename U::id_t>();
    }
    CompMap<COMP>& v;
};

struct WriterComp {
    typedef WriterBase type;

    template<typename DATA, typename ID>
    static std::shared_ptr<type> make() {
        return std::make_shared<Writer<DATA,ID>>();
    }
};

template<typename COMP>
CompMap<COMP> makeMap() {
    CompMap<COMP> ret;

    typedef rashm::apply_all_data_ids<data_vector_t, rashm::packet_functor_t>::type all_packets_t;

    boost::mpl::for_each<all_packets_t>(Coll<COMP>(ret));

    return ret;
}

int main( int argc, char** argv ) {

    typedef WriterComp comp_t;

    CompMap<comp_t> map = makeMap<comp_t>();

    for ( auto const& i : map ) {
        std::cout << i.first << std::endl;
    }

    return 0;
}
