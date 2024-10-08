#ifndef Reflection_FieldInfo_H
#define Reflection_FieldInfo_H

#include <any>
#include <typeindex>
#include <functional>
#include <stdexcept>

#include <ExtendedCpp/Reflection/MemberInfo.h>
#include <ExtendedCpp/Reflection/TypeTraits.h>

namespace ExtendedCpp::Reflection
{
    class FieldInfo final : public MemberInfo
    {
    private:
        std::type_index _typeIndex;
        std::any _fieldHelper;
        std::any (*_fieldGetter)(const std::any& helper, std::any&& object);
        std::any (*_fieldReader)(const std::any& helper, std::any&& object);

    public:
        template<typename TObject, typename TField>
        struct Helper final
        {
        private:
            TField TObject::*_fieldPtr;

        public:
            explicit Helper(TField TObject::* fieldPtr) noexcept :
                _fieldPtr(fieldPtr) {}

            TField* GetField(std::any&& object) const
            {
                return &(std::any_cast<TObject*>(std::move(object))->*_fieldPtr);
            }

            const TField* ReadField(std::any&& object) const
            {
                return &(std::any_cast<TObject*>(std::move(object))->*_fieldPtr);
            }
        };

        template<typename THelper>
        FieldInfo(const std::string& fieldName, std::type_index typeIndex, THelper&& fieldHelper) noexcept :
            MemberInfo(fieldName),
            _typeIndex(typeIndex),
            _fieldHelper(std::forward<THelper>(fieldHelper)),
            _fieldGetter([](const std::any& helper, std::any&& object)
                { return std::any(std::any_cast<const THelper&>(helper).GetField(std::move(object))); }),
            _fieldReader([](const std::any& helper, std::any&& object)
                { return std::any(std::any_cast<const THelper&>(helper).ReadField(std::move(object))); }) {}

        template<typename THelper>
        FieldInfo(std::string&& fieldName, std::type_index typeIndex, THelper&& fieldHelper) noexcept :
            MemberInfo(std::move(fieldName)),
            _typeIndex(typeIndex),
            _fieldHelper(std::forward<THelper>(fieldHelper)),
            _fieldGetter([](const std::any& helper, std::any&& object)
                { return std::any(std::any_cast<const THelper&>(helper).GetField(std::move(object))); }),
            _fieldReader([](const std::any& helper, std::any&& object)
                { return std::any(std::any_cast<const THelper&>(helper).ReadField(std::move(object))); }) {}

        ~FieldInfo() override = default;

        template<typename TField, typename TObject>
        TField* GetField(TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<TField*>(_fieldGetter(_fieldHelper, object));
        }

        template<typename TObject>
        std::any GetField(TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _fieldGetter(_fieldHelper, object);
        }

        template<typename TField, typename TObject>
        const TField* ReadField(TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<TField*>(_fieldReader(_fieldHelper, object));
        }

        template<typename TObject>
        std::any ReadField(TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _fieldReader(_fieldHelper, object);
        }

        [[nodiscard]]
        std::type_index TypeIndex() const noexcept;
        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override;
    };

    template<typename TObject, typename TField>
    std::shared_ptr<MemberInfo> CreateFieldInfo(const std::string& name, TField TObject::*fieldPtr) noexcept
    {
        return std::make_shared<FieldInfo>(name, typeid(TField), FieldInfo::Helper(fieldPtr));
    }

    template<typename TObject, typename TField>
    std::shared_ptr<MemberInfo> CreateFieldInfo(std::string&& name, TField TObject::*fieldPtr) noexcept
    {
        return std::make_shared<FieldInfo>(std::move(name), typeid(TField), FieldInfo::Helper(fieldPtr));
    }
}

#define FIELD(name) \
ExtendedCpp::Reflection::CreateFieldInfo(#name, &ThisClassType::name)

#endif