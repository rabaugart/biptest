#include <iostream>
#include <filesystem>
#include <optional>
#include <variant>
#include <string>
#include <type_traits>
#include <typeinfo>
#include "boost/core/demangle.hpp"

namespace fs = std::filesystem;

using myvar = std::variant<int,std::string,std::optional<long long>,long double>;

class printer {
  template<typename T>
  void operator()( T&& v ) const { std::cout << v << std::endl; }
};

template<typename T> struct form;

#if 0
template<> struct form<int>{
  static const char* name() { return "int"; }
};

template<> struct form<std::string>{
  static const char* name() { return "str"; }
};
#endif

template<typename T> struct form{
    static std::string name() {
        return std::string{"Unknown/"} + std::to_string(sizeof(T)) + "/" +
                boost::core::demangle(typeid(T).name());
    }
};

void pf( myvar const& v ) {
  size_t counter=0;
  std::visit( 
    [&counter]( auto&& v ) {
      std::cout << "pf: " << counter++ << " "
        << form<std::decay_t<decltype(v)>>::name() << " -- " << std::endl; }, v );
}

template<typename... T> struct tprinter;
template<typename ...Args> struct tprinter<std::variant<Args...>> {
    static std::string name() {
        return tprinter<Args...>::name();
    }
};
template<typename Head, typename ...Args> struct tprinter<Head,Args...> {
    static std::string name() {
        return form<Head>::name() + ", " + tprinter<Args...>::name();
    }
};
template<> struct tprinter<> {
    static std::string name() { return ""; }
};

int main()
{
  try {
    const fs::path p(".");
    const fs::path p2{ p.parent_path().parent_path() };
    std::cout << "Hello " << fs::absolute(p2) 
      << std::boolalpha << fs::is_directory(p) << std::endl;
    for ( auto const& de : fs::directory_iterator{p/".."} ) {
      std::cout << de.path() << std::endl;
    }
  } catch ( std::exception const& e ) {
    std::cout << "Error: " << e.what() << std::endl;
  }

  std::optional<int> oi;
  oi = 5;
  std::cout << "oi " << oi.value() << std::endl;

  myvar val;
  pf( val );
  val = std::optional<long>(5);
  pf( val );
  val = "abc";
  pf( val );
  val = myvar();
  pf( val );

  std::cout << std::endl << tprinter<myvar>::name() << std::endl;
}
