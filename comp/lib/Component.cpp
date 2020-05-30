#include "Component.hpp"

Component::Component(const ComponentInfo& ci ) :
    log(ci.componentType(),ci.componentName()),
    itsInfo(ci)
{
}

Component::~Component() = default;
