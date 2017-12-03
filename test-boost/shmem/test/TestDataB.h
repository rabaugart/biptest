
#if !defined(TEST_DATA_B_HEADER_817893793)
#define TEST_DATA_B_HEADER_817893793

#include <iostream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

struct TestDataB {

    TestDataB() { a = 33.3; b = 5; }
    TestDataB( double a_, int b_ ) : a(a_), b(b_) {}

	bool operator==( TestDataB const & ) const;
	double a;
	int    b;
};

struct TIdB1 {};
struct TIdB2 {};

std::ostream& operator << ( std::ostream& os, const TestDataB& d );

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, TestDataB & d, const unsigned int version)
{
    ar & d.a;
    ar & d.b;
}

} // namespace serialization
} // namespace boost


#endif
