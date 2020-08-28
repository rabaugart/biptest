
#include "Environment.hpp"
#include "ComponentInfo.hpp"
#include "ComponentManager.hpp"
#include "Component.hpp"
#include "logging.hpp"

namespace {
    static Environment makeEnvironment( int argc, char* argv[] )
    {
        return Environment();
    }
}

int main(int argc, char* argv[]) {

    init_logging();

    Logger log("main");

    BOOST_LOG_SEV(log,loglvl::info) << "Startup";

    Environment e = makeEnvironment(argc,argv);

    ComponentManager cm(e);
    cm.loadPlugins();

    {
        const std::string mycomp="mycompa";
        const auto& ca1 = cm.createComponent(ComponentInfo(mycomp,"ca1"));
        BOOST_LOG_SEV(log,loglvl::info) << "Created component: " << ca1.info().toString();
        const auto& ca2 = cm.createComponent(ComponentInfo(mycomp,"ca2"));
        BOOST_LOG_SEV(log,loglvl::info) << "Created component: " << ca2.info().toString();
    }

    BOOST_LOG_SEV(log,loglvl::info) << "Finished";
}
