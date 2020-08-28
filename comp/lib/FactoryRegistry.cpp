#include <map>
#include <memory>

#include <boost/format.hpp>

#include "FactoryRegistry.hpp"
#include "FactoryBase.hpp"
#include "Component.hpp"
#include "ComponentInfo.hpp"

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

std::unique_ptr<Component> FactoryRegistry::createComponent( const Environment& e, const ComponentInfo& ci )
{
    return pimpl->factoryMap.at(ci.componentType())->create(e,ci);
}

void FactoryRegistry::clear()
{
    pimpl->factoryMap.clear();
}

std::vector<FactoryRegistry::Descriptor> FactoryRegistry::descriptorList() const
{
    std::vector<Descriptor> result;
    for ( const auto& i : pimpl->factoryMap ) {
        Descriptor d;
        d.name = i.first;
        result.push_back(d);
    }
    return result;
}

std::string FactoryRegistry::Descriptor::shortDescription() const
{
    return (boost::format("n:%1%") % name).str();
}
