/*
 * message.cpp
 *
 *  Created on: May 12, 2020
 *      Author: s0096047
 */

#include <iostream>

#include "message.hpp"

struct Message::Impl
{
  static int count;

  Impl(const std::string& s_) :
      s(s_), id(count++)
  {
    //std::cout << "<<<< ctor " << id << "\n";
  }

  ~Impl()
  {
    //std::cout << ">>>> dtor " << id << "\n";
  }

  std::string s;
  int id;
};

int Message::Impl::count = 0;

Message::Message() = default;

Message::Message( Message&& m ) = default;

Message& Message::operator = ( Message&& ) = default;

Message::~Message() = default;

void Message::packMsg( const std::string& s )
{
  pimpl = std::unique_ptr<Impl>( new Impl(s) );
}

std::ostream& operator << ( std::ostream& os, const Message& m )
{
  if (m.pimpl)
  {
    os << m.pimpl->s;
    return os;
  }
  throw std::runtime_error("Invalid message");
}
