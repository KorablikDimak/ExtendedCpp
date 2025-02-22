#ifndef Reflection_StaticFieldInfo_H
#define Reflection_StaticFieldInfo_H

#include <any>
#include <typeindex>
#include <functional>

#include <ExtendedCpp/Reflection/MemberInfo.h>
#include <ExtendedCpp/Reflection/TypeTraits.h>

namespace ExtendedCpp::Reflection
{
    /// @brief 
    class StaticFieldInfo final : public MemberInfo
    {
    private:
        std::type_index _typeIndex;
        std::any _fieldHelper;
        std::any (*_fieldGetter)(const std::any& helper);

    public:
        /// @brief 
        /// @tparam TField 
        template<typename TField>
        struct Helper final
        {
        private:
            TField* _fieldPtr;

        public:
            /// @brief 
            /// @param fieldPtr 
            explicit Helper(TField* fieldPtr) noexcept :
                _fieldPtr(fieldPtr) {}

            /// @brief 
            /// @return 
            TField* GetField() const noexcept
            {
                return _fieldPtr;
            }
        };

        /// @brief 
        /// @tparam THelper 
        /// @param fieldName 
        /// @param typeIndex 
        /// @param fieldHelper 
        template<typename THelper>
        StaticFieldInfo(const std::string& fieldName, std::type_index typeIndex, THelper&& fieldHelper) noexcept :
            MemberInfo(fieldName),
            _typeIndex(typeIndex),
            _fieldHelper(std::forward<THelper>(fieldHelper)),
            _fieldGetter([](const std::any& helper)
                { return std::any(std::any_cast<const THelper&>(helper).GetField()); }) {}

        /// @brief 
        /// @tparam THelper 
        /// @param fieldName 
        /// @param typeIndex 
        /// @param fieldHelper 
        template<typename THelper>
        StaticFieldInfo(std::string&& fieldName, std::type_index typeIndex, THelper&& fieldHelper) noexcept :
            MemberInfo(std::move(fieldName)),
            _typeIndex(typeIndex),
            _fieldHelper(std::forward<THelper>(fieldHelper)),
            _fieldGetter([](const std::any& helper)
                { return std::any(std::any_cast<const THelper&>(helper).GetField()); }) {}

        /// @brief 
        /// @tparam TField 
        /// @return 
        template<typename TField>
        TField* GetField() const
        {
            return std::any_cast<TField*>(_fieldGetter(_fieldHelper));
        }

        /// @brief 
        /// @return 
        [[nodiscard]]
        std::any GetField() const noexcept;

        /// @brief 
        /// @return 
        [[nodiscard]]
        std::type_index TypeIndex() const noexcept;

        /// @brief 
        /// @return 
        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override;
    };

    /// @brief 
    /// @tparam TField 
    /// @param name 
    /// @param fieldPtr 
    /// @return 
    template<typename TField>
    std::shared_ptr<MemberInfo> CreateStaticFieldInfo(const std::string& name, TField* fieldPtr) noexcept
    {
        return std::make_shared<StaticFieldInfo>(std::move(name), typeid(TField), StaticFieldInfo::Helper(fieldPtr));
    }

    /// @brief 
    /// @tparam TField 
    /// @param name 
    /// @param fieldPtr 
    /// @return 
    template<typename TField>
    std::shared_ptr<MemberInfo> CreateStaticFieldInfo(std::string&& name, TField* fieldPtr) noexcept
    {
        return std::make_shared<StaticFieldInfo>(std::move(name), typeid(TField), StaticFieldInfo::Helper(fieldPtr));
    }
}

#define STATIC_FIELD(name) \
ExtendedCpp::Reflection::CreateStaticFieldInfo(#name, &ThisClassType::name)

#endif