#ifndef Reflection_StaticFieldInfo_H
#define Reflection_StaticFieldInfo_H

#include <any>
#include <typeindex>
#include <functional>

#include <ExtendedCpp/Reflection/MemberInfo.h>
#include <ExtendedCpp/Reflection/TypeTraits.h>

namespace ExtendedCpp::Reflection
{
    class StaticFieldInfo final : public MemberInfo
    {
    private:
        std::type_index _typeIndex;
        std::any _fieldHelper;
        std::any (*_fieldGetter)(const std::any& helper);

    public:
        template<typename TField>
        struct Helper final
        {
        private:
            TField* _fieldPtr;

        public:
            explicit Helper(TField* fieldPtr) noexcept :
                _fieldPtr(fieldPtr) {}

            TField* GetField() const noexcept
            {
                return _fieldPtr;
            }
        };

        template<typename THelper>
        StaticFieldInfo(const std::string& fieldName, std::type_index typeIndex, THelper&& fieldHelper) noexcept :
            MemberInfo(fieldName),
            _typeIndex(typeIndex),
            _fieldHelper(std::forward<THelper>(fieldHelper)),
            _fieldGetter([](const std::any& helper)
                { return std::any(std::any_cast<const THelper&>(helper).GetField()); }) {}

        template<typename THelper>
        StaticFieldInfo(std::string&& fieldName, std::type_index typeIndex, THelper&& fieldHelper) noexcept :
            MemberInfo(std::move(fieldName)),
            _typeIndex(typeIndex),
            _fieldHelper(std::forward<THelper>(fieldHelper)),
            _fieldGetter([](const std::any& helper)
                { return std::any(std::any_cast<const THelper&>(helper).GetField()); }) {}

        template<typename TField>
        TField* GetField() const
        {
            return std::any_cast<TField*>(_fieldGetter(_fieldHelper));
        }

        [[nodiscard]]
        std::any GetField() const noexcept;

        [[nodiscard]]
        std::type_index TypeIndex() const noexcept;
        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override;
    };

    template<typename TField>
    std::shared_ptr<MemberInfo> CreateStaticFieldInfo(const std::string& name, TField* fieldPtr) noexcept
    {
        return std::make_shared<StaticFieldInfo>(std::move(name), typeid(TField), StaticFieldInfo::Helper(fieldPtr));
    }

    template<typename TField>
    std::shared_ptr<MemberInfo> CreateStaticFieldInfo(std::string&& name, TField* fieldPtr) noexcept
    {
        return std::make_shared<StaticFieldInfo>(std::move(name), typeid(TField), StaticFieldInfo::Helper(fieldPtr));
    }
}

#define STATIC_FIELD(name) \
ExtendedCpp::Reflection::CreateStaticFieldInfo(#name, &ThisClassType::name)

#endif