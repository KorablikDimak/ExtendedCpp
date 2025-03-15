#include <ExtendedCpp/Reflection/StaticFieldInfo.h>

std::any ExtendedCpp::Reflection::StaticFieldInfo::GetField() const noexcept
{
    return _fieldGetter(_fieldHelper);
}

void* ExtendedCpp::Reflection::StaticFieldInfo::GetFieldPtr() const noexcept
{
    return _fieldGetterPtr(_fieldHelper);
}

std::any ExtendedCpp::Reflection::StaticFieldInfo::ReadField() const noexcept
{
    return _fieldReader(_fieldHelper);
}

const void* ExtendedCpp::Reflection::StaticFieldInfo::ReadFieldPtr() const noexcept
{
    return _fieldReaderPtr(_fieldHelper);
}

std::type_index ExtendedCpp::Reflection::StaticFieldInfo::TypeIndex() const noexcept
{
    return _typeIndex;
}

ExtendedCpp::Reflection::MemberType ExtendedCpp::Reflection::StaticFieldInfo::MemberType() const noexcept
{
    return MemberType::StaticField;
}