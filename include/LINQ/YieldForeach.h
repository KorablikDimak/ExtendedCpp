#ifndef LINQ_YieldForeach_H
#define LINQ_YieldForeach_H

#include <LINQ/Future.h>
#include <LINQ/TypeTraits.h>

namespace LINQ
{
    template<ConstIterable TCollection, typename TSource = typename TCollection::value_type>
    Future<TSource> YieldForeach(const TCollection& collection) noexcept
    {
        for (const auto& element : collection)
            co_yield element;
    }

    template<Iterable TCollection, typename TSource = typename TCollection::value_type>
    Future<TSource> YieldForeach(TCollection&& collection) noexcept
    {
        for (auto& element : collection)
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
        for (const auto& element : collection)
            co_yield std::move(element);
    }

    template<typename TConstIterator, typename TSource = typename std::iterator_traits<TConstIterator>::value_type>
    requires std::is_const<TConstIterator>::value_type
    Future<TSource> YieldForeach(const TConstIterator& begin, const TConstIterator& end) noexcept
    {
        for (const auto& it = begin; it != end; ++it)
            co_yield *it;
    }

    template<typename TIterator, typename TSource = typename std::iterator_traits<TIterator>::value_type>
    requires (!std::is_const<TIterator>::value_type)
    Future<TSource> YieldForeach(TIterator&& begin, TIterator&& end) noexcept
    {
        for (auto& it = begin; it != end; ++it)
            co_yield std::move(*it);
    }
}

#endif