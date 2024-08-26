#ifndef Reflection_StaticFieldInfo_H
#define Reflection_StaticFieldInfo_H

#include <any>
#include <typeindex>
#include <functional>

#include <Reflection/MemberInfo.h>

namespace Reflection
{
    class StaticFieldInfo final : public MemberInfo
    {
    private:
        std::type_index _typeIndex;
        std::any _fieldHelper;
        std::any (*_fieldGetter)(std::any& helper);

    public:
        template<typename TField>
        struct Helper final
        {
            std::function<TField*()> _fieldGetter;

            explicit Helper(std::function<TField*()>&& fieldGetter) noexcept :
                _fieldGetter(std::move(fieldGetter)) {}

            TField* GetField() noexcept
            {
                return _fieldGetter();
            }
        };

        template<typename THelper>
        StaticFieldInfo(const std::string& fieldName, std::type_index typeIndex, THelper&& fieldHelper) noexcept :
            _typeIndex(typeIndex),
            _fieldHelper(std::forward<THelper>(fieldHelper)),
            _fieldGetter([](std::any& helper)
                { return std::any(std::any_cast<THelper&>(helper).GetField()); }),
            MemberInfo(fieldName) {}

        template<typename THelper>
        StaticFieldInfo(std::string&& fieldName, std::type_index typeIndex, THelper&& fieldHelper) noexcept :
            _typeIndex(typeIndex),
            _fieldHelper(std::forward<THelper>(fieldHelper)),
            _fieldGetter([](std::any& helper)
                { return std::any(std::any_cast<THelper&>(helper).GetField()); }),
            MemberInfo(std::move(fieldName)) {}

        auto GetField() noexcept
        {
            return _fieldGetter(_fieldHelper);
        }

        [[nodiscard]]
        std::type_index TypeIndex() const noexcept;
        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override;
    };
}

#define STATIC_FIELD(name) \
[]()->std::shared_ptr<Reflection::MemberInfo> \
{ \
    using FieldType = decltype(name); \
    return std::make_shared<Reflection::StaticFieldInfo>(#name, typeid(FieldType), \
        Reflection::StaticFieldInfo::Helper<FieldType>([]()->FieldType* \
        { \
            return &(name); \
        })); \
}()

#endif