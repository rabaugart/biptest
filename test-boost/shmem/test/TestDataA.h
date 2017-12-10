#if !defined(TEST_DATA_A_HEADER_817893793)
#define TEST_DATA_A_HEADER_817893793

#include <iostream>

#include <boost/mpl/vector.hpp>

#include <rashm/index_vector.h>

#include "rashm/default_id.h"

struct TestDataA {
    bool operator==(TestDataA const &) const;
    double a;
    int b;
};

struct TIdA {
};

namespace rashm {

template<>
struct index_vector_t<TestDataA> {
    typedef boost::mpl::vector<DefaultId, TIdA>::type type;
};

}

std::ostream& operator <<(std::ostream& os, const TestDataA& d);

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, TestDataA & d, const unsigned int version) {
    ar & d.a;
    ar & d.b;
}

} // namespace serialization
} // namespace boost

#endif
