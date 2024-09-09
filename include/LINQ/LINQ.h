#ifndef LINQ_LINQ_H
#define LINQ_LINQ_H

#include <LINQ/LinqContainer.h>
#include <LINQ/LinqGenerator.h>

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
        return LinqContainer(std::move<std::vector<TSource>>(collection));
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

    template<Concepts::ConstIterable TCollection, typename TSource = typename TCollection::value_type>
    LinqContainer<TSource> From(const TCollection& collection) noexcept
    {
        return LinqContainer(std::move(std::vector<TSource>(collection.cbegin(), collection.cend())));
    }

    template<Concepts::Iterable TCollection, typename TSource = typename TCollection::value_type>
    LinqContainer<TSource> From(TCollection&& collection) noexcept
    {
        return LinqContainer(std::move(std::vector<TSource>(collection.begin(), collection.end())));
    }

    template<typename TIterator, typename TSource = typename std::iterator_traits<TIterator>::value_type>
    LinqContainer<TSource> From(const TIterator& begin, const TIterator& end) noexcept
    {
        return LinqContainer(std::move(std::vector<TSource>(begin, end)));
    }

    template<Concepts::ConstIterable TCollection, typename TSource = typename TCollection::value_type>
    LinqGenerator<TSource> Generator(const TCollection& collection) noexcept
    {
        return LinqGenerator<TSource>(collection);
    }

    template<Concepts::Iterable TCollection, typename TSource = typename TCollection::value_type>
    LinqGenerator<TSource> Generator(TCollection&& collection) noexcept
    {
        return LinqGenerator<TSource>(std::forward<TCollection>(collection));
    }

    template<typename TKey, typename TValue>
    LinqGenerator<std::pair<TKey, TValue>> Generator(const std::unordered_map<TKey, TValue>& collection) noexcept
    {
        std::vector<std::pair<TKey, TValue>> pairs;
        for (const auto& kv : collection)
            pairs.push_back(kv);
        return LinqGenerator<std::pair<TKey, TValue>>(std::move(pairs));
    }

    template<typename TKey, typename TValue>
    LinqGenerator<std::pair<TKey, TValue>> Generator(std::unordered_map<TKey, TValue>&& collection) noexcept
    {
        std::vector<std::pair<TKey, TValue>> pairs;
        for (auto& kv : collection)
            pairs.push_back(std::move(kv));
        return LinqGenerator<std::pair<TKey, TValue>>(std::move(pairs));
    }

    template<typename TKey, typename TValue>
    LinqGenerator<std::pair<TKey, TValue>> Generator(const std::map<TKey, TValue>& collection) noexcept
    {
        return LinqGenerator<std::pair<TKey, TValue>>(collection);
    }

    template<typename TKey, typename TValue>
    LinqGenerator<std::pair<TKey, TValue>> Generator(std::map<TKey, TValue>&& collection) noexcept
    {
        return LinqGenerator<std::pair<TKey, TValue>>(std::forward<std::map<TKey, TValue>>(collection));
    }

    template<typename TConstIterator, typename TSource = typename std::iterator_traits<TConstIterator>::value_type>
    requires std::is_const<TConstIterator>::value_type
    LinqGenerator<TSource> Generator(const TConstIterator& begin, const TConstIterator& end) noexcept
    {
        return LinqGenerator<TSource>(begin, end);
    }

    template<typename TIterator, typename TSource = typename std::iterator_traits<TIterator>::value_type>
    requires (!std::is_const<TIterator>::value_type)
    LinqGenerator<TSource> Generator(TIterator&& begin, TIterator&& end) noexcept
    {
        return LinqGenerator<TSource>(std::forward<TIterator>(begin), std::forward<TIterator>(end));
    }

    template<typename TSource>
    LinqGenerator<TSource> Generator(const std::stack<TSource>& collection) noexcept
    {
        std::stack<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (int i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.top());
            copy.pop();
        }

        return LinqGenerator<TSource>(std::move(vectorCollection));
    }

    template<typename TSource>
    LinqGenerator<TSource> Generator(std::stack<TSource>&& collection) noexcept
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (int i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(collection.top());
            collection.pop();
        }

        return LinqGenerator<TSource>(std::move(vectorCollection));
    }

    template<typename TSource>
    LinqGenerator<TSource> Generator(const std::queue<TSource>& collection) noexcept
    {
        std::queue<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (int i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.front());
            copy.pop();
        }

        return LinqGenerator<TSource>(std::move(vectorCollection));
    }

    template<typename TSource>
    LinqGenerator<TSource> Generator(std::queue<TSource>&& collection) noexcept
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (int i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = collection.front();
            collection.pop();
        }

        return LinqGenerator<TSource>(std::move(vectorCollection));
    }

    template<typename TSource>
    LinqGenerator<TSource> Generator(const std::priority_queue<TSource>& collection) noexcept
    {
        std::priority_queue<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (int i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.top());
            copy.pop();
        }

        return LinqGenerator<TSource>(std::move(vectorCollection));
    }

    template<typename TSource>
    LinqGenerator<TSource> Generator(std::priority_queue<TSource>&& collection) noexcept
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (int i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(collection.top());
            collection.pop();
        }

        return LinqGenerator<TSource>(std::move(vectorCollection));
    }
}

#endif