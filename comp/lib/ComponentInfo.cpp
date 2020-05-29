#include "ComponentInfo.hpp"

ComponentInfo::ComponentInfo(const std::string& name) : itsName(name)
{
    //ctor
}

ComponentInfo::~ComponentInfo()
{
    //dtor
}

std::string ComponentInfo::toString() const
{
    return itsName;
}
