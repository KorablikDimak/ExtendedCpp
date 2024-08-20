#ifndef Reflection_Assembly_H
#define Reflection_Assembly_H

#include <map>
#include <mutex>
#include <Reflection/TypeInfo.h>

namespace Reflection
{
    class Assembly final
    {
    private:
        inline static std::map<std::type_index, TypeInfo>  _typeList;
        inline static std::mutex _listMutex;

    public:
        Assembly() = delete;

        inline static void AddType(const TypeInfo& typeInfo) noexcept
        {
            std::lock_guard<std::mutex> guard(_listMutex);
            _typeList.insert(std::make_pair(typeInfo.TypeIndex(), typeInfo));
        }

        inline static std::vector<TypeInfo> GetTypes() noexcept
        {
            std::lock_guard<std::mutex> guard(_listMutex);
            std::vector<TypeInfo> types;
            types.reserve(_typeList.size());
            for (const auto& type : _typeList)
                types.push_back(type.second);
            return types;
        }
    };
}

#endif