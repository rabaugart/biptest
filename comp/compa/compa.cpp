#include <iostream>

#include <boost/config.hpp> // for BOOST_SYMBOL_EXPORT

#include "lcomp.hpp"

namespace my_namespace {

class my_plugin_sum : public my_plugin_api {
public:
    my_plugin_sum() {
        std::cout << "Constructing my_plugin_sum" << std::endl;
    }

    std::string name() const {
        return "sum";
    }

    float calculate(float x, float y) {
        return x + y;
    }

    ~my_plugin_sum() {
        std::cout << "Destructing my_plugin_sum ;o)" << std::endl;
    }
};

// Exporting `my_namespace::plugin` variable with alias name `plugin`
// (Has the same effect as `BOOST_DLL_ALIAS(my_namespace::plugin, plugin)`)
extern "C" BOOST_SYMBOL_EXPORT my_plugin_sum plugin;
my_plugin_sum plugin;

} // namespace my_namespace

void load()
{
    std::cout << "Entering compa" << std::endl;
    print();
}

