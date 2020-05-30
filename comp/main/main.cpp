#include <iostream>
#include <boost/dll/import.hpp> // for import_alias
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include "lcomp.hpp"
#include "Environment.hpp"
#include "ComponentInfo.hpp"
#include "ComponentManager.hpp"
#include "Component.hpp"

namespace {
    static Environment makeEnvironment( int argc, char* argv[] )
    {
        return Environment();
    }
}

int main(int argc, char* argv[]) {

    Environment e = makeEnvironment(argc,argv);

    ComponentManager cm(e);
    cm.loadPlugins();

    {
        const std::string mycomp="mycompa";
        const auto& ca1 = cm.createComponent(ComponentInfo(mycomp,"ca1"));
        BOOST_LOG_TRIVIAL(info) << "Created component: " << ca1.info().toString();
        const auto& ca2 = cm.createComponent(ComponentInfo(mycomp,"ca2"));
        BOOST_LOG_TRIVIAL(info) << "Created component: " << ca2.info().toString();
    }

    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
}
