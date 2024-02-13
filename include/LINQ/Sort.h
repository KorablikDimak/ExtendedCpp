#ifndef LINQ_Sort_H
#define LINQ_Sort_H

#include <cmath>
#include <map>
#include <memory>

#include <LINQ/OrderType.h>
#include <LINQ/Aggregate.h>

namespace LINQ::Sort
{
    template<Comparable T>
    void SelectionSort(T *const collection, const std::size_t start, const std::size_t end) noexcept
    {
        for (std::size_t i = start; i <= end; ++i)
        {
            T smallest = collection[i];
            std::size_t smallestIndex = i;

            for (std::size_t j = i; j <= end; ++j)
            {
                if (collection[j] < smallest)
                {
                    smallest = collection[j];
                    smallestIndex = j;
                }
            }

            collection[smallestIndex] = std::move(collection[i]);
            collection[i] = std::move(smallest);
        }
    }

    template<Comparable T>
    void SelectionSort(T* *const collection, const std::size_t start, const std::size_t end) noexcept
    {
        for (std::size_t i = start; i <= end; ++i)
        {
            T* smallest = collection[i];
            std::size_t smallestIndex = i;

            for (std::size_t j = i; j <= end; ++j)
            {
                if (*collection[j] < *smallest)
                {
                    smallest = collection[j];
                    smallestIndex = j;
                }
            }

            collection[smallestIndex] = std::move(collection[i]);
            collection[i] = std::move(smallest);
        }
    }

    template<typename T, typename TSelector>
    requires IsFunctor<TSelector, T> && Comparable<typename FunctorTraits<TSelector(T)>::ReturnType>
    void SelectionSort(T *const collection, const std::size_t start, const std::size_t end, TSelector&& selector) noexcept
    {
        for (std::size_t i = start; i <= end; ++i)
        {
            T smallest = collection[i];
            std::size_t smallestIndex = i;

            for (std::size_t j = i; j <= end; ++j)
            {
                if (selector(collection[j]) < selector(smallest))
                {
                    smallest = collection[j];
                    smallestIndex = j;
                }
            }

            collection[smallestIndex] = std::move(collection[i]);
            collection[i] = std::move(smallest);
        }
    }

    template<Comparable T>
    void InsertionSort(T *const collection, const std::size_t start, const std::size_t end, const OrderType orderType = OrderType::ASC) noexcept
    {
        for (std::size_t i = start + 1; i <= end; ++i)
        {
            T select = collection[i];
            long long j = static_cast<long long>(i) - 1;

            if (orderType == OrderType::ASC)
            {
                while (j >= 0 && select < collection[j])
                {
                    collection[j + 1] = collection[j];
                    --j;
                }
            }
            else
            {
                while (j >= 0 && collection[j] < select)
                {
                    collection[j + 1] = collection[j];
                    --j;
                }
            }

            collection[j + 1] = select;
        }
    }

    template<Comparable T>
    void InsertionSort(T* *const collection, const std::size_t start, const std::size_t end, const OrderType orderType = OrderType::ASC) noexcept
    {
        for (std::size_t i = start + 1; i <= end; ++i)
        {
            T* select = collection[i];
            long long j = static_cast<long long>(i) - 1;

            if (orderType == OrderType::ASC)
            {
                while (j >= 0 && *select < *collection[j])
                {
                    collection[j + 1] = collection[j];
                    --j;
                }
            }
            else
            {
                while (j >= 0 && *collection[j] < *select)
                {
                    collection[j + 1] = collection[j];
                    --j;
                }
            }

            collection[j + 1] = select;
        }
    }

    template<typename T, typename TSelector>
    requires IsFunctor<TSelector, T> && Comparable<typename FunctorTraits<TSelector(T)>::ReturnType>
    void InsertionSort(T *const collection, const std::size_t start, const std::size_t end,
                       TSelector&& selector, const OrderType orderType = OrderType::ASC) noexcept
    {
        for (std::size_t i = start + 1; i <= end; ++i)
        {
            T select = collection[i];
            long long j = static_cast<long long>(i) - 1;

            if (orderType == OrderType::ASC)
            {
                while (j >= 0 && selector(select) < selector(collection[j]))
                {
                    collection[j + 1] = collection[j];
                    --j;
                }
            }
            else
            {
                while (j >= 0 && selector(collection[j]) < selector(select))
                {
                    collection[j + 1] = collection[j];
                    --j;
                }
            }

            collection[j + 1] = select;
        }
    }

    constexpr double FACTOR = 1.2473309;

    template<Comparable T>
    void CombSort(T *const collection, const std::size_t start, const std::size_t end, const OrderType orderType = OrderType::ASC) noexcept
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

    template<Comparable T>
    void CombSort(T* *const collection, const std::size_t start, const std::size_t end, const OrderType orderType = OrderType::ASC) noexcept
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

    template<typename T, typename TSelector>
    requires IsFunctor<TSelector, T> && Comparable<typename FunctorTraits<TSelector(T)>::ReturnType>
    void CombSort(T *const collection, const std::size_t start, const std::size_t end,
                  TSelector&& selector, const OrderType orderType = OrderType::ASC) noexcept
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
    requires Comparable<T> && Divisible<T>
    void BucketSort(T *const collection, std::size_t start, std::size_t end, OrderType orderType = OrderType::ASC) noexcept
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

    template<typename T, typename TSelector>
    requires IsFunctor<TSelector, T> &&
             Comparable<typename FunctorTraits<TSelector(T)>::ReturnType> &&
             Divisible<typename FunctorTraits<TSelector(T)>::ReturnType>
    void BucketSort(T *const collection, std::size_t start, std::size_t end,
                    TSelector&& selector, OrderType orderType = OrderType::ASC) noexcept
    {
        if (start >= end) return;

        using TSelect = typename FunctorTraits<TSelector(T)>::ReturnType;

        const TSelect min = Aggregate::Min(collection, start, end, selector);
        const TSelect max = Aggregate::Max(collection, start, end, selector);
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
                InsertionSort(block.second.data(), 0, block.second.size() - 1, selector, orderType);
            }
            else
            {
                CombSort(block.second.data(), 0, block.second.size() - 1, selector, orderType);
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

    template<Comparable T>
    void Merge(T *const collection, std::size_t start, std::size_t mid, std::size_t end, const OrderType orderType) noexcept
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

    template<Comparable T>
    void MergeSort(T *const collection, std::size_t start, std::size_t end, OrderType orderType = OrderType::ASC) noexcept
    {
        if (start >= end) return;
        const std::size_t mid = (start + end) / 2;
        MergeSort(collection, start, mid, orderType);
        MergeSort(collection, mid + 1, end, orderType);
        Merge(collection, start, mid, end, orderType);
    }

    template<Comparable T>
    void Merge(T* *const collection, const std::size_t start, const std::size_t mid, const std::size_t end, const OrderType orderType) noexcept
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

    template<Comparable T>
    void MergeSort(T* *const collection, std::size_t start, std::size_t end, OrderType orderType = OrderType::ASC) noexcept
    {
        if (start >= end) return;
        const std::size_t mid = (start + end) / 2;
        MergeSort(collection, start, mid, orderType);
        MergeSort(collection, mid + 1, end, orderType);
        Merge(collection, start, mid, end, orderType);
    }

    template<typename T, typename TSelector>
    requires IsFunctor<TSelector, T> && Comparable<typename FunctorTraits<TSelector(T)>::ReturnType>
    void Merge(T *const collection, std::size_t start, std::size_t mid, std::size_t end,
               TSelector&& selector, const OrderType orderType) noexcept
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

    template<typename T, typename TSelector>
    requires IsFunctor<TSelector, T> && Comparable<typename FunctorTraits<TSelector(T)>::ReturnType>
    void MergeSort(T *const collection, std::size_t start, std::size_t end,
                   TSelector&& selector, OrderType orderType = OrderType::ASC) noexcept
    {
        if (start >= end) return;
        const std::size_t mid = (start + end) / 2;
        MergeSort(collection, start, mid, selector, orderType);
        MergeSort(collection, mid + 1, end, selector, orderType);
        Merge(collection, start, mid, end, selector, orderType);
    }

    constexpr std::size_t RUN = 64;

    template<Comparable T>
    void TimSort(T *const collection, const std::size_t start, const std::size_t end, OrderType orderType = OrderType::ASC) noexcept
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

    template<Comparable T>
    void TimSort(T* *const collection, const std::size_t start, const std::size_t end, OrderType orderType = OrderType::ASC) noexcept
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

    template<typename T, typename TSelector>
    requires IsFunctor<TSelector, T> && Comparable<typename FunctorTraits<TSelector(T)>::ReturnType>
    void TimSort(T *const collection, const std::size_t start, const std::size_t end,
                 TSelector selector, OrderType orderType = OrderType::ASC) noexcept
    {
        if (start >= end) return;
        const std::size_t count = end + 1 - start;

        for (std::size_t i = 0; i < count; i += RUN)
        {
            if (i + RUN - 1 < count - 1) InsertionSort(collection, i, i + RUN - 1, selector, orderType);
            else InsertionSort(collection, i, count - 1, selector, orderType);
        }

        for (std::size_t size = RUN; size < count; size = 2 * size)
            for (std::size_t left = 0; left < count; left += 2 * size)
            {
                std::size_t mid = left + size - 1;
                std::size_t right = left + 2 * size - 1;
                if (count - 1 < (left + 2 * size - 1)) right = count - 1;
                if (mid < right) Merge(collection, left, mid, right, selector, orderType);
            }
    }

    template<Comparable T>
    std::pair<std::size_t, std::size_t> Partition(T *const collection, std::size_t start, std::size_t end, const OrderType orderType) noexcept
    {
        std::size_t mid = start;
        const T pivot = collection[end];

        if (orderType == OrderType::ASC)
        {
            while (mid <= end)
            {
                if (collection[mid] < pivot)
                {
                    std::swap(collection[start], collection[mid]);
                    ++start;
                    ++mid;
                }
                else if (pivot < collection[mid])
                {
                    std::swap(collection[mid], collection[end]);
                    --end;
                }
                else
                {
                    ++mid;
                }
            }
        }
        else
        {
            while (mid <= end)
            {
                if (pivot < collection[mid])
                {
                    std::swap(collection[start], collection[mid]);
                    ++start;
                    ++mid;
                }
                else if (collection[mid] < pivot)
                {
                    std::swap(collection[mid], collection[end]);
                    --end;
                }
                else
                {
                    ++mid;
                }
            }
        }

        return std::make_pair(start - 1, mid);
    }

    template<Comparable T>
    void QuickSort(T *const collection, std::size_t start, std::size_t end, OrderType orderType = OrderType::ASC) noexcept
    {
        while (start < end)
        {
            if (end - start < 65)
            {
                InsertionSort(collection, start, end, orderType);
                return;
            }

            const std::pair<std::size_t, std::size_t> mid = Partition(collection, start, end, orderType);

            if (mid.first - start < end - mid.first)
            {
                QuickSort(collection, start, mid.first, orderType);
                start = mid.second + 1;
            }
            else
            {
                QuickSort(collection, mid.second, end, orderType);
                end = mid.first - 1;
            }
        }
    }

    template<Comparable T>
    std::pair<std::size_t, std::size_t> Partition(T* *const collection, std::size_t start, std::size_t end, const OrderType orderType) noexcept
    {
        std::size_t mid = start;
        const T pivot = *collection[end];

        if (orderType == OrderType::ASC)
        {
            while (mid <= end)
            {
                if (*collection[mid] < pivot)
                {
                    std::swap(collection[start], collection[mid]);
                    ++start;
                    ++mid;
                }
                else if (pivot < *collection[mid])
                {
                    std::swap(collection[mid], collection[end]);
                    --end;
                }
                else
                {
                    ++mid;
                }
            }
        }
        else
        {
            while (mid <= end)
            {
                if (pivot < *collection[mid])
                {
                    std::swap(collection[start], collection[mid]);
                    ++start;
                    ++mid;
                }
                else if (*collection[mid] < pivot)
                {
                    std::swap(collection[mid], collection[end]);
                    --end;
                }
                else
                {
                    ++mid;
                }
            }
        }

        return std::make_pair(start - 1, mid);
    }

    template<Comparable T>
    void QuickSort(T* *const collection, std::size_t start, std::size_t end, OrderType orderType = OrderType::ASC) noexcept
    {
        while (start < end)
        {
            if (end - start < 65)
            {
                InsertionSort(collection, start, end, orderType);
                return;
            }

            const std::pair<std::size_t, std::size_t> mid = Partition(collection, start, end, orderType);

            if (mid.first - start < end - mid.first)
            {
                QuickSort(collection, start, mid.first, orderType);
                start = mid.second + 1;
            }
            else
            {
                QuickSort(collection, mid.second, end, orderType);
                end = mid.first - 1;
            }
        }
    }

    template<typename T, typename TSelector>
    requires IsFunctor<TSelector, T> && Comparable<typename FunctorTraits<TSelector(T)>::ReturnType>
    std::pair<std::size_t, std::size_t> Partition(T *const collection, std::size_t start, std::size_t end,
                                                  TSelector&& selector, const OrderType orderType) noexcept
    {
        using TSelect = typename FunctorTraits<TSelector(T)>::ReturnType;

        std::size_t mid = start;
        const TSelect pivot = selector(collection[end]);

        if (orderType == OrderType::ASC)
        {
            while (mid <= end)
            {
                if (selector(collection[mid]) < pivot)
                {
                    std::swap(collection[start], collection[mid]);
                    ++start;
                    ++mid;
                }
                else if (pivot < selector(collection[mid]))
                {
                    std::swap(collection[mid], collection[end]);
                    --end;
                }
                else
                {
                    ++mid;
                }
            }
        }
        else
        {
            while (mid <= end)
            {
                if (pivot < selector(collection[mid]))
                {
                    std::swap(collection[start], collection[mid]);
                    ++start;
                    ++mid;
                }
                else if (selector(collection[mid]) < pivot)
                {
                    std::swap(collection[mid], collection[end]);
                    --end;
                }
                else
                {
                    ++mid;
                }
            }
        }

        return std::make_pair(start - 1, mid);
    }

    template<typename T, typename TSelector>
    requires IsFunctor<TSelector, T> && Comparable<typename FunctorTraits<TSelector(T)>::ReturnType>
    void QuickSort(T *const collection, std::size_t start, std::size_t end,
                   TSelector&& selector, OrderType orderType = OrderType::ASC) noexcept
    {
        while (start < end)
        {
            if (end - start < 65)
            {
                InsertionSort(collection, start, end, selector, orderType);
                return;
            }

            const std::pair<std::size_t, std::size_t> mid = Partition(collection, start, end, selector, orderType);

            if (mid.first - start < end - mid.first)
            {
                QuickSort(collection, start, mid.first, selector, orderType);
                start = mid.second + 1;
            }
            else
            {
                QuickSort(collection, mid.second, end, selector, orderType);
                end = mid.first - 1;
            }
        }
    }
}

#endif