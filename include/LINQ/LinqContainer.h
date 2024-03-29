#ifndef LINQ_LinqContainer_H
#define LINQ_LinqContainer_H

#include <cstdint>
#include <array>
#include <list>
#include <forward_list>
#include <stack>
#include <queue>
#include <deque>
#include <set>
#include <unordered_set>
#include <unordered_map>

#include <LINQ/Algorithm.h>
#include <LINQ/Sort.h>

namespace LINQ
{
    template<typename TSource>
    class LinqContainer final
    {
    private:
        std::vector<TSource> _collection;
        TSource* _data;
        std::size_t _size;

    public:
        using value_type = TSource;
        using iterator = typename std::vector<TSource>::iterator;
        using const_iterator = typename std::vector<TSource>::const_iterator;
        using reverse_iterator = typename std::vector<TSource>::reverse_iterator;
        using const_reverse_iterator = typename std::vector<TSource>::const_reverse_iterator;

        explicit LinqContainer(const std::vector<TSource>& collection) noexcept
        {
            _collection = collection;
            _data = _collection.data();
            _size = _collection.size();
        }

        explicit LinqContainer(std::vector<TSource>&& collection) noexcept
        {
            _collection = collection;
            _data = _collection.data();
            _size = _collection.size();
        }

        LinqContainer(const LinqContainer& container) noexcept
        {
            _collection = container.ToVector();
            _data = _collection.data();
            _size = _collection.size();
        }

        LinqContainer(LinqContainer&& container) noexcept
        {
            _collection = container.GetCollection();
            _data = _collection.data();
            _size = _collection.size();
        }

        LinqContainer& operator=(const std::vector<TSource>& collection) noexcept
        {
            _collection = collection;
            _data = _collection.data();
            _size = _collection.size();
            return *this;
        }

        LinqContainer& operator=(std::vector<TSource>&& collection) noexcept
        {
            _collection = collection;
            _data = _collection.data();
            _size = _collection.size();
            return *this;
        }

        LinqContainer& operator=(const LinqContainer& container) noexcept
        {
            if (this == &container) return *this;
            _collection = container.ToVector();
            _data = _collection.data();
            _size = _collection.size();
            return *this;
        }

        LinqContainer& operator=(LinqContainer&& container) noexcept
        {
            _collection = container.GetCollection();
            _data = _collection.data();
            _size = _collection.size();
            return *this;
        }

        ~LinqContainer() noexcept = default;

        [[nodiscard]]
        std::size_t size() const noexcept
        {
            return _size;
        }

        [[nodiscard]]
        bool empty() const noexcept
        {
            return _size == 0;
        }

        iterator begin() noexcept
        {
            return _collection.begin();
        }

        iterator end() noexcept
        {
            return _collection.end();
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

        reverse_iterator rend() noexcept
        {
            return _collection.rend();
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
            return _data;
        }

        std::vector<TSource> GetCollection() noexcept
        {
            return std::move(_collection);
        }

        std::vector<TSource> ToVector() const noexcept
        {
            return std::vector<TSource>(_collection);
        }

        template<std::size_t SIZE>
        std::array<TSource, SIZE> ToArray() const noexcept
        {
            std::array<TSource, SIZE> array;
            for (std::size_t i = 0; i < SIZE && i < _size; ++i)
                array[i] = _data[i];
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
            for (std::size_t i = 0; i < _size; ++i)
                stack.push(_data[i]);
            return stack;
        }

        std::queue<TSource> ToQueue() const noexcept
        {
            std::queue<TSource> queue;
            for (std::size_t i = 0; i < _size; ++i)
                queue.push(_data[i]);
            return queue;
        }

        std::deque<TSource> ToDeque() const noexcept
        {
            return std::deque<TSource>(_collection.cbegin(), _collection.cend());
        }

        std::priority_queue<TSource> ToPriorityQueue() const noexcept
        {
            std::priority_queue<TSource> priorityQueue;
            for (std::size_t i = 0; i < _size; ++i)
                priorityQueue.push(_data[i]);
            return priorityQueue;
        }

        std::set<TSource> ToSet() const noexcept
        {
            std::set<TSource> newCollection;
            for (std::size_t i = 0; i < _size; ++i)
                newCollection.insert(_data[i]);
            return newCollection;
        }

        std::unordered_set<TSource> ToUnorderedSet() const noexcept
        {
            std::unordered_set<TSource> newCollection;
            for (std::size_t i = 0; i < _size; ++i)
                newCollection.insert(_data[i]);
            return newCollection;
        }

        template<typename TKey = typename PairTraits<TSource>::FirstType,
                 typename TValue = typename PairTraits<TSource>::SecondType>
        requires IsPair<TSource>
        std::map<TKey, TValue> ToMap() const noexcept
        {
            std::map<TKey, TValue> newCollection;
            for (std::size_t i = 0; i < _size; ++i)
                newCollection.insert(_data[i]);
            return newCollection;
        }

        template<typename TKey = typename PairTraits<TSource>::FirstType,
                 typename TValue = typename PairTraits<TSource>::SecondType>
        requires IsPair<TSource>
        std::unordered_map<TKey, TValue> ToUnorderedMap() const noexcept
        {
            std::unordered_map<TKey, TValue> newCollection;
            for (std::size_t i = 0; i < _size; ++i)
                newCollection.insert(_data[i]);
            return newCollection;
        }

        template<typename TSelector, typename TResult = typename FunctorTraits<TSelector(TSource)>::ReturnType>
        requires IsFunctor<TSelector, TSource>
        LinqContainer<TResult> Select(TSelector&& selector) const noexcept
        {
            std::vector<TResult> newCollection;
            newCollection.reserve(_size);

            for (std::size_t i = 0; i < _size; ++i)
                newCollection.push_back(selector(_data[i]));

            return LinqContainer<TResult>(std::move(newCollection));
        }

        template<typename TSelector, typename TResult = typename FunctorTraits<TSelector(TSource)>::ReturnType::value_type>
        requires IsFunctor<TSelector, TSource>
        LinqContainer<TResult> SelectMany(TSelector&& selector) const noexcept
        {
            std::vector<TResult> newCollection;

            for (std::size_t i = 0; i < _size; ++i)
            {
                std::vector<TResult> tempCollection = selector(_data[i]);
                for (std::size_t j = 0; j < tempCollection.size(); ++j)
                    newCollection.push_back(tempCollection[j]);
            }

            return LinqContainer<TResult>(std::move(newCollection));
        }

        template<typename TCollectionSelector,
                 Iterable TCollection = typename FunctorTraits<TCollectionSelector(TSource)>::ReturnType,
                 typename TCollectionValueType = typename TCollection::value_type,
                 typename TResultSelector,
                 typename TResult = typename FunctorTraits<TResultSelector(TSource, TCollectionValueType)>::ReturnType>
        requires IsFunctor<TCollectionSelector, TSource> && IsFunctor<TResultSelector, TSource, TCollectionValueType>
        LinqContainer<TResult> SelectMany(TCollectionSelector&& collectionSelector, TResultSelector&& resultSelector) const noexcept
        {
            std::vector<TResult> newCollection;

            for (std::size_t i = 0; i < _size; ++i)
            {
                TCollection tempCollection = collectionSelector(_data[i]);
                for (auto&& element : tempCollection)
                    newCollection.push_back(resultSelector(_data[i], std::move(element)));
            }

            return LinqContainer<TResult>(std::move(newCollection));
        }

        LinqContainer Where(std::function<bool(TSource)> predicate) const noexcept
        {
            std::vector<TSource> newCollection;

            for (std::size_t i = 0; i < _size; ++i)
                if (predicate(_data[i]))
                    newCollection.push_back(_data[i]);

            return LinqContainer(std::move(newCollection));
        }

        LinqContainer Order(OrderType orderType = OrderType::ASC) const noexcept requires Comparable<TSource>
        {
            std::vector<TSource> newCollection(_collection.cbegin(), _collection.cend());
            Sort::CombSort(newCollection.data(), 0, _size - 1, orderType);
            return LinqContainer(std::move(newCollection));
        }

        template<typename TSelector>
        requires IsFunctor<TSelector, TSource> && Comparable<typename FunctorTraits<TSelector(TSource)>::ReturnType>
        LinqContainer OrderBy(TSelector&& selector, OrderType orderType = OrderType::ASC) const noexcept
        {
            std::vector<TSource> newCollection(_collection.cbegin(), _collection.cend());
            Sort::CombSort(newCollection.data(), 0, _size - 1, std::forward<TSelector>(selector), orderType);
            return LinqContainer(std::move(newCollection));
        }

        LinqContainer Reverse() const noexcept
        {
            std::vector<TSource> newCollection(_collection.crbegin(), _collection.crend());
            return LinqContainer(std::move(newCollection));
        }

        template<typename TOtherCollection>
        requires ConstIterable<TOtherCollection> && HasSize<TOtherCollection> && Equalable<TSource>
        LinqContainer Except(const TOtherCollection& otherCollection) const noexcept
        {
            std::set<TSource> newCollection;

            for (std::size_t i = 0; i < _size; ++i)
            {
                std::size_t j = 0;
                for (const auto& element : otherCollection)
                {
                    if (_data[i] == element) break;
                    if (j == otherCollection.size() - 1) newCollection.insert(_data[i]);
                    ++j;
                }
            }

            std::vector<TSource> assignCollection;
            assignCollection.assign(newCollection.cbegin(), newCollection.cend());
            return LinqContainer(std::move(assignCollection));
        }

        template<ConstIterable TOtherCollection>
        requires Equalable<TSource>
        LinqContainer Intersect(const TOtherCollection& otherCollection) const noexcept
        {
            std::set<TSource> newCollection;

            for (std::size_t i = 0; i < _size; ++i)
                for (const auto& element : otherCollection)
                    if (_data[i] == element)
                    {
                        newCollection.insert(_data[i]);
                        break;
                    }

            std::vector<TSource> assignCollection;
            assignCollection.assign(newCollection.cbegin(), newCollection.cend());
            return LinqContainer(std::move(assignCollection));
        }

        LinqContainer Distinct() const noexcept requires Equalable<TSource>
        {
            std::set<TSource> newCollection;
            for (std::size_t i = 0; i < _size; ++i)
                newCollection.insert(_data[i]);

            std::vector<TSource> assignCollection;
            assignCollection.assign(newCollection.cbegin(), newCollection.cend());
            return LinqContainer(std::move(assignCollection));
        }

        template<ConstIterable TOtherCollection>
        requires Equalable<TSource>
        LinqContainer Union(const TOtherCollection& otherCollection) const noexcept
        {
            std::set<TSource> newCollection;

            for (std::size_t i = 0; i < _size; ++i)
                newCollection.insert(_data[i]);

            for (const auto& element : otherCollection)
                newCollection.insert(element);

            std::vector<TSource> assignCollection;
            assignCollection.assign(newCollection.cbegin(), newCollection.cend());
            return LinqContainer(std::move(assignCollection));
        }

        template<typename TResult>
        TResult Aggregate(std::function<TResult(TResult, TSource)> aggregateFunction) const
        {
            if (_size == 0)
                throw std::out_of_range("Collection is empty");
            return Aggregate::Aggregate(_data, 0, _size - 1, aggregateFunction);
        }

        [[nodiscard]]
        std::size_t Count() const noexcept
        {
            return _size;
        }

        std::size_t Count(std::function<bool(TSource)> predicate) const noexcept
        {
            return Aggregate::Count(_data, 0, _size - 1, predicate);
        }

        TSource Sum() const requires Summarizable<TSource>
        {
            if (_size == 0)
                throw std::out_of_range("Collection is empty");
            return Aggregate::Sum(_data, 0, _size - 1);
        }

        template<typename TSelector, Summarizable TResult = typename FunctorTraits<TSelector(TSource)>::ReturnType>
        requires IsFunctor<TSelector, TSource>
        TResult Sum(TSelector&& selector) const
        {
            if (_size == 0)
                throw std::out_of_range("Collection is empty");
            return Aggregate::Sum(_data, 0, _size - 1, std::forward<TSelector>(selector));
        }

        TSource Min() const requires Comparable<TSource>
        {
            if (_size == 0)
                throw std::out_of_range("Collection is empty");
            return Aggregate::Min(_data, 0, _size - 1);
        }

        template<typename TSelector, Comparable TResult = typename FunctorTraits<TSelector(TSource)>::ReturnType>
        requires IsFunctor<TSelector, TSource>
        TResult Min(TSelector&& selector) const
        {
            if (_size == 0)
                throw std::out_of_range("Collection is empty");

            return Aggregate::Min(_data, 0, _size - 1, std::forward<TSelector>(selector));
        }

        TSource Max() const requires Comparable<TSource>
        {
            if (_size == 0)
                throw std::out_of_range("Collection is empty");
            return Aggregate::Max(_data, 0, _size - 1);
        }

        template<typename TSelector, Comparable TResult = typename FunctorTraits<TSelector(TSource)>::ReturnType>
        requires IsFunctor<TSelector, TSource>
        TResult Max(TSelector&& selector) const
        {
            if (_size == 0)
                throw std::out_of_range("Collection is empty");
            return Aggregate::Max(_data, 0, _size - 1, std::forward<TSelector>(selector));
        }

        TSource Average() const requires Divisible<TSource>
        {
            if (_size == 0)
                throw std::out_of_range("Collection is empty");
            return Aggregate::Average(_data, 0, _size - 1);
        }

        template<typename TSelector, Divisible TResult = typename FunctorTraits<TSelector(TSource)>::ReturnType>
        requires IsFunctor<TSelector, TSource>
        TResult Average(TSelector&& selector) const
        {
            if (_size == 0)
                throw std::out_of_range("Collection is empty");
            return Aggregate::Average(_data, 0, _size - 1, std::forward<TSelector>(selector));
        }

        TSource First() const
        {
            if (_size == 0)
                throw std::out_of_range("Element not found");
            return _data[0];
        }

        TSource First(std::function<bool(TSource)> predicate) const
        {
            for (std::size_t i = 0; i < _size; ++i)
                if (predicate(_data[i])) return _data[i];
            throw std::out_of_range("Element not found");
        }

        TSource FirstOrDefault(const TSource& defaultValue = TSource()) const noexcept
        {
            if (_size == 0) return defaultValue;
            return _data[0];
        }

        TSource FirstOrDefault(std::function<bool(TSource)> predicate, const TSource& defaultValue = TSource()) const noexcept
        {
            for (std::size_t i = 0; i < _size; ++i)
                if (predicate(_data[i])) return _data[i];
            return defaultValue;
        }

        TSource Last() const
        {
            if (_size == 0)
                throw std::out_of_range("Element not found");
            return _data[_size - 1];
        }

        TSource Last(std::function<bool(TSource)> predicate) const
        {
            for (std::int64_t i = _size - 1; i >= 0; --i)
                if (predicate(_data[i])) return _data[i];
            throw std::out_of_range("Element not found");
        }

        TSource LastOrDefault(const TSource& defaultValue = TSource()) const noexcept
        {
            if (_size == 0) return defaultValue;
            return _data[_size - 1];
        }

        TSource LastOrDefault(std::function<bool(TSource)> predicate, const TSource& defaultValue = TSource()) const noexcept
        {
            for (std::int64_t i = _size - 1; i >= 0; --i)
                if (predicate(_data[i])) return _data[i];
            return defaultValue;
        }

        TSource At(std::size_t position) const noexcept
        {
            return _data[position];
        }

        [[nodiscard]]
        bool IsEmpty() const noexcept
        {
            return _size == 0;
        }

        LinqContainer Skip(const std::size_t count) const
        {
            if (count >= _size)
                throw std::overflow_error("Skip count must be less then collection size");

            std::vector<TSource> newCollection;
            newCollection.reserve(_size - count);

            for (std::size_t i = count; i < _size; ++i)
                newCollection.push_back(_data[i]);

            return LinqContainer(std::move(newCollection));
        }

        LinqContainer SkipLast(const std::size_t count) const
        {
            if (count >= _size)
                throw std::overflow_error("SkipLast count must be less then collection size");

            std::vector<TSource> newCollection;
            newCollection.reserve(_size - count);

            for (std::size_t i = 0; i < _size - count; ++i)
                newCollection.push_back(_data[i]);

            return LinqContainer(std::move(newCollection));
        }

        LinqContainer SkipWhile(std::function<bool(TSource)> predicate) const noexcept
        {
            std::vector<TSource> newCollection;

            std::size_t i = 0;
            for (; i < _size; ++i)
                if(!predicate(_data[i])) break;

            for (; i < _size; ++i)
                newCollection.push_back(_data[i]);

            return LinqContainer(std::move(newCollection));
        }

        LinqContainer Take(std::size_t count) const
        {
            if (count >= _size)
                throw std::overflow_error("Take count must be less then collection size");

            std::vector<TSource> newCollection;
            newCollection.reserve(count);

            for (std::size_t i = 0; i < count; ++i)
                newCollection.push_back(_data[i]);

            return LinqContainer(std::move(newCollection));
        }

        LinqContainer TakeLast(std::size_t count) const
        {
            if (count >= _size)
                throw std::overflow_error("TakeLast count must be less then collection size");

            std::vector<TSource> newCollection;
            newCollection.reserve(count);

            for (std::size_t i = _size - count; i < _size; ++i)
                newCollection.push_back(_data[i]);

            return LinqContainer(std::move(newCollection));
        }

        LinqContainer TakeWhile(std::function<bool(TSource)> predicate) const noexcept
        {
            std::vector<TSource> newCollection;

            for (std::size_t i = 0; i < _size; ++i)
            {
                if (!predicate(_data[i])) break;
                newCollection.push_back(_data[i]);
            }

            return LinqContainer(std::move(newCollection));
        }

        template<typename TKeySelector, typename TKey = typename FunctorTraits<TKeySelector(TSource)>::ReturnType>
        requires IsFunctor<TKeySelector, TSource>
        std::map<TKey, std::vector<TSource>> GroupBy(TKeySelector&& keySelector) const noexcept
        {
            std::map<TKey, std::vector<TSource>> result;

            for (std::size_t i = 0; i < _size; ++i)
            {
                if (!result.contains(keySelector(_data[i])))
                    result.insert(std::pair<TKey, std::vector<TSource>>(keySelector(_data[i]), std::vector<TSource>()));
                result.at(keySelector(_data[i])).push_back(_data[i]);
            }

            return result;
        }

        template<Iterable TOtherCollection,
                 typename TInnerKeySelector,
                 typename TOtherKeySelector,
                 typename TResultSelector,
                 typename TResult = typename FunctorTraits<TResultSelector(TSource, typename TOtherCollection::value_type)>::ReturnType>
        requires IsFunctor<TInnerKeySelector, TSource> &&
                 IsFunctor<TOtherKeySelector, typename TOtherCollection::value_type> &&
                 IsFunctor<TResultSelector, TSource, typename TOtherCollection::value_type> &&
                 std::same_as<typename FunctorTraits<TInnerKeySelector(TSource)>::ReturnType, typename FunctorTraits<TOtherKeySelector(typename TOtherCollection::value_type)>::ReturnType> &&
                 Equalable<typename FunctorTraits<TInnerKeySelector(TSource)>::ReturnType>
        LinqContainer<TResult> Join(const TOtherCollection& otherCollection,
                                    TInnerKeySelector&& innerKeySelector,
                                    TOtherKeySelector&& otherKeySelector,
                                    TResultSelector&& resultSelector) const
        {
            if (otherCollection.empty())
                throw std::invalid_argument("other collection must not be empty");

            std::vector<TResult> newCollection;

            for (std::size_t i = 0; i < _size; ++i)
                for (const auto& element : otherCollection)
                    if (innerKeySelector(_data[i]) == otherKeySelector(element))
                        newCollection.push_back(resultSelector(_data[i], element));

            return LinqContainer<TResult>(std::move(newCollection));
        }

        template<Iterable TOtherCollection,
                 typename TInnerKeySelector,
                 Equalable TKey = typename FunctorTraits<TInnerKeySelector(TSource)>::ReturnType,
                 typename TOtherKeySelector,
                 typename TResultSelector,
                 typename TResult = typename FunctorTraits<TResultSelector(const std::vector<TSource>&, typename TOtherCollection::value_type)>::ReturnType>
        requires IsFunctor<TInnerKeySelector, TSource> &&
                 IsFunctor<TOtherKeySelector, typename TOtherCollection::value_type> &&
                 IsFunctor<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type> &&
                 std::same_as<TKey, typename FunctorTraits<TOtherKeySelector(typename TOtherCollection::value_type)>::ReturnType>
        LinqContainer<TResult> GroupJoin(const TOtherCollection& otherCollection,
                                         TInnerKeySelector&& innerKeySelector,
                                         TOtherKeySelector&& otherKeySelector,
                                         TResultSelector&& resultSelector) const
        {
            if (otherCollection.empty())
                throw std::invalid_argument("other collection must not be empty");

            std::vector<TResult> newCollection;

            std::map<TKey, std::vector<TSource>> groups = GroupBy(innerKeySelector);

            for (const auto& [key, group] : groups)
                for (const auto& element : otherCollection)
                    if (key == otherKeySelector(element))
                        newCollection.push_back(resultSelector(group, element));

            return LinqContainer<TResult>(std::move(newCollection));
        }

        template<typename TOtherCollection,
                 typename TOtherCollectionValueType = typename TOtherCollection::value_type>
        requires Iterable<TOtherCollection> && HasSize<TOtherCollection>
        LinqContainer<std::pair<TSource, TOtherCollectionValueType>> Zip(const TOtherCollection& otherCollection) const
        {
            if (otherCollection.empty())
                throw std::invalid_argument("other collection must not be empty");
            if (otherCollection.size() != _size)
                throw std::invalid_argument("the size of inner collection is not equal to the size of other collection");

            std::vector<std::pair<TSource, TOtherCollectionValueType>> newCollection;

            std::size_t i = 0;
            for (const auto& element : otherCollection)
            {
                newCollection.push_back(std::pair<TSource, TOtherCollectionValueType>(_data[i], element));
                ++i;
            }

            return LinqContainer<std::pair<TSource, TOtherCollectionValueType>>(std::move(newCollection));
        }

        bool All(std::function<bool(TSource)> predicate) const noexcept
        {
            for (std::size_t i = 0; i < _size; ++i)
                if (!predicate(_data[i])) return false;
            return true;
        }

        bool Any(std::function<bool(TSource)> predicate) const noexcept
        {
            for (std::size_t i = 0; i < _size; ++i)
                if (predicate(_data[i])) return true;
            return false;
        }

        bool Contains(const TSource& element) const noexcept
        {
            for (std::size_t i = 0; i < _size; ++i)
                if (_data[i] == element) return true;
            return false;
        }

        template<typename TSelector>
        requires IsFunctor<TSelector, TSource> && Equalable<typename FunctorTraits<TSelector(TSource)>::ReturnType>
        bool Contains(const TSource& element, TSelector&& selector) const noexcept
        {
            for (std::size_t i = 0; i < _size; ++i)
                if (selector(_data[i]) == selector(element)) return true;
            return false;
        }

        bool Contains(const std::vector<TSource>& subCollection) const noexcept requires Equalable<TSource>
        {
            if (subCollection.empty()) return true;
            return Algorithm::Contains(_data, _size, subCollection.data(), subCollection.size());
        }

        std::size_t IndexAt(const TSource& element) const noexcept requires Equalable<TSource>
        {
            for (int i = 0; i < _size; ++i)
                if (_data[i] == element) return i;
            return NPOS;
        }

        template<typename TSelector>
        requires IsFunctor<TSelector, TSource> && Equalable<typename FunctorTraits<TSelector(TSource)>::ReturnType>
        std::size_t IndexAt(const TSource& element, TSelector&& selector) const noexcept
        {
            for (int i = 0; i < _size; ++i)
                if (selector(_data[i]) == selector(element)) return i;
            return NPOS;
        }

        std::size_t IndexAt(const std::vector<TSource>& subCollection) const noexcept requires Equalable<TSource>
        {
            return Algorithm::IndexAt(_data, _size, subCollection.data(), subCollection.size());
        }

        std::size_t BinarySearch(const TSource& element) const noexcept requires Comparable<TSource>
        {
            if (_size == 0) return NPOS;
            return Algorithm::BinarySearch(std::forward<TSource>(element), _data, 0, _size - 1);
        }

        template<typename TSelector>
        requires IsFunctor<TSelector, TSource> && Comparable<typename FunctorTraits<TSelector(TSource)>::ReturnType>
        std::size_t BinarySearch(const TSource& element, TSelector&& selector) const noexcept
        {
            if (_size == 0) return NPOS;
            return Algorithm::BinarySearch(std::forward<TSource>(element), _data, 0, _size - 1, std::forward<TSelector>(selector));
        }

        LinqContainer PushBack(const TSource& element) const noexcept
        {
            std::vector<TSource> newCollection;
            newCollection.reserve(_size + 1);

            std::copy(_collection.cbegin(), _collection.cend(),
                      std::inserter(newCollection, newCollection.begin()));

            newCollection.push_back(std::forward<TSource>(element));

            return LinqContainer(std::move(newCollection));
        }

        LinqContainer PushBack(TSource&& element) const noexcept
        {
            std::vector<TSource> newCollection;
            newCollection.reserve(_size + 1);

            std::copy(_collection.cbegin(), _collection.cend(),
                      std::inserter(newCollection, newCollection.begin()));

            newCollection.push_back(std::forward<TSource>(element));

            return LinqContainer(std::move(newCollection));
        }

        template<typename TCollection>
        requires ConstIterable<TCollection> && HasSize<TCollection>
        LinqContainer PushBack(const TCollection& otherCollection) const noexcept
        {
            std::vector<TSource> newCollection;
            newCollection.reserve(_size + otherCollection.size());

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
        requires ConstIterable<TCollection> && HasSize<TCollection>
        LinqContainer PushFront(const TCollection& otherCollection) const noexcept
        {
            return Insert(otherCollection, 0);
        }

        LinqContainer Insert(const TSource& element, std::size_t position) const noexcept
        {
            std::vector<TSource> newCollection;
            newCollection.reserve(_size + 1);

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
            newCollection.reserve(_size + 1);

            std::copy(_collection.cbegin(), _collection.cbegin() + position,
                      std::inserter(newCollection, newCollection.begin()));

            newCollection.push_back(std::forward<TSource>(element));

            std::copy(_collection.cbegin() + position, _collection.cend(),
                      std::inserter(newCollection, newCollection.end()));

            return LinqContainer(std::move(newCollection));
        }

        template<typename TCollection>
        requires ConstIterable<TCollection> && HasSize<TCollection>
        LinqContainer Insert(const TCollection& otherCollection, std::size_t position) const noexcept
        {
            std::vector<TSource> newCollection;
            newCollection.reserve(_size + otherCollection.size());

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

        LinqContainer Erase(std::size_t begin, std::size_t end) const
        {
            if (_size <= end - begin)
                throw std::invalid_argument("you try erase more element than collection has");

            std::vector<TSource> newCollection;
            newCollection.reserve(_size - (end - begin));

            std::copy(_collection.cbegin(), _collection.cbegin() + begin,
                      std::inserter(newCollection, newCollection.begin()));

            std::copy(_collection.cbegin() + end + 1, _collection.cend(),
                      std::inserter(newCollection, newCollection.end()));

            return LinqContainer(std::move(newCollection));
        }
    };
}

#endif