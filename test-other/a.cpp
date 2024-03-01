#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include <boost/date_time/gregorian/gregorian.hpp>

//
template< class, class = std::void_t<> >
struct has_iter_member : std::false_type { };
 
// specialization recognizes types that do have a nested ::type member:
template< class T >
struct has_iter_member<T, std::void_t<typename T::const_iterator>> : std::true_type { };

template<typename T>
typename std::enable_if<std::is_fundamental<T>::value,std::string>::type stype( const T& ) {
	return "fund";
}

std::string stype( const std::string& ) {
	return "string";
}

template<typename T>
typename std::enable_if<has_iter_member<T>::value,std::string>::type stype( const T& ) {
	return "iter";
}

#if 0
template< class, class = std::void_t<> >
struct has_stream : std::false_type { };
 
struct A {};
int operator<<(A&,const int&);

// specialization recognizes types that do have a nested ::type member:
template< class T >
struct has_stream<T, std::void_t<std::is_invocable<decltype(operator<<()(A,T)),A, T>::value>> : std::true_type { };
#endif

int main() {
  std::cout << has_iter_member<int>() << stype(5) << std::endl;
  std::cout << has_iter_member<std::string>() << stype(std::string("abc")) << std::endl;
  std::cout << has_iter_member<std::vector<int>>() << stype(std::vector<int>())<< std::endl;
  using namespace boost::gregorian;
  date weekstart(2002,Feb,1);
  date weekend  = weekstart + weeks(1);
  std::cout << weekend << std::endl;
  return 0;
}
