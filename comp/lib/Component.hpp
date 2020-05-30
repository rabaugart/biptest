#ifndef COMPONENT_H
#define COMPONENT_H

#include "logging.hpp"
#include "ComponentInfo.hpp"

class Component
{
    public:

        Component(const ComponentInfo& ci);
        virtual ~Component();

        const ComponentInfo& info() const { return itsInfo; }
    protected:
        Logger log;
        const ComponentInfo itsInfo;
};

#endif // COMPONENT_H
