#ifndef LINQ_LINQ_H
#define LINQ_LINQ_H

#include <LINQ/LinqContainer.h>

namespace LINQ
{
    template<typename TSource>
    LinqContainer<TSource> From(const std::vector<TSource>& collection) noexcept
    {
        return LinqContainer(std::move(std::vector<TSource>(collection.cbegin(), collection.cend())));
    }

    template<typename TSource>
    LinqContainer<TSource> From(std::vector<TSource>&& collection) noexcept
    {
        return LinqContainer(std::forward<std::vector<TSource>>(collection));
    }

    template<typename TSource, std::size_t SIZE>
    LinqContainer<TSource> From(const std::array<TSource, SIZE>& collection) noexcept
    {
        return LinqContainer(std::move(std::vector<TSource>(collection.cbegin(), collection.cend())));
    }

    template<typename TSource>
    LinqContainer<TSource> From(const std::list<TSource>& collection) noexcept
    {
        return LinqContainer(std::move(std::vector<TSource>(collection.cbegin(), collection.cend())));
    }

    template<typename TSource>
    LinqContainer<TSource> From(const std::forward_list<TSource>& collection) noexcept
    {
        return LinqContainer(std::move(std::vector<TSource>(collection.cbegin(), collection.cend())));
    }

    template<typename TSource>
    LinqContainer<TSource> From(const std::stack<TSource>& collection) noexcept
    {
        std::stack<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (int i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.top());
            copy.pop();
        }

        return LinqContainer(std::move(vectorCollection));
    }

    template<typename TSource>
    LinqContainer<TSource> From(std::stack<TSource>&& collection) noexcept
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (int i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(collection.top());
            collection.pop();
        }

        return LinqContainer(std::move(vectorCollection));
    }

    template<typename TSource>
    LinqContainer<TSource> From(const std::queue<TSource>& collection) noexcept
    {
        std::queue<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (int i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.front());
            copy.pop();
        }

        return LinqContainer(std::move(vectorCollection));
    }

    template<typename TSource>
    LinqContainer<TSource> From(std::queue<TSource>&& collection) noexcept
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (int i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = collection.front();
            collection.pop();
        }

        return LinqContainer(std::move(vectorCollection));
    }

    template<typename TSource>
    LinqContainer<TSource> From(const std::deque<TSource>& collection) noexcept
    {
        return LinqContainer(std::move(std::vector<TSource>(collection.cbegin(), collection.cend())));
    }

    template<typename TSource>
    LinqContainer<TSource> From(const std::priority_queue<TSource>& collection) noexcept
    {
        std::priority_queue<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (int i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.top());
            copy.pop();
        }

        return LinqContainer(std::move(vectorCollection));
    }

    template<typename TSource>
    LinqContainer<TSource> From(std::priority_queue<TSource>&& collection) noexcept
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (int i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(collection.top());
            collection.pop();
        }

        return LinqContainer(std::move(vectorCollection));
    }

    template<typename TSource>
    LinqContainer<TSource> From(const std::set<TSource>& collection) noexcept
    {
        return LinqContainer(std::move(std::vector<TSource>(collection.cbegin(), collection.cend())));
    }

    template<typename TSource>
    LinqContainer<TSource> From(const std::unordered_set<TSource>& collection) noexcept
    {
        return LinqContainer(std::move(std::vector<TSource>(collection.cbegin(), collection.cend())));
    }

    template<typename TKey, typename TValue>
    LinqContainer<std::pair<TKey, TValue>> From(const std::map<TKey, TValue>& collection) noexcept
    {
        return LinqContainer(std::move(std::vector<std::pair<TKey, TValue>>(collection.cbegin(), collection.cend())));
    }

    template<typename TKey, typename TValue>
    LinqContainer<std::pair<TKey, TValue>> From(const std::unordered_map<TKey, TValue>& collection) noexcept
    {
        return LinqContainer(std::move(std::vector<std::pair<TKey, TValue>>(collection.cbegin(), collection.cend())));
    }

    template<ConstIterable TCollection, typename TSource = typename TCollection::value_type>
    LinqContainer<TSource> From(const TCollection& collection) noexcept
    {
        return LinqContainer(std::move(std::vector<TSource>(collection.cbegin(), collection.cend())));
    }

    template<Iterable TCollection, typename TSource = typename TCollection::value_type>
    LinqContainer<TSource> From(TCollection&& collection) noexcept
    {
        return LinqContainer(std::move(std::vector<TSource>(collection.begin(), collection.end())));
    }

    template<typename TIterator, typename TSource = typename std::iterator_traits<TIterator>::value_type>
    LinqContainer<TSource> From(const TIterator& begin, const TIterator& end) noexcept
    {
        return LinqContainer(std::move(std::vector<TSource>(begin, end)));
    }
}

#endif