#ifndef LINQ_Aggregate_H
#define LINQ_Aggregate_H

#include <ExtendedCpp/LINQ/TypeTraits.h>
#include <ExtendedCpp/LINQ/Concepts.h>

namespace ExtendedCpp::LINQ::Aggregate
{
    template<typename TResult, Concepts::RandomAccess TCollection,
             std::invocable<TResult, RandomAccessValueType<TCollection>> TAggregate>
    TResult Aggregate(TCollection&& collection, const std::size_t start, const std::size_t end,
                      TAggregate&& aggregateFunction)
    {
        TResult result = collection[start];
        for (std::size_t i = start + 1; i <= end; ++i)
            result = aggregateFunction(result, collection[i]);
        return result;
    }

    template<Concepts::RandomAccess TCollection, typename TPredicate,
             typename T = RandomAccessValueType<TCollection>>
    requires Concepts::IsPredicate<TPredicate, T>
    std::size_t Count(TCollection&& collection, const std::size_t start,
                      const std::size_t end, TPredicate&& predicate)
    noexcept(std::is_nothrow_invocable_v<TPredicate, T>)
    {
        std::size_t count = 0;
        for (std::size_t i = start; i <= end; ++i)
            if (predicate(collection[i]))
                ++count;
        return count;
    }

    template<Concepts::RandomAccess TCollection, Concepts::Summarize T = RandomAccessValueType<TCollection>>
    T Sum(TCollection&& collection, const std::size_t start, const std::size_t end) noexcept
    {
        T sum = collection[start];
        for (std::size_t i = start + 1; i <= end; ++i)
            sum += collection[i];
        return sum;
    }

    template<Concepts::RandomAccess TCollection,
             typename T = RandomAccessValueType<TCollection>,
             std::invocable<T> TSelector,
             Concepts::Summarize TResult = std::invoke_result_t<TSelector, T>>
    TResult Sum(TCollection&& collection, const std::size_t start, const std::size_t end, TSelector&& selector)
    noexcept(std::is_nothrow_invocable_v<TSelector, T>)
    {
        TResult sum = selector(collection[start]);
        for (std::size_t i = start + 1; i <= end; ++i)
            sum += selector(collection[i]);
        return sum;
    }

    template<Concepts::RandomAccess TCollection,
             Concepts::Comparable T = RandomAccessValueType<TCollection>>
    T Min(TCollection&& collection, const std::size_t start, const std::size_t end) noexcept
    {
        T min = collection[start];
        for (std::size_t i = start + 1; i <= end; ++i)
            if (collection[i] < min)
                min = collection[i];
        return min;
    }

    template<Concepts::RandomAccess TCollection,
             typename T = RandomAccessValueType<TCollection>,
             std::invocable<T> TSelector,
             Concepts::Comparable TResult = std::invoke_result_t<TSelector, T>>
    TResult Min(TCollection&& collection, const std::size_t start, const std::size_t end, TSelector&& selector)
    noexcept(std::is_nothrow_invocable_v<TSelector, T>)
    {
        TResult min = selector(collection[start]);
        for (std::size_t i = start + 1; i <= end; ++i)
            if (selector(collection[i]) < min)
                min = selector(collection[i]);
        return min;
    }

    template<Concepts::RandomAccess TCollection, Concepts::Comparable T = RandomAccessValueType<TCollection>>
    T Max(TCollection&& collection, const std::size_t start, const std::size_t end) noexcept
    {
        T max = collection[start];
        for (std::size_t i = start + 1; i <= end; ++i)
            if (max < collection[i])
                max = collection[i];
        return max;
    }

    template<Concepts::RandomAccess TCollection,
             typename T = RandomAccessValueType<TCollection>,
             std::invocable<T> TSelector,
             Concepts::Comparable TResult = std::invoke_result_t<TSelector, T>>
    TResult Max(TCollection&& collection, const std::size_t start, const std::size_t end, TSelector&& selector)
    noexcept(std::is_nothrow_invocable_v<TSelector, T>)
    {
        TResult max = selector(collection[start]);
        for (std::size_t i = start + 1; i <= end; ++i)
            if (max < selector(collection[i]))
                max = selector(collection[i]);
        return max;
    }

    template<Concepts::RandomAccess TCollection, Concepts::Divisible T = RandomAccessValueType<TCollection>>
    T Average(TCollection&& collection, const std::size_t start, const std::size_t end) noexcept
    {
        T sum = collection[start];
        for (std::size_t i = start + 1; i <= end; ++i)
            sum += collection[i];
        return static_cast<T>(sum / (end + 1 - start));
    }

    template<Concepts::RandomAccess TCollection,
             typename T = RandomAccessValueType<TCollection>,
             std::invocable<T> TSelector,
             Concepts::Divisible TResult = std::invoke_result_t<TSelector, T>>
    TResult Average(TCollection&& collection, const std::size_t start, const std::size_t end, TSelector&& selector)
    noexcept(std::is_nothrow_invocable_v<TSelector, T>)
    {
        TResult sum = selector(collection[start]);
        for (std::size_t i = start + 1; i <= end; ++i)
            sum += selector(collection[i]);
        return static_cast<TResult>(sum / (end + 1 - start));
    }
}

#endif