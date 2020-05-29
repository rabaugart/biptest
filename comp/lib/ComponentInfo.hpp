#ifndef COMPONENTINFO_H
#define COMPONENTINFO_H

#include <string>

class ComponentInfo
{
    public:
        ComponentInfo(const std::string& name);
        virtual ~ComponentInfo();

        std::string toString() const;

    private:
        const std::string itsName;
};

#endif // COMPONENTINFO_H
