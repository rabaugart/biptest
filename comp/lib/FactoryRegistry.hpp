#ifndef FACTORYREGISTRY_H
#define FACTORYREGISTRY_H

#include <memory>

class FactoryBase;

class FactoryRegistry
{
    public:
        FactoryRegistry();
        virtual ~FactoryRegistry();

        void registrate( std::unique_ptr<FactoryBase>&& f );

        void clear();

    protected:

    private:

    struct Impl;

    std::unique_ptr<Impl> pimpl;
};

#endif // FACTORYREGISTRY_H
