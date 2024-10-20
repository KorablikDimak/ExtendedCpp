#ifndef LINQ_LinqGenerator_H
#define LINQ_LinqGenerator_H

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

#include <ExtendedCpp/LINQ/Algorithm.h>
#include <ExtendedCpp/LINQ/Sort.h>
#include <ExtendedCpp/LINQ/YieldForeach.h>

namespace ExtendedCpp::LINQ
{
    template<typename TSource>
    class LinqGenerator final
    {
    private:
        Future<TSource> _yieldContext;

        class Iterator final
        {
        public:
            Iterator(Future<TSource>& yieldContext, bool isEnd) noexcept :
                    _yieldContext(yieldContext), _isEnd(isEnd)
            {
                if (_yieldContext)
                    _value = _yieldContext.Value();
                else
                    _isEnd = true;
            }

            TSource& operator*() noexcept
            {
                return _value;
            }

            const TSource& operator*() const noexcept
            {
                return _value;
            }

            Iterator& operator++() noexcept
            {
                if (_yieldContext)
                    _value = _yieldContext.Next();
                else
                    _isEnd = true;
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

    public:
        using value_type = TSource;
        using iterator = Iterator;
        using promise_type = typename Future<TSource>::promise_type;
        using handle_type = typename Future<TSource>::handle_type;

        template<typename TGenerator, typename... Args>
        requires std::invocable<TGenerator, Args...>
        explicit LinqGenerator(TGenerator&& generator, Args... args) noexcept :
            _yieldContext(generator(std::forward<Args>(args)...)) {}

        template<Concepts::ConstIterable TCollection>
        explicit LinqGenerator(const TCollection& collection) noexcept :
            _yieldContext(YieldForeach(collection)) {}

        template<Concepts::Iterable TCollection>
        explicit LinqGenerator(TCollection&& collection) noexcept :
            _yieldContext(YieldForeach(std::forward<TCollection>(collection))) {}

        template<std::forward_iterator TIterator>
        LinqGenerator(const TIterator begin, const TIterator end) noexcept :
            _yieldContext(YieldForeach(begin, end)) {}

        ~LinqGenerator() = default;

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
            return collection;
        }

        template<std::size_t SIZE>
        std::array<TSource, SIZE> ToArray() noexcept
        {
            std::array<TSource, SIZE> collection;
            for (std::size_t i = 0; i < SIZE && _yieldContext; ++i)
                collection[i] = _yieldContext.Next();
            return collection;
        }

        std::list<TSource> ToList() noexcept
        {
            std::list<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());
            return collection;
        }

        std::forward_list<TSource> ToForwardList() noexcept
        {
            std::forward_list<TSource> collection;
            while (_yieldContext)
                collection.push_front(_yieldContext.Next());
            return collection;
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
        requires Concepts::IsPair<TSource>
        std::map<TKey, TValue> ToMap() noexcept
        {
            std::map<TKey, TValue> collection;
            while (_yieldContext)
                collection.insert(_yieldContext.Next());
            return collection;
        }

        template<typename TKey = typename PairTraits<TSource>::FirstType,
                 typename TValue = typename PairTraits<TSource>::SecondType>
        requires Concepts::IsPair<TSource>
        std::unordered_map<TKey, TValue> ToUnorderedMap() noexcept
        {
            std::unordered_map<TKey, TValue> collection;
            while (_yieldContext)
                collection.insert(_yieldContext.Next());
            return collection;
        }

        template<std::invocable<TSource> TSelector,
                 typename TResult = std::invoke_result_t<TSelector, TSource>>
        LinqGenerator<TResult> Select(TSelector&& selector)
        noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
        {
            return LinqGenerator<TResult>([this](TSelector&& selector_)
                { return SelectGenerator(std::forward<TSelector>(selector_)); },
                        std::forward<TSelector>(selector));
        }

        template<std::invocable<TSource> TSelector,
                 typename TResult = typename std::invoke_result_t<TSelector, TSource>::value_type>
        LinqGenerator<TResult> SelectMany(TSelector&& selector)
        noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
        {
            return LinqGenerator<TResult>([this](TSelector&& selector_)
                { return SelectManyGenerator(std::forward<TSelector>(selector_)); },
                        std::forward<TSelector>(selector));
        }

        template<std::invocable<TSource> TCollectionSelector,
                 Concepts::Iterable TCollection = std::invoke_result_t<TCollectionSelector, TSource>,
                 typename TCollectionValueType = typename TCollection::value_type,
                 std::invocable<TSource, TCollectionValueType> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, TSource, TCollectionValueType>>
        LinqGenerator<TResult> SelectMany(TCollectionSelector&& collectionSelector, TResultSelector&& resultSelector)
        noexcept(std::is_nothrow_invocable_v<TCollectionSelector, TSource> &&
                 std::is_nothrow_invocable_v<TResultSelector, TSource, TCollectionValueType>)
        {
            return LinqGenerator<TResult>([this](TCollectionSelector&& collectionSelector_, TResultSelector&& resultSelector_)
                { return SelectManyGenerator(std::forward<TCollectionSelector>(collectionSelector_),
                                             std::forward<TResultSelector>(resultSelector_)); },
                        std::forward<TCollectionSelector>(collectionSelector),
                        std::forward<TResultSelector>(resultSelector));
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        LinqGenerator Where(TPredicate&& predicate)
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            return LinqGenerator([this](TPredicate&& predicate_)
                { return WhereGenerator(std::forward<TPredicate>(predicate_)); },
                        std::forward<TPredicate>(predicate));
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        LinqGenerator RemoveWhere(TPredicate&& predicate)
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            return LinqGenerator([this](TPredicate&& predicate_)
                { return RemoveWhereGenerator(std::forward<TPredicate>(predicate_)); },
                        std::forward<TPredicate>(predicate));
        }

        LinqGenerator Order(OrderType orderType = OrderType::ASC) noexcept
        requires Concepts::Comparable<TSource>
        {
            std::vector<TSource> newCollection;
            while (_yieldContext)
                newCollection.push_back(_yieldContext.Next());
            if (newCollection.empty())
                return *this;

            Sort::QuickSort(newCollection.data(), 0, newCollection.size() - 1, orderType);

            return LinqGenerator([this, newCollection = std::move(newCollection)]()
                { return Generator(newCollection); });
        }

        template<std::invocable<TSource> TSelector>
        requires Concepts::Comparable<std::invoke_result_t<TSelector, TSource>>
        LinqGenerator OrderBy(TSelector&& selector, OrderType orderType = OrderType::ASC)
        noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
        {
            std::vector<TSource> newCollection;
            while (_yieldContext)
                newCollection.push_back(_yieldContext.Next());
            if (newCollection.empty())
                return *this;

            Sort::QuickSort(newCollection.data(), 0, newCollection.size() - 1,
                            std::forward<TSelector>(selector), orderType);

            return LinqGenerator([this, newCollection = std::move(newCollection)]()
                { return Generator(newCollection); });
        }

        LinqGenerator Reverse() noexcept
        {
            std::vector<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());

            return LinqGenerator([this, collection = std::move(collection)]()
                { return ReverseGenerator(collection); });
        }

        template<typename TOtherCollection>
        requires Concepts::ConstIterable<TOtherCollection> &&
                 Concepts::HasSize<TOtherCollection> &&
                 Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
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

            return LinqGenerator([this, newCollection = std::move(newCollection)]()
                { return Generator(newCollection); });
        }

        template<typename TOtherCollection>
        requires Concepts::Iterable<TOtherCollection> &&
                 Concepts::HasSize<TOtherCollection> &&
                 Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
        LinqGenerator Except(TOtherCollection&& otherCollection) noexcept
        {
            std::set<TSource> newCollection;

            while (_yieldContext)
            {
                auto element = _yieldContext.Next();

                std::size_t i = 0;
                for (auto&& otherElement : otherCollection)
                {
                    if (element == std::move(otherElement))
                        break;
                    if (i == otherCollection.size() - 1)
                        newCollection.insert(element);
                    ++i;
                }
            }

            return LinqGenerator([this, newCollection = std::move(newCollection)]()
                { return Generator(newCollection); });
        }

        template<Concepts::ConstIterable TOtherCollection>
        requires Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
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

            return LinqGenerator([this, newCollection = std::move(newCollection)]()
                { return Generator(newCollection); });
        }

        template<Concepts::Iterable TOtherCollection>
        requires Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
        LinqGenerator Intersect(TOtherCollection&& otherCollection) noexcept
        {
            std::set<TSource> newCollection;
            auto inner = std::forward<TOtherCollection>(otherCollection);

            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                for (auto&& otherElement : inner)
                    if (element == otherElement)
                    {
                        newCollection.insert(std::move(otherElement));
                        break;
                    }
            }

            return LinqGenerator([this, newCollection = std::move(newCollection)]()
                { return Generator(newCollection); });
        }

        LinqGenerator Distinct() noexcept
        requires Concepts::Equatable<TSource>
        {
            std::set<TSource> newCollection;
            while (_yieldContext)
                newCollection.insert(_yieldContext.Next());

            return LinqGenerator([this, newCollection = std::move(newCollection)]()
                { return Generator(newCollection); });
        }

        template<Concepts::ConstIterable TOtherCollection>
        requires Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
        LinqGenerator Union(const TOtherCollection& otherCollection) noexcept
        {
            std::set<TSource> newCollection;

            while (_yieldContext)
                newCollection.insert(_yieldContext.Next());

            for (const auto& element : otherCollection)
                newCollection.insert(element);

            return LinqGenerator([this, newCollection = std::move(newCollection)]()
                { return Generator(newCollection); });
        }

        template<Concepts::Iterable TOtherCollection>
        requires Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
        LinqGenerator Union(TOtherCollection&& otherCollection) noexcept
        {
            std::set<TSource> newCollection;

            while (_yieldContext)
                newCollection.insert(_yieldContext.Next());

            for (auto&& element : otherCollection)
                newCollection.insert(std::move(element));

            return LinqGenerator([this, newCollection = std::move(newCollection)]()
                { return Generator(newCollection); });
        }

        template<typename TResult, std::invocable<TResult, TSource> TAggregate>
        TResult Aggregate(TAggregate&& aggregateFunction)
        {
            std::vector<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Aggregate<TResult>(collection.data(), 0, collection.size() - 1,
                                                 std::forward<TAggregate>(aggregateFunction));
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        std::size_t Count(TPredicate&& predicate)
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            std::size_t result = 0;
            while (_yieldContext)
                if (predicate(_yieldContext.Next())) ++result;

            return result;
        }

        TSource Sum()
        requires Concepts::Summarize<TSource>
        {
            std::vector<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Sum(collection.data(), 0, collection.size() - 1);
        }

        template<std::invocable<TSource> TSelector,
                 Concepts::Summarize TResult = std::invoke_result_t<TSelector, TSource>>
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
        requires Concepts::Comparable<TSource>
        {
            std::vector<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Min(collection.data(), 0, collection.size() - 1);
        }

        template<std::invocable<TSource> TSelector,
                 Concepts::Comparable TResult = std::invoke_result_t<TSelector, TSource>>
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
        requires Concepts::Comparable<TSource>
        {
            std::vector<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Max(collection.data(), 0, collection.size() - 1);
        }

        template<std::invocable<TSource> TSelector,
                 Concepts::Comparable TResult = std::invoke_result_t<TSelector, TSource>>
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
        requires Concepts::Divisible<TSource>
        {
            std::vector<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Average(collection.data(), 0, collection.size() - 1);
        }

        template<std::invocable<TSource> TSelector,
                 Concepts::Divisible TResult = std::invoke_result_t<TSelector, TSource>>
        TResult Average(TSelector&& selector)
        {
            std::vector<TSource> collection;
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Average(collection.data(), 0, collection.size() - 1,
                                      std::forward<TSelector>(selector));
        }

        LinqGenerator Skip(const std::size_t count) noexcept
        {
            return LinqGenerator([this](const std::size_t count_)
                { return SkipGenerator(count_); }, count);
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        LinqGenerator SkipWhile(TPredicate&& predicate)
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            return LinqGenerator([this](TPredicate&& predicate_)
                { return SkipWhileGenerator(std::forward<TPredicate>(predicate_)); },
                        std::forward<TPredicate>(predicate));
        }

        LinqGenerator Take(const std::size_t count)
        {
            return LinqGenerator([this](const std::size_t count_)
                { return TakeGenerator(count_); }, count);
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        LinqGenerator TakeWhile(TPredicate&& predicate) noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            return LinqGenerator([this](TPredicate&& predicate_)
                { return TakeWhileGenerator(std::forward<TPredicate>(predicate_)); },
                        std::forward<TPredicate>(predicate));
        }

        template<std::invocable<TSource> TKeySelector,
                 typename TKey = std::invoke_result_t<TKeySelector, TSource>>
        LinqGenerator<std::pair<TKey, std::vector<TSource>>> GroupBy(TKeySelector&& keySelector)
        noexcept(std::is_nothrow_invocable_v<TKeySelector, TSource>)
        {
            return LinqGenerator<std::pair<TKey, std::vector<TSource>>>([this](TKeySelector&& keySelector_)
                { return GroupByGenerator(std::forward<TKeySelector>(keySelector_)); },
                        std::forward<TKeySelector>(keySelector));
        }

        template<Concepts::ConstIterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, TSource, typename TOtherCollection::value_type>>
        requires std::same_as<std::invoke_result_t<TInnerKeySelector, TSource>,
                              std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
                 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, TSource>>
        LinqGenerator<TResult> Join(const TOtherCollection& otherCollection,
                                    TInnerKeySelector&& innerKeySelector,
                                    TOtherKeySelector&& otherKeySelector,
                                    TResultSelector&& resultSelector)
        {
            if (otherCollection.empty())
                throw std::invalid_argument("other collection must not be empty");

            return LinqGenerator<TResult>([this, otherCollection](
                    TInnerKeySelector&& innerKeySelector_,
                    TOtherKeySelector&& otherKeySelector_,
                    TResultSelector&& resultSelector_)
                { return JoinGenerator(otherCollection,
                    std::forward<TInnerKeySelector>(innerKeySelector_),
                    std::forward<TOtherKeySelector>(otherKeySelector_),
                    std::forward<TResultSelector>(resultSelector_)); },
                std::forward<TInnerKeySelector>(innerKeySelector),
                std::forward<TOtherKeySelector>(otherKeySelector),
                std::forward<TResultSelector>(resultSelector));
        }

        template<Concepts::Iterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, TSource, typename TOtherCollection::value_type>>
        requires std::same_as<std::invoke_result_t<TInnerKeySelector, TSource>,
                              std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
                 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, TSource>>
        LinqGenerator<TResult> Join(TOtherCollection&& otherCollection,
                                    TInnerKeySelector&& innerKeySelector,
                                    TOtherKeySelector&& otherKeySelector,
                                    TResultSelector&& resultSelector)
        {
            if (otherCollection.empty())
                throw std::invalid_argument("other collection must not be empty");

            return LinqGenerator<TResult>([this, otherCollection = std::forward<TOtherCollection>(otherCollection)](
                    TInnerKeySelector&& innerKeySelector_,
                    TOtherKeySelector&& otherKeySelector_,
                    TResultSelector&& resultSelector_)
                { return JoinGenerator(otherCollection,
                    std::forward<TInnerKeySelector>(innerKeySelector_),
                    std::forward<TOtherKeySelector>(otherKeySelector_),
                    std::forward<TResultSelector>(resultSelector_)); },
                std::forward<TInnerKeySelector>(innerKeySelector),
                std::forward<TOtherKeySelector>(otherKeySelector),
                std::forward<TResultSelector>(resultSelector));
        }

        template<Concepts::ConstIterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, TSource>,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<const std::vector<TSource>&, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>>
        requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>>
        LinqGenerator<TResult> GroupJoin(const TOtherCollection& otherCollection,
                                         TInnerKeySelector&& innerKeySelector,
                                         TOtherKeySelector&& otherKeySelector,
                                         TResultSelector&& resultSelector)
        {
            if (otherCollection.empty())
                throw std::invalid_argument("other collection must not be empty");

            return LinqGenerator<TResult>([this, otherCollection](
                    TInnerKeySelector&& innerKeySelector_,
                    TOtherKeySelector&& otherKeySelector_,
                    TResultSelector&& resultSelector_)
                { return GroupJoinGenerator(otherCollection,
                    std::forward<TInnerKeySelector>(innerKeySelector_),
                    std::forward<TOtherKeySelector>(otherKeySelector_),
                    std::forward<TResultSelector>(resultSelector_)); },
                std::forward<TInnerKeySelector>(innerKeySelector),
                std::forward<TOtherKeySelector>(otherKeySelector),
                std::forward<TResultSelector>(resultSelector));
        }

        template<Concepts::Iterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, TSource>,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<const std::vector<TSource>&, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>>
        requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>>
        LinqGenerator<TResult> GroupJoin(TOtherCollection&& otherCollection,
                                         TInnerKeySelector&& innerKeySelector,
                                         TOtherKeySelector&& otherKeySelector,
                                         TResultSelector&& resultSelector)
        {
            if (otherCollection.empty())
                throw std::invalid_argument("other collection must not be empty");

            return LinqGenerator<TResult>([this, otherCollection = std::forward<TOtherCollection>(otherCollection)](
                    TInnerKeySelector&& innerKeySelector_,
                    TOtherKeySelector&& otherKeySelector_,
                    TResultSelector&& resultSelector_)
                { return GroupJoinGenerator(otherCollection,
                    std::forward<TInnerKeySelector>(innerKeySelector_),
                    std::forward<TOtherKeySelector>(otherKeySelector_),
                    std::forward<TResultSelector>(resultSelector_)); },
                std::forward<TInnerKeySelector>(innerKeySelector),
                std::forward<TOtherKeySelector>(otherKeySelector),
                std::forward<TResultSelector>(resultSelector));
        }

        template<typename TOtherCollection,
                 typename TOtherCollectionValueType = typename TOtherCollection::value_type>
        requires Concepts::ConstIterable<TOtherCollection> && Concepts::HasSize<TOtherCollection>
        LinqGenerator<std::pair<TSource, TOtherCollectionValueType>> Zip(const TOtherCollection& otherCollection) noexcept
        {
            return LinqGenerator<std::pair<TSource, TOtherCollectionValueType>>(
                    [this, otherCollection]
                    { return ZipGenerator(otherCollection); });
        }

        template<typename TOtherCollection,
                 typename TOtherCollectionValueType = typename TOtherCollection::value_type>
        requires Concepts::Iterable<TOtherCollection> && Concepts::HasSize<TOtherCollection>
        LinqGenerator<std::pair<TSource, TOtherCollectionValueType>> Zip(TOtherCollection&& otherCollection) noexcept
        {
            return LinqGenerator<std::pair<TSource, TOtherCollectionValueType>>(
                    [this, otherCollection = std::forward<TOtherCollection>(otherCollection)]
                    { return ZipGenerator(otherCollection); });
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        bool All(TPredicate&& predicate) noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            while (_yieldContext)
                if (!predicate(_yieldContext.Next()))
                    return false;
            return true;
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        bool Any(TPredicate&& predicate) noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            while (_yieldContext)
                if (predicate(_yieldContext.Next()))
                    return true;
            return false;
        }

    private:
        template<Concepts::ConstIterable TCollection>
        Future<TSource> Generator(TCollection collection) noexcept
        {
            for (auto&& element : collection)
                co_yield std::move(element);
        }

        template<Concepts::Iterable TCollection>
        Future<TSource> Generator(TCollection&& collection) noexcept
        {
            auto inner = std::forward<TCollection>(collection);
            for (auto&& element : inner)
                co_yield std::move(element);
        }

        template<Concepts::ConstIterable TCollection>
        Future<TSource> ReverseGenerator(TCollection collection) noexcept
        {
            for (auto it = collection.rbegin(); it != collection.rend(); ++it)
                co_yield std::move(*it);
        }

        template<Concepts::Iterable TCollection>
        Future<TSource> ReverseGenerator(TCollection&& collection) noexcept
        {
            auto inner = std::forward<TCollection>(collection);
            for (auto it = inner.rbegin(); it != inner.rend(); ++it)
                co_yield std::move(*it);
        }

        template<std::invocable<TSource> TSelector,
                 typename TResult = std::invoke_result_t<TSelector, TSource>>
        Future<TResult> SelectGenerator(TSelector&& selector)
        noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
        {
            while (_yieldContext)
                co_yield selector(_yieldContext.Next());
        }

        template<std::invocable<TSource> TSelector,
                 typename TResult = std::invoke_result_t<TSelector, TSource>::value_type>
        Future<TResult> SelectManyGenerator(TSelector&& selector)
        noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
        {
            while (_yieldContext)
            {
                std::vector<TResult> tempCollection = selector(_yieldContext.Next());
                for (auto& tempElement : tempCollection)
                    co_yield std::move(tempElement);
            }
        }

        template<std::invocable<TSource> TCollectionSelector,
                 Concepts::Iterable TCollection = std::invoke_result_t<TCollectionSelector, TSource>,
                 typename TCollectionValueType = TCollection::value_type,
                 std::invocable<TSource, TCollectionValueType> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, TSource, TCollectionValueType>>
        Future<TResult> SelectManyGenerator(TCollectionSelector&& collectionSelector, TResultSelector&& resultSelector)
        noexcept(std::is_nothrow_invocable_v<TCollectionSelector, TSource> &&
                 std::is_nothrow_invocable_v<TResultSelector, TSource, TCollectionValueType>)
        {
            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                TCollection tempCollection = collectionSelector(element);
                for (auto&& tempElement : tempCollection)
                    co_yield resultSelector(std::move(element), std::move(tempElement));
            }
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        Future<TSource> WhereGenerator(TPredicate&& predicate)
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                if (predicate(element))
                    co_yield std::move(element);
            }
        }

        template<Concepts::IsPredicate<TSource> TPredicate>
        Future<TSource> RemoveWhereGenerator(TPredicate&& predicate)
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                if (predicate(element))
                    continue;
                else
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

        template<Concepts::IsPredicate<TSource> TPredicate>
        Future<TSource> SkipWhileGenerator(TPredicate&& predicate)
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
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

        template<Concepts::IsPredicate<TSource> TPredicate>
        Future<TSource> TakeWhileGenerator(TPredicate&& predicate)
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                if (!predicate(element))
                    break;
                co_yield std::move(element);
            }

            while (_yieldContext)
                _yieldContext.Next();
        }

        template<std::invocable<TSource> TKeySelector,
                 typename TKey = std::invoke_result_t<TKeySelector, TSource>>
        Future<std::pair<TKey, std::vector<TSource>>> GroupByGenerator(TKeySelector&& keySelector)
        noexcept(std::is_nothrow_invocable_v<TKeySelector, TSource>)
        {
            std::map<TKey, std::vector<TSource>> result;

            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                if (!result.contains(keySelector(element)))
                    result.insert(std::pair<TKey, std::vector<TSource>>(keySelector(element), std::vector<TSource>()));
                result.at(keySelector(element)).push_back(std::move(element));
            }

            for (auto&& kv : result)
                co_yield std::move(kv);
        }

        template<Concepts::ConstIterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, TSource, typename TOtherCollection::value_type>>
        requires std::same_as<std::invoke_result_t<TInnerKeySelector, TSource>,
                              std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
                 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, TSource>>
        Future<TResult> JoinGenerator(TOtherCollection otherCollection,
                                      TInnerKeySelector&& innerKeySelector,
                                      TOtherKeySelector&& otherKeySelector,
                                      TResultSelector&& resultSelector)
        noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
                 std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
                 std::is_nothrow_invocable_v<TResultSelector, TSource, typename TOtherCollection::value_type>)
        {
            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                for (auto&& otherElement : otherCollection)
                    if (innerKeySelector(element) == otherKeySelector(otherElement))
                        co_yield resultSelector(std::move(element), std::move(otherElement));
            }
        }

        template<Concepts::Iterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, TSource, typename TOtherCollection::value_type>>
        requires std::same_as<std::invoke_result_t<TInnerKeySelector, TSource>,
                              std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
                 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, TSource>>
        Future<TResult> JoinGenerator(TOtherCollection&& otherCollection,
                                      TInnerKeySelector&& innerKeySelector,
                                      TOtherKeySelector&& otherKeySelector,
                                      TResultSelector&& resultSelector)
        noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
                 std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
                 std::is_nothrow_invocable_v<TResultSelector, TSource, typename TOtherCollection::value_type>)
        {
            auto inner = std::forward<TOtherCollection>(otherCollection);
            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                for (auto&& otherElement : inner)
                    if (innerKeySelector(element) == otherKeySelector(otherElement))
                        co_yield resultSelector(std::move(element), std::move(otherElement));
            }
        }

        template<Concepts::ConstIterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, TSource>,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<const std::vector<TSource>&, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>>
        requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>>
        Future<TResult> GroupJoinGenerator(TOtherCollection otherCollection,
                                           TInnerKeySelector&& innerKeySelector,
                                           TOtherKeySelector&& otherKeySelector,
                                           TResultSelector&& resultSelector)
        noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
                 std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
                 std::is_nothrow_invocable_v<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>)
        {
            auto groups = GroupBy(std::forward<TInnerKeySelector>(innerKeySelector));

            while (groups)
            {
                auto&& [key, group] = groups.Next();
                for (auto&& element : otherCollection)
                    if (std::move(key) == otherKeySelector(element))
                        co_yield resultSelector(std::move(group), std::move(element));
            }
        }

        template<Concepts::Iterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, TSource>,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<const std::vector<TSource>&, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>>
        requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>>
        Future<TResult> GroupJoinGenerator(TOtherCollection&& otherCollection,
                                           TInnerKeySelector&& innerKeySelector,
                                           TOtherKeySelector&& otherKeySelector,
                                           TResultSelector&& resultSelector)
        noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
                 std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
                 std::is_nothrow_invocable_v<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>)
        {
            auto inner = std::forward<TOtherCollection>(otherCollection);
            auto groups = GroupBy(std::forward<TInnerKeySelector>(innerKeySelector));

            while (groups)
            {
                auto&& [key, group] = groups.Next();
                for (auto&& element : inner)
                    if (std::move(key) == otherKeySelector(element))
                        co_yield resultSelector(std::move(group), std::move(element));
            }
        }

        template<typename TOtherCollection,
                 typename TOtherCollectionValueType = typename TOtherCollection::value_type>
        requires Concepts::ConstIterable<TOtherCollection> && Concepts::HasSize<TOtherCollection>
        Future<std::pair<TSource, TOtherCollectionValueType>> ZipGenerator(TOtherCollection otherCollection) noexcept
        {
            for (auto&& element : otherCollection)
                if (_yieldContext)
                    co_yield std::make_pair(_yieldContext.Next(), std::move(element));
        }

        template<typename TOtherCollection,
                 typename TOtherCollectionValueType = typename TOtherCollection::value_type>
        requires Concepts::Iterable<TOtherCollection> && Concepts::HasSize<TOtherCollection>
        Future<std::pair<TSource, TOtherCollectionValueType>> ZipGenerator(TOtherCollection&& otherCollection) noexcept
        {
            auto inner = std::forward<TOtherCollection>(otherCollection);
            for (auto&& element : inner)
                if (_yieldContext)
                    co_yield std::make_pair(_yieldContext.Next(), std::move(element));
        }
    };
}

#endif