#include <iostream>

#include <boost/config.hpp> // for BOOST_SYMBOL_EXPORT

#include "FactoryRegistry.hpp"
#include "FactoryBase.hpp"
#include "lcomp.hpp"
#include "Component.hpp"

namespace {

static const std::string COMP_NAME{"mycompa"};

class ThisComponent : public Component {
public:
    ThisComponent( Environment& ) {
        std::cout << "Ctor compa" << std::endl;
    }
};

class Factory : public FactoryBase {
public:
    Factory() {
        std::cout << "Factory compa created" << std::endl;
    }

    std::string name() const {
        return COMP_NAME;
    }

    std::unique_ptr<Component> create(Environment& e) {
        return std::make_unique<ThisComponent>(e);
    }
};

class this_component_api : public component_api {
public:
    this_component_api() {
        std::cout << "Constructing my_plugin_sum" << std::endl;
    }

    std::string name() const {
        return COMP_NAME;
    }

    void registerComponent(FactoryRegistry& r)
    {
        r.registrate( std::make_unique<Factory>() );
    }

    ~this_component_api() {
        std::cout << "Destructing my_plugin_sum ;o)" << std::endl;
    }
};

// Exporting `my_namespace::plugin` variable with alias name `plugin`
// (Has the same effect as `BOOST_DLL_ALIAS(my_namespace::plugin, plugin)`)
extern "C" BOOST_SYMBOL_EXPORT this_component_api plugin;
this_component_api plugin;

} // namespace my_namespace

void load()
{
    std::cout << "Entering compa" << std::endl;
}

