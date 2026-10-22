#include <ExtendedCpp/Reflection/CollectionFieldInfo.h>

std::any ExtendedCpp::Reflection::CollectionFieldInfo::GetElementValue(std::any object, const std::size_t position) const
{
    return _elementValue(_collectionFieldHelper, std::move(object), position);
}

std::any ExtendedCpp::Reflection::CollectionFieldInfo::GetElementValue(const std::shared_ptr<void>& object, const std::size_t position) const
{
    if (!object)
        throw std::invalid_argument("Object is null");

    return _elementValueFromPtr(_collectionFieldHelper, object, position);
}

std::any ExtendedCpp::Reflection::CollectionFieldInfo::GetElementRef(std::any object, const std::size_t position) const
{
    return _elementRef(_collectionFieldHelper, std::move(object), position);
}

std::any ExtendedCpp::Reflection::CollectionFieldInfo::GetElementRef(std::shared_ptr<void>& object, const std::size_t position) const
{
    if (!object)
        throw std::invalid_argument("Object is null");

    return _elementRefFromPtr(_collectionFieldHelper, object, position);
}

std::any ExtendedCpp::Reflection::CollectionFieldInfo::GetElementConstRef(std::any object, const std::size_t position) const
{
    return _elementConstRef(_collectionFieldHelper, std::move(object), position);
}

std::any ExtendedCpp::Reflection::CollectionFieldInfo::GetElementConstRef(const std::shared_ptr<void>& object, const std::size_t position) const
{
    if (!object)
        throw std::invalid_argument("Object is null");

    return _elementConstRefFromPtr(_collectionFieldHelper, object, position);
}

std::any ExtendedCpp::Reflection::CollectionFieldInfo::GetElementPtr(std::any object, const std::size_t position) const
{
    return _elementRef(_collectionFieldHelper, std::move(object), position);
}

std::any ExtendedCpp::Reflection::CollectionFieldInfo::GetElementPtr(std::shared_ptr<void>& object, const std::size_t position) const
{
    if (!object)
        throw std::invalid_argument("Object is null");

    return _elementPtrFromPtr(_collectionFieldHelper, object, position);
}

std::any ExtendedCpp::Reflection::CollectionFieldInfo::GetElementConstPtr(std::any object, const std::size_t position) const
{
    return _elementConstRef(_collectionFieldHelper, std::move(object), position);
}

std::any ExtendedCpp::Reflection::CollectionFieldInfo::GetElementConstPtr(const std::shared_ptr<void>& object, const std::size_t position) const
{
    if (!object)
        throw std::invalid_argument("Object is null");

    return _elementConstRef(_collectionFieldHelper, object, position);
}

std::size_t ExtendedCpp::Reflection::CollectionFieldInfo::Size(std::any object) const
{
    return _size(_collectionFieldHelper, std::move(object));
}

std::size_t ExtendedCpp::Reflection::CollectionFieldInfo::Size(const std::shared_ptr<void>& object) const
{
    if (!object)
        throw std::invalid_argument("Object is null");

    return _sizeFromPtr(_collectionFieldHelper, object);
}

void ExtendedCpp::Reflection::CollectionFieldInfo::Clear(std::any object) const
{
    _clear(_collectionFieldHelper, std::move(object));
}

void ExtendedCpp::Reflection::CollectionFieldInfo::Clear(std::shared_ptr<void>& object) const
{
    if (!object)
        throw std::invalid_argument("Object is null");

    _clearFromPtr(_collectionFieldHelper, object);
}

std::type_index ExtendedCpp::Reflection::CollectionFieldInfo::ValueTypeIndex() const noexcept
{
    return _valueTypeIndex;
}

bool ExtendedCpp::Reflection::CollectionFieldInfo::IsCollection() const noexcept
{
    return true;
}