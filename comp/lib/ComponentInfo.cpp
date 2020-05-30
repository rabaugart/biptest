#include "ComponentInfo.hpp"

ComponentInfo::ComponentInfo(const std::string& type, const std::string& name) : itsType(type), itsName(name)
{
    //ctor
}

ComponentInfo::~ComponentInfo()
{
    //dtor
}

std::string ComponentInfo::toString() const
{
    return itsType+"/"+itsName;
}
