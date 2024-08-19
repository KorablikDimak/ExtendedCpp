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
        typedef std::vector<std::shared_ptr<ConstructorInfo>> Constructors;
        typedef std::vector<std::shared_ptr<StaticMethodInfo>> StaticMethods;
        typedef std::vector<std::shared_ptr<StaticFieldInfo>> StaticFields;

        inline static std::map<std::type_index, std::tuple<Constructors, StaticMethods, StaticFields>>  _typeList;
        inline static std::mutex _listMutex;

    public:
        Assembly() = delete;

        inline static void AddType(const TypeInfo& typeInfo) noexcept
        {
            std::lock_guard<std::mutex> guard(_listMutex);
            _typeList.insert(std::make_pair(typeInfo.TypeIndex(), std::make_tuple(typeInfo.GetConstructors(),
                                                                                  typeInfo.GetStaticMethods(),
                                                                                  typeInfo.GetStaticFields())));
        }

        inline static Constructors GetConstructors(const std::string_view& typeName) noexcept
        {
            std::lock_guard<std::mutex> guard(_listMutex);
            for (const auto& type : _typeList)
                if (std::string_view(type.first.name()) == typeName)
                    return std::get<0>(type.second);
            return {};
        }

        inline static Constructors GetConstructors(std::type_index typeIndex) noexcept
        {
            std::lock_guard<std::mutex> guard(_listMutex);
            for (const auto& type : _typeList)
                if (type.first == typeIndex)
                    return std::get<0>(type.second);
            return {};
        }

        template<typename T>
        inline static Constructors GetConstructors() noexcept
        {
            std::lock_guard<std::mutex> guard(_listMutex);
            for (const auto& type : _typeList)
                if (type.first == typeid(T))
                    return std::get<0>(type.second);
            return {};
        }

        inline static StaticMethods GetStaticMethods(const std::string_view& typeName) noexcept
        {
            std::lock_guard<std::mutex> guard(_listMutex);
            for (const auto& type : _typeList)
                if (std::string_view(type.first.name()) == typeName)
                    return std::get<1>(type.second);
            return {};
        }

        inline static StaticMethods GetStaticMethods(std::type_index typeIndex) noexcept
        {
            std::lock_guard<std::mutex> guard(_listMutex);
            for (const auto& type : _typeList)
                if (type.first == typeIndex)
                    return std::get<1>(type.second);
            return {};
        }

        template<typename T>
        inline static StaticMethods GetStaticMethods() noexcept
        {
            std::lock_guard<std::mutex> guard(_listMutex);
            for (const auto& type : _typeList)
                if (type.first == typeid(T))
                    return std::get<1>(type.second);
            return {};
        }

        inline static StaticFields GetStaticFields(const std::string_view& typeName) noexcept
        {
            std::lock_guard<std::mutex> guard(_listMutex);
            for (const auto& type : _typeList)
                if (std::string_view(type.first.name()) == typeName)
                    return std::get<2>(type.second);
            return {};
        }

        inline static StaticFields GetStaticFields(std::type_index typeIndex) noexcept
        {
            std::lock_guard<std::mutex> guard(_listMutex);
            for (const auto& type : _typeList)
                if (type.first == typeIndex)
                    return std::get<2>(type.second);
            return {};
        }

        template<typename T>
        inline static StaticFields GetStaticFields() noexcept
        {
            std::lock_guard<std::mutex> guard(_listMutex);
            for (const auto& type : _typeList)
                if (type.first == typeid(T))
                    return std::get<2>(type.second);
            return {};
        }
    };
}

#endif