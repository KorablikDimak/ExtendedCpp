#include <ExtendedCpp/Reflection/CollectionFieldInfo.h>

std::any ExtendedCpp::Reflection::CollectionFieldInfo::GetElementValue(std::any object, const std::size_t position) const
{
    return _elementValueGetter(_fieldHelper, std::move(object), position);
}

std::any ExtendedCpp::Reflection::CollectionFieldInfo::GetElement(std::any object, const std::size_t position) const
{
    return _elementGetter(_fieldHelper, std::move(object), position);
}

void* ExtendedCpp::Reflection::CollectionFieldInfo::GetElementPtr(std::any object, const std::size_t position) const
{
    return _elementGetterPtr(_fieldHelper, std::move(object), position);
}

std::any ExtendedCpp::Reflection::CollectionFieldInfo::ReadElement(std::any object, const std::size_t position) const
{
    return _elementReader(_fieldHelper, std::move(object), position);
}

const void* ExtendedCpp::Reflection::CollectionFieldInfo::ReadElementPtr(std::any object, const std::size_t position) const
{
    return _elementReaderPtr(_fieldHelper, std::move(object), position);
}

std::size_t ExtendedCpp::Reflection::CollectionFieldInfo::Size(std::any object) const
{
    return _sizeGetter(_fieldHelper, std::move(object));
}

std::type_index ExtendedCpp::Reflection::CollectionFieldInfo::ValueTypeIndex() const noexcept
{
    return _valueTypeIndex;
}

bool ExtendedCpp::Reflection::CollectionFieldInfo::IsCollection() const noexcept
{
    return true;
}