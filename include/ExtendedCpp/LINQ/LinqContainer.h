#ifndef LINQ_LinqContainer_H
#define LINQ_LinqContainer_H

#include <array>
#include <list>
#include <forward_list>
#include <stack>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include <ExtendedCpp/LINQ/Algorithm.h>
#include <ExtendedCpp/LINQ/Sort.h>

namespace ExtendedCpp::LINQ
{
    template<typename TSource>
    class LinqContainer final
    {
    private:
        std::vector<TSource> _collection;

    public:
        using value_type = TSource;
        using iterator = typename std::vector<TSource>::iterator;
        using const_iterator = typename std::vector<TSource>::const_iterator;
        using reverse_iterator = typename std::vector<TSource>::reverse_iterator;
        using const_reverse_iterator = typename std::vector<TSource>::const_reverse_iterator;

        explicit LinqContainer(const std::vector<TSource>& collection) noexcept
        {
            _collection = collection;
        }

        explicit LinqContainer(std::vector<TSource>&& collection) noexcept
        {
            _collection = std::move(collection);
        }

        LinqContainer(const LinqContainer& container) noexcept
        {
            _collection = container.ToVector();
        }

        LinqContainer(LinqContainer&& container) noexcept
        {
            _collection = std::move(container.ToVector());
        }

        LinqContainer& operator=(const std::vector<TSource>& collection) noexcept
        {
            _collection = collection;
            return *this;
        }

        LinqContainer& operator=(std::vector<TSource>&& collection) noexcept
        {
            _collection = std::move(collection);
            return *this;
        }

        LinqContainer& operator=(const LinqContainer& container) noexcept
        {
            if (this == &container) return *this;
            _collection = container.ToVector();
            return *this;
        }

        LinqContainer& operator=(LinqContainer&& container) noexcept
        {
            _collection = std::move(container.ToVector());
            return *this;
        }

        ~LinqContainer() = default;

        [[nodiscard]]
        std::size_t size() const noexcept
        {
            return _collection.size();
        }

        [[nodiscard]]
        bool empty() const noexcept
        {
            return _collection.empty();
        }

        iterator begin() noexcept
        {
            return _collection.begin();
        }

        const_iterator begin() const noexcept
        {
            return _collection.cbegin();
        }

        iterator end() noexcept
        {
            return _collection.end();
        }

        const_iterator end() const noexcept
        {
            return _collection.cend();
        }

        const_iterator cbegin() const noexcept
        {
            return _collection.cbegin();
        }

        const_iterator cend() const noexcept
        {
            return _collection.cend();
        }

        reverse_iterator rbegin() noexcept
        {
            return _collection.rbegin();
        }

        const_reverse_iterator rbegin() const noexcept
        {
            return _collection.crbegin();
        }

        reverse_iterator rend() noexcept
        {
            return _collection.rend();
        }

        const_reverse_iterator rend() const noexcept
        {
            return _collection.crend();
        }

        const_reverse_iterator crbegin() const noexcept
        {
            return _collection.crbegin();
        }

        const_reverse_iterator crend() const noexcept
        {
            return _collection.crend();
        }

        TSource* data() noexcept
        {
            return _collection.data();
        }

        const TSource* data() const noexcept
        {
            return _collection.data();
        }

        std::vector<TSource> ToVector() const noexcept
        {
            return _collection;
        }

        template<std::size_t SIZE>
        std::array<TSource, SIZE> ToArray() const noexcept
        {
            std::array<TSource, SIZE> array;
            for (std::size_t i = 0; i < SIZE && i < _collection.size(); ++i)
                array[i] = _collection[i];
            return array;
        }

        std::list<TSource> ToList() const noexcept
        {
            return std::list<TSource>(_collection.cbegin(), _collection.cend());
        }

        std::forward_list<TSource> ToForwardList() const noexcept
        {
            return std::forward_list<TSource>(_collection.cbegin(), _collection.cend());
        }

        std::stack<TSource> ToStack() const noexcept
        {
            std::stack<TSource> stack;
            for (const TSource& element : _collection)
                stack.push(element);
            return stack;
        }

        std::queue<TSource> ToQueue() const noexcept
        {
            std::queue<TSource> queue;
            for (const TSource& element : _collection)
                queue.push(element);
            return queue;
        }

        std::deque<TSource> ToDeque() const noexcept
        {
            return std::deque<TSource>(_collection.cbegin(), _collection.cend());
        }

        std::priority_queue<TSource> ToPriorityQueue() const noexcept
        {
            std::priority_queue<TSource> priorityQueue;
            for (const TSource& element : _collection)
                priorityQueue.push(element);
            return priorityQueue;
        }

        std::set<TSource> ToSet() const noexcept
        {
            std::set<TSource> set;
            for (const TSource& element : _collection)
                set.insert(element);
            return set;
        }

        std::unordered_set<TSource> ToUnorderedSet() const noexcept
        {
            std::unordered_set<TSource> unorderedSet;
            for (const TSource& element : _collection)
                unorderedSet.insert(element);
            return unorderedSet;
        }

        template<typename TKey = typename PairTraits<TSource>::FirstType,
                 typename TValue = typename PairTraits<TSource>::SecondType>
        requires Concepts::IsPair<TSource>
        std::map<TKey, TValue> ToMap() const noexcept
        {
            std::map<TKey, TValue> map;
            for (const TSource& element : _collection)
                map.insert(element);
            return map;
        }

        template<typename TKey = typename PairTraits<TSource>::FirstType,
                 typename TValue = typename PairTraits<TSource>::SecondType>
        requires Concepts::IsPair<TSource>
        std::unordered_map<TKey, TValue> ToUnorderedMap() const noexcept
        {
            std::unordered_map<TKey, TValue> unorderedMap;
            for (const TSource& element : _collection)
                unorderedMap.insert(element);
            return unorderedMap;
        }

        template<std::invocable<TSource> TSelector,
                 typename TResult = std::invoke_result_t<TSelector, TSource>>
        LinqContainer<TResult> Select(TSelector&& selector)
            const noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
        {
            std::vector<TResult> newCollection;
            newCollection.reserve(_collection.size());

            for (const TSource& element : _collection)
                newCollection.push_back(selector(element));

            return LinqContainer<TResult>(std::move(newCollection));
        }

        template<std::invocable<TSource> TSelector,
                 typename TResult = typename std::invoke_result_t<TSelector, TSource>::value_type>
        LinqContainer<TResult> SelectMany(TSelector&& selector)
            const noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
        {
            std::vector<TResult> newCollection;

            for (const TSource& element : _collection)
            {
                std::vector<TResult> tempCollection = selector(element);
                for (auto&& tempElement : tempCollection)
                    newCollection.push_back(std::move(tempElement));
            }

            return LinqContainer<TResult>(std::move(newCollection));
        }

        template<std::invocable<TSource> TCollectionSelector,
                 Concepts::Iterable TCollection = std::invoke_result_t<TCollectionSelector, TSource>,
                 typename TCollectionValueType = typename TCollection::value_type,
                 std::invocable<TSource, TCollectionValueType> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, TSource, TCollectionValueType>>
        LinqContainer<TResult> SelectMany(TCollectionSelector&& collectionSelector, TResultSelector&& resultSelector)
        const noexcept(std::is_nothrow_invocable_v<TCollectionSelector, TSource> &&
                       std::is_nothrow_invocable_v<TCollectionSelector, TSource, TCollectionValueType>)
        {
            std::vector<TResult> newCollection;

            for (const TSource& element : _collection)
            {
                TCollection tempCollection = collectionSelector(element);
                for (auto&& tempElement : tempCollection)
                    newCollection.push_back(resultSelector(element, std::move(tempElement)));
            }

            return LinqContainer<TResult>(std::move(newCollection));
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        LinqContainer Where(TPredicate&& predicate)
        const noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            std::vector<TSource> newCollection;

            for (const TSource& element : _collection)
                if (predicate(element))
                    newCollection.push_back(element);

            return LinqContainer(std::move(newCollection));
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        LinqContainer RemoveWhere(TPredicate&& predicate)
        const noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            std::vector<TSource> newCollection;

            for (const TSource& element : _collection)
            {
                if (predicate(element))
                    continue;
                else newCollection.push_back(element);
            }

            return LinqContainer(std::move(newCollection));
        }

        LinqContainer Order(OrderType orderType = OrderType::ASC) const noexcept
        requires Concepts::Comparable<TSource>
        {
            if (_collection.empty())
                return *this;
            std::vector<TSource> newCollection(_collection.cbegin(), _collection.cend());
            Sort::QuickSort(newCollection.data(), 0, _collection.size() - 1, orderType);
            return LinqContainer(std::move(newCollection));
        }

        template<std::invocable<TSource> TSelector>
        requires Concepts::Comparable<std::invoke_result_t<TSelector, TSource>>
        LinqContainer OrderBy(TSelector&& selector, OrderType orderType = OrderType::ASC)
            const noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
        {
            if (_collection.empty())
                return *this;
            std::vector<TSource> newCollection(_collection.cbegin(), _collection.cend());
            Sort::QuickSort(newCollection.data(), 0, _collection.size() - 1, std::forward<TSelector>(selector), orderType);
            return LinqContainer(std::move(newCollection));
        }

        LinqContainer Reverse() const noexcept
        {
            std::vector<TSource> newCollection(_collection.crbegin(), _collection.crend());
            return LinqContainer(std::move(newCollection));
        }

        template<typename TOtherCollection>
        requires Concepts::ConstIterable<TOtherCollection> &&
                 Concepts::HasSize<TOtherCollection> &&
                 Concepts::Equatable<TSource>
        LinqContainer Except(const TOtherCollection& otherCollection) const noexcept
        {
            std::set<TSource> newCollection;

            for (const TSource& element : _collection)
            {
                long long j = 0;
                for (const TSource& otherElement : otherCollection)
                {
                    if (element == otherElement)
                        break;
                    if (j == static_cast<long long>(otherCollection.size() - 1))
                        newCollection.insert(element);
                    ++j;
                }
            }

            std::vector<TSource> assignCollection;
            assignCollection.assign(newCollection.cbegin(), newCollection.cend());
            return LinqContainer(std::move(assignCollection));
        }

        template<typename TOtherCollection>
        requires Concepts::Iterable<TOtherCollection> &&
                 Concepts::HasSize<TOtherCollection> &&
                 Concepts::Equatable<TSource>
        LinqContainer Except(TOtherCollection&& otherCollection) const noexcept
        {
            std::set<TSource> newCollection;

            for (const TSource& element : _collection)
            {
                std::size_t j = 0;
                for (TSource&& otherElement : otherCollection)
                {
                    if (element == std::move(otherElement))
                        break;
                    if (j == otherCollection.size() - 1)
                        newCollection.insert(element);
                    ++j;
                }
            }

            std::vector<TSource> assignCollection;
            assignCollection.assign(newCollection.cbegin(), newCollection.cend());
            return LinqContainer(std::move(assignCollection));
        }

        template<Concepts::ConstIterable TOtherCollection>
        requires Concepts::Equatable<TSource>
        LinqContainer Intersect(const TOtherCollection& otherCollection) const noexcept
        {
            std::set<TSource> newCollection;

            for (const TSource& element : _collection)
                for (const TSource& otherElement : otherCollection)
                    if (element == otherElement)
                    {
                        newCollection.insert(element);
                        break;
                    }

            std::vector<TSource> assignCollection;
            assignCollection.assign(newCollection.cbegin(), newCollection.cend());
            return LinqContainer(std::move(assignCollection));
        }

        template<Concepts::Iterable TOtherCollection>
        requires Concepts::Equatable<TSource>
        LinqContainer Intersect(TOtherCollection&& otherCollection) const noexcept
        {
            std::set<TSource> newCollection;

            for (const TSource& element : _collection)
                for (TSource&& otherElement : otherCollection)
                    if (element == otherElement)
                    {
                        newCollection.insert(std::move(otherElement));
                        break;
                    }

            std::vector<TSource> assignCollection;
            assignCollection.assign(newCollection.cbegin(), newCollection.cend());
            return LinqContainer(std::move(assignCollection));
        }

        LinqContainer Distinct() const noexcept
        requires Concepts::Equatable<TSource>
        {
            std::set<TSource> newCollection;
            for (const TSource& element : _collection)
                newCollection.insert(element);

            std::vector<TSource> assignCollection;
            assignCollection.assign(newCollection.cbegin(), newCollection.cend());
            return LinqContainer(std::move(assignCollection));
        }

        template<Concepts::ConstIterable TOtherCollection>
        requires Concepts::Equatable<TSource>
        LinqContainer Union(const TOtherCollection& otherCollection) const noexcept
        {
            std::set<TSource> newCollection;

            for (const TSource& element : _collection)
                newCollection.insert(element);

            for (const TSource& element : otherCollection)
                newCollection.insert(element);

            std::vector<TSource> assignCollection;
            assignCollection.assign(newCollection.cbegin(), newCollection.cend());
            return LinqContainer(std::move(assignCollection));
        }

        template<Concepts::Iterable TOtherCollection>
        requires Concepts::Equatable<TSource>
        LinqContainer Union(TOtherCollection&& otherCollection) const noexcept
        {
            std::set<TSource> newCollection;

            for (const TSource& element : _collection)
                newCollection.insert(element);

            for (TSource&& element : otherCollection)
                newCollection.insert(std::move(element));

            std::vector<TSource> assignCollection;
            assignCollection.assign(newCollection.cbegin(), newCollection.cend());
            return LinqContainer(std::move(assignCollection));
        }

        template<typename TResult, std::invocable<TResult, TSource> TAggregate>
        TResult Aggregate(TAggregate&& aggregateFunction) const
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return Aggregate::Aggregate<TResult>(_collection.data(), 0, _collection.size() - 1,
                                                 std::forward<TAggregate>(aggregateFunction));
        }

        [[nodiscard]]
        std::size_t Count() const noexcept
        {
            return _collection.size();
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        std::size_t Count(TPredicate&& predicate)
        const noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            if (_collection.empty())
                return 0;
            return Aggregate::Count(_collection.data(), 0, _collection.size() - 1, std::forward<TPredicate>(predicate));
        }

        TSource Sum() const
        requires Concepts::Summarize<TSource>
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return Aggregate::Sum(_collection.data(), 0, _collection.size() - 1);
        }

        template<std::invocable<TSource> TSelector,
                 Concepts::Summarize TResult = std::invoke_result_t<TSelector, TSource>>
        TResult Sum(TSelector&& selector) const
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return Aggregate::Sum(_collection.data(), 0, _collection.size() - 1, std::forward<TSelector>(selector));
        }

        TSource Min() const
        requires Concepts::Comparable<TSource>
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return Aggregate::Min(_collection.data(), 0, _collection.size() - 1);
        }

        template<std::invocable<TSource> TSelector,
                 Concepts::Comparable TResult = std::invoke_result_t<TSelector, TSource>>
        TResult Min(TSelector&& selector) const
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Min(_collection.data(), 0, _collection.size() - 1, std::forward<TSelector>(selector));
        }

        TSource Max() const
        requires Concepts::Comparable<TSource>
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return Aggregate::Max(_collection.data(), 0, _collection.size() - 1);
        }

        template<std::invocable<TSource> TSelector,
                 Concepts::Comparable TResult = std::invoke_result_t<TSelector, TSource>>
        TResult Max(TSelector&& selector) const
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return Aggregate::Max(_collection.data(), 0, _collection.size() - 1, std::forward<TSelector>(selector));
        }

        TSource Average() const
        requires Concepts::Divisible<TSource>
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return Aggregate::Average(_collection.data(), 0, _collection.size() - 1);
        }

        template<std::invocable<TSource> TSelector,
                 Concepts::Divisible TResult = std::invoke_result_t<TSelector, TSource>>
        TResult Average(TSelector&& selector) const
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return Aggregate::Average(_collection.data(), 0, _collection.size() - 1, std::forward<TSelector>(selector));
        }

        TSource First() const
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return _collection[0];
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        TSource First(TPredicate&& predicate) const
        {
            for (const TSource& element : _collection)
                if (predicate(element))
                    return element;
            throw std::out_of_range("Element not found");
        }

        TSource FirstOrDefault() const noexcept
        requires std::is_default_constructible_v<TSource>
        {
            if (_collection.empty())
                return {};
            return _collection[0];
        }

        TSource FirstOrDefault(const TSource& defaultValue) const noexcept
        {
            if (_collection.empty())
                return defaultValue;
            return _collection[0];
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        requires std::is_default_constructible_v<TSource>
        TSource FirstOrDefault(TPredicate&& predicate)
        const noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            for (const TSource& element : _collection)
                if (predicate(element))
                    return element;
            return {};
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        TSource FirstOrDefault(TPredicate&& predicate, const TSource& defaultValue)
        const noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            for (const TSource& element : _collection)
                if (predicate(element))
                    return element;
            return defaultValue;
        }

        TSource Last() const
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return _collection[_collection.size() - 1];
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        TSource Last(TPredicate&& predicate) const
        {
            for (const_reverse_iterator it = _collection.crbegin(); it != _collection.crend(); ++it)
                if (predicate(*it))
                    return *it;
            throw std::out_of_range("Element not found");
        }

        TSource LastOrDefault() const noexcept
        requires std::is_default_constructible_v<TSource>
        {
            if (_collection.empty())
                return {};
            return _collection[_collection.size() - 1];
        }

        TSource LastOrDefault(const TSource& defaultValue) const noexcept
        {
            if (_collection.empty())
                return defaultValue;
            return _collection[_collection.size() - 1];
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        requires std::is_default_constructible_v<TSource>
        TSource LastOrDefault(TPredicate&& predicate)
        const noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            for (const_reverse_iterator it = _collection.crbegin(); it != _collection.crend(); ++it)
                if (predicate(*it))
                    return *it;
            return {};
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        TSource LastOrDefault(TPredicate&& predicate, const TSource& defaultValue)
        const noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            for (const_reverse_iterator it = _collection.crbegin(); it != _collection.crend(); ++it)
                if (predicate(*it))
                    return *it;
            return defaultValue;
        }

        TSource At(const std::size_t position) const
        {
            if (position >= _collection.size())
                throw std::out_of_range("Position out of range");
            return _collection[position];
        }

        TSource AtOrDefault(const std::size_t position) const noexcept
        requires std::is_default_constructible_v<TSource>
        {
            if (position >= _collection.size())
                return {};
            return _collection[position];
        }

        TSource AtOrDefault(const std::size_t position, const TSource& defaultValue) const noexcept
        {
            if (position >= _collection.size())
                return defaultValue;
            return _collection[position];
        }

        [[nodiscard]]
        bool IsEmpty() const noexcept
        {
            return _collection.empty();
        }

        LinqContainer Skip(const std::size_t count) const noexcept
        {
            std::vector<TSource> newCollection;

            if (count >= _collection.size())
                return LinqContainer(std::move(newCollection));

            newCollection.reserve(_collection.size() - count);
            for (std::size_t i = count; i < _collection.size(); ++i)
                newCollection.push_back(_collection[i]);

            return LinqContainer(std::move(newCollection));
        }

        LinqContainer SkipLast(const std::size_t count) const noexcept
        {
            std::vector<TSource> newCollection;

            if (count >= _collection.size())
                return LinqContainer(std::move(newCollection));

            newCollection.reserve(_collection.size() - count);

            for (std::size_t i = 0; i < _collection.size() - count; ++i)
                newCollection.push_back(_collection[i]);

            return LinqContainer(std::move(newCollection));
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        LinqContainer SkipWhile(TPredicate&& predicate)
        const noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            std::vector<TSource> newCollection;

            std::size_t i = 0;
            for (; i < _collection.size(); ++i)
                if(!predicate(_collection[i]))
                    break;

            for (; i < _collection.size(); ++i)
                newCollection.push_back(_collection[i]);

            return LinqContainer(std::move(newCollection));
        }

        LinqContainer Take(const std::size_t count) const noexcept
        {
            std::vector<TSource> newCollection;

            if (count >= _collection.size())
                return LinqContainer(std::move(newCollection));

            newCollection.reserve(count);
            for (std::size_t i = 0; i < count; ++i)
                newCollection.push_back(_collection[i]);

            return LinqContainer(std::move(newCollection));
        }

        LinqContainer TakeLast(const std::size_t count) const noexcept
        {
            std::vector<TSource> newCollection;

            if (count >= _collection.size())
                return LinqContainer(std::move(newCollection));

            newCollection.reserve(count);
            for (std::size_t i = _collection.size() - count; i < _collection.size(); ++i)
                newCollection.push_back(_collection[i]);

            return LinqContainer(std::move(newCollection));
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        LinqContainer TakeWhile(TPredicate&& predicate)
        const noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            std::vector<TSource> newCollection;

            for (const TSource& element : _collection)
            {
                if (!predicate(element))
                    break;
                newCollection.push_back(element);
            }

            return LinqContainer(std::move(newCollection));
        }

        template<std::invocable<TSource> TKeySelector,
                 typename TKey = std::invoke_result_t<TKeySelector, TSource>>
        std::map<TKey, std::vector<TSource>> GroupBy(TKeySelector&& keySelector)
        const noexcept(std::is_nothrow_invocable_v<TKeySelector, TSource>)
        {
            std::map<TKey, std::vector<TSource>> result;

            for (const TSource& element : _collection)
            {
                if (!result.contains(keySelector(element)))
                    result.insert(std::pair<TKey, std::vector<TSource>>(keySelector(element), std::vector<TSource>()));
                result.at(keySelector(element)).push_back(element);
            }

            return result;
        }

        template<Concepts::ConstIterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, TSource, typename TOtherCollection::value_type>>
        requires std::same_as<std::invoke_result_t<TInnerKeySelector, TSource>,
                              std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
                 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, TSource>>
        LinqContainer<TResult> Join(const TOtherCollection& otherCollection,
                                    TInnerKeySelector&& innerKeySelector,
                                    TOtherKeySelector&& otherKeySelector,
                                    TResultSelector&& resultSelector)
        const noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
                       std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
                       std::is_nothrow_invocable_v<TResultSelector, TSource, typename TOtherCollection::value_type>)
        {
            std::vector<TResult> newCollection;

            if (otherCollection.empty())
                return LinqContainer<TResult>(std::move(newCollection));

            for (const TSource& element : _collection)
                for (const auto& otherElement : otherCollection)
                    if (innerKeySelector(element) == otherKeySelector(otherElement))
                        newCollection.push_back(resultSelector(element, otherElement));

            return LinqContainer<TResult>(std::move(newCollection));
        }

        template<Concepts::Iterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, TSource, typename TOtherCollection::value_type>>
        requires std::same_as<std::invoke_result_t<TInnerKeySelector, TSource>,
                              std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
                 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, TSource>>
        LinqContainer<TResult> Join(TOtherCollection&& otherCollection,
                                    TInnerKeySelector&& innerKeySelector,
                                    TOtherKeySelector&& otherKeySelector,
                                    TResultSelector&& resultSelector)
        const noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
                       std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
                       std::is_nothrow_invocable_v<TResultSelector, TSource, typename TOtherCollection::value_type>)
        {
            std::vector<TResult> newCollection;

            if (otherCollection.empty())
                return LinqContainer<TResult>(std::move(newCollection));

            for (const TSource& element : _collection)
                for (auto& otherElement : otherCollection)
                    if (innerKeySelector(element) == otherKeySelector(otherElement))
                        newCollection.push_back(resultSelector(element, std::move(otherElement)));

            return LinqContainer<TResult>(std::move(newCollection));
        }

        template<Concepts::ConstIterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, TSource>,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<const std::vector<TSource>&, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>>
        requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>>
        LinqContainer<TResult> GroupJoin(const TOtherCollection& otherCollection,
                                         TInnerKeySelector&& innerKeySelector,
                                         TOtherKeySelector&& otherKeySelector,
                                         TResultSelector&& resultSelector)
        const noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
                       std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
                       std::is_nothrow_invocable_v<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>)
        {
            std::vector<TResult> newCollection;

            if (otherCollection.empty())
                return LinqContainer<TResult>(std::move(newCollection));

            std::map<TKey, std::vector<TSource>> groups = GroupBy(std::forward<TInnerKeySelector>(innerKeySelector));

            for (const auto& [key, group] : groups)
                for (const auto& element : otherCollection)
                    if (key == otherKeySelector(element))
                        newCollection.push_back(resultSelector(group, element));

            return LinqContainer<TResult>(std::move(newCollection));
        }

        template<Concepts::Iterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, TSource>,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<const std::vector<TSource>&, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>>
        requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>>
        LinqContainer<TResult> GroupJoin(TOtherCollection&& otherCollection,
                                         TInnerKeySelector&& innerKeySelector,
                                         TOtherKeySelector&& otherKeySelector,
                                         TResultSelector&& resultSelector)
        const noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
                       std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
                       std::is_nothrow_invocable_v<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>)
        {
            std::vector<TResult> newCollection;

            if (otherCollection.empty())
                return LinqContainer<TResult>(std::move(newCollection));

            std::map<TKey, std::vector<TSource>> groups = GroupBy(innerKeySelector);

            for (const auto& [key, group] : groups)
                for (auto& element : otherCollection)
                    if (key == otherKeySelector(element))
                        newCollection.push_back(resultSelector(group, std::move(element)));

            return LinqContainer<TResult>(std::move(newCollection));
        }

        template<typename TOtherCollection,
                 typename TOtherCollectionValueType = typename TOtherCollection::value_type>
        requires Concepts::ConstIterable<TOtherCollection> && Concepts::HasSize<TOtherCollection>
        LinqContainer<std::pair<TSource, TOtherCollectionValueType>> Zip(const TOtherCollection& otherCollection) const noexcept
        {
            std::vector<std::pair<TSource, TOtherCollectionValueType>> newCollection;

            if (otherCollection.empty())
                return LinqContainer<std::pair<TSource, TOtherCollectionValueType>>(std::move(newCollection));
            if (otherCollection.size() != _collection.size())
                return LinqContainer<std::pair<TSource, TOtherCollectionValueType>>(std::move(newCollection));

            std::size_t i = 0;
            for (const auto& element : otherCollection)
            {
                newCollection.push_back(std::pair<TSource, TOtherCollectionValueType>(_collection[i], element));
                ++i;
            }

            return LinqContainer<std::pair<TSource, TOtherCollectionValueType>>(std::move(newCollection));
        }

        template<typename TOtherCollection,
                 typename TOtherCollectionValueType = typename TOtherCollection::value_type>
        requires Concepts::Iterable<TOtherCollection> && Concepts::HasSize<TOtherCollection>
        LinqContainer<std::pair<TSource, TOtherCollectionValueType>> Zip(TOtherCollection&& otherCollection) const noexcept
        {
            std::vector<std::pair<TSource, TOtherCollectionValueType>> newCollection;

            if (otherCollection.empty())
                return LinqContainer<std::pair<TSource, TOtherCollectionValueType>>(std::move(newCollection));
            if (otherCollection.size() != _collection.size())
                return LinqContainer<std::pair<TSource, TOtherCollectionValueType>>(std::move(newCollection));

            std::size_t i = 0;
            for (auto& element : otherCollection)
            {
                newCollection.push_back(std::pair<TSource, TOtherCollectionValueType>(_collection[i], std::move(element)));
                ++i;
            }

            return LinqContainer<std::pair<TSource, TOtherCollectionValueType>>(std::move(newCollection));
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        bool All(TPredicate&& predicate)
        const noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            for (const TSource& element : _collection)
                if (!predicate(element))
                    return false;
            return true;
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        bool Any(TPredicate&& predicate)
        const noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            for (const TSource& element : _collection)
                if (predicate(element))
                    return true;
            return false;
        }

        bool Contains(const TSource& target) const noexcept
        {
            for (const TSource& element : _collection)
                if (element == target)
                    return true;
            return false;
        }

        template<std::invocable<TSource> TSelector>
        requires Concepts::Equatable<std::invoke_result_t<TSelector, TSource>>
        bool Contains(const TSource& target, TSelector&& selector)
            const noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
        {
            for (const TSource& element : _collection)
                if (selector(element) == selector(target))
                    return true;
            return false;
        }

        bool Contains(const std::vector<TSource>& subCollection) const noexcept
        requires Concepts::Equatable<TSource>
        {
            if (subCollection.empty())
                return true;
            return Algorithm::Contains(_collection.data(), _collection.size(), subCollection.data(), subCollection.size());
        }

        std::size_t IndexAt(const TSource& target) const noexcept
        requires Concepts::Equatable<TSource>
        {
            for (std::size_t i = 0; i < _collection.size(); ++i)
                if (_collection[i] == target)
                    return i;
            return NPOS;
        }

        template<std::invocable<TSource> TSelector>
        requires Concepts::Equatable<std::invoke_result_t<TSelector, TSource>>
        std::size_t IndexAt(const TSource& target, TSelector&& selector)
        const noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
        {
            for (std::size_t i = 0; i < _collection.size(); ++i)
                if (selector(_collection[i]) == selector(target))
                    return i;
            return NPOS;
        }

        std::size_t IndexAt(const std::vector<TSource>& subCollection) const noexcept
        requires Concepts::Equatable<TSource>
        {
            return Algorithm::IndexAt(_collection.data(), _collection.size(), subCollection.data(), subCollection.size());
        }

        std::size_t BinarySearch(const TSource& element) const noexcept
        requires Concepts::Comparable<TSource>
        {
            if (_collection.empty())
                return NPOS;
            return Algorithm::BinarySearch(std::forward<TSource>(element), _collection.data(), 0, _collection.size() - 1);
        }

        template<std::invocable<TSource> TSelector>
        requires Concepts::Comparable<std::invoke_result_t<TSelector, TSource>>
        std::size_t BinarySearch(const TSource& element, TSelector&& selector)
        const noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
        {
            if (_collection.empty())
                return NPOS;
            return Algorithm::BinarySearch(std::forward<TSource>(element), _collection.data(), 0, _collection.size() - 1, std::forward<TSelector>(selector));
        }

        LinqContainer PushBack(const TSource& element) const noexcept
        {
            std::vector<TSource> newCollection;
            newCollection.reserve(_collection.size() + 1);

            std::copy(_collection.cbegin(), _collection.cend(),
                      std::inserter(newCollection, newCollection.begin()));

            newCollection.push_back(std::forward<TSource>(element));

            return LinqContainer(std::move(newCollection));
        }

        LinqContainer PushBack(TSource&& element) const noexcept
        {
            std::vector<TSource> newCollection;
            newCollection.reserve(_collection.size() + 1);

            std::copy(_collection.cbegin(), _collection.cend(),
                      std::inserter(newCollection, newCollection.begin()));

            newCollection.push_back(std::forward<TSource>(element));

            return LinqContainer(std::move(newCollection));
        }

        template<typename TCollection>
        requires Concepts::ConstIterable<TCollection> && Concepts::HasSize<TCollection>
        LinqContainer PushBack(const TCollection& otherCollection) const noexcept
        {
            std::vector<TSource> newCollection;
            newCollection.reserve(_collection.size() + otherCollection.size());

            std::copy(_collection.cbegin(), _collection.cend(),
                      std::inserter(newCollection, newCollection.begin()));

            std::copy(otherCollection.cbegin(), otherCollection.cend(),
                      std::inserter(newCollection, newCollection.end()));

            return LinqContainer(std::move(newCollection));
        }

        LinqContainer PushFront(const TSource& element) const noexcept
        {
            return Insert(std::forward<TSource>(element), 0);
        }

        LinqContainer PushFront(TSource&& element) const noexcept
        {
            return Insert(std::forward<TSource>(element), 0);
        }

        template<typename TCollection>
        requires Concepts::ConstIterable<TCollection> && Concepts::HasSize<TCollection>
        LinqContainer PushFront(const TCollection& otherCollection) const noexcept
        {
            return Insert(otherCollection, 0);
        }

        LinqContainer Insert(const TSource& element, std::size_t position) const noexcept
        {
            std::vector<TSource> newCollection;
            newCollection.reserve(_collection.size() + 1);

            std::copy(_collection.cbegin(), _collection.cbegin() + position,
                      std::inserter(newCollection, newCollection.begin()));

            newCollection.push_back(std::forward<TSource>(element));

            std::copy(_collection.cbegin() + position, _collection.cend(),
                      std::inserter(newCollection, newCollection.end()));

            return LinqContainer(std::move(newCollection));
        }

        LinqContainer Insert(TSource&& element, std::size_t position) const noexcept
        {
            std::vector<TSource> newCollection;
            newCollection.reserve(_collection.size() + 1);

            std::copy(_collection.cbegin(), _collection.cbegin() + position,
                      std::inserter(newCollection, newCollection.begin()));

            newCollection.push_back(std::forward<TSource>(element));

            std::copy(_collection.cbegin() + position, _collection.cend(),
                      std::inserter(newCollection, newCollection.end()));

            return LinqContainer(std::move(newCollection));
        }

        template<typename TCollection>
        requires Concepts::ConstIterable<TCollection> && Concepts::HasSize<TCollection>
        LinqContainer Insert(const TCollection& otherCollection, std::size_t position) const noexcept
        {
            std::vector<TSource> newCollection;
            newCollection.reserve(_collection.size() + otherCollection.size());

            std::copy(_collection.cbegin(), _collection.cbegin() + position,
                      std::inserter(newCollection, newCollection.begin()));

            std::copy(otherCollection.cbegin(), otherCollection.cend(),
                      std::inserter(newCollection, newCollection.begin() + position));

            std::copy(_collection.cbegin() + position, _collection.cend(),
                      std::inserter(newCollection, newCollection.end()));

            return LinqContainer(std::move(newCollection));
        }

        LinqContainer Erase(const std::size_t position) const noexcept
        {
            return Erase(position, position);
        }

        LinqContainer Erase(std::size_t begin, std::size_t end) const noexcept
        {
            std::vector<TSource> newCollection;

            if (begin > end || _collection.size() <= end - begin)
                return LinqContainer(std::move(newCollection));

            newCollection.reserve(_collection.size() - (end - begin));

            std::copy(_collection.cbegin(), _collection.cbegin() + begin,
                      std::inserter(newCollection, newCollection.begin()));

            std::copy(_collection.cbegin() + end + 1, _collection.cend(),
                      std::inserter(newCollection, newCollection.end()));

            return LinqContainer(std::move(newCollection));
        }
    };
}

#endif