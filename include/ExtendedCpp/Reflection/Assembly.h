#ifndef Reflection_Assembly_H
#define Reflection_Assembly_H

#include <map>
#include <mutex>

#include <ExtendedCpp/Reflection/TypeInfo.h>

namespace ExtendedCpp::Reflection
{
    class Assembly final
    {
    private:
        static std::map<std::type_index, TypeInfo>  _typeList;
        static std::mutex _listMutex;

    public:
        Assembly() = delete;

        static void AddType(const TypeInfo& typeInfo) noexcept;
        static std::vector<TypeInfo> GetTypes() noexcept;
    };
}

#endif