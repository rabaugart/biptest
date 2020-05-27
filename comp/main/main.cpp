#include <iostream>
#include <boost/dll/import.hpp> // for import_alias
#include <boost/filesystem.hpp>
#include "lcomp.hpp"
#include "FactoryRegistry.hpp"

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

    registry.clear();
}
