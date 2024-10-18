#ifndef LINQ_Algorithm_H
#define LINQ_Algorithm_H

#include <map>
#include <memory>
#include <limits>

#include <ExtendedCpp/LINQ/TypeTraits.h>
#include <ExtendedCpp/LINQ/Concepts.h>

namespace ExtendedCpp::LINQ
{
    constexpr std::size_t NPOS = std::numeric_limits<std::size_t>::max();
}

namespace ExtendedCpp::LINQ::Algorithm
{
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

    template<Concepts::Comparable TTarget,
             Concepts::RandomAccess TCollection,
             typename TCollectionType = RandomAccessValueType<TCollection>,
             std::invocable<TCollectionType> TSelector>
    requires std::same_as<std::invoke_result_t<TSelector, TCollectionType>, TTarget>
    std::size_t BinarySearch(TTarget&& target, TCollection&& collection,
                             const std::size_t start, const std::size_t end, TSelector&& selector)
    noexcept(std::is_nothrow_invocable_v<TSelector, TCollectionType>)
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

    template<Concepts::RandomAccess TCollection, Concepts::Equatable T = RandomAccessValueType<TCollection>>
    std::map<T, std::size_t> CountEqualKeys(TCollection&& collection, const std::size_t start, const std::size_t end) noexcept
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

    template<Concepts::RandomAccess TCollection, Concepts::RandomAccess TOtherCollection>
    requires std::same_as<RandomAccessValueType<TCollection>, RandomAccessValueType<TOtherCollection>> &&
             Concepts::Equatable<RandomAccessValueType<TCollection>>
    std::size_t CountCommonSubsequence(TCollection&& firstSequence, const std::size_t firstSize,
                                       TOtherCollection&& secondSequence, const std::size_t secondSize) noexcept
    {
        std::unique_ptr<std::unique_ptr<size_t[]>[]> LCSTable(new std::unique_ptr<size_t[]>[firstSize + 1]);

        for (std::size_t i = 0; i <= firstSize; ++i)
        {
            LCSTable[i] = std::unique_ptr<std::size_t[]>(new std::size_t[secondSize + 1]);
            for (std::size_t j = 0; j <= secondSize; ++j)
                LCSTable[i][j] = 0;
        }

        for (std::size_t i = 1; i <= firstSize; ++i)
            for (std::size_t j = 1; j <= secondSize; ++j)
            {
                if (firstSequence[i - 1] == secondSequence[j - 1])
                {
                    LCSTable[i][j] = LCSTable[i - 1][j - 1] + 1;
                }
                else
                {
                    if (LCSTable[i][j - 1] > LCSTable[i - 1][j])
                        LCSTable[i][j] = LCSTable[i][j - 1];
                    else
                        LCSTable[i][j] = LCSTable[i - 1][j];
                }
            }

        return LCSTable[firstSize][secondSize];
    }

    template<Concepts::RandomAccess TCollection, Concepts::RandomAccess TSubCollection>
    requires std::same_as<RandomAccessValueType<TCollection>, RandomAccessValueType<TSubCollection>> &&
             Concepts::Equatable<RandomAccessValueType<TCollection>>
    bool Contains(TCollection&& collection, const std::size_t collectionSize,
                  TSubCollection&& subCollection, const std::size_t subCollectionSize) noexcept
    {
        if (subCollectionSize > collectionSize)
            return false;

        std::unique_ptr<std::size_t[]> prefixSizeSubCollection(new std::size_t[subCollectionSize]);
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

    template<Concepts::RandomAccess TCollection, Concepts::RandomAccess TOtherCollection>
    requires std::same_as<RandomAccessValueType<TCollection>, RandomAccessValueType<TOtherCollection>> &&
             Concepts::Equatable<RandomAccessValueType<TCollection>>
    std::size_t IndexAt(TCollection&& collection, const std::size_t collectionSize,
                        TOtherCollection&& subCollection, const std::size_t subCollectionSize) noexcept
    {
        std::unique_ptr<std::size_t[]> prefixSizeSubCollection(new std::size_t[subCollectionSize]);
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