#include <map>

#include "FactoryRegistry.hpp"
#include "FactoryBase.hpp"

struct FactoryRegistry::Impl {

    std::map<std::string,std::unique_ptr<FactoryBase>> factoryMap;

};

FactoryRegistry::FactoryRegistry()
    : pimpl{std::make_unique<Impl>()}
{
    //ctor
}

FactoryRegistry::~FactoryRegistry()
{
    //dtor
}

void FactoryRegistry::registrate( std::unique_ptr<FactoryBase>&& f )
{
    pimpl->factoryMap.emplace( std::make_pair(f->name(), std::move(f)) );
}

void FactoryRegistry::clear()
{
    pimpl->factoryMap.clear();
}
