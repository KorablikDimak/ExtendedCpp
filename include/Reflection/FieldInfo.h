#ifndef Reflection_FieldInfo_H
#define Reflection_FieldInfo_H

#include <any>
#include <typeindex>
#include <functional>

#include <Reflection/MemberInfo.h>

namespace Reflection
{
    class FieldInfo final : public MemberInfo
    {
    private:
        std::type_index _typeIndex;
        std::any _fieldHelper;
        std::any (*_fieldGetter)(const std::any& helper, std::any&& object);

    public:
        template<typename TObject, typename TField>
        struct Helper final
        {
        private:
            std::function<TField*(TObject*)> _fieldGetter;

        public:
            explicit Helper(const std::function<TField*(TObject*)>& fieldGetter) noexcept :
                    _fieldGetter(fieldGetter) {}

            explicit Helper(std::function<TField*(TObject*)>&& fieldGetter) noexcept :
                _fieldGetter(std::move(fieldGetter)) {}

            TField* GetField(std::any&& object) const noexcept
            {
                return _fieldGetter(std::any_cast<TObject*>(object));
            }
        };

        template<typename THelper>
        FieldInfo(const std::string& fieldName, std::type_index typeIndex, THelper&& fieldHelper) noexcept :
            _typeIndex(typeIndex),
            _fieldHelper(std::forward<THelper>(fieldHelper)),
            _fieldGetter([](const std::any& helper, std::any&& object)
                { return std::any(std::any_cast<const THelper&>(helper).GetField(std::move(object))); }),
            MemberInfo(fieldName) {}

        template<typename THelper>
        FieldInfo(std::string&& fieldName, std::type_index typeIndex, THelper&& fieldHelper) noexcept :
            _typeIndex(typeIndex),
            _fieldHelper(std::forward<THelper>(fieldHelper)),
            _fieldGetter([](const std::any& helper, std::any&& object)
                { return std::any(std::any_cast<const THelper&>(helper).GetField(std::move(object))); }),
            MemberInfo(std::move(fieldName)) {}

        ~FieldInfo() override = default;

        template<typename TField, typename TObject>
        TField* GetField(TObject* object) const noexcept
        {
            if (object == nullptr) return {};
            return std::any_cast<TField*>(_fieldGetter(_fieldHelper, object));
        }

        template<typename TObject>
        std::any GetField(TObject* object) const noexcept
        {
            if (object == nullptr) return {};
            return _fieldGetter(_fieldHelper, object);
        }

        [[nodiscard]]
        std::type_index TypeIndex() const noexcept;
        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override;
    };
}

#define FIELD(name) \
[]()->std::shared_ptr<Reflection::MemberInfo> \
{ \
    using FieldType = decltype(std::declval<ThisClassType>().name); \
    return std::make_shared<Reflection::FieldInfo>(#name, typeid(FieldType), \
        Reflection::FieldInfo::Helper<ThisClassType, FieldType>([](ThisClassType* object)->FieldType* \
        { \
            return &(object->name);\
        })); \
}()

#endif