#ifndef LINQ_LINQ_H
#define LINQ_LINQ_H

#include <ExtendedCpp/LINQ/LinqContainer.h>
#include <ExtendedCpp/LINQ/LinqGenerator.h>
#include <ExtendedCpp/LINQ/LinqView.h>

namespace ExtendedCpp::LINQ
{
    template<Concepts::ConstIterable TCollection, typename TSource = typename TCollection::value_type>
    LinqContainer<TSource> From(const TCollection& collection) noexcept
    {
        return LinqContainer(std::vector<TSource>(collection.cbegin(), collection.cend()));
    }

    template<Concepts::Iterable TCollection, typename TSource = typename TCollection::value_type>
    LinqContainer<TSource> From(TCollection&& collection) noexcept
    {
        return LinqContainer(std::vector<TSource>(collection.begin(), collection.end()));
    }

    template<std::forward_iterator TIterator, typename TSource = typename std::iterator_traits<TIterator>::value_type>
    LinqContainer<TSource> From(const TIterator begin, const TIterator end) noexcept
    {
        return LinqContainer(std::vector<TSource>(begin, end));
    }

    template<typename TSource>
    LinqContainer<TSource> From(std::vector<TSource>&& collection) noexcept
    {
        return LinqContainer(std::move(collection));
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

    template<typename TKey, typename TValue>
    LinqContainer<std::pair<TKey, TValue>> From(const std::map<TKey, TValue>& collection) noexcept
    {
        return LinqContainer(std::vector<std::pair<TKey, TValue>>(collection.cbegin(), collection.cend()));
    }

    template<typename TKey, typename TValue>
    LinqContainer<std::pair<TKey, TValue>> From(std::map<TKey, TValue>&& collection) noexcept
    {
        return LinqContainer(std::vector<std::pair<TKey, TValue>>(collection.begin(), collection.end()));
    }

    template<typename TKey, typename TValue>
    LinqContainer<std::pair<TKey, TValue>> From(const std::unordered_map<TKey, TValue>& collection) noexcept
    {
        return LinqContainer(std::vector<std::pair<TKey, TValue>>(collection.cbegin(), collection.cend()));
    }

    template<typename TKey, typename TValue>
    LinqContainer<std::pair<TKey, TValue>> From(std::unordered_map<TKey, TValue>&& collection) noexcept
    {
        return LinqContainer(std::vector<std::pair<TKey, TValue>>(collection.begin(), collection.end()));
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

    template<std::forward_iterator TIterator, typename TSource = typename std::iterator_traits<TIterator>::value_type>
    LinqGenerator<TSource> Generator(const TIterator begin, const TIterator end) noexcept
    {
        return LinqGenerator<TSource>(begin, end);
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

    template<typename TKey, typename TValue>
    LinqGenerator<std::pair<TKey, TValue>> Generator(const std::map<TKey, TValue>& collection) noexcept
    {
        return LinqGenerator<std::pair<TKey, TValue>>(collection);
    }

    template<typename TKey, typename TValue>
    LinqGenerator<std::pair<TKey, TValue>> Generator(std::map<TKey, TValue>&& collection) noexcept
    {
    return LinqGenerator<std::pair<TKey, TValue>>(std::move(collection));
    }

    template<typename TKey, typename TValue>
    LinqGenerator<std::pair<TKey, TValue>> Generator(const std::unordered_map<TKey, TValue>& collection) noexcept
    {
        return LinqGenerator<std::pair<TKey, TValue>>(collection);
    }

    template<typename TKey, typename TValue>
    LinqGenerator<std::pair<TKey, TValue>> Generator(std::unordered_map<TKey, TValue>&& collection) noexcept
    {
        return LinqGenerator<std::pair<TKey, TValue>>(std::move(collection));
    }

    template<Concepts::ConstIterable TCollection, typename TIterator = TCollection::const_iterator>
    LinqView<OptionalIterator<TIterator>> View(const TCollection& collection) noexcept
    {
        return LinqView<OptionalIterator<TIterator>>(
                OptionalIterator<TIterator>(collection.cbegin()),
                OptionalIterator<TIterator>(collection.cend()));
    }

    template<Concepts::Iterable TCollection, typename TIterator = TCollection::iterator>
    LinqView<OptionalIterator<TIterator>> View(TCollection& collection) noexcept
    {
        return LinqView<OptionalIterator<TIterator>>(
                OptionalIterator<TIterator>(collection.begin()),
                OptionalIterator<TIterator>(collection.end()));
    }

    template<std::forward_iterator TIterator>
    LinqView<OptionalIterator<TIterator>> View(const TIterator begin, const TIterator end) noexcept
    {
        return LinqView<OptionalIterator<TIterator>>(
                OptionalIterator<TIterator>(begin),
                OptionalIterator<TIterator>(end));
    }

    template<typename TSource, typename TIterator = std::vector<TSource>::iterator>
    LinqView<OptionalIterator<TIterator>> View(std::stack<TSource> collection) = delete;

    template<typename TSource, typename TIterator = std::vector<TSource>::iterator>
    LinqView<OptionalIterator<TIterator>> View(std::queue<TSource> collection) = delete;

    template<typename TSource, typename TIterator = std::vector<TSource>::iterator>
    LinqView<OptionalIterator<TIterator>> View(std::priority_queue<TSource> collection) = delete;
}

#endif