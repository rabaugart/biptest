#ifndef FACTORYBASE_H
#define FACTORYBASE_H

#include <string>

class FactoryBase
{
    public:
        FactoryBase();
        virtual ~FactoryBase();

        virtual std::string name() const = 0;

    protected:

    private:
};

#endif // FACTORYBASE_H
