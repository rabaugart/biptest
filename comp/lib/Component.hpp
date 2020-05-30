#ifndef COMPONENT_H
#define COMPONENT_H

#include "ComponentInfo.hpp"

class Component
{
    public:

        Component(const ComponentInfo& ci);
        virtual ~Component();

        const ComponentInfo& info() const { return itsInfo; }
    private:
        const ComponentInfo itsInfo;
};

#endif // COMPONENT_H
