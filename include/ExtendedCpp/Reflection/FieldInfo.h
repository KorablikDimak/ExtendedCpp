#ifndef Reflection_FieldInfo_H
#define Reflection_FieldInfo_H

#include <any>
#include <typeindex>
#include <functional>
#include <stdexcept>

#include <ExtendedCpp/Reflection/MemberInfo.h>
#include <ExtendedCpp/Reflection/TypeTraits.h>

/// @brief 
namespace ExtendedCpp::Reflection
{
    /// @brief 
    class FieldInfo final : public MemberInfo
    {
    private:
        std::type_index _typeIndex;
        std::any _fieldHelper;
        std::any (*_fieldGetter)(const std::any& helper, std::any&& object);
        std::any (*_fieldReader)(const std::any& helper, std::any&& object);
        void* (*_fieldGetterPtr)(const std::any& helper, std::any&& object);
        const void* (*_fieldReaderPtr)(const std::any& helper, std::any&& object);

    public:
        /// @brief 
        /// @tparam TObject 
        /// @tparam TField 
        template<typename TObject, typename TField>
        struct Helper final
        {
        private:
            TField TObject::*_fieldPtr;

        public:
            /// @brief 
            /// @param fieldPtr 
            explicit Helper(TField TObject::* fieldPtr) noexcept :
                _fieldPtr(fieldPtr) {}

            /// @brief 
            /// @param object 
            /// @return 
            TField* GetField(std::any&& object) const
            {
                return &(std::any_cast<TObject*>(std::forward<std::any>(object))->*_fieldPtr);
            }

            /// @brief 
            /// @param object 
            /// @return 
            const TField* ReadField(std::any&& object) const
            {
                return &(std::any_cast<TObject*>(std::forward<std::any>(object))->*_fieldPtr);
            }
        };

        /// @brief 
        /// @tparam THelper 
        /// @param fieldName 
        /// @param typeIndex 
        /// @param fieldHelper 
        template<typename THelper>
        FieldInfo(std::string&& fieldName, std::type_index typeIndex, THelper&& fieldHelper) noexcept :
            MemberInfo(std::move(fieldName)),
            _typeIndex(typeIndex),
            _fieldHelper(std::forward<THelper>(fieldHelper)),
            _fieldGetter([](const std::any& helper, std::any&& object)
                { return std::any(std::any_cast<const THelper&>(helper).GetField(std::forward<std::any>(object))); }),
            _fieldReader([](const std::any& helper, std::any&& object)
                { return std::any(std::any_cast<const THelper&>(helper).ReadField(std::forward<std::any>(object))); }),
            _fieldGetterPtr([](const std::any& helper, std::any&& object)
                { return static_cast<void*>(std::any_cast<const THelper&>(helper).GetField(std::forward<std::any>(object))); }),
            _fieldReaderPtr([](const std::any& helper, std::any&& object)
                { return static_cast<const void*>(std::any_cast<const THelper&>(helper).ReadField(std::forward<std::any>(object))); }) {}

        /// @brief 
        ~FieldInfo() override = default;

        /// @brief 
        /// @tparam TField 
        /// @tparam TObject 
        /// @param object 
        /// @return 
        template<typename TField, typename TObject>
        TField* GetField(TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<TField*>(_fieldGetter(_fieldHelper, object));
        }

        /// @brief 
        /// @tparam TObject 
        /// @param object 
        /// @return 
        template<typename TObject>
        std::any GetField(TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _fieldGetter(_fieldHelper, object);
        }

        /// @brief 
        /// @tparam TObject 
        /// @param object 
        /// @return 
        template<typename TObject>
        void* GetFieldPtr(TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _fieldGetterPtr(_fieldHelper, object);
        }

        /// @brief 
        /// @tparam TField 
        /// @tparam TObject 
        /// @param object 
        /// @return 
        template<typename TField, typename TObject>
        const TField* ReadField(TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<TField*>(_fieldReader(_fieldHelper, object));
        }

        /// @brief 
        /// @tparam TObject 
        /// @param object 
        /// @return 
        template<typename TObject>
        std::any ReadField(TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _fieldReader(_fieldHelper, object);
        }

        /// @brief 
        /// @tparam TObject 
        /// @param object 
        /// @return 
        template<typename TObject>
        const void* ReadFieldPtr(TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _fieldReaderPtr(_fieldHelper, object);
        }

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
    /// @tparam TObject 
    /// @tparam TField 
    /// @param name 
    /// @param fieldPtr 
    /// @return 
    template<typename TObject, typename TField>
    std::shared_ptr<MemberInfo> CreateFieldInfo(std::string&& name, TField TObject::*fieldPtr) noexcept
    {
        return std::make_shared<FieldInfo>(std::move(name), typeid(TField), FieldInfo::Helper(fieldPtr));
    }
}

#define FIELD(name) \
ExtendedCpp::Reflection::CreateFieldInfo(#name, &ThisClassType::name)

#endif