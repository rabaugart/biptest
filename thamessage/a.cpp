
#include <iostream>

#include "message.hpp"

struct A
{
  A() : a(55), s("sta") {}
  int a;
  std::string s;
};

template<>
void Message::serialize( std::ostream& os, const A& a )
{
  os << "structa:";
  serialize(os,a.a,a.s);
}

Message testfun()
{
  Message m;
  m.pack(55,66);
  return m;
}

int main(int argc, char**argv)
{
  try
  {
    Message m;

    std::cout << m.pack(1, 2) << std::endl;
    std::cout << "Pack " << m.pack(1, 2, std::string{ "abc" }, A(), "def", 3) << std::endl;

    std::cout << "M1: " << m << std::endl;

    Message m2{ std::move(m) };

    std::cout << "M2: " << m2 << std::endl;

    Message m3;
    m3 = std::move(m2);

    std::cout << "M3: " << m3 << std::endl;

    m3 = testfun();
    std::cout << "M3b: " << m3 << std::endl;

    const Message m4 = testfun();
    std::cout << "M4: " << m4 << std::endl;

    const std::vector<int> v(3,3);
    m3.pack(v);
    std::cout << "M3c: " << m3 << std::endl;

    throw std::runtime_error("Error");

  } catch (const std::exception& e)
  {
    std::cout << "===== err =====: " << e.what() << std::endl;
    return 1;
  }
  std::cout << "Ok" << std::endl;
  return 0;
}
