#include <ExtendedCpp/Reflection/CollectionFieldInfo.h>

std::any ExtendedCpp::Reflection::CollectionFieldInfo::GetElementValue(std::any object, const std::size_t position) const
{
    return _elementValue(_fieldHelper, std::move(object), position);
}

std::any ExtendedCpp::Reflection::CollectionFieldInfo::GetElementValue(const std::shared_ptr<void>& object, const std::size_t position) const
{
    if (!object)
        throw std::invalid_argument("Object is null");
    return _elementValueGetterFromPtr(_fieldHelper, object, position);
}

std::any ExtendedCpp::Reflection::CollectionFieldInfo::GetElement(std::any object, const std::size_t position) const
{
    return _elementRef(_fieldHelper, std::move(object), position);
}

std::any ExtendedCpp::Reflection::CollectionFieldInfo::GetElement(std::shared_ptr<void> object, const std::size_t position) const
{
    if (!object)
        throw std::invalid_argument("Object is null");
    return _elementGetterFromPtr(_fieldHelper, object, position);
}

std::any ExtendedCpp::Reflection::CollectionFieldInfo::ReadElement(std::any object, const std::size_t position) const
{
    return _elementConstRef(_fieldHelper, std::move(object), position);
}

std::any ExtendedCpp::Reflection::CollectionFieldInfo::ReadElement(const std::shared_ptr<void>& object, const std::size_t position) const
{
    if (!object)
        throw std::invalid_argument("Object is null");
    return _elementConstRef(_fieldHelper, object, position);
}

std::size_t ExtendedCpp::Reflection::CollectionFieldInfo::Size(std::any object) const
{
    return _sizeGetter(_fieldHelper, std::move(object));
}

std::size_t ExtendedCpp::Reflection::CollectionFieldInfo::Size(const std::shared_ptr<void>& object) const
{
    if (!object)
        throw std::invalid_argument("Object is null");
    return _sizeGetter(_fieldHelper, object);
}

void ExtendedCpp::Reflection::CollectionFieldInfo::Clear(std::any object) const
{
    _clear(_fieldHelper, std::move(object));
}

void ExtendedCpp::Reflection::CollectionFieldInfo::Clear(std::shared_ptr<void>& object) const
{
    if (!object)
        throw std::invalid_argument("Object is null");
    _clear(_fieldHelper, object);
}

std::type_index ExtendedCpp::Reflection::CollectionFieldInfo::ValueTypeIndex() const noexcept
{
    return _valueTypeIndex;
}
