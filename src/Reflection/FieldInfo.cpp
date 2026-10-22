#include <ExtendedCpp/Reflection/FieldInfo.h>

std::any ExtendedCpp::Reflection::FieldInfo::GetValue(std::any object) const
{
    return _value(_fieldHelper, std::move(object));
}

std::any ExtendedCpp::Reflection::FieldInfo::GetValue(const std::shared_ptr<void>& object) const
{
    if (!object)
        throw std::invalid_argument("Object is null");

    return _valueFromPtr(_fieldHelper, object);
}

std::any ExtendedCpp::Reflection::FieldInfo::GetRef(std::any object) const
{
    return _ref(_fieldHelper, std::move(object));
}

std::any ExtendedCpp::Reflection::FieldInfo::GetRef(std::shared_ptr<void>& object) const
{
    if (!object)
        throw std::invalid_argument("Object is null");

    return _refFromPtr(_fieldHelper, object);
}

std::any ExtendedCpp::Reflection::FieldInfo::GetConstRef(std::any object) const
{
    return _constRef(_fieldHelper, std::move(object));
}

std::any ExtendedCpp::Reflection::FieldInfo::GetConstRef(const std::shared_ptr<void>& object) const
{
    if (!object)
        throw std::invalid_argument("Object is null");

    return _constRefFromPtr(_fieldHelper, object);
}

std::any ExtendedCpp::Reflection::FieldInfo::GetPtr(std::any object) const
{
    return _ptr(_fieldHelper, std::move(object));
}

std::any ExtendedCpp::Reflection::FieldInfo::GetPtr(std::shared_ptr<void>& object) const
{
    if (!object)
        throw std::invalid_argument("Object is null");

    return _ptrFromPtr(_fieldHelper, object);
}

std::any ExtendedCpp::Reflection::FieldInfo::GetConstPtr(std::any object) const
{
    return _constPtr(_fieldHelper, std::move(object));
}

std::any ExtendedCpp::Reflection::FieldInfo::GetConstPtr(const std::shared_ptr<void>& object) const
{
    if (!object)
        throw std::invalid_argument("Object is null");

    return _constPtrFromPtr(_fieldHelper, object);
}

std::type_index ExtendedCpp::Reflection::FieldInfo::TypeIndex() const noexcept
{
    return _typeIndex;
}

ExtendedCpp::Reflection::MemberType ExtendedCpp::Reflection::FieldInfo::GetMemberType() const noexcept
{
    return MemberType::Field;
}

bool ExtendedCpp::Reflection::FieldInfo::IsCollection() const noexcept
{
    return false;
}

bool ExtendedCpp::Reflection::FieldInfo::IsConst() const noexcept
{
    return _isConst;
}

void ExtendedCpp::Reflection::FieldInfo::SetValue(std::any object, std::any value) const
{
    _valueSetter(_fieldHelper, std::move(object), std::move(value));
}

void ExtendedCpp::Reflection::FieldInfo::SetValue(std::shared_ptr<void>& object, std::any value) const
{
    _valueSetterFromPtr(_fieldHelper, object, std::move(value));
}