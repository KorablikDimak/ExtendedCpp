#ifndef LINQ_YieldForeach_H
#define LINQ_YieldForeach_H

#include <ExtendedCpp/LINQ/Future.h>
#include <ExtendedCpp/LINQ/TypeTraits.h>

namespace ExtendedCpp::LINQ
{
    template<Concepts::ConstIterable TCollection, typename TSource = typename TCollection::value_type>
    Future<TSource> YieldForeach(const TCollection& collection) noexcept
    {
        for (const auto& element : collection)
            co_yield element;
    }

    template<Concepts::Iterable TCollection, typename TSource = typename TCollection::value_type>
    Future<TSource> YieldForeach(TCollection&& collection) noexcept
    {
        auto inner = std::move(collection);
        for (auto&& element : inner)
            co_yield std::move(element);
    }

    template<typename TKey, typename TValue>
    Future<std::pair<TKey, TValue>> YieldForeach(const std::map<TKey, TValue>& collection) noexcept
    {
        for (const auto& element : collection)
            co_yield element;
    }

    template<typename TKey, typename TValue>
    Future<std::pair<TKey, TValue>> YieldForeach(std::map<TKey, TValue>&& collection) noexcept
    {
        auto inner = std::move(collection);
        for (auto&& element : inner)
            co_yield std::move(element);
    }

    template<typename TKey, typename TValue>
    Future<std::pair<TKey, TValue>> YieldForeach(const std::unordered_map<TKey, TValue>& collection) noexcept
    {
        for (const auto& element : collection)
            co_yield element;
    }

    template<typename TKey, typename TValue>
    Future<std::pair<TKey, TValue>> YieldForeach(std::unordered_map<TKey, TValue>&& collection) noexcept
    {
        auto inner = std::move(collection);
        for (auto&& element : inner)
            co_yield std::move(element);
    }

    template<std::forward_iterator TIterator,
             typename TSource = typename std::iterator_traits<TIterator>::value_type>
    Future<TSource> YieldForeach(const TIterator begin, const TIterator end) noexcept
    {
        for (auto it = begin; it != end; ++it)
            co_yield *it;
    }
}

#endif