#ifndef LINQ_LinqGenerator_H
#define LINQ_LinqGenerator_H

#include <LINQ/Future.h>
#include <LINQ/YieldForeach.h>

namespace LINQ
{
    template<typename TSource>
    class LinqGenerator final
    {
        class Iterator
        {
        public:
            Iterator(Future<TSource>& yieldContext, bool isEnd) noexcept : _yieldContext(yieldContext), _isEnd(isEnd)
            {
                if (_yieldContext) _value = _yieldContext.Value();
                else _isEnd = true;
            }

            TSource& operator*() noexcept
            {
                return _value;
            }

            TSource operator*() const noexcept
            {
                return _value;
            }

            Iterator& operator++() noexcept
            {
                if (_yieldContext) _value = _yieldContext.Next();
                else _isEnd = true;
                return *this;
            }

            bool operator!=(const Iterator& other) const noexcept
            {
                return _isEnd != other._isEnd;
            }

        private:
            Future<TSource>& _yieldContext;
            TSource _value;
            bool _isEnd;
        };

    private:
        Future<TSource> _yieldContext;

    public:
        using value_type = TSource;
        using promise_type = typename Future<TSource>::promise_type;
        using handle_type = typename Future<TSource>::handle_type;

        template<typename TGenerator, typename... Args>
        requires IsFunctor<TGenerator, Args...>
        explicit LinqGenerator(TGenerator&& generator, Args... args) noexcept :
            _yieldContext(generator(std::forward<Args>(args)...)) {}

        template<ConstIterable TCollection>
        explicit LinqGenerator(const TCollection& collection) noexcept :
            _yieldContext(YieldForeach(collection)) {}

        template<Iterable TCollection>
        explicit LinqGenerator(TCollection&& collection) noexcept :
            _yieldContext(YieldForeach(std::forward<TCollection>(collection))) {}

        template<typename TConstIterator>
        requires std::is_const<TConstIterator>::value_type
        explicit LinqGenerator(TConstIterator&& begin, TConstIterator&& end) noexcept :
            _yieldContext(YieldForeach(begin, end)) {}

        template<typename TIterator>
        requires (!std::is_const<TIterator>::value_type)
        explicit LinqGenerator(TIterator&& begin, TIterator&& end) noexcept :
            _yieldContext(YieldForeach(std::forward<TIterator>(begin), std::forward<TIterator>(end))) {}

        ~LinqGenerator() noexcept = default;

        explicit operator bool() noexcept
        {
            return static_cast<bool>(_yieldContext);
        }

        TSource Next() noexcept
        {
            return _yieldContext.Next();
        }

        Iterator begin() noexcept
        {
            return Iterator(_yieldContext, false);
        }

        Iterator end() noexcept
        {
            return Iterator(_yieldContext, true);
        }

        std::vector<TSource> ToVector() noexcept
        {
            std::vector<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());
            return std::move(collection);
        }

        template<std::size_t SIZE>
        std::array<TSource, SIZE> ToArray() noexcept
        {
            std::array<TSource, SIZE> collection;
            for (std::size_t i = 0; i < SIZE && _yieldContext; ++i)
                collection[i] = _yieldContext.Next();
            return std::move(collection);
        }

        std::list<TSource> ToList() noexcept
        {
            std::list<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());
            return std::move(collection);
        }

        std::forward_list<TSource> ToForwardList() noexcept
        {
            std::forward_list<TSource> collection;
            while (_yieldContext)
                collection.push_front(_yieldContext.Next());
            return std::move(collection);
        }

        std::stack<TSource> ToStack() noexcept
        {
            std::stack<TSource> collection;
            while (_yieldContext)
                collection.push(_yieldContext.Next());
            return collection;
        }

        std::queue<TSource> ToQueue() noexcept
        {
            std::queue<TSource> collection;
            while (_yieldContext)
                collection.push(_yieldContext.Next());
            return collection;
        }

        std::deque<TSource> ToDeque() noexcept
        {
            std::deque<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());
            return collection;
        }

        std::priority_queue<TSource> ToPriorityQueue() noexcept
        {
            std::priority_queue<TSource> collection;
            while (_yieldContext)
                collection.push(_yieldContext.Next());
            return collection;
        }

        std::set<TSource> ToSet() noexcept
        {
            std::set<TSource> collection;
            while (_yieldContext)
                collection.insert(_yieldContext.Next());
            return collection;
        }

        std::unordered_set<TSource> ToUnorderedSet() noexcept
        {
            std::unordered_set<TSource> collection;
            while (_yieldContext)
                collection.insert(_yieldContext.Next());
            return collection;
        }

        template<typename TKey = typename PairTraits<TSource>::FirstType,
                 typename TValue = typename PairTraits<TSource>::SecondType>
        requires IsPair<TSource>
        std::map<TKey, TValue> ToMap() noexcept
        {
            std::map<TKey, TValue> collection;
            while (_yieldContext)
                collection.insert(_yieldContext.Next());
            return collection;
        }

        template<typename TKey = typename PairTraits<TSource>::FirstType,
                 typename TValue = typename PairTraits<TSource>::SecondType>
        requires IsPair<TSource>
        std::unordered_map<TKey, TValue> ToUnorderedMap() noexcept
        {
            std::unordered_map<TKey, TValue> collection;
            while (_yieldContext)
                collection.insert(_yieldContext.Next());
            return collection;
        }

        template<typename TSelector, typename TResult = typename FunctorTraits<TSelector(TSource)>::ReturnType>
        requires IsFunctor<TSelector, TSource>
        LinqGenerator<TResult> Select(TSelector&& selector) noexcept
        {
            return LinqGenerator<TResult>([this](TSelector&& selector_)
                { return SelectGenerator(std::forward<TSelector>(selector_)); }, std::forward<TSelector>(selector));
        }

        template<typename TSelector, typename TResult = typename FunctorTraits<TSelector(TSource)>::ReturnType::value_type>
        requires IsFunctor<TSelector, TSource>
        LinqGenerator<TResult> SelectMany(TSelector&& selector) noexcept
        {
            return LinqGenerator<TResult>([this](TSelector&& selector_)
                { return SelectManyGenerator(std::forward<TSelector>(selector_)); }, std::forward<TSelector>(selector));
        }

        template<typename TCollectionSelector,
                Iterable TCollection = typename FunctorTraits<TCollectionSelector(TSource)>::ReturnType,
                typename TCollectionValueType = typename TCollection::value_type,
                typename TResultSelector,
                typename TResult = typename FunctorTraits<TResultSelector(TSource, TCollectionValueType)>::ReturnType>
        requires IsFunctor<TCollectionSelector, TSource> && IsFunctor<TResultSelector, TSource, TCollectionValueType>
        LinqGenerator<TResult> SelectMany(TCollectionSelector&& collectionSelector, TResultSelector&& resultSelector) noexcept
        {
            return LinqGenerator<TResult>([this](TCollectionSelector&& collectionSelector_, TResultSelector&& resultSelector_)
                { return SelectManyGenerator(std::forward<TCollectionSelector>(collectionSelector_), std::forward<TResultSelector>(resultSelector_)); },
                std::forward<TCollectionSelector>(collectionSelector), std::forward<TResultSelector>(resultSelector));
        }

        LinqGenerator Where(std::function<bool(TSource)> predicate) noexcept
        {
            return LinqGenerator([this](std::function<bool(TSource)> predicate_)
                { return WhereGenerator(std::move(predicate_)); }, std::move(predicate));
        }

        LinqGenerator Order(OrderType orderType = OrderType::ASC) noexcept
        requires Comparable<TSource>
        {
            std::vector<TSource> newCollection;
            while (_yieldContext)
                newCollection.push_back(_yieldContext.Next());
            if (newCollection.empty()) return *this;

            Sort::QuickSort(newCollection.data(), 0, newCollection.size() - 1, orderType);
            return LinqGenerator(std::move(newCollection));
        }

        template<typename TSelector>
        requires IsFunctor<TSelector, TSource> && Comparable<typename FunctorTraits<TSelector(TSource)>::ReturnType>
        LinqGenerator OrderBy(TSelector&& selector, OrderType orderType = OrderType::ASC) noexcept
        {
            std::vector<TSource> newCollection;
            while (_yieldContext)
                newCollection.push_back(_yieldContext.Next());
            if (newCollection.empty()) return *this;

            Sort::QuickSort(newCollection.data(), 0, newCollection.size() - 1, std::forward<TSelector>(selector), orderType);
            return LinqGenerator(std::move(newCollection));
        }

        LinqGenerator Reverse() noexcept
        {
            std::vector<TSource> newCollection;
            while (_yieldContext)
                newCollection.push_back(_yieldContext.Next());
            return LinqGenerator(std::move(std::vector<TSource>(newCollection.crbegin(), newCollection.crend())));
        }

        template<typename TOtherCollection>
        requires ConstIterable<TOtherCollection> && HasSize<TOtherCollection> && Equalable<TSource>
        LinqGenerator Except(const TOtherCollection& otherCollection) noexcept
        {
            std::set<TSource> newCollection;

            while (_yieldContext)
            {
                auto element = _yieldContext.Next();

                std::size_t i = 0;
                for (const auto& otherElement : otherCollection)
                {
                    if (element == otherElement) break;
                    if (i == otherCollection.size() - 1) newCollection.insert(element);
                    ++i;
                }
            }

            return LinqGenerator(std::move(newCollection));
        }

        template<typename TOtherCollection>
        requires Iterable<TOtherCollection> && HasSize<TOtherCollection> && Equalable<TSource>
        LinqGenerator Except(TOtherCollection&& otherCollection) noexcept
        {
            std::set<TSource> newCollection;

            while (_yieldContext)
            {
                auto element = _yieldContext.Next();

                std::size_t i = 0;
                for (auto& otherElement : otherCollection)
                {
                    if (element == std::move(otherElement)) break;
                    if (i == otherCollection.size() - 1) newCollection.insert(element);
                    ++i;
                }
            }

            return LinqGenerator(std::move(newCollection));
        }

        template<ConstIterable TOtherCollection>
        requires Equalable<TSource>
        LinqGenerator Intersect(const TOtherCollection& otherCollection) noexcept
        {
            std::set<TSource> newCollection;

            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                for (const auto& otherElement : otherCollection)
                    if (element == otherElement)
                    {
                        newCollection.insert(element);
                        break;
                    }
            }

            return LinqGenerator(std::move(newCollection));
        }

        template<Iterable TOtherCollection>
        requires Equalable<TSource>
        LinqGenerator Intersect(TOtherCollection&& otherCollection) noexcept
        {
            std::set<TSource> newCollection;

            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                for (auto& otherElement : otherCollection)
                    if (element == otherElement)
                    {
                        newCollection.insert(std::move(otherElement));
                        break;
                    }
            }

            return LinqGenerator(std::move(newCollection));
        }

        LinqGenerator Distinct() noexcept
        requires Equalable<TSource>
        {
            std::set<TSource> newCollection;
            while (_yieldContext)
                newCollection.insert(_yieldContext.Next());

            return LinqGenerator(std::move(newCollection));
        }

        template<ConstIterable TOtherCollection>
        requires Equalable<TSource>
        LinqGenerator Union(const TOtherCollection& otherCollection) noexcept
        {
            std::set<TSource> newCollection;

            while (_yieldContext)
                newCollection.insert(_yieldContext.Next());

            for (const auto& element : otherCollection)
                newCollection.insert(element);

            std::vector<TSource> assignCollection;
            assignCollection.assign(newCollection.cbegin(), newCollection.cend());
            return LinqGenerator(std::move(assignCollection));
        }

        template<Iterable TOtherCollection>
        requires Equalable<TSource>
        LinqGenerator Union(TOtherCollection&& otherCollection) noexcept
        {
            std::set<TSource> newCollection;

            while (_yieldContext)
                newCollection.insert(_yieldContext.Next());

            for (auto& element : otherCollection)
                newCollection.insert(std::move(element));

            std::vector<TSource> assignCollection;
            assignCollection.assign(newCollection.cbegin(), newCollection.cend());
            return LinqGenerator(std::move(assignCollection));
        }

        template<typename TResult>
        TResult Aggregate(std::function<TResult(TResult, TSource)> aggregateFunction)
        {
            std::vector<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Aggregate(collection.data(), 0, collection.size() - 1, std::move(aggregateFunction));
        }

        std::size_t Count(std::function<bool(TSource)> predicate) noexcept
        {
            std::size_t result = 0;
            while (_yieldContext)
                if (predicate(_yieldContext.Next())) ++result;

            return result;
        }

        TSource Sum()
        requires Summarizable<TSource>
        {
            std::vector<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Sum(collection.data(), 0, collection.size() - 1);
        }

        template<typename TSelector, Summarizable TResult = typename FunctorTraits<TSelector(TSource)>::ReturnType>
        requires IsFunctor<TSelector, TSource>
        TResult Sum(TSelector&& selector)
        {
            std::vector<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Sum(collection.data(), 0, collection.size() - 1, std::forward<TSelector>(selector));
        }

        TSource Min()
        requires Comparable<TSource>
        {
            std::vector<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Min(collection.data(), 0, collection.size() - 1);
        }

        template<typename TSelector, Comparable TResult = typename FunctorTraits<TSelector(TSource)>::ReturnType>
        requires IsFunctor<TSelector, TSource>
        TResult Min(TSelector&& selector)
        {
            std::vector<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Min(collection.data(), 0, collection.size() - 1, std::forward<TSelector>(selector));
        }

        TSource Max()
        requires Comparable<TSource>
        {
            std::vector<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Max(collection.data(), 0, collection.size() - 1);
        }

        template<typename TSelector, Comparable TResult = typename FunctorTraits<TSelector(TSource)>::ReturnType>
        requires IsFunctor<TSelector, TSource>
        TResult Max(TSelector&& selector)
        {
            std::vector<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Max(collection.data(), 0, collection.size() - 1, std::forward<TSelector>(selector));
        }

        TSource Average()
        requires Divisible<TSource>
        {
            std::vector<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Average(collection.data(), 0, collection.size() - 1);
        }

        template<typename TSelector, Divisible TResult = typename FunctorTraits<TSelector(TSource)>::ReturnType>
        requires IsFunctor<TSelector, TSource>
        TResult Average(TSelector&& selector)
        {
            std::vector<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Average(collection.data(), 0, collection.size() - 1, std::forward<TSelector>(selector));
        }

        LinqGenerator Skip(const std::size_t count) noexcept
        {
            return LinqGenerator([this](const std::size_t count_)
                { return SkipGenerator(count_); }, count);
        }

        LinqGenerator SkipWhile(std::function<bool(TSource)> predicate) noexcept
        {
            return LinqGenerator([this](std::function<bool(TSource)> predicate_)
                { return SkipWhileGenerator(predicate_); }, std::move(predicate));
        }

        LinqGenerator Take(const std::size_t count)
        {
            return LinqGenerator([this](const std::size_t count_)
                { return TakeGenerator(count_); }, count);
        }

        LinqGenerator TakeWhile(std::function<bool(TSource)> predicate) noexcept
        {
            return LinqGenerator([this](std::function<bool(TSource)> predicate_)
                { return TakeWhileGenerator(predicate_); }, std::move(predicate));
        }

        template<typename TKeySelector, typename TKey = typename FunctorTraits<TKeySelector(TSource)>::ReturnType>
        requires IsFunctor<TKeySelector, TSource>
        LinqGenerator<std::pair<TKey, std::vector<TSource>>> GroupBy(TKeySelector&& keySelector) noexcept
        {
            return LinqGenerator<std::pair<TKey, std::vector<TSource>>>([this](TKeySelector&& keySelector_)
                { return GroupByGenerator(std::forward<TKeySelector>(keySelector_)); },
                                          std::forward<TKeySelector>(keySelector));
        }

        template<ConstIterable TOtherCollection,
                 typename TInnerKeySelector,
                 typename TOtherKeySelector,
                 typename TResultSelector,
                 typename TResult = typename FunctorTraits<TResultSelector(TSource, typename TOtherCollection::value_type)>::ReturnType>
        requires IsFunctor<TInnerKeySelector, TSource> &&
                 IsFunctor<TOtherKeySelector, typename TOtherCollection::value_type> &&
                 IsFunctor<TResultSelector, TSource, typename TOtherCollection::value_type> &&
                 std::same_as<typename FunctorTraits<TInnerKeySelector(TSource)>::ReturnType, typename FunctorTraits<TOtherKeySelector(typename TOtherCollection::value_type)>::ReturnType> &&
                 Equalable<typename FunctorTraits<TInnerKeySelector(TSource)>::ReturnType>
        LinqGenerator<TResult> Join(const TOtherCollection& otherCollection,
                                    TInnerKeySelector&& innerKeySelector,
                                    TOtherKeySelector&& otherKeySelector,
                                    TResultSelector&& resultSelector)
        {
            if (otherCollection.empty())
                throw std::invalid_argument("other collection must not be empty");

            return LinqGenerator<TResult>([this](const TOtherCollection& otherCollection_,
                    TInnerKeySelector&& innerKeySelector_,
                    TOtherKeySelector&& otherKeySelector_,
                    TResultSelector&& resultSelector_)
                { return JoinGenerator(otherCollection_,
                    std::forward<TInnerKeySelector>(innerKeySelector_),
                    std::forward<TOtherKeySelector>(otherKeySelector_),
                    std::forward<TResultSelector>(resultSelector_)); },
                otherCollection,
                std::forward<TInnerKeySelector>(innerKeySelector),
                std::forward<TOtherKeySelector>(otherKeySelector),
                std::forward<TResultSelector>(resultSelector));
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
        LinqGenerator<TResult> Join(TOtherCollection&& otherCollection,
                                    TInnerKeySelector&& innerKeySelector,
                                    TOtherKeySelector&& otherKeySelector,
                                    TResultSelector&& resultSelector)
        {
            if (otherCollection.empty())
                throw std::invalid_argument("other collection must not be empty");

            return LinqGenerator<TResult>([this](TOtherCollection&& otherCollection_,
                    TInnerKeySelector&& innerKeySelector_,
                    TOtherKeySelector&& otherKeySelector_,
                    TResultSelector&& resultSelector_)
                { return JoinGenerator(std::forward<TOtherCollection>(otherCollection_),
                    std::forward<TInnerKeySelector>(innerKeySelector_),
                    std::forward<TOtherKeySelector>(otherKeySelector_),
                    std::forward<TResultSelector>(resultSelector_)); },
                std::forward<TOtherCollection>(otherCollection),
                std::forward<TInnerKeySelector>(innerKeySelector),
                std::forward<TOtherKeySelector>(otherKeySelector),
                std::forward<TResultSelector>(resultSelector));
        }

        template<ConstIterable TOtherCollection,
                 typename TInnerKeySelector,
                 Equalable TKey = typename FunctorTraits<TInnerKeySelector(TSource)>::ReturnType,
                 typename TOtherKeySelector,
                 typename TResultSelector,
                 typename TResult = typename FunctorTraits<TResultSelector(const std::vector<TSource>&, typename TOtherCollection::value_type)>::ReturnType>
        requires IsFunctor<TInnerKeySelector, TSource> &&
                 IsFunctor<TOtherKeySelector, typename TOtherCollection::value_type> &&
                 IsFunctor<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type> &&
                 std::same_as<TKey, typename FunctorTraits<TOtherKeySelector(typename TOtherCollection::value_type)>::ReturnType>
        LinqGenerator<TResult> GroupJoin(const TOtherCollection& otherCollection,
                                         TInnerKeySelector&& innerKeySelector,
                                         TOtherKeySelector&& otherKeySelector,
                                         TResultSelector&& resultSelector)
        {
            if (otherCollection.empty())
                throw std::invalid_argument("other collection must not be empty");

            return LinqGenerator<TResult>([this](const TOtherCollection& otherCollection_,
                    TInnerKeySelector&& innerKeySelector_,
                    TOtherKeySelector&& otherKeySelector_,
                    TResultSelector&& resultSelector_)
                { return GroupJoinGenerator(otherCollection_,
                    std::forward<TInnerKeySelector>(innerKeySelector_),
                    std::forward<TOtherKeySelector>(otherKeySelector_),
                    std::forward<TResultSelector>(resultSelector_)); },
                otherCollection,
                std::forward<TInnerKeySelector>(innerKeySelector),
                std::forward<TOtherKeySelector>(otherKeySelector),
                std::forward<TResultSelector>(resultSelector));
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
        LinqGenerator<TResult> GroupJoin(TOtherCollection&& otherCollection,
                                         TInnerKeySelector&& innerKeySelector,
                                         TOtherKeySelector&& otherKeySelector,
                                         TResultSelector&& resultSelector)
        {
            if (otherCollection.empty())
                throw std::invalid_argument("other collection must not be empty");

            return LinqGenerator<TResult>([this](TOtherCollection&& otherCollection_,
                    TInnerKeySelector&& innerKeySelector_,
                    TOtherKeySelector&& otherKeySelector_,
                    TResultSelector&& resultSelector_)
                { return GroupJoinGenerator(std::forward<TOtherCollection>(otherCollection_),
                    std::forward<TInnerKeySelector>(innerKeySelector_),
                    std::forward<TOtherKeySelector>(otherKeySelector_),
                    std::forward<TResultSelector>(resultSelector_)); },
                std::forward<TOtherCollection>(otherCollection),
                std::forward<TInnerKeySelector>(innerKeySelector),
                std::forward<TOtherKeySelector>(otherKeySelector),
                std::forward<TResultSelector>(resultSelector));
        }

        template<typename TOtherCollection,
                 typename TOtherCollectionValueType = typename TOtherCollection::value_type>
        requires ConstIterable<TOtherCollection> && HasSize<TOtherCollection>
        LinqGenerator<std::pair<TSource, TOtherCollectionValueType>> Zip(const TOtherCollection& otherCollection) noexcept
        {
            return LinqGenerator<std::pair<TSource, TOtherCollectionValueType>>([this](const TOtherCollection& otherCollection_)
                { return ZipGenerator(otherCollection_); }, otherCollection);
        }

        template<typename TOtherCollection,
                 typename TOtherCollectionValueType = typename TOtherCollection::value_type>
        requires Iterable<TOtherCollection> && HasSize<TOtherCollection>
        LinqGenerator<std::pair<TSource, TOtherCollectionValueType>> Zip(TOtherCollection&& otherCollection) noexcept
        {
            return LinqGenerator<std::pair<TSource, TOtherCollectionValueType>>([this](TOtherCollection&& otherCollection_)
                { return ZipGenerator(otherCollection_); }, std::forward<TOtherCollection>(otherCollection));
        }

        bool All(std::function<bool(TSource)> predicate) noexcept
        {
            while (_yieldContext)
                if (!predicate(_yieldContext.Next())) return false;
            return true;
        }

        bool Any(std::function<bool(TSource)> predicate) noexcept
        {
            while (_yieldContext)
                if (predicate(_yieldContext.Next())) return true;
            return false;
        }

    private:
        template<typename TSelector, typename TResult = typename FunctorTraits<TSelector(TSource)>::ReturnType>
        requires IsFunctor<TSelector, TSource>
        Future<TResult> SelectGenerator(TSelector&& selector) noexcept
        {
            while (_yieldContext)
                co_yield selector(_yieldContext.Next());
        }

        template<typename TSelector, typename TResult = typename FunctorTraits<TSelector(TSource)>::ReturnType::value_type>
        requires IsFunctor<TSelector, TSource>
        Future<TResult> SelectManyGenerator(TSelector&& selector) noexcept
        {
            while (_yieldContext)
            {
                std::vector<TResult> tempCollection = selector(_yieldContext.Next());
                for (auto& tempElement : tempCollection)
                    co_yield std::move(tempElement);
            }
        }

        template<typename TCollectionSelector,
                Iterable TCollection = typename FunctorTraits<TCollectionSelector(TSource)>::ReturnType,
                typename TCollectionValueType = typename TCollection::value_type,
                typename TResultSelector,
                typename TResult = typename FunctorTraits<TResultSelector(TSource, TCollectionValueType)>::ReturnType>
        requires IsFunctor<TCollectionSelector, TSource> && IsFunctor<TResultSelector, TSource, TCollectionValueType>
        Future<TResult> SelectManyGenerator(TCollectionSelector&& collectionSelector, TResultSelector&& resultSelector) noexcept
        {
            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                TCollection tempCollection = collectionSelector(element);
                for (auto& tempElement : tempCollection)
                    co_yield resultSelector(std::move(element), std::move(tempElement));
            }
        }

        Future<TSource> WhereGenerator(std::function<bool(TSource)> predicate) noexcept
        {
            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                if (predicate(element))
                    co_yield std::move(element);
            }
        }

        Future<TSource> SkipGenerator(const std::size_t count) noexcept
        {
            std::size_t i = 0;
            while (_yieldContext && i < count)
            {
                _yieldContext.Next();
                ++i;
            }

            while (_yieldContext)
                co_yield _yieldContext.Next();
        }

        Future<TSource> SkipWhileGenerator(std::function<bool(TSource)> predicate) noexcept
        {
            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                if (!predicate(element))
                {
                    co_yield std::move(element);
                    break;
                }
            }

            while (_yieldContext)
                co_yield _yieldContext.Next();
        }

        Future<TSource> TakeGenerator(const std::size_t count)
        {
            std::size_t i = 0;
            while (_yieldContext && i < count)
            {
                co_yield _yieldContext.Next();
                ++i;
            }

            while (_yieldContext)
                _yieldContext.Next();
        }

        Future<TSource> TakeWhileGenerator(std::function<bool(TSource)> predicate) noexcept
        {
            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                if (!predicate(element)) break;
                co_yield std::move(element);
            }

            while (_yieldContext)
                _yieldContext.Next();
        }

        template<typename TKeySelector, typename TKey = typename FunctorTraits<TKeySelector(TSource)>::ReturnType>
        requires IsFunctor<TKeySelector, TSource>
        Future<std::pair<TKey, std::vector<TSource>>> GroupByGenerator(TKeySelector&& keySelector) noexcept
        {
            std::map<TKey, std::vector<TSource>> result;

            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                if (!result.contains(keySelector(element)))
                    result.insert(std::pair<TKey, std::vector<TSource>>(keySelector(element), std::vector<TSource>()));
                result.at(keySelector(element)).push_back(std::move(element));
            }

            for (auto& kv : result)
                co_yield std::move(kv);
        }

        template<ConstIterable TOtherCollection,
                typename TInnerKeySelector,
                typename TOtherKeySelector,
                typename TResultSelector,
                typename TResult = typename FunctorTraits<TResultSelector(TSource, typename TOtherCollection::value_type)>::ReturnType>
        requires IsFunctor<TInnerKeySelector, TSource> &&
                 IsFunctor<TOtherKeySelector, typename TOtherCollection::value_type> &&
                 IsFunctor<TResultSelector, TSource, typename TOtherCollection::value_type> &&
                 std::same_as<typename FunctorTraits<TInnerKeySelector(TSource)>::ReturnType, typename FunctorTraits<TOtherKeySelector(typename TOtherCollection::value_type)>::ReturnType> &&
                 Equalable<typename FunctorTraits<TInnerKeySelector(TSource)>::ReturnType>
        Future<TResult> JoinGenerator(const TOtherCollection& otherCollection,
                                      TInnerKeySelector&& innerKeySelector,
                                      TOtherKeySelector&& otherKeySelector,
                                      TResultSelector&& resultSelector) noexcept
        {
            std::vector<TResult> result;

            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                for (const auto& otherElement : otherCollection)
                    if (innerKeySelector(element) == otherKeySelector(otherElement))
                        result.push_back(resultSelector(std::move(element), otherElement));
            }

            for (auto& element : result)
                co_yield std::move(element);
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
        Future<TResult> JoinGenerator(TOtherCollection&& otherCollection,
                                      TInnerKeySelector&& innerKeySelector,
                                      TOtherKeySelector&& otherKeySelector,
                                      TResultSelector&& resultSelector) noexcept
        {
            std::vector<TResult> result;

            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                for (auto& otherElement : otherCollection)
                    if (innerKeySelector(element) == otherKeySelector(otherElement))
                        result.push_back(resultSelector(std::move(element), std::move(otherElement)));
            }

            for (auto& element : result)
                co_yield std::move(element);
        }

        template<ConstIterable TOtherCollection,
                 typename TInnerKeySelector,
                 Equalable TKey = typename FunctorTraits<TInnerKeySelector(TSource)>::ReturnType,
                 typename TOtherKeySelector,
                 typename TResultSelector,
                 typename TResult = typename FunctorTraits<TResultSelector(const std::vector<TSource>&, typename TOtherCollection::value_type)>::ReturnType>
        requires IsFunctor<TInnerKeySelector, TSource> &&
                 IsFunctor<TOtherKeySelector, typename TOtherCollection::value_type> &&
                 IsFunctor<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type> &&
                 std::same_as<TKey, typename FunctorTraits<TOtherKeySelector(typename TOtherCollection::value_type)>::ReturnType>
        Future<TResult> GroupJoinGenerator(const TOtherCollection& otherCollection,
                                           TInnerKeySelector&& innerKeySelector,
                                           TOtherKeySelector&& otherKeySelector,
                                           TResultSelector&& resultSelector) noexcept
        {
            auto groups = GroupBy(std::forward<TInnerKeySelector>(innerKeySelector));
            std::vector<TResult> result;

            while (groups)
            {
                auto&& [key, group] = groups.Next();
                for (const auto& element : otherCollection)
                    if (std::move(key) == otherKeySelector(element))
                        result.push_back(resultSelector(std::move(group), element));
            }

            for (auto& element : result)
                co_yield std::move(element);
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
        Future<TResult> GroupJoinGenerator(TOtherCollection&& otherCollection,
                                           TInnerKeySelector&& innerKeySelector,
                                           TOtherKeySelector&& otherKeySelector,
                                           TResultSelector&& resultSelector) noexcept
        {
            auto groups = GroupBy(std::forward<TInnerKeySelector>(innerKeySelector));
            std::vector<TResult> result;

            while (groups)
            {
                auto&& [key, group] = groups.Next();
                for (const auto& element : otherCollection)
                    if (std::move(key) == otherKeySelector(element))
                        result.push_back(resultSelector(std::move(group), std::move(element)));
            }

            for (auto& element : result)
                co_yield std::move(element);
        }

        template<typename TOtherCollection,
                 typename TOtherCollectionValueType = typename TOtherCollection::value_type>
        requires ConstIterable<TOtherCollection> && HasSize<TOtherCollection>
        Future<std::pair<TSource, TOtherCollectionValueType>> ZipGenerator(const TOtherCollection& otherCollection) noexcept
        {
            for (const auto& element : otherCollection)
                if (_yieldContext)
                    co_yield std::make_pair(_yieldContext.Next(), element);
        }

        template<typename TOtherCollection,
                 typename TOtherCollectionValueType = typename TOtherCollection::value_type>
        requires Iterable<TOtherCollection> && HasSize<TOtherCollection>
        Future<std::pair<TSource, TOtherCollectionValueType>> ZipGenerator(TOtherCollection&& otherCollection) noexcept
        {
            for (auto& element : otherCollection)
                if (_yieldContext)
                    co_yield std::make_pair(_yieldContext.Next(), std::move(element));
        }
    };
}

#endif