#ifndef LINQ_YIELD_FOREACH_H
#define LINQ_YIELD_FOREACH_H

#include <iterator>
#include <utility>

#include <ExtendedCpp/LINQ/Concepts.h>
#include <ExtendedCpp/LINQ/Future.h>

/// @brief
// ReSharper disable once CppInconsistentNaming
namespace ExtendedCpp::LINQ
{
    /// @brief
    /// @tparam TCollection
    /// @tparam TSource
    /// @tparam NothrowIterable
    /// @param collection 
    /// @return
    template<Concepts::InputIterable TCollection,
             typename TSource = typename std::decay_t<TCollection>::value_type,
             bool NothrowIterable = Concepts::IsNothrowIterable<TCollection>>
    requires (!Concepts::IsPair<TSource>)
    Future<TSource, NothrowIterable> YieldForeach(TCollection collection) noexcept(NothrowIterable)
    {
        for (auto&& element : collection)
            co_yield element;
        co_return;
    }

    /// @brief
    /// @tparam TCollection
    /// @tparam TSource
    /// @tparam TKey
    /// @tparam TValue
    /// @tparam NothrowIterable
    /// @param collection
    /// @return
    template<Concepts::InputIterable TCollection,
             Concepts::IsPair TSource = typename std::decay_t<TCollection>::value_type,
             typename TKey = std::remove_const_t<typename PairTraits<TSource>::FirstType>,
             typename TValue = typename PairTraits<TSource>::SecondType,
             bool NothrowIterable = Concepts::IsNothrowIterable<TCollection>>
    Future<std::pair<TKey, TValue>, NothrowIterable> YieldForeach(TCollection collection) noexcept(NothrowIterable)
    {
        for (auto&& element : collection)
            co_yield element;
        co_return;
    }

    /// @brief
    /// @tparam TIterator
    /// @tparam TSource
    /// @tparam NothrowIterable
    /// @param begin 
    /// @param end 
    /// @return
    template<std::input_iterator TIterator,
             typename TSource = std::iter_value_t<TIterator>,
             bool NothrowIterable = noexcept(noexcept(++std::declval<TIterator&>()) && noexcept(*std::declval<TIterator&>()))>
    Future<TSource, NothrowIterable> YieldForeach(const TIterator begin, const TIterator end) noexcept(NothrowIterable)
    {
        for (auto it = begin; it != end; ++it)
            co_yield *it;
        co_return;
    }
}

#endif