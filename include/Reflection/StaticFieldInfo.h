#ifndef Reflection_StaticFieldInfo_H
#define Reflection_StaticFieldInfo_H

#include <typeindex>

#include <Reflection/MemberInfo.h>

namespace Reflection
{
    #define STATIC_FIELD(name) \
    [this]{ \
        return std::make_shared<StaticFieldInfo>(#name, typeid(name), &(name)); \
    }()

    class StaticFieldInfo final : public MemberInfo
    {
    private:
        std::type_index _typeIndex;
        void* _field;

    public:
        StaticFieldInfo(const std::string& fieldName, std::type_index typeIndex, void* field) noexcept;
        StaticFieldInfo(std::string&& fieldName, std::type_index typeIndex, void* field) noexcept;
        ~StaticFieldInfo() override = default;

        [[nodiscard]]
        std::type_index TypeIndex() const noexcept;
        void* Value() noexcept;
        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override;
    };
}

#endif