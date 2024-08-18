#ifndef Reflection_FieldInfo_H
#define Reflection_FieldInfo_H

#include <typeindex>

#include <Reflection/MemberInfo.h>

namespace Reflection
{
    #define FIELD(name) \
    std::make_shared<FieldInfo>(#name, typeid(name), &(name))

    class FieldInfo final : public MemberInfo
    {
    private:
        std::type_index _typeIndex;
        void* _field;

    public:
        FieldInfo(const std::string& fieldName, std::type_index typeIndex, void* field) noexcept;
        FieldInfo(std::string&& fieldName, std::type_index typeIndex, void* field) noexcept;
        ~FieldInfo() override = default;

        [[nodiscard]]
        std::type_index TypeIndex() const noexcept;
        void* Value() noexcept;
        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override;
    };
}

#endif