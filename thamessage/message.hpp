/*
 * message.hpp
 *
 *  Created on: May 12, 2020
 *      Author: s0096047
 */

#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <string>
#include <sstream>
#include <memory>
#include <vector>

class Message
{
public:

  Message();
  Message( const Message& ) = delete;
  Message( Message&& m );

  Message& operator = ( const Message& ) = delete;
  Message& operator = ( Message&& m );

  ~Message();

  template<typename... Args>
  std::string pack( const Args&... args )
  {
    std::ostringstream os;
    serialize( os, args... );
    packMsg(os.str());
    return os.str();
  }

  friend std::ostream& operator << ( std::ostream& os, const Message& m );

protected:

  void serialize( std::ostream& os, const int& a )
  {
    os << "int:" << a;
  }

  void serialize( std::ostream& os, const char* const& s )
  {
    os << "charp:" << s;
  }

  void serialize( std::ostream& os, const std::string& s )
  {
    os << "str:" << s;
  }

  template<typename T>
  void serialize( std::ostream& os, const std::vector<T>& v )
  {
    bool sep = false;
    for ( auto& i: v )
    {
      if (sep)
      {
        os << ";";
      }
      serialize( os, i);
      sep = true;
    }
  }

  template<typename Arg0, typename... Args>
  void serialize( std::ostream& os, const Arg0& a0, const Args&... args )
  {
    serialize(os,a0);
    os << ",";
    serialize(os,args...);
  }

  void packMsg( const std::string& s );

  struct Impl;

  std::unique_ptr<Impl> pimpl;
};



#endif /* MESSAGE_HPP_ */
