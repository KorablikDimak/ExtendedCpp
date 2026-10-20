#include <ExtendedCpp/Reflection/StaticFieldInfo.h>

std::any ExtendedCpp::Reflection::StaticFieldInfo::GetValue() const
{
    return _value(_fieldHelper);
}

std::any ExtendedCpp::Reflection::StaticFieldInfo::GetRef() const
{
    return _ref(_fieldHelper);
}

std::any ExtendedCpp::Reflection::StaticFieldInfo::GetConstRef() const noexcept
{
    return _constRef(_fieldHelper);
}

std::any ExtendedCpp::Reflection::StaticFieldInfo::GetPtr() const
{
    return _ptr(_fieldHelper);
}

std::any ExtendedCpp::Reflection::StaticFieldInfo::GetConstPtr() const noexcept
{
    return _constPtr(_fieldHelper);
}

void ExtendedCpp::Reflection::StaticFieldInfo::SetValue(std::any value) const
{
    _valueSetter(_fieldHelper, std::move(value));
}

std::type_index ExtendedCpp::Reflection::StaticFieldInfo::TypeIndex() const noexcept
{
    return _typeIndex;
}

ExtendedCpp::Reflection::MemberType ExtendedCpp::Reflection::StaticFieldInfo::GetMemberType() const noexcept
{
    return MemberType::StaticField;
}

bool ExtendedCpp::Reflection::StaticFieldInfo::IsConst() const noexcept
{
    return _isConst;
}