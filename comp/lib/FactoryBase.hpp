#ifndef FACTORYBASE_H
#define FACTORYBASE_H

#include <string>
#include <memory>

class Component;
class Environment;
class ComponentInfo;

class FactoryBase
{
    public:
        FactoryBase();
        virtual ~FactoryBase();

        virtual std::string name() const = 0;

        virtual std::unique_ptr<Component> create( const Environment&, const ComponentInfo& ) const = 0;

    protected:

    private:
};

#endif // FACTORYBASE_H
