#include <vector>
#include <iostream>
#include <memory>

#include <boost/dll/import.hpp> // for import_alias

#include "ComponentManager.hpp"
#include "Component.hpp"
#include "Environment.hpp"
#include "FactoryRegistry.hpp"
#include "lcomp.hpp"
#include "logging.hpp"

namespace dll = boost::dll;

struct ComponentManager::Impl {
    Impl(const Environment& e) : log("compmgr"),env(e) {}

    Component& createComponent( const ComponentInfo& ci )
    {
        components.emplace_back(registry.createComponent(env,ci));
        return *(components.back());
    }

    Logger log;

    const Environment env;

    FactoryRegistry registry;
    std::vector<boost::shared_ptr<component_api>> pluginVector;
    std::vector<std::unique_ptr<Component>> components;
};

ComponentManager::ComponentManager(const Environment& e) : pimpl{std::make_unique<Impl>(e)}
{
    //ctor
}

ComponentManager::~ComponentManager()
{
    //pimpl->registry.clear();
}

void ComponentManager::loadPlugins()
{
    boost::filesystem::path lib_path("../compa");             // argv[1] contains path to directory with our plugin library
    boost::shared_ptr<component_api> plugin;            // variable to hold a pointer to plugin variable
    pimpl->pluginVector.push_back(plugin);
    BOOST_LOG_SEV(pimpl->log, loglvl::info) << "Loading the plugin";

    plugin = dll::import<component_api>(          // type of imported symbol is located between `<` and `>`
        lib_path / "compa",                     // path to the library and library name
        "plugin",                                       // name of the symbol to import
        dll::load_mode::append_decorations              // makes `libmy_plugin_sum.so` or `my_plugin_sum.dll` from `my_plugin_sum`
    );

    BOOST_LOG_SEV(pimpl->log, loglvl::debug)<< "plugin->name call:  " << plugin->name() << std::endl;

    plugin->registerComponent(pimpl->registry);

    BOOST_LOG_SEV(pimpl->log, loglvl::debug) << "Components\n";
    for ( const auto& i : pimpl->registry.descriptorList()) {
        BOOST_LOG_SEV(pimpl->log, loglvl::info) << "C: " << i.shortDescription() << std::endl;
    }
}

Component& ComponentManager::createComponent( const ComponentInfo& ci )
{
    return pimpl->createComponent(ci);
}
