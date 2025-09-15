#include <ExtendedCpp/Reflection/FieldInfo.h>

std::any ExtendedCpp::Reflection::FieldInfo::GetValue(std::any object) const
{
    return _fieldValueGetter(_fieldHelper, std::move(object));
}

std::any ExtendedCpp::Reflection::FieldInfo::GetField(std::any object) const
{
    return _fieldGetter(_fieldHelper, std::move(object));
}

void* ExtendedCpp::Reflection::FieldInfo::GetFieldPtr(std::any object) const
{
	return _fieldGetterPtr(_fieldHelper, std::move(object));
}

std::any ExtendedCpp::Reflection::FieldInfo::ReadField(std::any object) const
{
    return _fieldReader(_fieldHelper, std::move(object));
}

const void* ExtendedCpp::Reflection::FieldInfo::ReadFieldPtr(std::any object) const
{
    return _fieldReaderPtr(_fieldHelper, std::move(object));
}

std::type_index ExtendedCpp::Reflection::FieldInfo::TypeIndex() const noexcept
{
    return _typeIndex;
}

ExtendedCpp::Reflection::MemberType ExtendedCpp::Reflection::FieldInfo::MemberType() const noexcept
{
    return MemberType::Field;
}