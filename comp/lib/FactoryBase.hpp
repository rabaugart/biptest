#ifndef FACTORYBASE_H
#define FACTORYBASE_H

#include <string>
#include <memory>

class Component;
class Environment;

class FactoryBase
{
    public:
        FactoryBase();
        virtual ~FactoryBase();

        virtual std::string name() const = 0;

        virtual std::unique_ptr<Component> create( Environment& ) = 0;

    protected:

    private:
};

#endif // FACTORYBASE_H
