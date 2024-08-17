#ifndef Reflection_FieldInfo_H
#define Reflection_FieldInfo_H

#include <any>

#include <Reflection/MemberInfo.h>

namespace Reflection
{
    #define FIELD(name) \
    std::make_shared<FieldInfo>(#name, &(name))

    class FieldInfo final : public MemberInfo
    {
    private:
        void* _field;

    public:
        FieldInfo(const std::string& fieldName, void* field) noexcept;
        FieldInfo(std::string&& fieldName, void* field) noexcept;
        ~FieldInfo() override = default;

        void* Value() noexcept;
        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override;
    };
}

#endif