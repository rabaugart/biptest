#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <memory>

class Environment;
class Component;
class ComponentInfo;

class ComponentManager
{
    public:
        ComponentManager( const Environment& );
        virtual ~ComponentManager();

        void loadPlugins();

        Component& createComponent( const ComponentInfo& );
        Component& findComponentByName( const std::string& name ) const;

    private:
        struct Impl;

        std::unique_ptr<Impl> pimpl;
};

#endif // COMPONENTMANAGER_H
