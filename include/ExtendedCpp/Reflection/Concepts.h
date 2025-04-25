#ifndef Reflection_Concepts_H
#define Reflection_Concepts_H

#include <memory>
#include <iterator>

/// @brief 
namespace ExtendedCpp::Reflection::Concepts
{
    template<typename TCollection>
    concept Insertable = requires(TCollection collection)
    {
        { collection.begin() } -> std::same_as<typename TCollection::iterator>;
        { collection.end() } -> std::same_as<typename TCollection::iterator>;
        { collection.begin() + std::declval<std::size_t>() } -> std::same_as<typename TCollection::iterator>;
        { collection.size() } -> std::same_as<std::size_t>;
        { std::copy(collection.begin(), collection.end(), std::inserter(collection, collection.begin())) };
        { std::copy(collection.begin(), collection.end(), std::front_inserter(collection)) };
        { std::copy(collection.begin(), collection.end(), std::back_inserter(collection)) };
    };

    template<typename T>
    concept HasMetaInfo = requires
    {
        T::MetaInfo;
    };

    template<typename T>
    concept IsSharedPtr = std::same_as<std::decay_t<T>, std::shared_ptr<typename std::decay_t<T>::element_type>>;
}

#endif