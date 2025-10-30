#ifndef LINQ_YieldForeach_H
#define LINQ_YieldForeach_H

#include <utility>
#include <iterator>

#include <ExtendedCpp/LINQ/Future.h>
#include <ExtendedCpp/LINQ/Concepts.h>

/// @brief 
namespace ExtendedCpp::LINQ
{
    /// @brief 
    /// @tparam TSource 
    /// @tparam TCollection 
    /// @param collection 
    /// @return 
    template<Concepts::InputIterable TCollection,
             typename TSource = typename std::decay_t<TCollection>::value_type,
             bool NothrowIterable = Concepts::IsNothrowIterable<TCollection>>
    requires (!Concepts::IsPair<TSource>)
    Future<TSource, std::integral_constant<bool, NothrowIterable>>
    YieldForeach(TCollection&& collection) noexcept(NothrowIterable)
    {
        auto inner = std::forward<TCollection>(collection);
        for (auto&& element : inner)
            co_yield element;
    }

    template<Concepts::InputIterable TCollection,
             Concepts::IsPair TSource = typename std::decay_t<TCollection>::value_type,
             typename TKey = std::remove_const_t<typename PairTraits<TSource>::FirstType>,
             typename TValue = typename PairTraits<TSource>::SecondType,
             bool NothrowIterable = Concepts::IsNothrowIterable<TCollection>>
    Future<std::pair<TKey, TValue>, std::integral_constant<bool, NothrowIterable>>
    YieldForeach(TCollection&& collection) noexcept(NothrowIterable)
    {
        auto inner = std::forward<TCollection>(collection);
        for (auto&& element : inner)
            co_yield element;
    }

    /// @brief 
    /// @tparam TSource 
    /// @tparam TIterator 
    /// @param begin 
    /// @param end 
    /// @return 
    template<std::input_iterator TIterator,
             typename TSource = typename TIterator::value_type,
	         bool NothrowIterable = noexcept(++std::declval<TIterator>())>
    Future<TSource, std::integral_constant<bool, NothrowIterable>>
    YieldForeach(const TIterator begin, const TIterator end) noexcept(NothrowIterable)
    {
        for (auto it = begin; it != end; ++it)
            co_yield *it;
    }
}

#endif