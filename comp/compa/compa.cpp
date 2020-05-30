#include <iostream>

#include <boost/config.hpp> // for BOOST_SYMBOL_EXPORT
#include <boost/log/trivial.hpp>

#include "FactoryRegistry.hpp"
#include "FactoryBase.hpp"
#include "lcomp.hpp"
#include "Component.hpp"
#include "ComponentInfo.hpp"

namespace {

static const std::string COMP_NAME{"mycompa"};

class ThisComponent : public Component {
public:

    ThisComponent( const Environment&, const ComponentInfo& ci ) : Component(ci), itsComponentName(ci.componentName()) {
        BOOST_LOG_SEV(log, loglvl::info) << "Ctor compa:" << ci.toString();
    }

    ~ThisComponent() {
        BOOST_LOG_SEV(log, loglvl::info) << "Dtor compa:" << itsComponentName;
    }

    const std::string itsComponentName;
};

class Factory : public FactoryBase {
public:
    Factory() {
    }

    std::string name() const {
        return COMP_NAME;
    }

    std::unique_ptr<Component> create(const Environment& e, const ComponentInfo& ci ) const {
        return std::make_unique<ThisComponent>(e,ci);
    }
};

class this_component_api : public component_api {
public:
    this_component_api() : log(COMP_NAME,"plugin") {
        BOOST_LOG_SEV(log, loglvl::info) << "Constructing my_plugin_sum";
    }

    std::string name() const {
        return COMP_NAME;
    }

    void registerComponent(FactoryRegistry& r)
    {
        r.registrate( std::make_unique<Factory>() );
    }

    ~this_component_api() {
        BOOST_LOG_SEV(log, loglvl::info) << "Destructing my_plugin_sum";
    }

    Logger log;
};

// Exporting `my_namespace::plugin` variable with alias name `plugin`
// (Has the same effect as `BOOST_DLL_ALIAS(my_namespace::plugin, plugin)`)
extern "C" BOOST_SYMBOL_EXPORT this_component_api plugin;
this_component_api plugin;

} // namespace my_namespace

void xxload()
{
    std::cout << "Entering compa" << std::endl;
}

