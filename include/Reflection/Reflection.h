#ifndef Reflection_Reflection_H
#define Reflection_Reflection_H

#include <Reflection/Assembly.h>

namespace Reflection
{
    typedef std::vector<std::shared_ptr<ConstructorInfo>> Constructors;
    typedef std::vector<std::shared_ptr<FieldInfo>> Fields;
    typedef std::vector<std::shared_ptr<StaticFieldInfo>> StaticFields;
    typedef std::vector<std::shared_ptr<MethodInfo>> Methods;
    typedef std::vector<std::shared_ptr<StaticMethodInfo>> StaticMethods;

    inline TypeInfo GetType(const std::string& typeName)
    {
        for (const auto& type : Assembly::GetTypes())
            if (std::string_view(type.TypeIndex().name()) == typeName)
                return type;
        throw std::invalid_argument("Assembly doesnt contain type " + typeName);
    }

    inline TypeInfo GetType(std::type_index typeIndex)
    {
        for (const auto& type : Assembly::GetTypes())
            if (type.TypeIndex() == typeIndex)
                return type;
        throw std::invalid_argument("Assembly doesnt contain type " + std::string(typeIndex.name()));
    }

    template<typename T>
    TypeInfo GetType() noexcept
    {
        return T::MetaInfo;
    }

    inline Constructors GetConstructors(const std::string_view& typeName) noexcept
    {
        for (const auto& type : Assembly::GetTypes())
            if (std::string_view(type.TypeIndex().name()) == typeName)
                return type.GetConstructors();
        return {};
    }

    inline Constructors GetConstructors(std::type_index typeIndex) noexcept
    {
        for (const auto& type : Assembly::GetTypes())
            if (type.TypeIndex() == typeIndex)
                return type.GetConstructors();
        return {};
    }

    template<typename T>
    inline Constructors GetConstructors() noexcept
    {
        for (const auto& type : Assembly::GetTypes())
            if (type.TypeIndex() == typeid(T))
                return type.GetConstructors();
        return {};
    }

    inline Fields GetFields(const std::string_view& typeName) noexcept
    {
        for (const auto& type : Assembly::GetTypes())
            if (std::string_view(type.TypeIndex().name()) == typeName)
                return type.GetFields();
        return {};
    }

    inline Fields GetFields(std::type_index typeIndex) noexcept
    {
        for (const auto& type : Assembly::GetTypes())
            if (type.TypeIndex() == typeIndex)
                return type.GetFields();
        return {};
    }

    template<typename T>
    inline Fields GetFields() noexcept
    {
        for (const auto& type : Assembly::GetTypes())
            if (type.TypeIndex() == typeid(T))
                return type.GetFields();
        return {};
    }

    inline StaticFields GetStaticFields(const std::string_view& typeName) noexcept
    {
        for (const auto& type : Assembly::GetTypes())
            if (std::string_view(type.TypeIndex().name()) == typeName)
                return type.GetStaticFields();
        return {};
    }

    inline StaticFields GetStaticFields(std::type_index typeIndex) noexcept
    {
        for (const auto& type : Assembly::GetTypes())
            if (type.TypeIndex() == typeIndex)
                return type.GetStaticFields();
        return {};
    }

    template<typename T>
    inline StaticFields GetStaticFields() noexcept
    {
        for (const auto& type : Assembly::GetTypes())
            if (type.TypeIndex() == typeid(T))
                return type.GetStaticFields();
        return {};
    }

    inline Methods GetMethods(const std::string_view& typeName) noexcept
    {
        for (const auto& type : Assembly::GetTypes())
            if (std::string_view(type.TypeIndex().name()) == typeName)
                return type.GetMethods();
        return {};
    }

    inline Methods GetMethods(std::type_index typeIndex) noexcept
    {
        for (const auto& type : Assembly::GetTypes())
            if (type.TypeIndex() == typeIndex)
                return type.GetMethods();
        return {};
    }

    template<typename T>
    inline Methods GetMethods() noexcept
    {
        for (const auto& type : Assembly::GetTypes())
            if (type.TypeIndex() == typeid(T))
                return type.GetMethods();
        return {};
    }

    inline StaticMethods GetStaticMethods(const std::string_view& typeName) noexcept
    {
        for (const auto& type : Assembly::GetTypes())
            if (std::string_view(type.TypeIndex().name()) == typeName)
                return type.GetStaticMethods();
        return {};
    }

    inline StaticMethods GetStaticMethods(std::type_index typeIndex) noexcept
    {
        for (const auto& type : Assembly::GetTypes())
            if (type.TypeIndex() == typeIndex)
                return type.GetStaticMethods();
        return {};
    }

    template<typename T>
    inline StaticMethods GetStaticMethods() noexcept
    {
        for (const auto& type : Assembly::GetTypes())
            if (type.TypeIndex() == typeid(T))
                return type.GetStaticMethods();
        return {};
    }
}

#endif