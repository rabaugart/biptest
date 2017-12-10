#if !defined(TEST_DATA_B_HEADER_817893793)
#define TEST_DATA_B_HEADER_817893793

#include <iostream>

#include <boost/mpl/vector.hpp>

#include "rashm/index_vector.h"

#include "rashm/default_id.h"

struct TestDataB {
    bool operator==(TestDataB const &) const;
    long l1;
    long l2;
    long l3;
};

struct TIdB1 {
};
struct TIdB2 {
};

std::ostream& operator <<(std::ostream& os, const TestDataB& d);

namespace rashm {

template<>
struct index_vector_t<TestDataB> {
    typedef boost::mpl::vector<DefaultId, TIdB1, TIdB2>::type type;
};

}

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, TestDataB & d, const unsigned int version) {
    ar & d.l1;
    ar & d.l2;
    ar & d.l3;
}

} // namespace serialization
} // namespace boost

#endif
