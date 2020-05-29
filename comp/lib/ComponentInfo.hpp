#ifndef COMPONENTINFO_H
#define COMPONENTINFO_H

#include <string>

class ComponentInfo
{
    public:
        ComponentInfo(const std::string& name);
        virtual ~ComponentInfo();

        std::string toString() const;

        const std::string componentName() const {
            return itsName;
        }

    private:
        const std::string itsName;
};

#endif // COMPONENTINFO_H
