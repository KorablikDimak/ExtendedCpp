module;

#include <any>
#include <typeindex>
#include <string>
#include <functional>

export module ExtendedCpp.Reflection.FieldInfo;

import ExtendedCpp.Reflection.MemberInfo;
import ExtendedCpp.Reflection.MemberType;

export namespace ExtendedCpp::Reflection
{
    class FieldInfo final : public MemberInfo
    {
    private:
        std::type_index _typeIndex;
        std::any _fieldHelper;
        std::any (*_fieldGetter)(const std::any& helper, std::any&& object);

    public:
        template<typename TObject, typename TField, typename TFieldGetter>
        struct Helper final
        {
        private:
            TFieldGetter _fieldGetter;

        public:
            explicit Helper(TFieldGetter&& fieldGetter) noexcept :
                    _fieldGetter(std::forward<TFieldGetter>(fieldGetter)) {}

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
        std::type_index TypeIndex() const noexcept
        {
            return _typeIndex;
        }

        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override
        {
            return MemberType::Field;
        }
    };
}