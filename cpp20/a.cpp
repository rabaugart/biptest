#include <iostream>
#include <filesystem>
#include <optional>
#include <variant>
#include <string>
#include <type_traits>

namespace fs = std::filesystem;

using myvar = std::variant<int,std::string>;

class printer {
  template<typename T>
  void operator()( T&& v ) const { std::cout << v << std::endl; }
};

template<typename T> struct form;

template<> struct form<int>{
  static const char* name() { return "int"; }
};

template<> struct form<std::string>{
  static const char* name() { return "str"; }
};

void pf( myvar const& v ) {
  std::visit( 
    []( auto&& v ) {
      std::cout << "pf: "
        << form<std::decay_t<decltype(v)>>::name() << " -- " << v << std::endl; }, v );
}

int main()
{
  const fs::path p(".");
  const fs::path p2{ p.parent_path().parent_path() };
  std::cout << "Hello " << fs::absolute(p2) 
    << std::boolalpha << fs::is_directory(p) << std::endl;
  for ( auto const& de : fs::directory_iterator{p/".."} ) {
    std::cout << de.path() << std::endl;
  }

  std::optional<int> oi;
  oi = 5;
  std::cout << "oi " << oi.value() << std::endl;

  myvar val;
  pf( val );
  val = 5;
  pf( val );
  val = "abc";
  pf( val );
  val = myvar();
  pf( val );
}
