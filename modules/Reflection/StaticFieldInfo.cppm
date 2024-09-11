module;

#include <any>
#include <typeindex>
#include <functional>
#include <string>

export module ExtendedCpp.Reflection.StaticFieldInfo;

import ExtendedCpp.Reflection.MemberInfo;
import ExtendedCpp.Reflection.MemberType;

export namespace ExtendedCpp::Reflection
{
    class StaticFieldInfo final : public MemberInfo
    {
    private:
        std::type_index _typeIndex;
        std::any _fieldHelper;
        std::any (*_fieldGetter)(const std::any& helper);

    public:
        template<typename TField, typename TFieldGetter>
        struct Helper final
        {
        private:
            TFieldGetter _fieldGetter;

        public:
            explicit Helper(TFieldGetter&& fieldGetter) noexcept :
                    _fieldGetter(std::forward<TFieldGetter>(fieldGetter)) {}

            TField* GetField() const noexcept
            {
                return _fieldGetter();
            }
        };

        template<typename THelper>
        StaticFieldInfo(const std::string& fieldName, std::type_index typeIndex, THelper&& fieldHelper) noexcept :
                _typeIndex(typeIndex),
                _fieldHelper(std::forward<THelper>(fieldHelper)),
                _fieldGetter([](const std::any& helper)
                             { return std::any(std::any_cast<const THelper&>(helper).GetField()); }),
                MemberInfo(fieldName) {}

        template<typename THelper>
        StaticFieldInfo(std::string&& fieldName, std::type_index typeIndex, THelper&& fieldHelper) noexcept :
                _typeIndex(typeIndex),
                _fieldHelper(std::forward<THelper>(fieldHelper)),
                _fieldGetter([](const std::any& helper)
                             { return std::any(std::any_cast<const THelper&>(helper).GetField()); }),
                MemberInfo(std::move(fieldName)) {}

        template<typename TField>
        TField* GetField() const noexcept
        {
            return std::any_cast<TField*>(_fieldGetter(_fieldHelper));
        }

        [[nodiscard]]
        std::any GetField() const noexcept
        {
            return _fieldGetter(_fieldHelper);
        }

        [[nodiscard]]
        std::type_index TypeIndex() const noexcept
        {
            return _typeIndex;
        }

        [[nodiscard]]
        Reflection::MemberType MemberType() const noexcept override
        {
            return MemberType::StaticField;
        }
    };
}