#include <iostream>
#include <boost/dll/import.hpp> // for import_alias
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include "lcomp.hpp"
#include "FactoryRegistry.hpp"
#include "Environment.hpp"
#include "Component.hpp"
#include "ComponentInfo.hpp"

namespace dll = boost::dll;

int main(int argc, char* argv[]) {

    std::vector<boost::shared_ptr<component_api>> pluginVector;

    FactoryRegistry registry;

    boost::filesystem::path lib_path("../compa");             // argv[1] contains path to directory with our plugin library
    boost::shared_ptr<component_api> plugin;            // variable to hold a pointer to plugin variable
    pluginVector.push_back(plugin);
    std::cout << "Loading the plugin" << std::endl;

    plugin = dll::import<component_api>(          // type of imported symbol is located between `<` and `>`
        lib_path / "compa",                     // path to the library and library name
        "plugin",                                       // name of the symbol to import
        dll::load_mode::append_decorations              // makes `libmy_plugin_sum.so` or `my_plugin_sum.dll` from `my_plugin_sum`
    );

    std::cout << "plugin->name call:  " << plugin->name() << std::endl;

    plugin->registerComponent(registry);

    std::cout << "Components\n";
    for ( const auto& i : registry.descriptorList()) {
        std::cout << "C: " << i.shortDescription() << std::endl;
    }

    {
        Environment e;
        std::unique_ptr<Component> c1 = registry.createComponent("mycompa",e,ComponentInfo("ca1"));
        std::unique_ptr<Component> c2 = registry.createComponent("mycompa",e,ComponentInfo("ca2"));
    }

    registry.clear();
    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
}
