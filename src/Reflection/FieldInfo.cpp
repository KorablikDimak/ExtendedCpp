#include <ExtendedCpp/Reflection/FieldInfo.h>

std::any ExtendedCpp::Reflection::FieldInfo::GetValue(std::any object) const
{
    return _fieldValueGetter(_fieldHelper, std::move(object));
}

std::any ExtendedCpp::Reflection::FieldInfo::GetValue(const std::shared_ptr<void>& object) const
{
    if (!object)
        throw std::invalid_argument("Object is null");
    return _fieldValueGetterToPtr(_fieldHelper, object);
}

std::any ExtendedCpp::Reflection::FieldInfo::GetField(std::any object) const
{
    return _fieldGetter(_fieldHelper, std::move(object));
}

std::any ExtendedCpp::Reflection::FieldInfo::GetField(std::shared_ptr<void>& object) const
{
    if (!object)
        throw std::invalid_argument("Object is null");
    return _fieldGetterToPtr(_fieldHelper, object);
}

std::any ExtendedCpp::Reflection::FieldInfo::ReadField(std::any object) const
{
    return _fieldReader(_fieldHelper, std::move(object));
}

std::any ExtendedCpp::Reflection::FieldInfo::ReadField(const std::shared_ptr<void>& object) const
{
    if (!object)
        throw std::invalid_argument("Object is null");
    return _fieldReaderToPtr(_fieldHelper, object);
}

std::type_index ExtendedCpp::Reflection::FieldInfo::TypeIndex() const noexcept
{
    return _typeIndex;
}

ExtendedCpp::Reflection::MemberType ExtendedCpp::Reflection::FieldInfo::GetMemberType() const noexcept
{
    return MemberType::Field;
}