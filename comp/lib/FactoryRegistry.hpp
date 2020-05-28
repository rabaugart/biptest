#ifndef FACTORYREGISTRY_H
#define FACTORYREGISTRY_H

#include <memory>
#include <vector>

class FactoryBase;

class FactoryRegistry
{
    public:
        FactoryRegistry();
        virtual ~FactoryRegistry();

        void registrate( std::unique_ptr<FactoryBase>&& f );

        void clear();

        struct Descriptor {
            std::string name;

            std::string shortDescription() const;
        };

        std::vector<Descriptor> descriptorList() const;

    protected:

    private:

    struct Impl;

    std::unique_ptr<Impl> pimpl;
};

#endif // FACTORYREGISTRY_H
