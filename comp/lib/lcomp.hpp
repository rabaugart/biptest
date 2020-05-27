
#ifndef LCOMP_H
#define LCOMP_H

#include <string>
#include <memory>
#include <boost/config.hpp>

class FactoryRegistry;

class BOOST_SYMBOL_VISIBLE component_api {
public:
   virtual std::string name() const = 0;

   virtual void registerComponent(FactoryRegistry& ) = 0;

   virtual ~component_api() {}
};

void print(void);

#endif // LCOMP_H

