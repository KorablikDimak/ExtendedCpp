#include <ExtendedCpp/Reflection/CollectionFieldInfo.h>

std::type_index ExtendedCpp::Reflection::CollectionFieldInfo::ValueTypeIndex() const noexcept
{
    return _valueTypeIndex;
}

bool ExtendedCpp::Reflection::CollectionFieldInfo::IsCollection() const noexcept
{
    return true;
}