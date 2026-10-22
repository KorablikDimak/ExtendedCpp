#ifndef REFLECTION_STATIC_FIELD_INFO_H
#define REFLECTION_STATIC_FIELD_INFO_H

#include <any>
#include <format>
#include <memory>
#include <stdexcept>
#include <typeindex>

#include <ExtendedCpp/Reflection/MemberInfo.h>

/// @brief
namespace ExtendedCpp::Reflection
{
    /// @brief
    class StaticFieldInfo final : public MemberInfo
    {
    private:
        const std::type_index _typeIndex;
        const std::any _fieldHelper;
        const bool _isConst;

        std::any (*const _value)(const std::any& helper);
        std::any (*const _ref)(const std::any& helper);
        std::any (*const _constRef)(const std::any& helper) noexcept;
        std::any (*const _ptr)(const std::any& helper);
        std::any (*const _constPtr)(const std::any& helper) noexcept;
        void (*const _valueSetter)(const std::any& helper, std::any&& value);

        template<typename TField>
        struct Helper final
        {
        private:
            TField *const _fieldPtr;

        public:
            using FieldType = TField;
            static constexpr bool IS_CONST_FIELD = std::is_const_v<TField>;

            explicit Helper(TField* fieldPtr) noexcept :
                _fieldPtr(fieldPtr) {}

            [[nodiscard]]
            TField Value() const noexcept
            {
                return *_fieldPtr;
            }

            [[nodiscard]]
            std::reference_wrapper<TField> Ref() const noexcept
            {
                return std::ref(*_fieldPtr);
            }

            [[nodiscard]]
            std::reference_wrapper<const TField> ConstRef() const noexcept
            {
                return std::cref(*_fieldPtr);
            }

            [[nodiscard]]
            TField* Ptr() const noexcept
            {
                return _fieldPtr;
            }

            [[nodiscard]]
            const TField* ConstPtr() const noexcept
            {
                return _fieldPtr;
            }

            void SetValue(std::any&& value) const
            {
                *_fieldPtr = std::any_cast<TField>(std::move(value));
            }
        };

    public:
        /// @brief 
        /// @tparam THelper 
        /// @param fieldName 
        /// @param typeIndex 
        /// @param fieldHelper
        template<typename THelper>
        StaticFieldInfo(std::string&& fieldName, const std::type_index typeIndex, THelper&& fieldHelper) noexcept :
            MemberInfo(std::move(fieldName)),
            _typeIndex(typeIndex),
            _fieldHelper(std::forward<THelper>(fieldHelper)),
            _isConst(std::remove_cvref_t<THelper>::IS_CONST_FIELD),

            _value([](const std::any& helper) -> std::any
                {
                    if constexpr (!std::copy_constructible<typename std::remove_cvref_t<THelper>::FieldType>)
                        throw std::domain_error(std::format("You try get value, but field {} is not copy constructible.",
                            typeid(typename std::remove_cvref_t<THelper>::FieldType).name()));
                    else
                        return std::any(std::any_cast<THelper>(helper).Value());
                }),
            _ref([](const std::any& helper) -> std::any
                {
                    if constexpr (std::remove_cvref_t<THelper>::IS_CONST_FIELD)
                        throw std::domain_error(std::format("You try get non const reference, but field {} is const.",
                            typeid(typename std::remove_cvref_t<THelper>::FieldType).name()));
                    else
                        return std::any(std::any_cast<THelper>(helper).Ref());
                }),
            _constRef([](const std::any& helper) noexcept
                { return std::any(std::any_cast<THelper>(helper).ConstRef()); }),
            _ptr([](const std::any& helper) -> std::any
                {
                    if constexpr (std::remove_cvref_t<THelper>::IS_CONST_FIELD)
                        throw std::domain_error(std::format("You try get non const pointer, but field {} is const.",
                            typeid(typename std::remove_cvref_t<THelper>::FieldType).name()));
                    else
                        return std::any(std::any_cast<THelper>(helper).Ptr());
                }),
            _constPtr([](const std::any& helper) noexcept
                { return std::any(std::any_cast<THelper>(helper).ConstPtr()); }),
            _valueSetter([](const std::any& helper, std::any&& value)
                {
                    if constexpr (std::remove_cvref_t<THelper>::IS_CONST_FIELD)
                        throw std::domain_error(std::format("You try set value, but field {} is const.",
                            typeid(typename std::remove_cvref_t<THelper>::FieldType).name()));
                    else
                        std::any_cast<THelper>(helper).SetValue(std::move(value));
                }) {}

        /// @brief
        ~StaticFieldInfo() noexcept override = default;

        /// @brief 
        /// @tparam TField
        /// @throw std::bad_cast
        /// @return
        template<std::copy_constructible TField>
        [[nodiscard]]
        TField GetValue() const
        {
            return std::any_cast<TField>(_value(_fieldHelper));
        }

        /// @brief
        /// @throw std::domain_error
        /// @return
        [[nodiscard]]
        std::any GetValue() const;

        /// @brief
        /// @tparam TField
        /// @throw std::bad_cast
        /// @throw std::domain_error
        /// @return
        template<typename TField>
        [[nodiscard]]
        TField& GetRef() const
        {
            return std::any_cast<std::reference_wrapper<TField>>(_ref(_fieldHelper)).get();
        }

        /// @brief
        /// @throw std::domain_error
        /// @return
        [[nodiscard]]
        std::any GetRef() const;

        /// @brief
        /// @tparam TField
        /// @throw std::bad_cast
        /// @return
        template<typename TField>
        [[nodiscard]]
        const TField& GetConstRef() const
        {
            return std::any_cast<std::reference_wrapper<const TField>>(_constRef(_fieldHelper));
        }

        /// @brief
        /// @return
        [[nodiscard]]
        std::any GetConstRef() const noexcept;

        /// @brief 
        /// @tparam TField
        /// @throw std::bad_cast
        /// @throw std::domain_error
        /// @return
        template<typename TField>
        [[nodiscard]]
        TField* GetPtr() const
        {
            return std::any_cast<TField*>(_ptr(_fieldHelper));
        }

        /// @brief
        /// @throw std::domain_error
        /// @return
        [[nodiscard]]
        std::any GetPtr() const;

        /// @brief 
        /// @tparam TField
        /// @throw std::bad_cast
        /// @return
        template<typename TField>
        [[nodiscard]]
        const TField* GetConstPtr() const
        {
            return std::any_cast<TField*>(_constPtr(_fieldHelper));
        }

        /// @brief 
        /// @return
        [[nodiscard]]
        std::any GetConstPtr() const noexcept;

        /// @brief
        /// @throw std::bad_cast
        /// @throw std::domain_error
        /// @param value
        void SetValue(std::any value) const;

        /// @brief 
        /// @return
        [[nodiscard]]
        std::type_index TypeIndex() const noexcept;

        /// @brief 
        /// @return
        [[nodiscard]]
        MemberType GetMemberType() const noexcept override;

        /// @brief
        /// @return
        [[nodiscard]]
        bool IsConst() const noexcept;

        /// @brief
        /// @tparam TField
        /// @param name
        /// @param fieldPtr
        /// @return
        template<typename TField>
        [[nodiscard]]
        static std::shared_ptr<MemberInfo> Create(std::string&& name, TField *const fieldPtr) noexcept
        {
            return std::make_shared<StaticFieldInfo>(std::move(name), typeid(TField), Helper(fieldPtr));
        }
    };
}

#define STATIC_FIELD(name) \
ExtendedCpp::Reflection::StaticFieldInfo::Create(#name, &ThisClassType::name)

#endif