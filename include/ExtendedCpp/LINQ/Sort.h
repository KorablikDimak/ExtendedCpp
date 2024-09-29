#ifndef LINQ_Sort_H
#define LINQ_Sort_H

#include <cmath>
#include <map>
#include <memory>

#include <ExtendedCpp/LINQ/OrderType.h>
#include <ExtendedCpp/LINQ/Aggregate.h>

namespace ExtendedCpp::LINQ::Sort
{
    template<Concepts::Comparable T>
    void SelectionSort(T *const collection, const std::size_t start, const std::size_t end,
                       const OrderType orderType = OrderType::ASC) noexcept
    {
        if (orderType == OrderType::ASC)
        {
            for (std::size_t i = start; i <= end; ++i)
            {
                std::size_t smallestIndex = i;

                for (std::size_t j = i; j <= end; ++j)
                    if (collection[j] < collection[smallestIndex])
                        smallestIndex = j;

                std::swap(collection[smallestIndex], collection[i]);
            }
        }
        else
        {
            for (std::size_t i = start; i <= end; ++i)
            {
                std::size_t smallestIndex = i;

                for (std::size_t j = i; j <= end; ++j)
                    if (collection[j] > collection[smallestIndex])
                        smallestIndex = j;

                std::swap(collection[smallestIndex], collection[i]);
            }
        }
    }

    template<Concepts::Comparable T>
    void SelectionSort(T* *const collection, const std::size_t start, const std::size_t end,
                       const OrderType orderType = OrderType::ASC) noexcept
    {
        if (orderType == OrderType::ASC)
        {
            for (std::size_t i = start; i <= end; ++i)
            {
                std::size_t smallestIndex = i;

                for (std::size_t j = i; j <= end; ++j)
                    if (*collection[j] < *collection[smallestIndex])
                        smallestIndex = j;

                std::swap(collection[smallestIndex], collection[i]);
            }
        }
        else
        {
            for (std::size_t i = start; i <= end; ++i)
            {
                std::size_t smallestIndex = i;

                for (std::size_t j = i; j <= end; ++j)
                    if (*collection[j] > *collection[smallestIndex])
                        smallestIndex = j;

                std::swap(collection[smallestIndex], collection[i]);
            }
        }
    }

    template<typename T, std::invocable<T> TSelector>
    requires Concepts::Comparable<std::invoke_result_t<TSelector, T>>
    void SelectionSort(T *const collection, const std::size_t start, const std::size_t end, TSelector&& selector,
                       const OrderType orderType = OrderType::ASC)
    noexcept(std::is_nothrow_invocable_v<TSelector, T>)
    {
        if (orderType == OrderType::ASC)
        {
            for (std::size_t i = start; i <= end; ++i)
            {
                std::size_t smallestIndex = i;

                for (std::size_t j = i; j <= end; ++j)
                    if (selector(collection[j]) < selector(collection[smallestIndex]))
                        smallestIndex = j;

                std::swap(collection[smallestIndex], collection[i]);
            }
        }
        else
        {
            for (std::size_t i = start; i <= end; ++i)
            {
                std::size_t smallestIndex = i;

                for (std::size_t j = i; j <= end; ++j)
                    if (selector(collection[j]) > selector(collection[smallestIndex]))
                        smallestIndex = j;

                std::swap(collection[smallestIndex], collection[i]);
            }
        }
    }

    template<Concepts::Comparable T>
    void InsertionSort(T *const collection, const std::size_t start, const std::size_t end,
                       const OrderType orderType = OrderType::ASC) noexcept
    {
        if (orderType == OrderType::ASC)
        {
            for (std::size_t i = start + 1; i <= end; ++i)
                for(std::size_t j = i; j > 0; --j)
                {
                    if (collection[j] < collection[j - 1])
                        std::swap(collection[j], collection[j - 1]);
                    else
                        break;
                }
        }
        else
        {
            for (std::size_t i = start + 1; i <= end; ++i)
                for(std::size_t j = i; j > 0; --j)
                {
                    if (collection[j] > collection[j - 1])
                        std::swap(collection[j], collection[j - 1]);
                    else
                        break;
                }
        }
    }

    template<Concepts::Comparable T>
    void InsertionSort(T* *const collection, const std::size_t start, const std::size_t end,
                       const OrderType orderType = OrderType::ASC) noexcept
    {
        if (orderType == OrderType::ASC)
        {
            for (std::size_t i = start + 1; i <= end; ++i)
                for(std::size_t j = i; j > 0; --j)
                {
                    if (*collection[j] < *collection[j - 1])
                        std::swap(collection[j], collection[j - 1]);
                    else
                        break;
                }
        }
        else
        {
            for (std::size_t i = start + 1; i <= end; ++i)
                for(std::size_t j = i; j > 0; --j)
                {
                    if (*collection[j] > *collection[j - 1])
                        std::swap(collection[j], collection[j - 1]);
                    else
                        break;
                }
        }
    }

    template<typename T, std::invocable<T> TSelector>
    requires Concepts::Comparable<std::invoke_result_t<TSelector, T>>
    void InsertionSort(T *const collection, const std::size_t start, const std::size_t end,
                       TSelector&& selector, const OrderType orderType = OrderType::ASC)
    noexcept(std::is_nothrow_invocable_v<TSelector, T>)
    {
        if (orderType == OrderType::ASC)
        {
            for (std::size_t i = start + 1; i <= end; ++i)
                for(std::size_t j = i; j > 0; --j)
                {
                    if (selector(collection[j]) < selector(collection[j - 1]))
                        std::swap(collection[j], collection[j - 1]);
                    else
                        break;
                }
        }
        else
        {
            for (std::size_t i = start + 1; i <= end; ++i)
                for(std::size_t j = i; j > 0; --j)
                {
                    if (selector(collection[j]) > selector(collection[j - 1]))
                        std::swap(collection[j], collection[j - 1]);
                    else
                        break;
                }
        }
    }

    constexpr double FACTOR = 1.2473309;

    template<Concepts::Comparable T>
    void CombSort(T *const collection, const std::size_t start, const std::size_t end,
                  const OrderType orderType = OrderType::ASC) noexcept
    {
        if (start >= end) return;
        std::size_t step = end - start;

        if (orderType == OrderType::ASC)
        {
            while (step >= 1)
            {
                for (std::size_t i = 0; i + step < end + 1 - start; ++i)
                    if (collection[i + step] < collection[i])
                        std::swap(collection[i], collection[i + step]);

                step = static_cast<std::size_t>(static_cast<double>(step) / FACTOR);
            }
        }
        else
        {
            while (step >= 1)
            {
                for (std::size_t i = 0; i + step < end + 1 - start; ++i)
                    if (collection[i] < collection[i + step])
                        std::swap(collection[i], collection[i + step]);

                step = static_cast<std::size_t>(static_cast<double>(step) / FACTOR);
            }
        }
    }

    template<Concepts::Comparable T>
    void CombSort(T* *const collection, const std::size_t start, const std::size_t end,
                  const OrderType orderType = OrderType::ASC) noexcept
    {
        if (start >= end) return;
        std::size_t step = end - start;

        if (orderType == OrderType::ASC)
        {
            while (step >= 1)
            {
                for (std::size_t i = 0; i + step < end + 1 - start; ++i)
                    if (*collection[i + step] < *collection[i])
                        std::swap(collection[i], collection[i + step]);

                step = static_cast<std::size_t>(static_cast<double>(step) / FACTOR);
            }
        }
        else
        {
            while (step >= 1)
            {
                for (std::size_t i = 0; i + step < end + 1 - start; ++i)
                    if (*collection[i] < *collection[i + step])
                        std::swap(collection[i], collection[i + step]);

                step = static_cast<std::size_t>(static_cast<double>(step) / FACTOR);
            }
        }
    }

    template<typename T, std::invocable<T> TSelector>
    requires Concepts::Comparable<std::invoke_result_t<TSelector, T>>
    void CombSort(T *const collection, const std::size_t start, const std::size_t end,
                  TSelector&& selector, const OrderType orderType = OrderType::ASC)
    noexcept(std::is_nothrow_invocable_v<TSelector, T>)
    {
        if (start >= end) return;
        std::size_t step = end - start;

        if (orderType == OrderType::ASC)
        {
            while (step >= 1)
            {
                for (std::size_t i = 0; i + step < end + 1 - start; ++i)
                    if (selector(collection[i + step]) < selector(collection[i]))
                        std::swap(collection[i], collection[i + step]);

                step = static_cast<std::size_t>(static_cast<double>(step) / FACTOR);
            }
        }
        else
        {
            while (step >= 1)
            {
                for (std::size_t i = 0; i + step < end + 1 - start; ++i)
                    if (selector(collection[i]) < selector(collection[i + step]))
                        std::swap(collection[i], collection[i + step]);

                step = static_cast<std::size_t>(static_cast<double>(step) / FACTOR);
            }
        }
    }

    template<typename T>
    requires Concepts::Comparable<T> && Concepts::Divisible<T>
    void BucketSort(T *const collection, const std::size_t start, const std::size_t end,
                    const OrderType orderType = OrderType::ASC) noexcept
    {
        if (start >= end) return;

        const T min = Aggregate::Min(collection, start, end);
        const T max = Aggregate::Max(collection, start, end);
        const auto blockCount = static_cast<std::size_t>(std::ceil(std::log2(end + 1 - start)));
        if (blockCount == 0) return;

        std::map<T, std::vector<T>> bucket;
        std::unique_ptr<T[]> blockIndexes(new T[blockCount]);

        for (std::size_t i = 0; i < blockCount; ++i)
        {
            const T index = static_cast<T>(min + (max - min) / blockCount * i);
            bucket.insert(std::make_pair(index, std::vector<T>()));
            blockIndexes[i] = index;
        }

        for (std::size_t i = start; i <= end; ++i)
            for (long long j = static_cast<long long>(blockCount) - 1; j >= 0; --j)
                if (collection[i] >= blockIndexes[j])
                {
                    bucket[blockIndexes[j]].push_back(collection[i]);
                    break;
                }

        for (auto& block : bucket)
        {
            if (block.second.empty()) continue;
            if (block.second.size() < 1000)
            {
                InsertionSort(block.second.data(), 0, block.second.size() - 1, orderType);
            }
            else
            {
                CombSort(block.second.data(), 0, block.second.size() - 1, orderType);
            }
        }

        std::size_t count = 0;

        if (orderType == OrderType::ASC)
        {
            for (std::size_t i = 0; i < blockCount; ++i)
            {
                std::vector<T> block = std::move(bucket[blockIndexes[i]]);
                if (block.empty()) continue;
                std::copy(block.data(), block.data() + block.size(), collection + count);
                count += block.size();
            }
        }
        else
        {
            for (long long i = static_cast<long long>(blockCount) - 1; i >= 0; --i)
            {
                std::vector<T> block = std::move(bucket[blockIndexes[i]]);
                if (block.empty()) continue;
                std::copy(block.data(), block.data() + block.size(), collection + count);
                count += block.size();
            }
        }
    }

    template<typename T, std::invocable<T> TSelector>
    requires Concepts::Comparable<std::invoke_result_t<TSelector, T>> &&
             Concepts::Divisible<std::invoke_result_t<TSelector, T>>
    void BucketSort(T *const collection, const std::size_t start, const std::size_t end,
                    TSelector&& selector, const OrderType orderType = OrderType::ASC)
    noexcept(std::is_nothrow_invocable_v<TSelector, T>)
    {
        if (start >= end) return;

        using TSelect = std::invoke_result_t<TSelector, T>;

        const TSelect min = Aggregate::Min(collection, start, end, std::forward<TSelector>(selector));
        const TSelect max = Aggregate::Max(collection, start, end, std::forward<TSelector>(selector));
        const auto blockCount = static_cast<std::size_t>(std::ceil(std::log2(end + 1 - start)));
        if (blockCount == 0) return;

        std::map<TSelect, std::vector<T>> bucket;
        std::unique_ptr<TSelect[]> blockIndexes(new TSelect[blockCount]);

        for (std::size_t i = 0; i < blockCount; ++i)
        {
            const auto index = static_cast<TSelect>(min + (max - min) / blockCount * i);
            bucket.insert(std::make_pair(index, std::vector<T>()));
            blockIndexes[i] = index;
        }

        for (std::size_t i = start; i <= end; ++i)
            for (long long j = static_cast<long long>(blockCount) - 1; j >= 0; --j)
                if (selector(collection[i]) >= blockIndexes[j])
                {
                    bucket[blockIndexes[j]].push_back(collection[i]);
                    break;
                }

        for (auto& block : bucket)
        {
            if (block.second.empty()) continue;
            if (block.second.size() < 1000)
            {
                InsertionSort(block.second.data(), 0, block.second.size() - 1, std::forward<TSelector>(selector), orderType);
            }
            else
            {
                CombSort(block.second.data(), 0, block.second.size() - 1, std::forward<TSelector>(selector), orderType);
            }
        }

        std::size_t count = 0;

        if (orderType == OrderType::ASC)
        {
            for (std::size_t i = 0; i < blockCount; ++i)
            {
                std::vector<T> block = std::move(bucket[blockIndexes[i]]);
                if (block.empty()) continue;
                std::copy(block.data(), block.data() + block.size(), collection + count);
                count += block.size();
            }
        }
        else
        {
            for (long long i = static_cast<long long>(blockCount) - 1; i >= 0; --i)
            {
                std::vector<T> block = std::move(bucket[blockIndexes[i]]);
                if (block.empty()) continue;
                std::copy(block.data(), block.data() + block.size(), collection + count);
                count += block.size();
            }
        }
    }

    template<Concepts::Comparable T>
    void Merge(T *const collection, const std::size_t start, const std::size_t mid, const std::size_t end,
               const OrderType orderType) noexcept
    {
        const std::size_t n1 = mid - start + 1;
        const std::size_t n2 = end - mid;

        std::unique_ptr<T[]> array1(new T[n1]);
        std::unique_ptr<T[]> array2(new T[n2]);

        std::copy(collection + start, collection + mid + 1, array1.get());
        std::copy(collection + mid + 1, collection + end + 1, array2.get());

        std::size_t i = 0, j = 0;

        if (orderType == OrderType::ASC)
        {
            for (std::size_t k = start; k <= end; ++k)
            {
                if (i != n1 && (j == n2 || array1[i] < array2[j]))
                {
                    collection[k] = array1[i];
                    ++i;
                }
                else if (j != n2 && (i == n1 || array2[j] < array1[i]))
                {
                    collection[k] = array2[j];
                    ++j;
                }
                else if (i != n1 && (j == n2 || array1[i] == array2[j]))
                {
                    collection[k] = array1[i];
                    ++i;
                }
            }
        }
        else
        {
            for (std::size_t k = start; k <= end; ++k)
            {
                if (i != n1 && (j == n2 || array2[j] < array1[i]))
                {
                    collection[k] = array1[i];
                    ++i;
                }
                else if (j != n2 && (i == n1 || array1[i] < array2[j]))
                {
                    collection[k] = array2[j];
                    ++j;
                }
                else if (j != n2 && (i == n1 || array1[i] == array2[j]))
                {
                    collection[k] = array2[j];
                    ++j;
                }
            }
        }
    }

    template<Concepts::Comparable T>
    void MergeSort(T *const collection, const std::size_t start, const std::size_t end,
                   const OrderType orderType = OrderType::ASC) noexcept
    {
        if (start >= end) return;
        const std::size_t mid = (start + end) / 2;
        MergeSort(collection, start, mid, orderType);
        MergeSort(collection, mid + 1, end, orderType);
        Merge(collection, start, mid, end, orderType);
    }

    template<Concepts::Comparable T>
    void Merge(T* *const collection, const std::size_t start, const std::size_t mid, const std::size_t end,
               const OrderType orderType) noexcept
    {
        const std::size_t n1 = mid - start + 1;
        const std::size_t n2 = end - mid;

        std::unique_ptr<T[]> array1(new T[n1]);
        std::unique_ptr<T[]> array2(new T[n2]);

        for (std::size_t i = start, j = 0; i <= mid; ++i, ++j) array1[j] = *collection[i];
        for (std::size_t i = mid + 1, j = 0; i <= end; ++i, ++j) array2[j] = *collection[i];

        std::size_t i = 0, j = 0;

        if (orderType == OrderType::ASC)
        {
            for (std::size_t k = start; k <= end; ++k)
            {
                if (i != n1 && (j == n2 || array1[i] < array2[j]))
                {
                    *collection[k] = array1[i];
                    ++i;
                }
                else if (j != n2 && (i == n1 || array2[j] < array1[i]))
                {
                    *collection[k] = array2[j];
                    ++j;
                }
                else if (i != n1 && (j == n2 || array1[i] == array2[j]))
                {
                    *collection[k] = array1[i];
                    ++i;
                }
            }
        }
        else
        {
            for (std::size_t k = start; k <= end; ++k)
            {
                if (i != n1 && (j == n2 || array2[j] < array1[i]))
                {
                    *collection[k] = array1[i];
                    ++i;
                }
                else if (j != n2 && (i == n1 || array1[i] < array2[j]))
                {
                    *collection[k] = array2[j];
                    ++j;
                }
                else if (j != n2 && (i == n1 || array1[i] == array2[j]))
                {
                    *collection[k] = array2[j];
                    ++j;
                }
            }
        }
    }

    template<Concepts::Comparable T>
    void MergeSort(T* *const collection, const std::size_t start, const std::size_t end,
                   const OrderType orderType = OrderType::ASC) noexcept
    {
        if (start >= end) return;
        const std::size_t mid = (start + end) / 2;
        MergeSort(collection, start, mid, orderType);
        MergeSort(collection, mid + 1, end, orderType);
        Merge(collection, start, mid, end, orderType);
    }

    template<typename T, std::invocable<T> TSelector>
    requires Concepts::Comparable<std::invoke_result_t<TSelector, T>>
    void Merge(T *const collection, const std::size_t start, const std::size_t mid, const std::size_t end,
               TSelector&& selector, const OrderType orderType)
    noexcept(std::is_nothrow_invocable_v<TSelector, T>)
    {
        const std::size_t n1 = mid - start + 1;
        const std::size_t n2 = end - mid;

        std::unique_ptr<T[]> array1(new T[n1]);
        std::unique_ptr<T[]> array2(new T[n2]);

        std::copy(collection + start, collection + mid + 1, array1.get());
        std::copy(collection + mid + 1, collection + end + 1, array2.get());

        std::size_t i = 0, j = 0;

        if (orderType == OrderType::ASC)
        {
            for (std::size_t k = start; k <= end; ++k)
            {
                if (i != n1 && (j == n2 || selector(array1[i]) < selector(array2[j])))
                {
                    collection[k] = array1[i];
                    ++i;
                }
                else if (j != n2 && (i == n1 || selector(array2[j]) < selector(array1[i])))
                {
                    collection[k] = array2[j];
                    ++j;
                }
                else if (i != n1 && (j == n2 || selector(array1[i]) == selector(array2[j])))
                {
                    collection[k] = array1[i];
                    ++i;
                }
            }
        }
        else
        {
            for (std::size_t k = start; k <= end; ++k)
            {
                if (i != n1 && (j == n2 || selector(array2[j]) < selector(array1[i])))
                {
                    collection[k] = array1[i];
                    ++i;
                }
                else if (j != n2 && (i == n1 || selector(array1[i]) < selector(array2[j])))
                {
                    collection[k] = array2[j];
                    ++j;
                }
                else if (j != n2 && (i == n1 || selector(array1[i]) == selector(array2[j])))
                {
                    collection[k] = array2[j];
                    ++j;
                }
            }
        }
    }

    template<typename T, std::invocable<T> TSelector>
    requires Concepts::Comparable<std::invoke_result_t<TSelector, T>>
    void MergeSort(T *const collection, const std::size_t start, const std::size_t end,
                   TSelector&& selector, const OrderType orderType = OrderType::ASC)
    noexcept(std::is_nothrow_invocable_v<TSelector, T>)
    {
        if (start >= end) return;
        const std::size_t mid = (start + end) / 2;
        MergeSort(collection, start, mid, std::forward<TSelector>(selector), orderType);
        MergeSort(collection, mid + 1, end, std::forward<TSelector>(selector), orderType);
        Merge(collection, start, mid, end, std::forward<TSelector>(selector), orderType);
    }

    constexpr std::size_t RUN = 64;

    template<Concepts::Comparable T>
    void TimSort(T *const collection, const std::size_t start, const std::size_t end,
                 const OrderType orderType = OrderType::ASC) noexcept
    {
        if (start >= end) return;
        const std::size_t count = end + 1 - start;

        for (std::size_t i = 0; i < count; i += RUN)
        {
            if (i + RUN - 1 < count - 1) InsertionSort(collection, i, i + RUN - 1, orderType);
            else InsertionSort(collection, i, count - 1, orderType);
        }

        for (std::size_t size = RUN; size < count; size = 2 * size)
            for (std::size_t left = 0; left < count; left += 2 * size)
            {
                std::size_t mid = left + size - 1;
                std::size_t right = left + 2 * size - 1;
                if (count - 1 < (left + 2 * size - 1)) right = count - 1;
                if (mid < right) Merge(collection, left, mid, right, orderType);
            }
    }

    template<Concepts::Comparable T>
    void TimSort(T* *const collection, const std::size_t start, const std::size_t end,
                 const OrderType orderType = OrderType::ASC) noexcept
    {
        if (start >= end) return;
        const std::size_t count = end + 1 - start;

        for (std::size_t i = 0; i < count; i += RUN)
        {
            if (i + RUN - 1 < count - 1) InsertionSort(collection, i, i + RUN - 1, orderType);
            else InsertionSort(collection, i, count - 1, orderType);
        }

        for (std::size_t size = RUN; size < count; size = 2 * size)
            for (std::size_t left = 0; left < count; left += 2 * size)
            {
                std::size_t mid = left + size - 1;
                std::size_t right = left + 2 * size - 1;
                if (count - 1 < (left + 2 * size - 1)) right = count - 1;
                if (mid < right) Merge(collection, left, mid, right, orderType);
            }
    }

    template<typename T, std::invocable<T> TSelector>
    requires Concepts::Comparable<std::invoke_result_t<TSelector, T>>
    void TimSort(T *const collection, const std::size_t start, const std::size_t end,
                 TSelector&& selector, const OrderType orderType = OrderType::ASC)
    noexcept(std::is_nothrow_invocable_v<TSelector, T>)
    {
        if (start >= end) return;
        const std::size_t count = end + 1 - start;

        for (std::size_t i = 0; i < count; i += RUN)
        {
            if (i + RUN - 1 < count - 1) InsertionSort(collection, i, i + RUN - 1, std::forward<TSelector>(selector), orderType);
            else InsertionSort(collection, i, count - 1, std::forward<TSelector>(selector), orderType);
        }

        for (std::size_t size = RUN; size < count; size = 2 * size)
            for (std::size_t left = 0; left < count; left += 2 * size)
            {
                std::size_t mid = left + size - 1;
                std::size_t right = left + 2 * size - 1;
                if (count - 1 < (left + 2 * size - 1)) right = count - 1;
                if (mid < right) Merge(collection, left, mid, right, std::forward<TSelector>(selector), orderType);
            }
    }

    template<Concepts::Comparable T>
    std::size_t Partition(T *const collection, const std::size_t start, const std::size_t end,
                        const OrderType orderType) noexcept
    {
        auto i = static_cast<long long>(start);
        auto j = static_cast<long long>(end);
        const T pivot = collection[(start + end) / 2];

        if (orderType == OrderType::ASC)
        {
            while (i <= j)
            {
                while (collection[i] < pivot)
                    ++i;
                while (collection[j] > pivot)
                    --j;

                if (i <= j)
                {
                    std::swap(collection[i], collection[j]);
                    ++i;
                    --j;
                }
            }
        }
        else
        {
            while (i <= j)
            {
                while (collection[i] > pivot)
                    ++i;
                while (collection[j] < pivot)
                    --j;

                if (i <= j)
                {
                    std::swap(collection[i], collection[j]);
                    ++i;
                    --j;
                }
            }
        }

        return i;
    }

    template<Concepts::Comparable T>
    void QuickSort(T *const collection, const std::size_t start, const std::size_t end,
                   const OrderType orderType = OrderType::ASC) noexcept
    {
        if (end - start < 65)
        {
            InsertionSort(collection, start, end, orderType);
            return;
        }

        const std::size_t mid = Partition(collection, start, end, orderType);

        if (start < mid - 1)
            QuickSort(collection, start, mid - 1, orderType);
        if (mid < end)
            QuickSort(collection, mid, end, orderType);
    }

    template<Concepts::Comparable T>
    std::size_t Partition(T* *const collection, const std::size_t start, const std::size_t end,
                          const OrderType orderType) noexcept
    {
        auto i = static_cast<long long>(start);
        auto j = static_cast<long long>(end);
        const T pivot = *collection[(start + end) / 2];

        if (orderType == OrderType::ASC)
        {
            while (i <= j)
            {
                while (*collection[i] < pivot)
                    ++i;
                while (*collection[j] > pivot)
                    --j;

                if (i <= j)
                {
                    std::swap(collection[i], collection[j]);
                    ++i;
                    --j;
                }
            }
        }
        else
        {
            while (i <= j)
            {
                while (*collection[i] > pivot)
                    ++i;
                while (*collection[j] < pivot)
                    --j;

                if (i <= j)
                {
                    std::swap(collection[i], collection[j]);
                    ++i;
                    --j;
                }
            }
        }

        return i;
    }

    template<Concepts::Comparable T>
    void QuickSort(T* *const collection, std::size_t start, std::size_t end,
                   const OrderType orderType = OrderType::ASC) noexcept
    {
        if (end - start < 65)
        {
            InsertionSort(collection, start, end, orderType);
            return;
        }

        const std::size_t mid = Partition(collection, start, end, orderType);

        if (start < mid - 1)
            QuickSort(collection, start, mid - 1, orderType);
        if (mid < end)
            QuickSort(collection, mid, end, orderType);
    }

    template<typename T, std::invocable<T> TSelector>
    requires Concepts::Comparable<std::invoke_result_t<TSelector, T>>
    std::size_t Partition(T *const collection, const std::size_t start, const std::size_t end,
                          TSelector&& selector, const OrderType orderType)
    noexcept(std::is_nothrow_invocable_v<TSelector, T>)
    {
        using TSelect = std::invoke_result_t<TSelector, T>;

        auto i = static_cast<long long>(start);
        auto j = static_cast<long long>(end);
        const TSelect pivot = selector(collection[(start + end) / 2]);

        if (orderType == OrderType::ASC)
        {
            while (i <= j)
            {
                while (selector(collection[i]) < pivot)
                    ++i;
                while (selector(collection[j]) > pivot)
                    --j;

                if (i <= j)
                {
                    std::swap(collection[i], collection[j]);
                    ++i;
                    --j;
                }
            }
        }
        else
        {
            while (i <= j)
            {
                while (selector(collection[i]) > pivot)
                    ++i;
                while (selector(collection[j]) < pivot)
                    --j;

                if (i <= j)
                {
                    std::swap(collection[i], collection[j]);
                    ++i;
                    --j;
                }
            }
        }

        return i;
    }

    template<typename T, std::invocable<T> TSelector>
    requires Concepts::Comparable<std::invoke_result_t<TSelector, T>>
    void QuickSort(T *const collection, std::size_t start, std::size_t end,
                   TSelector&& selector, const OrderType orderType = OrderType::ASC)
    noexcept(std::is_nothrow_invocable_v<TSelector, T>)
    {
        if (end - start < 65)
        {
            InsertionSort(collection, start, end, std::forward<TSelector>(selector), orderType);
            return;
        }

        const std::size_t mid = Partition(collection, start, end, std::forward<TSelector>(selector), orderType);

        if (start < mid - 1)
            QuickSort(collection, start, mid - 1, std::forward<TSelector>(selector), orderType);
        if (mid < end)
            QuickSort(collection, mid, end, std::forward<TSelector>(selector), orderType);
    }
}

#endif