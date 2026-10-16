#ifndef LINQ_ALGORITHM_H
#define LINQ_ALGORITHM_H

#include <concepts>
#include <limits>
#include <map>
#include <memory>
#include <type_traits>

#include <ExtendedCpp/LINQ/Concepts.h>
#include <ExtendedCpp/LINQ/TypeTraits.h>

/// @brief
// ReSharper disable once CppInconsistentNaming
namespace ExtendedCpp::LINQ
{
    /// @brief
    constexpr std::size_t NPOS = std::numeric_limits<std::size_t>::max();
}

/// @brief
namespace ExtendedCpp::LINQ::Algorithm
{
    /// @brief 
    /// @tparam TTarget 
    /// @tparam TCollection 
    /// @param target 
    /// @param collection 
    /// @param start 
    /// @param end 
    /// @return
    template<Concepts::Comparable TTarget, Concepts::RandomAccess TCollection>
    requires std::same_as<TTarget, RandomAccessValueType<TCollection>>
    std::size_t BinarySearch(TTarget&& target, TCollection&& collection,
                             const std::size_t start, const std::size_t end) noexcept
    {
        if (end - start <= 1)
        {
            if (target == collection[start])
                return start;
            if (target == collection[end])
                return end;
            return NPOS;
        }

        const std::size_t mid = (end + start) / 2;
        if (target < collection[mid])
            return BinarySearch(std::forward<TTarget>(target), std::forward<TCollection>(collection), start, mid - 1);
        if (collection[mid] < target)
            return BinarySearch(std::forward<TTarget>(target), std::forward<TCollection>(collection), mid + 1, end);
        return mid;
    }

    /// @brief 
    /// @tparam TCollectionType 
    /// @tparam TTarget 
    /// @tparam TCollection 
    /// @tparam TSelector 
    /// @param target 
    /// @param collection 
    /// @param start 
    /// @param end 
    /// @param selector 
    /// @return
    template<Concepts::Comparable TTarget,
             Concepts::RandomAccess TCollection,
             typename TCollectionType = RandomAccessValueType<TCollection>,
             std::invocable<const TCollectionType&> TSelector>
    requires std::same_as<std::invoke_result_t<TSelector, const TCollectionType&>, TTarget>
    std::size_t BinarySearch(TTarget&& target, TCollection&& collection,
                             const std::size_t start, const std::size_t end, TSelector&& selector)
    noexcept(std::is_nothrow_invocable_v<TSelector, const TCollectionType&>)
    {
        if (end - start <= 1)
        {
            if (target == selector(collection[start]))
                return start;
            if (target == selector(collection[end]))
                return end;
            return NPOS;
        }

        const std::size_t mid = (end + start) / 2;
        if (target < selector(collection[mid]))
            return BinarySearch(std::forward<TTarget>(target), std::forward<TCollection>(collection),
                                start, mid - 1, std::forward<TSelector>(selector));
        if (selector(collection[mid]) < target)
            return BinarySearch(std::forward<TTarget>(target), std::forward<TCollection>(collection),
                                mid + 1, end, std::forward<TSelector>(selector));
        return mid;
    }

    /// @brief 
    /// @tparam TCollection 
    /// @tparam T 
    /// @param collection 
    /// @param start 
    /// @param end 
    /// @return
    template<Concepts::RandomAccess TCollection, Concepts::Equatable T = RandomAccessValueType<TCollection>>
    std::map<T, std::size_t> CountEqualKeys(const TCollection& collection, const std::size_t start, const std::size_t end) noexcept
    {
        std::map<T, std::size_t> equalKeyCounts;

        for (std::size_t i = start; i < end - start; ++i)
        {
            if (!equalKeyCounts.contains(collection[i]))
                equalKeyCounts[collection[i]] = 0;
            ++equalKeyCounts[collection[i]];
        }

        return equalKeyCounts;
    }

    /// @brief 
    /// @tparam TCollection 
    /// @tparam TOtherCollection 
    /// @param firstSequence 
    /// @param firstSize 
    /// @param secondSequence 
    /// @param secondSize 
    /// @return
    template<Concepts::RandomAccess TCollection, Concepts::RandomAccess TOtherCollection>
    requires std::same_as<RandomAccessValueType<TCollection>, RandomAccessValueType<TOtherCollection>> &&
             Concepts::Equatable<RandomAccessValueType<TCollection>>
    std::size_t CountCommonSubsequence(const TCollection& firstSequence, const std::size_t firstSize,
                                       const TOtherCollection& secondSequence, const std::size_t secondSize) noexcept
    {
        const std::size_t size = (firstSize + 1) * (secondSize + 1);
        auto lcsTable = std::make_unique<std::size_t[]>(size); // NOLINT(*-avoid-c-arrays)
        memset(lcsTable.get(), 0, size * sizeof(std::size_t));

        for (std::size_t i = 1; i <= firstSize; ++i)
            for (std::size_t j = 1; j <= secondSize; ++j)
            {
                if (firstSequence[i - 1] == secondSequence[j - 1])
                {
                    lcsTable[i * (secondSize + 1) + j] = lcsTable[(i - 1) * (secondSize + 1) + (j - 1)] + 1;
                }
                else
                {
                    lcsTable[i * (secondSize + 1) + j] = std::max(
                        lcsTable[i * (secondSize + 1) + (j - 1)],
                        lcsTable[(i - 1) * (secondSize + 1) + j]);
                }
            }

        return lcsTable[firstSize * (secondSize + 1) + secondSize];
    }

    /// @brief 
    /// @tparam TCollection 
    /// @tparam TSubCollection 
    /// @param collection 
    /// @param collectionSize 
    /// @param subCollection 
    /// @param subCollectionSize 
    /// @return
    template<Concepts::RandomAccess TCollection, Concepts::RandomAccess TSubCollection>
    requires std::same_as<RandomAccessValueType<TCollection>, RandomAccessValueType<TSubCollection>> &&
             Concepts::Equatable<RandomAccessValueType<TCollection>>
    bool Contains(const TCollection& collection, const std::size_t collectionSize,
                  const TSubCollection& subCollection, const std::size_t subCollectionSize) noexcept
    {
        if (subCollectionSize > collectionSize)
            return false;

        auto prefixSizeSubCollection = std::make_unique<std::size_t[]>(subCollectionSize); // NOLINT(*-avoid-c-arrays)
        prefixSizeSubCollection[0] = 0;

        for (std::size_t i = 1; i < subCollectionSize; ++i)
        {
            std::size_t j = prefixSizeSubCollection[i - 1];
            while (j > 0 && subCollection[i] != subCollection[j])
                j = prefixSizeSubCollection[j - 1];
            if (subCollection[i] == subCollection[j])
                ++j;
            prefixSizeSubCollection[i] = j;
        }

        std::size_t j = 0;
        for (std::size_t i = 0; i < collectionSize; ++i)
        {
            while (j > 0 && collection[i] != subCollection[j])
                j = prefixSizeSubCollection[j - 1];
            if (collection[i] == subCollection[j])
                ++j;
            if (j == subCollectionSize)
                return true;
        }

        return false;
    }

    /// @brief 
    /// @tparam TCollection 
    /// @tparam TOtherCollection 
    /// @param collection 
    /// @param collectionSize 
    /// @param subCollection 
    /// @param subCollectionSize 
    /// @return
    template<Concepts::RandomAccess TCollection, Concepts::RandomAccess TOtherCollection>
    requires std::same_as<RandomAccessValueType<TCollection>, RandomAccessValueType<TOtherCollection>> &&
             Concepts::Equatable<RandomAccessValueType<TCollection>>
    std::size_t IndexAt(const TCollection& collection, const std::size_t collectionSize,
                        const TOtherCollection& subCollection, const std::size_t subCollectionSize) noexcept
    {
        auto prefixSizeSubCollection = std::make_unique<std::size_t[]>(subCollectionSize); // NOLINT(*-avoid-c-arrays)
        prefixSizeSubCollection[0] = 0;

        for (std::size_t i = 1; i < subCollectionSize; ++i)
        {
            std::size_t j = prefixSizeSubCollection[i - 1];
            while (j > 0 && subCollection[i] != subCollection[j])
                j = prefixSizeSubCollection[j - 1];
            if (subCollection[i] == subCollection[j])
                ++j;
            prefixSizeSubCollection[i] = j;
        }

        std::size_t j = 0;
        for (std::size_t i = 0; i < collectionSize; ++i)
        {
            while (j > 0 && collection[i] != subCollection[j])
                j = prefixSizeSubCollection[j - 1];
            if (collection[i] == subCollection[j])
                ++j;
            if (j == subCollectionSize)
                return i + 1 - subCollectionSize;
        }

        return NPOS;
    }
}

#endif