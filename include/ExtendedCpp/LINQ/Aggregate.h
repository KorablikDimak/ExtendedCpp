#ifndef LINQ_Aggregate_H
#define LINQ_Aggregate_H

#include <ExtendedCpp/LINQ/TypeTraits.h>
#include <ExtendedCpp/LINQ/Concepts.h>

namespace ExtendedCpp::LINQ::Aggregate
{
    template<typename TResult, typename T>
    TResult Aggregate(const T *const collection, std::size_t start, const std::size_t end,
        std::function<TResult(TResult, T)> aggregateFunction) // TODO noexcept
    {
        TResult result = collection[start];
        for (std::size_t i = start + 1; i <= end; ++i)
            result = aggregateFunction(result, collection[i]);
        return result;
    }

    template<typename T, typename TPredicate>
    requires Concepts::IsPredicate<TPredicate, T>
    std::size_t Count(const T *const collection, const std::size_t start,
                      const std::size_t end, TPredicate&& predicate)
                      noexcept(std::is_nothrow_invocable_v<TPredicate, T>)
    {
        std::size_t count = 0;
        for (std::size_t i = start; i <= end; ++i)
            if (predicate(collection[i])) ++count;
        return count;
    }

    template<Concepts::Summarize T>
    T Sum(const T *const collection, std::size_t start, const std::size_t end) noexcept
    {
        T sum = collection[start];
        for (std::size_t i = start + 1; i <= end; ++i)
            sum += collection[i];
        return sum;
    }

    template<typename T, typename TSelector, Concepts::Summarize TResult = typename FunctorTraits<TSelector(T)>::ReturnType>
    requires Concepts::IsFunctor<TSelector, T>
    TResult Sum(const T *const collection, std::size_t start, const std::size_t end, TSelector&& selector)
        noexcept(std::is_nothrow_invocable_v<TSelector, T>)
    {
        TResult sum = selector(collection[start]);
        for (std::size_t i = start + 1; i <= end; ++i)
            sum += selector(collection[i]);
        return sum;
    }

    template<Concepts::Comparable T>
    T Min(const T *const collection, std::size_t start, const std::size_t end) noexcept
    {
        T min = collection[start];
        for (std::size_t i = start + 1; i <= end; ++i)
            if (collection[i] < min)
                min = collection[i];
        return min;
    }

    template<typename T, typename TSelector, Concepts::Comparable TResult = typename FunctorTraits<TSelector(T)>::ReturnType>
    requires Concepts::IsFunctor<TSelector, T>
    TResult Min(const T *const collection, std::size_t start, const std::size_t end, TSelector&& selector)
        noexcept(std::is_nothrow_invocable_v<TSelector, T>)
    {
        TResult min = selector(collection[start]);
        for (std::size_t i = start + 1; i <= end; ++i)
            if (selector(collection[i]) < min)
                min = selector(collection[i]);
        return min;
    }

    template<Concepts::Comparable T>
    T Max(const T *const collection, std::size_t start, const std::size_t end) noexcept
    {
        T max = collection[start];
        for (std::size_t i = start + 1; i <= end; ++i)
            if (max < collection[i])
                max = collection[i];
        return max;
    }

    template<typename T, typename TSelector, Concepts::Comparable TResult = typename FunctorTraits<TSelector(T)>::ReturnType>
    requires Concepts::IsFunctor<TSelector, T>
    TResult Max(const T *const collection, std::size_t start, const std::size_t end, TSelector&& selector)
        noexcept(std::is_nothrow_invocable_v<TSelector, T>)
    {
        TResult max = selector(collection[start]);
        for (std::size_t i = start + 1; i <= end; ++i)
            if (max < selector(collection[i]))
                max = selector(collection[i]);
        return max;
    }

    template<Concepts::Divisible T>
    T Average(const T *const collection, std::size_t start, const std::size_t end) noexcept
    {
        T sum = collection[start];
        for (std::size_t i = start + 1; i <= end; ++i)
            sum += collection[i];
        return static_cast<T>(sum / (end + 1 - start));
    }

    template<typename T, typename TSelector, Concepts::Divisible TResult = typename FunctorTraits<TSelector(T)>::ReturnType>
    requires Concepts::IsFunctor<TSelector, T>
    TResult Average(const T *const collection, std::size_t start, const std::size_t end, TSelector&& selector)
        noexcept(std::is_nothrow_invocable_v<TSelector, T>)
    {
        TResult sum = selector(collection[start]);
        for (std::size_t i = start + 1; i <= end; ++i)
            sum += selector(collection[i]);
        return static_cast<TResult>(sum / (end + 1 - start));
    }
}

#endif