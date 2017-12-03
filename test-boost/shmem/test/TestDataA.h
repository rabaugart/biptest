
#if !defined(TEST_DATA_A_HEADER_817893793)
#define TEST_DATA_A_HEADER_817893793

#include <iostream>

struct TestDataA {

	TestDataA() { a = 33.3; b = 5; }
	TestDataA( double a_, int b_ ) : a(a_), b(b_) {}

	bool operator==( TestDataA const & ) const;
	double a;
	int    b;
};

struct TIdA {};

std::ostream& operator << ( std::ostream& os, const TestDataA& d );

#endif
