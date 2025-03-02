#ifndef LINQ_LinqContainer_H
#define LINQ_LinqContainer_H

#include <vector>
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
#include <stdexcept>
#include <iterator>
#include <type_traits>
#include <concepts>
#include <utility>

#include <ExtendedCpp/LINQ/Algorithm.h>
#include <ExtendedCpp/LINQ/Sort.h>
#include <ExtendedCpp/LINQ/Concepts.h>
#include <ExtendedCpp/LINQ/TypeTraits.h>
#include <ExtendedCpp/LINQ/OrderType.h>

/// @brief 
namespace ExtendedCpp::LINQ
{
    /// @brief Owning thread-safe container
    /// @tparam TSource any copyable type
    template<std::copyable TSource>
    class LinqContainer final
    {
    private:
        std::vector<TSource> _collection;

    public:
        /// @brief 
        using value_type = TSource;

        /// @brief 
        using iterator = std::vector<TSource>::iterator;

        /// @brief 
        using const_iterator = std::vector<TSource>::const_iterator;

        /// @brief 
        using reverse_iterator = std::vector<TSource>::reverse_iterator;

        /// @brief 
        using const_reverse_iterator = std::vector<TSource>::const_reverse_iterator;

        /// @brief Copy data from vector into LINQ conainer
        /// @param collection 
        explicit LinqContainer(const std::vector<TSource>& collection) noexcept
        {
            _collection = collection;
        }

        /// @brief Move data from vector into LINQ conainer
        /// @param collection 
        explicit LinqContainer(std::vector<TSource>&& collection) noexcept
        {
            _collection = std::move(collection);
        }

        /// @brief Copy constructor
        /// @param container 
        LinqContainer(const LinqContainer& container) noexcept
        {
            _collection = container.ToVector();
        }

        /// @brief Move constructor
        /// @param container 
        LinqContainer(LinqContainer&& container) noexcept
        {
            _collection = std::move(container.ToVector());
        }

        /// @brief Default destructor
        ~LinqContainer() = default;

        /// @brief Copy asign
        /// @param collection Copy data from vector into LINQ conainer
        /// @return
        LinqContainer& operator=(const std::vector<TSource>& collection) noexcept
        {
            _collection = collection;
            return *this;
        }

        /// @brief Move asign
        /// @param collection Move data from vector into LINQ conainer
        /// @return 
        LinqContainer& operator=(std::vector<TSource>&& collection) noexcept
        {
            _collection = std::move(collection);
            return *this;
        }

        /// @brief Copy asign
        /// @param container Copy other LINQ container
        /// @return 
        LinqContainer& operator=(const LinqContainer& container) noexcept
        {
            if (this == &container) return *this;
            _collection = container.ToVector();
            return *this;
        }

        /// @brief Move asign
        /// @param container Move data from other LINQ container
        /// @return 
        LinqContainer& operator=(LinqContainer&& container) noexcept
        {
            _collection = std::move(container.ToVector());
            return *this;
        }

        /// @brief Get container size
        /// @return Number of elements
        [[nodiscard]]
        std::size_t size() const noexcept
        {
            return _collection.size();
        }

        /// @brief Check container size
        /// @return false if empty, true if not empty
        [[nodiscard]]
        bool empty() const noexcept
        {
            return _collection.empty();
        }

        /// @brief Get iterator of implemented vector<T>
        /// @return Random access iterator
        iterator begin() noexcept
        {
            return _collection.begin();
        }

        /// @brief Get iterator of implemented vector<T>
        /// @return Random access iterator
        const_iterator begin() const noexcept
        {
            return _collection.cbegin();
        }

        /// @brief Get iterator of implemented vector<T>
        /// @return Random access iterator
        iterator end() noexcept
        {
            return _collection.end();
        }

        /// @brief Get iterator of implemented vector<T>
        /// @return Random access iterator
        const_iterator end() const noexcept
        {
            return _collection.cend();
        }

        /// @brief Get iterator of implemented vector<T>
        /// @return Random access iterator
        const_iterator cbegin() const noexcept
        {
            return _collection.cbegin();
        }

        /// @brief Get iterator of implemented vector<T>
        /// @return Random access iterator
        const_iterator cend() const noexcept
        {
            return _collection.cend();
        }

        /// @brief Get iterator of implemented vector<T>
        /// @return Random access iterator
        reverse_iterator rbegin() noexcept
        {
            return _collection.rbegin();
        }

        /// @brief Get iterator of implemented vector<T>
        /// @return Random access iterator
        const_reverse_iterator rbegin() const noexcept
        {
            return _collection.crbegin();
        }

        /// @brief Get iterator of implemented vector<T>
        /// @return Random access iterator
        reverse_iterator rend() noexcept
        {
            return _collection.rend();
        }

        /// @brief Get iterator of implemented vector<T>
        /// @return Random access iterator
        const_reverse_iterator rend() const noexcept
        {
            return _collection.crend();
        }

        /// @brief Get iterator of implemented vector<T>
        /// @return Random access iterator
        const_reverse_iterator crbegin() const noexcept
        {
            return _collection.crbegin();
        }

        /// @brief Get iterator of implemented vector<T>
        /// @return Random access iterator
        const_reverse_iterator crend() const noexcept
        {
            return _collection.crend();
        }

        /// @brief Get pointer to first element of container data
        /// @return Pointer to first element
        TSource* data() noexcept
        {
            return _collection.data();
        }

        /// @brief Get pointer to first element of container data
        /// @return Const pointer to first element
        const TSource* data() const noexcept
        {
            return _collection.data();
        }

        /// @brief Get copy of collection data
        /// @return Copy of implemented vector<T>
        std::vector<TSource> ToVector() const noexcept
        {
            return _collection;
        }

        /// @brief Get copy of collection data
        /// @tparam SIZE Size of returned array
        /// @return Copies of elements from 0 to min of array size or LINQ container size
        template<std::size_t SIZE>
        requires std::is_default_constructible_v<TSource>
        std::array<TSource, SIZE> ToArray() const noexcept
        {
            std::array<TSource, SIZE> array{};
            for (std::size_t i = 0; i < SIZE && i < _collection.size(); ++i)
                array[i] = _collection[i];
            return array;
        }

        /// @brief Get copy of collection data
        /// @return Copies of elements from LINQ container, maintaining order
        std::list<TSource> ToList() const noexcept
        {
            return std::list<TSource>(_collection.cbegin(), _collection.cend());
        }

        /// @brief Get copy of collection data
        /// @return Copies of elements from LINQ container, maintaining order
        std::forward_list<TSource> ToForwardList() const noexcept
        {
            return std::forward_list<TSource>(_collection.cbegin(), _collection.cend());
        }

        /// @brief Get copy of collection data
        /// @return Copies of elements from LINQ container, maintaining order
        std::stack<TSource> ToStack() const noexcept
        {
            std::stack<TSource> stack;
            for (const TSource& element : _collection)
                stack.push(element);
            return stack;
        }

        /// @brief Get copy of collection data
        /// @return Copies of elements from LINQ container, maintaining order
        std::queue<TSource> ToQueue() const noexcept
        {
            std::queue<TSource> queue;
            for (const TSource& element : _collection)
                queue.push(element);
            return queue;
        }

        /// @brief Get copy of collection data
        /// @return Copies of elements from LINQ container, maintaining order
        std::deque<TSource> ToDeque() const noexcept
        {
            return std::deque<TSource>(_collection.cbegin(), _collection.cend());
        }

        /// @brief Get copy of collection data
        /// @return Copies of elements from LINQ container, maintaining order
        std::priority_queue<TSource> ToPriorityQueue() const noexcept
        {
            std::priority_queue<TSource> priorityQueue;
            for (const TSource& element : _collection)
                priorityQueue.push(element);
            return priorityQueue;
        }

        /// @brief Get copy of collection data
        /// @return Unique elements from LINQ container
        std::set<TSource> ToSet() const noexcept
        {
            std::set<TSource> set;
            for (const TSource& element : _collection)
                set.insert(element);
            return set;
        }

        /// @brief Get copy of collection data
        /// @return Unique elements from LINQ container
        std::unordered_set<TSource> ToUnorderedSet() const noexcept
        {
            std::unordered_set<TSource> unorderedSet;
            for (const TSource& element : _collection)
                unorderedSet.insert(element);
            return unorderedSet;
        }

        /// @brief Get copy of collection data. The method is only valid for std::pair
        /// @tparam TKey decltype(pair.first)
        /// @tparam TValue decltype(pair.second)
        /// @return map<TKey, TValue> from vector<std::pair<TKey, TValue>>
        template<typename TKey = PairTraits<TSource>::FirstType,
                 typename TValue = PairTraits<TSource>::SecondType>
        requires Concepts::IsPair<TSource>
        std::map<TKey, TValue> ToMap() const noexcept
        {
            std::map<TKey, TValue> map;
            for (const TSource& element : _collection)
                map.insert(element);
            return map;
        }

        /// @brief Get copy of collection data. The method is only valid for std::pair
        /// @tparam TKey decltype(pair.first)
        /// @tparam TValue decltype(pair.second)
        /// @return unordered_map<TKey, TValue> from vector<std::pair<TKey, TValue>>
        template<typename TKey = PairTraits<TSource>::FirstType,
                 typename TValue = PairTraits<TSource>::SecondType>
        requires Concepts::IsPair<TSource>
        std::unordered_map<TKey, TValue> ToUnorderedMap() const noexcept
        {
            std::unordered_map<TKey, TValue> unorderedMap;
            for (const TSource& element : _collection)
                unorderedMap.insert(element);
            return unorderedMap;
        }

        /// @brief 
        /// @tparam TMap 
        /// @param mapFunction 
        /// @return 
        template<std::invocable<TSource> TMap>
        requires std::same_as<std::invoke_result_t<TMap, TSource>, TSource>
        LinqContainer Map(TMap&& mapFunction) const
        noexcept(std::is_nothrow_invocable_v<TMap, TSource>)
        {
            std::vector<TSource> newCollection;
            newCollection.reserve(_collection.size());

            for (const TSource& element : _collection)
                newCollection.push_back(mapFunction(element));

            return LinqContainer(std::move(newCollection));
        }

        /// @brief Applies an action to each item in the collection
        /// @tparam TTransform Any functional object with TSource argument
        /// @param transform Any functional object with TSource argument
        /// @return New collection LinqContainer<TSource>
        template<std::invocable<TSource&> TTransform>
        requires std::same_as<std::invoke_result_t<TTransform, TSource&>, void>
        LinqContainer Transform(TTransform&& transform) const
        noexcept(std::is_nothrow_invocable_v<TTransform, TSource&>)
        {
            std::vector<TSource> newCollection(_collection);

            for (TSource& element : newCollection)
                transform(element);

            return LinqContainer(std::move(newCollection));
        }

        /// @brief Iterates through all elements and applies a selector to each
        /// @tparam TResult Result of selector invoke
        /// @tparam TSelector Any functional object with TSource argument
        /// @param selector Any functional object with TSource argument
        /// @return New collection LinqContainer<TResult>
        template<std::invocable<TSource> TSelector,
                 typename TResult = std::invoke_result_t<TSelector, TSource>>
        LinqContainer<TResult> Select(TSelector&& selector) const 
        noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
        {
            std::vector<TResult> newCollection;
            newCollection.reserve(_collection.size());

            for (const TSource& element : _collection)
                newCollection.push_back(selector(element));

            return LinqContainer<TResult>(std::move(newCollection));
        }

        /// @brief Iterates through all elements and applies a selector to each
        /// @tparam TResult value_type of selector invoke result
        /// @tparam TSelector Any functional object with TSource argument
        /// @param selector Any functional object with TSource argument
        /// @return New collection LinqContainer<TResult>
        template<std::invocable<TSource> TSelector,
                 typename TResult = std::invoke_result_t<TSelector, TSource>::value_type>
        LinqContainer<TResult> SelectMany(TSelector&& selector) const 
        noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
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

        /// @brief 
        /// @tparam TCollectionValueType 
        /// @tparam TResult 
        /// @tparam TCollectionSelector 
        /// @tparam TCollection 
        /// @tparam TResultSelector 
        /// @param collectionSelector 
        /// @param resultSelector 
        /// @return 
        template<std::invocable<TSource> TCollectionSelector,
                 Concepts::Iterable TCollection = std::invoke_result_t<TCollectionSelector, TSource>,
                 typename TCollectionValueType = TCollection::value_type,
                 std::invocable<TSource, TCollectionValueType> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, TSource, TCollectionValueType>>
        LinqContainer<TResult> SelectMany(TCollectionSelector&& collectionSelector, TResultSelector&& resultSelector) const 
        noexcept(std::is_nothrow_invocable_v<TCollectionSelector, TSource> &&
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

        /// @brief Select elements from some set by condition
        /// @tparam TPredicate 
        /// @param predicate 
        /// @return 
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

        /// @brief Remove elements from some set by condition
        /// @tparam TPredicate 
        /// @param predicate 
        /// @return 
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

        /// @brief Sorts the elements of a collection
        /// @param orderType 
        /// @return 
        LinqContainer Order(OrderType orderType = OrderType::ASC) const noexcept
        requires Concepts::Comparable<TSource>
        {
            if (_collection.empty())
                return *this;
            std::vector<TSource> newCollection(_collection.cbegin(), _collection.cend());
            Sort::QuickSort(newCollection.data(), 0, _collection.size() - 1, orderType);
            return LinqContainer(std::move(newCollection));
        }

        /// @brief Sorts the elements of a collection with selector
        /// @tparam TSelector 
        /// @param selector 
        /// @param orderType 
        /// @return 
        template<std::invocable<TSource> TSelector>
        requires Concepts::Comparable<std::invoke_result_t<TSelector, TSource>>
        LinqContainer OrderBy(TSelector&& selector, OrderType orderType = OrderType::ASC) const 
        noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
        {
            if (_collection.empty())
                return *this;
            std::vector<TSource> newCollection(_collection.cbegin(), _collection.cend());
            Sort::QuickSort(newCollection.data(), 0, _collection.size() - 1, std::forward<TSelector>(selector), orderType);
            return LinqContainer(std::move(newCollection));
        }

        /// @brief Reverse the collection
        /// @return 
        LinqContainer Reverse() const noexcept
        {
            std::vector<TSource> newCollection(_collection.crbegin(), _collection.crend());
            return LinqContainer(std::move(newCollection));
        }

        /// @brief Get the difference of two sequences
        /// @tparam TOtherCollection 
        /// @param otherCollection 
        /// @return 
        template<typename TOtherCollection>
        requires Concepts::ConstIterable<TOtherCollection> &&
                 Concepts::HasSize<TOtherCollection> &&
                 Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
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

        /// @brief Get the difference of two sequences
        /// @tparam TOtherCollection 
        /// @param otherCollection 
        /// @return 
        template<typename TOtherCollection>
        requires Concepts::Iterable<TOtherCollection> &&
                 Concepts::HasSize<TOtherCollection> &&
                 Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
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

        /// @brief Get the intersection of sequences
        /// @tparam TOtherCollection 
        /// @param otherCollection 
        /// @return 
        template<Concepts::ConstIterable TOtherCollection>
        requires Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
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

        /// @brief Get the intersection of sequences
        /// @tparam TOtherCollection 
        /// @param otherCollection 
        /// @return 
        template<Concepts::Iterable TOtherCollection>
        requires Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
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

        /// @brief Remove duplicates in a set
        /// @return 
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

        /// @brief Join two sequences
        /// @tparam TOtherCollection 
        /// @param otherCollection 
        /// @return 
        template<Concepts::ConstIterable TOtherCollection>
        requires Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
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

        /// @brief Join two sequences
        /// @tparam TOtherCollection 
        /// @param otherCollection 
        /// @return 
        template<Concepts::Iterable TOtherCollection>
        requires Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
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

        /// @brief Performs a general aggregation of the elements of the collection depending on the specified expression
        /// @tparam TResult 
        /// @tparam TAggregate 
        /// @param aggregateFunction 
        /// @return 
        template<typename TResult, std::invocable<TResult, TSource> TAggregate>
        TResult Aggregate(TAggregate&& aggregateFunction) const
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return Aggregate::Aggregate<TResult>(_collection.data(), 0, _collection.size() - 1,
                                                 std::forward<TAggregate>(aggregateFunction));
        }

        /// @brief Get the number of elements
        /// @return 
        [[nodiscard]]
        std::size_t Count() const noexcept
        {
            return _collection.size();
        }

        /// @brief Get the number of elements
        /// @tparam TPredicate 
        /// @param predicate 
        /// @return 
        template<Concepts::IsPredicate<TSource> TPredicate>
        std::size_t Count(TPredicate&& predicate) const 
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            if (_collection.empty())
                return 0;
            return Aggregate::Count(_collection.data(), 0, _collection.size() - 1, std::forward<TPredicate>(predicate));
        }

        /// @brief Get the sum of values
        /// @return 
        TSource Sum() const
        requires Concepts::Summarize<TSource>
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return Aggregate::Sum(_collection.data(), 0, _collection.size() - 1);
        }

        /// @brief Get the sum of values
        /// @tparam TSelector 
        /// @tparam TResult 
        /// @param selector 
        /// @return 
        template<std::invocable<TSource> TSelector,
                 Concepts::Summarize TResult = std::invoke_result_t<TSelector, TSource>>
        TResult Sum(TSelector&& selector) const
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return Aggregate::Sum(_collection.data(), 0, _collection.size() - 1, std::forward<TSelector>(selector));
        }

        /// @brief Find element with the minimum value
        /// @return 
        TSource Min() const
        requires Concepts::Comparable<TSource>
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return Aggregate::Min(_collection.data(), 0, _collection.size() - 1);
        }

        /// @brief Find element with the minimum value
        /// @tparam TSelector 
        /// @tparam TResult 
        /// @param selector 
        /// @return 
        template<std::invocable<TSource> TSelector,
                 Concepts::Comparable TResult = std::invoke_result_t<TSelector, TSource>>
        TResult Min(TSelector&& selector) const
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Min(_collection.data(), 0, _collection.size() - 1, std::forward<TSelector>(selector));
        }

        /// @brief Find element with the maximum value
        /// @return 
        TSource Max() const
        requires Concepts::Comparable<TSource>
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return Aggregate::Max(_collection.data(), 0, _collection.size() - 1);
        }

        /// @brief Find element with the maximum value
        /// @tparam TSelector 
        /// @tparam TResult 
        /// @param selector 
        /// @return 
        template<std::invocable<TSource> TSelector,
                 Concepts::Comparable TResult = std::invoke_result_t<TSelector, TSource>>
        TResult Max(TSelector&& selector) const
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return Aggregate::Max(_collection.data(), 0, _collection.size() - 1, std::forward<TSelector>(selector));
        }

        /// @brief Find the average value of the collection
        /// @return 
        TSource Average() const
        requires Concepts::Divisible<TSource>
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return Aggregate::Average(_collection.data(), 0, _collection.size() - 1);
        }

        /// @brief Find the average value of the collection
        /// @tparam TSelector 
        /// @tparam TResult 
        /// @param selector 
        /// @return 
        template<std::invocable<TSource> TSelector,
                 Concepts::Divisible TResult = std::invoke_result_t<TSelector, TSource>>
        TResult Average(TSelector&& selector) const
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return Aggregate::Average(_collection.data(), 0, _collection.size() - 1, std::forward<TSelector>(selector));
        }

        /// @brief Get first element of collection
        /// @return 
        TSource First() const
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return _collection[0];
        }

        /// @brief Get first element of collection
        /// @tparam TPredicate 
        /// @param predicate 
        /// @return 
        template<Concepts::IsPredicate<TSource> TPredicate>
        TSource First(TPredicate&& predicate) const
        {
            for (const TSource& element : _collection)
                if (predicate(element))
                    return element;
            throw std::out_of_range("Element not found");
        }

        /// @brief Get first element of collection, return default if not found
        /// @return 
        TSource FirstOrDefault() const noexcept
        requires std::is_default_constructible_v<TSource>
        {
            if (_collection.empty())
                return {};
            return _collection[0];
        }

        /// @brief Get first element of collection, return default if not found
        /// @param defaultValue 
        /// @return 
        TSource FirstOrDefault(const TSource& defaultValue) const noexcept
        {
            if (_collection.empty())
                return defaultValue;
            return _collection[0];
        }

        /// @brief Get first element of collection, return default if not found
        /// @tparam TPredicate 
        /// @param predicate 
        /// @return 
        template<Concepts::IsPredicate<TSource> TPredicate>
        requires std::is_default_constructible_v<TSource>
        TSource FirstOrDefault(TPredicate&& predicate) const 
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            for (const TSource& element : _collection)
                if (predicate(element))
                    return element;
            return {};
        }

        /// @brief Get first element of collection, return default if not found
        /// @tparam TPredicate 
        /// @param predicate 
        /// @param defaultValue 
        /// @return 
        template<Concepts::IsPredicate<TSource> TPredicate>
        TSource FirstOrDefault(TPredicate&& predicate, const TSource& defaultValue) const 
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            for (const TSource& element : _collection)
                if (predicate(element))
                    return element;
            return defaultValue;
        }

        /// @brief Get last element of collection
        /// @return 
        TSource Last() const
        {
            if (_collection.empty())
                throw std::out_of_range("Collection is empty");
            return _collection[_collection.size() - 1];
        }

        /// @brief Get last element of collection
        /// @tparam TPredicate 
        /// @param predicate 
        /// @return 
        template<Concepts::IsPredicate<TSource> TPredicate>
        TSource Last(TPredicate&& predicate) const
        {
            for (const_reverse_iterator it = _collection.crbegin(); it != _collection.crend(); ++it)
                if (predicate(*it))
                    return *it;
            throw std::out_of_range("Element not found");
        }

        /// @brief Get last element of collection, return default if not found
        /// @return 
        TSource LastOrDefault() const noexcept
        requires std::is_default_constructible_v<TSource>
        {
            if (_collection.empty())
                return {};
            return _collection[_collection.size() - 1];
        }

        /// @brief Get last element of collection, return default if not found
        /// @param defaultValue 
        /// @return 
        TSource LastOrDefault(const TSource& defaultValue) const noexcept
        {
            if (_collection.empty())
                return defaultValue;
            return _collection[_collection.size() - 1];
        }

        /// @brief Get last element of collection, return default if not found
        /// @tparam TPredicate 
        /// @param predicate 
        /// @return 
        template<Concepts::IsPredicate<TSource> TPredicate>
        requires std::is_default_constructible_v<TSource>
        TSource LastOrDefault(TPredicate&& predicate) const 
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            for (const_reverse_iterator it = _collection.crbegin(); it != _collection.crend(); ++it)
                if (predicate(*it))
                    return *it;
            return {};
        }

        /// @brief Get last element of collection, return default if not found
        /// @tparam TPredicate 
        /// @param predicate 
        /// @param defaultValue 
        /// @return 
        template<Concepts::IsPredicate<TSource> TPredicate>
        TSource LastOrDefault(TPredicate&& predicate, const TSource& defaultValue) const 
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            for (const_reverse_iterator it = _collection.crbegin(); it != _collection.crend(); ++it)
                if (predicate(*it))
                    return *it;
            return defaultValue;
        }

        /// @brief 
        /// @param position 
        /// @return 
        TSource At(const std::size_t position) const
        {
            if (position >= _collection.size())
                throw std::out_of_range("Position out of range");
            return _collection[position];
        }

        /// @brief 
        /// @param position 
        /// @return 
        TSource AtOrDefault(const std::size_t position) const noexcept
        requires std::is_default_constructible_v<TSource>
        {
            if (position >= _collection.size())
                return {};
            return _collection[position];
        }

        /// @brief 
        /// @param position 
        /// @param defaultValue 
        /// @return 
        TSource AtOrDefault(const std::size_t position, const TSource& defaultValue) const noexcept
        {
            if (position >= _collection.size())
                return defaultValue;
            return _collection[position];
        }

        /// @brief Same as stl method empty()
        /// @return 
        [[nodiscard]]
        bool IsEmpty() const noexcept
        {
            return _collection.empty();
        }

        /// @brief Skips a certain number of elements
        /// @param count 
        /// @return 
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

        /// @brief Skips a certain number of elements from the end of the collection
        /// @param count 
        /// @return 
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

        /// @brief Skips a chain of elements, starting with the first element, as long as they satisfy a certain condition
        /// @tparam TPredicate 
        /// @param predicate 
        /// @return 
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

        /// @brief Retrieves a certain number of elements
        /// @param count 
        /// @return 
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

        /// @brief Retrieves a certain number of elements from the end of the collection
        /// @param count 
        /// @return 
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

        /// @brief Selects a chain of elements, starting with the first element, as long as they satisfy a certain condition
        /// @tparam TPredicate 
        /// @param predicate 
        /// @return 
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

        /// @brief Group data by certain parameters
        /// @tparam TKey 
        /// @tparam TKeySelector 
        /// @param keySelector 
        /// @return 
        template<std::invocable<TSource> TKeySelector,
                 typename TKey = std::invoke_result_t<TKeySelector, TSource>>
        std::map<TKey, std::vector<TSource>> GroupBy(TKeySelector&& keySelector) const 
        noexcept(std::is_nothrow_invocable_v<TKeySelector, TSource>)
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

        /// @brief Merge two different types of sets into one
        /// @tparam TResult 
        /// @tparam TOtherCollection 
        /// @tparam TInnerKeySelector 
        /// @tparam TOtherKeySelector 
        /// @tparam TResultSelector 
        /// @param otherCollection 
        /// @param innerKeySelector 
        /// @param otherKeySelector 
        /// @param resultSelector 
        /// @return 
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
                                    TResultSelector&& resultSelector) const 
        noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
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

        /// @brief Merge two different types of sets into one
        /// @tparam TResult 
        /// @tparam TOtherCollection 
        /// @tparam TInnerKeySelector 
        /// @tparam TOtherKeySelector 
        /// @tparam TResultSelector 
        /// @param otherCollection 
        /// @param innerKeySelector 
        /// @param otherKeySelector 
        /// @param resultSelector 
        /// @return 
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
                                    TResultSelector&& resultSelector) const 
        noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
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

        /// @brief In addition to joining sequences, it also performs grouping
        /// @tparam TResult 
        /// @tparam TOtherCollection 
        /// @tparam TInnerKeySelector 
        /// @tparam TKey 
        /// @tparam TOtherKeySelector 
        /// @tparam TResultSelector 
        /// @param otherCollection 
        /// @param innerKeySelector 
        /// @param otherKeySelector 
        /// @param resultSelector 
        /// @return 
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
                                         TResultSelector&& resultSelector) const 
        noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
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

        /// @brief In addition to joining sequences, it also performs grouping
        /// @tparam TResult 
        /// @tparam TOtherCollection 
        /// @tparam TInnerKeySelector 
        /// @tparam TKey 
        /// @tparam TOtherKeySelector 
        /// @tparam TResultSelector 
        /// @param otherCollection 
        /// @param innerKeySelector 
        /// @param otherKeySelector 
        /// @param resultSelector 
        /// @return 
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
                                         TResultSelector&& resultSelector) const 
        noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
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

        /// @brief Sequentially concatenates the corresponding elements of the current sequence with the second sequence
        /// @tparam TOtherCollection 
        /// @tparam TOtherCollectionValueType 
        /// @param otherCollection 
        /// @return 
        template<typename TOtherCollection,
                 typename TOtherCollectionValueType = TOtherCollection::value_type>
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

        /// @brief Sequentially concatenates the corresponding elements of the current sequence with the second sequence
        /// @tparam TOtherCollection 
        /// @tparam TOtherCollectionValueType 
        /// @param otherCollection 
        /// @return 
        template<typename TOtherCollection,
                 typename TOtherCollectionValueType = TOtherCollection::value_type>
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

        /// @brief Checks if all elements match a condition. If all elements match the condition, then true is returned
        /// @tparam TPredicate 
        /// @param predicate 
        /// @return 
        template<Concepts::IsPredicate<TSource> TPredicate>
        bool All(TPredicate&& predicate) const 
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            for (const TSource& element : _collection)
                if (!predicate(element))
                    return false;
            return true;
        }

        /// @brief Returns true if at least one element of the collection meets a certain condition
        /// @tparam TPredicate 
        /// @param predicate 
        /// @return 
        template<Concepts::IsPredicate<TSource> TPredicate>
        bool Any(TPredicate&& predicate) const 
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            for (const TSource& element : _collection)
                if (predicate(element))
                    return true;
            return false;
        }

        /// @brief Returns true if the collection contains a specific element
        /// @param target 
        /// @return 
        bool Contains(const TSource& target) const noexcept
        {
            for (const TSource& element : _collection)
                if (element == target)
                    return true;
            return false;
        }

        /// @brief Returns true if the collection contains a specific element
        /// @tparam TSelector 
        /// @param target 
        /// @param selector 
        /// @return 
        template<std::invocable<TSource> TSelector>
        requires Concepts::Equatable<std::invoke_result_t<TSelector, TSource>>
        bool Contains(const TSource& target, TSelector&& selector) const 
        noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
        {
            for (const TSource& element : _collection)
                if (selector(element) == selector(target))
                    return true;
            return false;
        }

        /// @brief Returns true if the collection contains a specific element or subcollection
        /// @param subCollection 
        /// @return 
        bool Contains(const std::vector<TSource>& subCollection) const noexcept
        requires Concepts::Equatable<TSource>
        {
            if (subCollection.empty())
                return true;
            return Algorithm::Contains(_collection.data(), _collection.size(), subCollection.data(), subCollection.size());
        }

        /// @brief 
        /// @param target 
        /// @return 
        std::size_t IndexAt(const TSource& target) const noexcept
        requires Concepts::Equatable<TSource>
        {
            for (std::size_t i = 0; i < _collection.size(); ++i)
                if (_collection[i] == target)
                    return i;
            return NPOS;
        }

        /// @brief 
        /// @tparam TSelector 
        /// @param target 
        /// @param selector 
        /// @return 
        template<std::invocable<TSource> TSelector>
        requires Concepts::Equatable<std::invoke_result_t<TSelector, TSource>>
        std::size_t IndexAt(const TSource& target, TSelector&& selector) const 
        noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
        {
            for (std::size_t i = 0; i < _collection.size(); ++i)
                if (selector(_collection[i]) == selector(target))
                    return i;
            return NPOS;
        }

        /// @brief 
        /// @param subCollection 
        /// @return 
        std::size_t IndexAt(const std::vector<TSource>& subCollection) const noexcept
        requires Concepts::Equatable<TSource>
        {
            return Algorithm::IndexAt(_collection.data(), _collection.size(), subCollection.data(), subCollection.size());
        }

        /// @brief 
        /// @param element 
        /// @return 
        std::size_t BinarySearch(const TSource& element) const noexcept
        requires Concepts::Comparable<TSource>
        {
            if (_collection.empty())
                return NPOS;
            return Algorithm::BinarySearch(std::forward<TSource>(element), _collection.data(), 0, _collection.size() - 1);
        }

        /// @brief 
        /// @tparam TSelector 
        /// @param element 
        /// @param selector 
        /// @return 
        template<std::invocable<TSource> TSelector>
        requires Concepts::Comparable<std::invoke_result_t<TSelector, TSource>>
        std::size_t BinarySearch(const TSource& element, TSelector&& selector) const 
        noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
        {
            if (_collection.empty())
                return NPOS;
            return Algorithm::BinarySearch(std::forward<TSource>(element), _collection.data(), 0, _collection.size() - 1, std::forward<TSelector>(selector));
        }

        /// @brief 
        /// @param element 
        /// @return 
        LinqContainer PushBack(const TSource& element) const noexcept
        {
            std::vector<TSource> newCollection;
            newCollection.reserve(_collection.size() + 1);

            std::copy(_collection.cbegin(), _collection.cend(),
                      std::inserter(newCollection, newCollection.begin()));

            newCollection.push_back(std::forward<TSource>(element));

            return LinqContainer(std::move(newCollection));
        }

        /// @brief 
        /// @param element 
        /// @return 
        LinqContainer PushBack(TSource&& element) const noexcept
        {
            std::vector<TSource> newCollection;
            newCollection.reserve(_collection.size() + 1);

            std::copy(_collection.cbegin(), _collection.cend(),
                      std::inserter(newCollection, newCollection.begin()));

            newCollection.push_back(std::forward<TSource>(element));

            return LinqContainer(std::move(newCollection));
        }

        /// @brief 
        /// @tparam TCollection 
        /// @param otherCollection 
        /// @return 
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

        /// @brief 
        /// @param element 
        /// @return 
        LinqContainer PushFront(const TSource& element) const noexcept
        {
            return Insert(std::forward<TSource>(element), 0);
        }

        /// @brief 
        /// @param element 
        /// @return 
        LinqContainer PushFront(TSource&& element) const noexcept
        {
            return Insert(std::forward<TSource>(element), 0);
        }

        /// @brief 
        /// @tparam TCollection 
        /// @param otherCollection 
        /// @return 
        template<typename TCollection>
        requires Concepts::ConstIterable<TCollection> && Concepts::HasSize<TCollection>
        LinqContainer PushFront(const TCollection& otherCollection) const noexcept
        {
            return Insert(otherCollection, 0);
        }

        /// @brief 
        /// @param element 
        /// @param position 
        /// @return 
        LinqContainer Insert(const TSource& element, const std::size_t position) const noexcept
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

        /// @brief 
        /// @param element 
        /// @param position 
        /// @return 
        LinqContainer Insert(TSource&& element, const std::size_t position) const noexcept
        {
            std::vector<std::decay_t<TSource>> newCollection;
            newCollection.reserve(_collection.size() + 1);

            std::copy(_collection.cbegin(), _collection.cbegin() + position,
                      std::inserter(newCollection, newCollection.begin()));

            newCollection.push_back(std::forward<TSource>(element));

            std::copy(_collection.cbegin() + position, _collection.cend(),
                      std::inserter(newCollection, newCollection.end()));

            return LinqContainer(std::move(newCollection));
        }

        /// @brief 
        /// @tparam TCollection 
        /// @param otherCollection 
        /// @param position 
        /// @return 
        template<typename TCollection>
        requires Concepts::ConstIterable<TCollection> && Concepts::HasSize<TCollection>
        LinqContainer Insert(const TCollection& otherCollection, const std::size_t position) const noexcept
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

        /// @brief 
        /// @param position 
        /// @return 
        LinqContainer Erase(const std::size_t position) const noexcept
        {
            return Erase(position, position);
        }

        /// @brief 
        /// @param begin 
        /// @param end 
        /// @return 
        LinqContainer Erase(const std::size_t begin, const std::size_t end) const noexcept
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