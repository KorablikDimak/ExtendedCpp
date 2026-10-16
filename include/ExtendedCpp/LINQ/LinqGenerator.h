#ifndef LINQ_LINQ_GENERATOR_H
#define LINQ_LINQ_GENERATOR_H

#include <algorithm>
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <ExtendedCpp/LINQ/Aggregate.h>
#include <ExtendedCpp/LINQ/Concepts.h>
#include <ExtendedCpp/LINQ/Future.h>
#include <ExtendedCpp/LINQ/OrderType.h>
#include <ExtendedCpp/LINQ/Sort.h>
#include <ExtendedCpp/LINQ/TypeTraits.h>
#include <ExtendedCpp/LINQ/YieldForeach.h>

/// @brief
// ReSharper disable once CppInconsistentNaming
namespace ExtendedCpp::LINQ
{
    /// @brief
    /// @tparam TCollection
    template<Concepts::InputIterable TCollection>
    requires std::copy_constructible<TCollection>
    class CollectionFactory final
    {
    public:
        /// @brief
        using TSource = typename std::decay_t<TCollection>::value_type;

        /// @brief
        /// @param collection
        explicit CollectionFactory(TCollection collection) noexcept :
            _collection(std::move(collection)) {}

        /// @brief
        /// @return
        auto operator()() const
        {
            return YieldForeach(_collection);
        }

    private:
        TCollection _collection;
    };

    /// @brief
    /// @tparam TIterator
    template<std::input_iterator TIterator>
    class IteratorFactory final
    {
    public:
        /// @brief
        using TSource = typename std::iterator_traits<TIterator>::value_type;

        /// @brief
        /// @param begin
        /// @param end
        explicit IteratorFactory(TIterator begin, TIterator end) noexcept :
            _begin(std::move(begin)),
            _end(std::move(end)) {}

        /// @brief
        /// @return
        auto operator()() const
        {
            return YieldForeach(_begin, _end);
        }

    private:
        TIterator _begin;
        TIterator _end;
    };

    /// @brief
    /// @tparam TSource
    /// @tparam NothrowType
    /// @tparam TFactory
    template<std::copyable TSource, std::invocable TFactory, typename NothrowType>
    class LinqGenerator final
    {
    public:
        /// @brief
        static constexpr bool IS_LINQ_COLLECTION = true;

        /// @brief
        static constexpr bool NOTHROW = std::same_as<NothrowType, std::true_type>;

        /// @brief
        using FutureType = Future<TSource, NOTHROW>;

        /// @brief
        using value_type = TSource;

        /// @brief
        struct Iterator final
        {
            /// @brief
            /// @param future
            explicit Iterator(FutureType future) noexcept(NOTHROW) :
                _future(std::move(future)),
                _hasValue(_future.Next()) {}

            /// @brief
            /// @return
            TSource& operator*() noexcept
            {
                return _future.Value();
            }

            /// @brief
            /// @return
            const TSource& operator*() const noexcept
            {
                return _future.Value();
            }

            /// @brief
            /// @return
            Iterator& operator++() noexcept(NOTHROW)
            {
                _hasValue = _future.Next();
                return *this;
            }

            /// @brief
            void operator++(int) noexcept(NOTHROW)
            {
                ++(*this);
            }

            /// @brief
            /// @return
            [[nodiscard]]
            bool operator==(std::default_sentinel_t) const noexcept
            {
                return !_hasValue;
            }

            /// @brief
            /// @return
            [[nodiscard]]
            bool operator!=(std::default_sentinel_t) const noexcept
            {
                return _hasValue;
            }

        private:
            FutureType _future;
            bool _hasValue = false;
        };

        /// @brief
        /// @param factory
        explicit LinqGenerator(TFactory factory) noexcept
        requires std::invocable<TFactory> :
            _factory(std::move(factory)) {}

        /// @brief
        /// @return
        Iterator begin() const noexcept(NOTHROW)
        {
            return Iterator(_factory());
        }

        /// @brief
        /// @return
        // ReSharper disable once CppMemberFunctionMayBeStatic
        [[nodiscard]]
        std::default_sentinel_t end() const noexcept
        {
            return {};
        }

    private:
        const TFactory _factory;

    public:
        /// @brief
        /// @param other
        LinqGenerator(const LinqGenerator& other) noexcept = default;

        /// @brief
        /// @param other
        LinqGenerator(LinqGenerator&& other) noexcept = default;

        /// @brief
        /// @param other
        /// @return
        LinqGenerator& operator=(const LinqGenerator& other) noexcept = default;

        /// @brief
        /// @param other
        /// @return
        LinqGenerator& operator=(LinqGenerator&& other) noexcept = default;

        /// @brief
        ~LinqGenerator() noexcept = default;

        /// @brief
        /// @return
        std::vector<TSource> ToVector() const noexcept(NOTHROW)
        {
            std::vector<TSource> collection;

            auto future = _factory();

            while (future.Next())
                collection.push_back(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data.
        /// @tparam Size Size of returned array
        /// @return Copies of elements from 0 to min of array size or LINQ generator size
        template<std::size_t Size>
        requires std::is_default_constructible_v<TSource>
        std::array<TSource, Size> ToArray() const noexcept(NOTHROW)
        {
            std::array<TSource, Size> collection{};
            auto future = _factory();

            for (std::size_t i = 0; i < Size && future.Next(); ++i)
                collection[i] = future.Value();

            return collection;
        }

        /// @brief Get copy of collection data.
        /// @return Copies of elements from LINQ generator, maintaining order
        std::list<TSource> ToList() const noexcept(NOTHROW)
        {
            std::list<TSource> collection;
            auto future = _factory();

            while (future.Next())
                collection.push_back(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data.
        /// @return Copies of elements from LINQ generator, maintaining order
        std::forward_list<TSource> ToForwardList() const noexcept(NOTHROW)
        {
            std::forward_list<TSource> collection;
            auto future = _factory();
            auto tail = collection.before_begin();

            while (future.Next())
                tail = collection.insert_after(tail, future.Value());

            return collection;
        }

        /// @brief Get copy of collection data.
        /// @return Copies of elements from LINQ generator, maintaining order
        std::stack<TSource> ToStack() const noexcept(NOTHROW)
        {
            std::stack<TSource> collection;
            auto future = _factory();

            while (future.Next())
                collection.push(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data.
        /// @return Copies of elements from LINQ generator, maintaining order
        std::queue<TSource> ToQueue() const noexcept(NOTHROW)
        {
            std::queue<TSource> collection;
            auto future = _factory();

            while (future.Next())
                collection.push(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data.
        /// @return Copies of elements from LINQ generator, maintaining order
        std::deque<TSource> ToDeque() const noexcept(NOTHROW)
        {
            std::deque<TSource> collection;
            auto future = _factory();

            while (future.Next())
                collection.push_back(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data.
        /// @return Copies of elements from LINQ generator, maintaining order
        std::priority_queue<TSource> ToPriorityQueue() const noexcept(NOTHROW)
        {
            std::priority_queue<TSource> collection;
            auto future = _factory();

            while (future.Next())
                collection.push(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data.
        /// @return Unique elements from LINQ generator
        std::set<TSource> ToSet() const noexcept(NOTHROW)
        {
            std::set<TSource> collection;
            auto future = _factory();

            while (future.Next())
                collection.insert(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data.
        /// @return Unique elements from LINQ generator
        std::unordered_set<TSource> ToUnorderedSet() const noexcept(NOTHROW)
        {
            std::unordered_set<TSource> collection;
            auto future = _factory();

            while (future.Next())
                collection.insert(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data.
        /// @tparam TKey
        /// @tparam TValue
        /// @return map<TKey, TValue> from vector<std::pair<TKey, TValue>>
        template<typename TKey = typename PairTraits<TSource>::FirstType,
                 typename TValue = typename PairTraits<TSource>::SecondType>
        requires Concepts::IsPair<TSource>
        std::map<TKey, TValue> ToMap() const noexcept(NOTHROW)
        {
            std::map<TKey, TValue> collection;
            auto future = _factory();

            while (future.Next())
                collection.insert(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data.
        /// @tparam TKey
        /// @tparam TValue
        /// @return unordered_map<TKey, TValue> from vector<std::pair<TKey, TValue>>
        template<typename TKey = typename PairTraits<TSource>::FirstType,
                 typename TValue = typename PairTraits<TSource>::SecondType>
        requires Concepts::IsPair<TSource>
        std::unordered_map<TKey, TValue> ToUnorderedMap() const noexcept(NOTHROW)
        {
            std::unordered_map<TKey, TValue> collection;
            auto future = _factory();

            while (future.Next())
                collection.insert(future.Value());

            return collection;
        }

        /// @brief Iterates through all elements and applies a selector to each
        /// @tparam TSelector Any functional object with TSource argument
        /// @tparam TResult Result of selector invoke
        /// @tparam NothrowNew
        /// @param selector Any functional object with TSource argument
        /// @return New collection LinqGenerator<TResult>
        template<std::invocable<const TSource&> TSelector,
                 typename TResult = std::invoke_result_t<TSelector, const TSource&>,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TSelector, const TSource&>>
        requires (!std::is_void_v<TResult>)
        auto Select(TSelector&& selector) const noexcept
        {
            auto newFactory = [ upstream = _factory, selector = std::forward<TSelector>(selector) ]
            {
                return SelectGenerator(upstream(), selector);
            };

            return LinqGenerator<TResult, decltype(newFactory), std::integral_constant<bool, NothrowNew>>(std::move(newFactory));
        }

        /// @brief
        /// @tparam TMap
        /// @tparam NothrowNew
        /// @param mapFunction
        /// @return
        template<std::invocable<const TSource&> TMap,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TMap, const TSource&>>
        requires std::same_as<std::invoke_result_t<TMap, const TSource&>, TSource>
        auto Map(TMap&& mapFunction) const noexcept
        {
            auto newFactory = [ upstream = _factory, mapFunction = std::forward<TMap>(mapFunction) ]
            {
                return SelectGenerator(upstream(), mapFunction);
            };

            return LinqGenerator<TSource, decltype(newFactory), std::integral_constant<bool, NothrowNew>>(std::move(newFactory));
        }

        /// @brief
        /// @tparam TTransform
        /// @tparam NothrowNew
        /// @param transform
        /// @return
        template<std::invocable<TSource&> TTransform,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TTransform, TSource&>>
        requires std::is_void_v<std::invoke_result_t<TTransform, TSource&>>
        auto Transform(TTransform&& transform) const noexcept
        {
            auto newFactory = [ upstream = _factory, transform = std::forward<TTransform>(transform) ]
            {
                return TransformGenerator(upstream(), transform);
            };

            return LinqGenerator<TSource, decltype(newFactory), std::integral_constant<bool, NothrowNew>>(std::move(newFactory));
        }

        /// @brief Iterates through all elements and applies a selector to each
        /// @tparam TSelector Any functional object with TSource argument
        /// @tparam TCollection
        /// @tparam TResult value_type of selector invoke result
        /// @tparam NothrowNew
        /// @param selector Any functional object with TSource argument
        /// @return New collection LinqGenerator<TResult>
        template<std::invocable<TSource> TSelector,
                 Concepts::InputIterable TCollection = std::invoke_result_t<TSelector, const TSource&>,
                 typename TResult = typename TCollection::value_type,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TSelector, const TSource&>>
        requires (!std::is_void_v<TResult>)
        auto SelectMany(TSelector&& selector) const noexcept
        {
            auto newFactory = [ upstream = _factory, selector = std::forward<TSelector>(selector) ]
            {
                return SelectManyGenerator(upstream(), selector);
            };

            return LinqGenerator<TResult, decltype(newFactory), std::integral_constant<bool, NothrowNew>>(std::move(newFactory));
        }

        /// @brief
        /// @tparam TCollectionSelector
        /// @tparam TCollection
        /// @tparam TCollectionValueType
        /// @tparam TResultSelector
        /// @tparam TResult
        /// @tparam NothrowNew
        /// @param collectionSelector
        /// @param resultSelector
        /// @return
        template<std::invocable<const TSource&> TCollectionSelector,
                 Concepts::InputIterable TCollection = std::invoke_result_t<TCollectionSelector, const TSource&>,
                 typename TCollectionValueType = typename TCollection::value_type,
                 std::invocable<TSource, const TCollectionValueType&> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, const TSource&, const TCollectionValueType&>,
                 bool NothrowNew = NOTHROW &&
                     std::is_nothrow_invocable_v<TCollectionSelector, const TSource&> &&
                     std::is_nothrow_invocable_v<TResultSelector, const TCollectionValueType&>>
        requires (!std::is_void_v<TResult>)
        auto SelectMany(TCollectionSelector&& collectionSelector, TResultSelector&& resultSelector) const noexcept
        {
            auto newFactory = [ upstream = _factory,
                collectionSelector = std::forward<TCollectionSelector>(collectionSelector),
                resultSelector = std::forward<TResultSelector>(resultSelector) ]
            {
                return SelectManyGenerator(upstream(), collectionSelector, resultSelector);
            };

            return LinqGenerator<TResult, decltype(newFactory), std::integral_constant<bool, NothrowNew>>(std::move(newFactory));
        }

        /// @brief
        /// @tparam TPredicate
        /// @tparam NothrowNew
        /// @param predicate
        /// @return
        template<Concepts::IsPredicate<const TSource&> TPredicate,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TPredicate, const TSource&>>
        auto Where(TPredicate&& predicate) const noexcept
        {
            auto newFactory = [ upstream = _factory, predicate = std::forward<TPredicate>(predicate) ]
            {
                return WhereGenerator(upstream(), predicate);
            };

            return LinqGenerator<TSource, decltype(newFactory), std::integral_constant<bool, NothrowNew>>(std::move(newFactory));
        }

        /// @brief
        /// @tparam TPredicate
        /// @tparam NothrowNew
        /// @param predicate
        /// @return
        template<Concepts::IsPredicate<const TSource&> TPredicate,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TPredicate, const TSource&>>
        auto RemoveWhere(TPredicate&& predicate) const noexcept
        {
            auto newFactory = [ upstream = _factory, predicate = std::forward<TPredicate>(predicate) ]
            {
                return RemoveWhereGenerator(upstream(), predicate);
            };

            return LinqGenerator<TSource, decltype(newFactory), std::integral_constant<bool, NothrowNew>>(std::move(newFactory));
        }

        /// @brief Sort elements in ascending or descending order
        /// @param orderType Sorting order
        /// @return A lazy generator that yields sorted elements
        auto Order(OrderType orderType = OrderType::ASC) const noexcept
        requires Concepts::Comparable<TSource>
        {
            auto factory = [ upstream = _factory, orderType ]
            {
                return OrderGenerator(upstream(), orderType);
            };

            return LinqGenerator<TSource, decltype(factory), std::integral_constant<bool, NOTHROW>>(std::move(factory));
        }

        /// @brief Sort elements by a selected key
        /// @tparam TSelector Type of the key selector
        /// @tparam NothrowNew
        /// @param selector Function that selects the sorting key
        /// @param orderType Sorting order
        /// @return A lazy generator that yields elements sorted by their keys
        template<std::invocable<const TSource&> TSelector,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TSelector, const TSource&>>
        requires Concepts::Comparable<std::invoke_result_t<TSelector, const TSource&>>
        auto OrderBy(TSelector&& selector, OrderType orderType = OrderType::ASC) const noexcept
        {
            auto factory = [ upstream = _factory, selector = std::forward<TSelector>(selector), orderType ]
            {
                return OrderByGenerator(upstream(), selector, orderType);
            };

            return LinqGenerator<TSource, decltype(factory), std::integral_constant<bool, NothrowNew>>(std::move(factory));
        }

        /// @brief Exclude elements present in another collection
        /// @tparam TOtherCollection Type of the other collection
        /// @tparam NothrowNew
        /// @param otherCollection Collection whose elements must be excluded
        /// @return A lazy generator yielding distinct elements absent from the other collection
        template<Concepts::ForwardIterable TOtherCollection,
                 bool NothrowNew = NOTHROW && Concepts::IsNothrowIterable<TOtherCollection>>
        requires Concepts::Equatable<TSource> &&
                 std::convertible_to<typename std::decay_t<TOtherCollection>::value_type, TSource>
        auto Except(TOtherCollection&& otherCollection) const noexcept
        {
            auto factory = [ upstream = _factory, otherCollection = std::forward<TOtherCollection>(otherCollection) ]
            {
                return ExceptGenerator(upstream(), otherCollection);
            };

            return LinqGenerator<TSource, decltype(factory), std::integral_constant<bool, NothrowNew>>(std::move(factory));
        }

        /// @brief Find distinct elements also present in another collection
        /// @tparam TOtherCollection Type of the other collection
        /// @tparam NothrowNew
        /// @param otherCollection Collection used for intersection
        /// @return A lazy generator yielding distinct common elements
        template<Concepts::ForwardIterable TOtherCollection,
                 bool NothrowNew = NOTHROW && Concepts::IsNothrowIterable<TOtherCollection>>
        requires Concepts::Equatable<TSource> &&
                 std::convertible_to<typename std::decay_t<TOtherCollection>::value_type, TSource>
        auto Intersect(TOtherCollection&& otherCollection) const noexcept
        {
            auto factory = [ upstream = _factory, otherCollection = std::forward<TOtherCollection>(otherCollection) ]
            {
                return IntersectGenerator(upstream(), otherCollection);
            };

            return LinqGenerator<TSource, decltype(factory), std::integral_constant<bool, NothrowNew>>(std::move(factory));
        }

        /// @brief Remove duplicate elements
        /// @return A lazy generator yielding unique elements in their original order
        auto Distinct() const noexcept
        requires Concepts::Equatable<TSource>
        {
            auto factory = [ upstream = _factory ]
            {
                return DistinctGenerator(upstream());
            };

            return LinqGenerator<TSource, decltype(factory), std::integral_constant<bool, NOTHROW>>(std::move(factory));
        }

        /// @brief Combine elements from two collections without duplicates
        /// @tparam TOtherCollection Type of the other collection
        /// @tparam NothrowNew
        /// @param otherCollection Collection whose elements are appended to the result
        /// @return A lazy generator yielding unique elements from both collections
        template<Concepts::ForwardIterable TOtherCollection,
                 bool NothrowNew = NOTHROW && Concepts::IsNothrowIterable<TOtherCollection>>
        requires Concepts::Equatable<TSource> &&
                 std::convertible_to<typename std::decay_t<TOtherCollection>::value_type, TSource>
        auto Union(TOtherCollection&& otherCollection)
        {
            auto factory = [ upstream = _factory, otherCollection = std::forward<TOtherCollection>(otherCollection) ]
            {
                return UnionGenerator(upstream(), otherCollection);
            };

            return LinqGenerator<TSource, decltype(factory), std::integral_constant<bool, NothrowNew>>(std::move(factory));
        }

        /// @brief Merge two different types of sets into one
        /// @tparam TOtherCollection
        /// @tparam TInnerKeySelector
        /// @tparam TOtherKeySelector
        /// @tparam TResultSelector
        /// @tparam TResult
        /// @tparam NothrowNew
        /// @param otherCollection
        /// @param innerKeySelector
        /// @param otherKeySelector
        /// @param resultSelector
        /// @return
        template<Concepts::ForwardIterable TOtherCollection,
                 std::invocable<const TSource&> TInnerKeySelector,
                 std::invocable<const typename std::decay_t<TOtherCollection>::value_type&> TOtherKeySelector,
                 std::invocable<const TSource&, const typename std::decay_t<TOtherCollection>::value_type&> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, const TSource&, const typename std::decay_t<TOtherCollection>::value_type&>,
                 bool NothrowNew =
                     NOTHROW &&
                     Concepts::IsNothrowIterable<TOtherCollection> &&
                     std::is_nothrow_invocable_v<TInnerKeySelector, const TSource&> &&
                     std::is_nothrow_invocable_v<TOtherKeySelector, const typename std::decay_t<TOtherCollection>::value_type&> &&
                     std::is_nothrow_invocable_v<TResultSelector, const TSource&, const typename std::decay_t<TOtherCollection>::value_type&>>
        requires std::same_as<std::invoke_result_t<TInnerKeySelector, const TSource&>,
                              std::invoke_result_t<TOtherKeySelector, const typename std::decay_t<TOtherCollection>::value_type&>> &&
                 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, const TSource&>>
        auto Join(
            TOtherCollection&& otherCollection,
            TInnerKeySelector&& innerKeySelector,
            TOtherKeySelector&& otherKeySelector,
            TResultSelector&& resultSelector) const noexcept
        {
            auto factory = [
                upstream = _factory,
                other = std::forward<TOtherCollection>(otherCollection),
                innerKeySelector = std::forward<TInnerKeySelector>(innerKeySelector),
                otherKeySelector = std::forward<TOtherKeySelector>(otherKeySelector),
                resultSelector = std::forward<TResultSelector>(resultSelector) ]
            {
                return JoinGenerator(upstream(), other, innerKeySelector, otherKeySelector, resultSelector);
            };

            return LinqGenerator<TResult, decltype(factory), std::integral_constant<bool, NothrowNew>>(std::move(factory));
        }

        /// @brief Group data by certain parameters
        /// @tparam TKey
        /// @tparam TKeySelector
        /// @tparam NothrowNew
        /// @param keySelector
        /// @return
        template<std::invocable<TSource> TKeySelector,
                 typename TKey = std::invoke_result_t<TKeySelector, TSource>,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TKeySelector, const TSource&>>
        auto GroupBy(TKeySelector&& keySelector) const noexcept
        {
            auto factory = [ upstream = _factory, selector = std::forward<TKeySelector>(keySelector) ]
            {
                return GroupByGenerator(upstream(), selector);
            };

            return LinqGenerator<std::pair<TKey, std::vector<TSource>>, decltype(factory), std::integral_constant<bool, NothrowNew>>(std::move(factory));
        }

        /// @brief In addition to joining sequences, it also performs grouping
        /// @tparam TOtherCollection
        /// @tparam TInnerKeySelector
        /// @tparam TKey
        /// @tparam TOtherKeySelector
        /// @tparam TResultSelector
        /// @tparam TResult
        /// @tparam NothrowNew
        /// @param otherCollection
        /// @param innerKeySelector
        /// @param otherKeySelector
        /// @param resultSelector
        /// @return
        template<Concepts::InputIterable TOtherCollection,
                 typename TOtherCollectionConstRef = const std::decay_t<TOtherCollection>::value_type&,
                 std::invocable<const TSource&> TInnerKeySelector,
                 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, const TSource&>,
                 std::invocable<TOtherCollectionConstRef> TOtherKeySelector,
                 std::invocable<const std::vector<TSource>&, TOtherCollectionConstRef> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, TOtherCollectionConstRef>,
                 bool NothrowNew =
                     NOTHROW &&
                     std::is_nothrow_invocable_v<TInnerKeySelector, const TSource&> &&
                     std::is_nothrow_invocable_v<TOtherKeySelector, TOtherCollectionConstRef> &&
                     std::is_nothrow_invocable_v<TResultSelector, const std::vector<TSource>&, TOtherCollectionConstRef> &&
                     noexcept(Concepts::IsNothrowIterable<TOtherCollection>)>
        requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, TOtherCollectionConstRef>>
        auto GroupJoin(
            TOtherCollection&& otherCollection,
            TInnerKeySelector&& innerKeySelector,
            TOtherKeySelector&& otherKeySelector,
            TResultSelector&& resultSelector) const noexcept
        {
            auto factory = [
                upstream = _factory,
                other = std::forward<TOtherCollection>(otherCollection),
                innerKeySelector = std::forward<TInnerKeySelector>(innerKeySelector),
                otherKeySelector = std::forward<TOtherKeySelector>(otherKeySelector),
                resultSelector = std::forward<TResultSelector>(resultSelector) ]
            {
                return GroupJoinGenerator(upstream(), other, innerKeySelector, otherKeySelector, resultSelector);
            };

            return LinqGenerator<TResult, decltype(factory), std::integral_constant<bool, NothrowNew>>(std::move(factory));
        }

        /// @brief
        /// @param count
        /// @return
        auto Skip(const std::size_t count) const noexcept
        {
            auto factory = [ upstream = _factory, count ]
            {
                return SkipGenerator(upstream(), count);
            };

            return LinqGenerator<TSource, decltype(factory), std::integral_constant<bool, NOTHROW>>(std::move(factory));
        }

        /// @brief
        /// @tparam TPredicate
        /// @tparam NothrowNew
        /// @param predicate
        /// @return
        template<Concepts::IsPredicate<const TSource&> TPredicate,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TPredicate, const TSource&>>
        auto SkipWhile(TPredicate&& predicate) const noexcept
        {
            auto factory = [ upstream = _factory, predicate = std::forward<TPredicate>(predicate) ]
            {
                return SkipWhileGenerator(upstream(), predicate);
            };

            return LinqGenerator<TSource, decltype(factory), std::integral_constant<bool, NothrowNew>>(std::move(factory));
        }

        /// @brief
        /// @param count
        /// @return
        auto Take(const std::size_t count) const noexcept
        {
            auto factory = [ upstream = _factory, count ]
            {
                return TakeGenerator(upstream(), count);
            };

            return LinqGenerator<TSource, decltype(factory), std::integral_constant<bool, NOTHROW>>(std::move(factory));
        }

        /// @brief
        /// @tparam TPredicate
        /// @tparam NothrowNew
        /// @param predicate
        /// @return
        template<Concepts::IsPredicate<const TSource&> TPredicate,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TPredicate, const TSource&>>
        auto TakeWhile(TPredicate&& predicate) const noexcept
        {
            auto factory = [ upstream = _factory, predicate = std::forward<TPredicate>(predicate) ]
            {
                return TakeWhileGenerator(upstream(), predicate);
            };

            return LinqGenerator<TSource, decltype(factory), std::integral_constant<bool, NothrowNew>>(std::move(factory));
        }

        /// @brief
        /// @tparam TOtherCollection
        /// @tparam TOtherCollectionValueType
        /// @tparam NothrowNew
        /// @param otherCollection
        /// @return
        template<Concepts::InputIterable TOtherCollection,
                 typename TOtherCollectionValueType = typename std::decay_t<TOtherCollection>::value_type,
                 bool NothrowNew = NOTHROW && Concepts::IsNothrowIterable<TOtherCollection>>
        auto Zip(TOtherCollection&& otherCollection) const noexcept
        {
            auto factory = [ upstream = _factory, other = std::forward<TOtherCollection>(otherCollection) ]
            {
                return ZipGenerator(upstream(), other);
            };

            return LinqGenerator<std::pair<TSource, TOtherCollectionValueType>, decltype(factory), std::integral_constant<bool, NothrowNew>>(std::move(factory));
        }

        /// @brief Reverse the collection
        /// @return
        auto Reverse() const noexcept(NOTHROW)
        {
            std::vector<TSource> collection = ToVector();
            return LinqGenerator<TSource, CollectionFactory<std::vector<TSource>>, std::true_type>
                (CollectionFactory<std::vector<TSource>>({ collection.rbegin(), collection.rend() }));
        }

        /// @brief Performs a general aggregation of the elements of the collection depending on the specified expression.
        /// After this method generator became invalid
        /// @tparam TResult
        /// @tparam TAggregate
        /// @param aggregateFunction
        /// @return
        /// @throw std::out_of_range
        template<typename TResult, std::invocable<const TResult&, const TSource&> TAggregate>
        TResult Aggregate(TAggregate&& aggregateFunction)
        {
            std::vector<TSource> collection;

            auto future = _factory();
            while (future.Next())
                collection.push_back(future.Value());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Aggregate<TResult>(collection.data(), 0, collection.size() - 1,
                std::forward<TAggregate>(aggregateFunction));
        }

        /// @brief Get the number of elements. After this method generator became invalid
        /// @tparam TPredicate
        /// @param predicate
        /// @return
        template<Concepts::IsPredicate<const TSource&> TPredicate>
        std::size_t Count(TPredicate predicate)
        noexcept(std::is_nothrow_invocable_v<TPredicate, const TSource&> && NOTHROW)
        {
            std::size_t result = 0;

            auto future = _factory();
            while (future.Next())
                if (predicate(future.Value()))
                    ++result;

            return result;
        }

        /// @brief Get the sum of values. After this method generator became invalid
        /// @return
        /// @throw std::out_of_range
        TSource Sum()
        requires Concepts::Addable<TSource>
        {
            std::vector<TSource> collection;

            auto future = _factory();
            while (future.Next())
                collection.push_back(future.Value());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Sum(collection.data(), 0, collection.size() - 1);
        }

        /// @brief Get the sum of values. After this method generator became invalid
        /// @tparam TSelector
        /// @tparam TResult
        /// @param selector
        /// @return
        /// @throw std::out_of_range
        template<std::invocable<const TSource&> TSelector,
                 Concepts::Addable TResult = std::invoke_result_t<TSelector, const TSource&>>
        TResult Sum(TSelector&& selector)
        {
            std::vector<TSource> collection;

            auto future = _factory();
            while (future.Next())
                collection.push_back(future.Value());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Sum(collection.data(), 0, collection.size() - 1, std::forward<TSelector>(selector));
        }

        /// @brief Find element with the minimum value. After this method generator became invalid
        /// @return
        /// @throw std::out_of_range
        TSource Min()
        requires Concepts::Comparable<TSource>
        {
            std::vector<TSource> collection;

            auto future = _factory();
            while (future.Next())
                collection.push_back(future.Value());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Min(collection.data(), 0, collection.size() - 1);
        }

        /// @brief Find element with the minimum value. After this method generator became invalid
        /// @tparam TSelector
        /// @tparam TResult
        /// @param selector
        /// @return
        /// @throw std::out_of_range
        template<std::invocable<const TSource&> TSelector,
                 Concepts::Comparable TResult = std::invoke_result_t<TSelector, const TSource&>>
        TResult Min(TSelector&& selector)
        {
            std::vector<TSource> collection;

            auto future = _factory();
            while (future.Next())
                collection.push_back(future.Value());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Min(collection.data(), 0, collection.size() - 1, std::forward<TSelector>(selector));
        }

        /// @brief Find element with the maximum value. After this method generator became invalid
        /// @return
        /// @throw std::out_of_range
        TSource Max()
        requires Concepts::Comparable<TSource>
        {
            std::vector<TSource> collection;

            auto future = _factory();
            while (future.Next())
                collection.push_back(future.Value());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Max(collection.data(), 0, collection.size() - 1);
        }

        /// @brief Find element with the maximum value. After this method generator became invalid
        /// @tparam TSelector
        /// @tparam TResult
        /// @param selector
        /// @return
        /// @throw std::out_of_range
        template<std::invocable<const TSource&> TSelector,
                 Concepts::Comparable TResult = std::invoke_result_t<TSelector, const TSource&>>
        TResult Max(TSelector&& selector)
        {
            std::vector<TSource> collection;

            auto future = _factory();
            while (future.Next())
                collection.push_back(future.Value());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Max(collection.data(), 0, collection.size() - 1, std::forward<TSelector>(selector));
        }

        /// @brief Find the average value of the collection. After this method generator became invalid
        /// @return
        /// @throw std::out_of_range
        TSource Average()
        requires Concepts::Divisible<TSource>
        {
            std::vector<TSource> collection;

            auto future = _factory();
            while (future.Next())
                collection.push_back(future.Value());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Average(collection.data(), 0, collection.size() - 1);
        }

        /// @brief Find the average value of the collection. After this method generator became invalid
        /// @tparam TSelector
        /// @tparam TResult
        /// @param selector
        /// @return
        /// @throw std::out_of_range
        template<std::invocable<const TSource&> TSelector,
                 Concepts::Divisible TResult = std::invoke_result_t<TSelector, const TSource&>>
        TResult Average(TSelector&& selector)
        {
            std::vector<TSource> collection;

            auto future = _factory();
            while (future.Next())
                collection.push_back(future.Value());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Average(collection.data(), 0, collection.size() - 1, std::forward<TSelector>(selector));
        }

        /// @brief Checks if all elements match a condition. If all elements match the condition, then true is returned.
        /// After this method generator became invalid
        /// @tparam TPredicate
        /// @param predicate
        /// @return
        template<Concepts::IsPredicate<const TSource&> TPredicate>
        bool All(TPredicate predicate)
        noexcept(std::is_nothrow_invocable_v<TPredicate, const TSource&> && NOTHROW)
        {
            auto future = _factory();
            while (future.Next())
                if (!predicate(future.Value()))
                    return false;
            return true;
        }

        /// @brief Returns true if at least one element of the collection meets a certain condition.
        /// After this method generator became invalid
        /// @tparam TPredicate
        /// @param predicate
        /// @return
        template<Concepts::IsPredicate<const TSource&> TPredicate>
        bool Any(TPredicate predicate)
        noexcept(std::is_nothrow_invocable_v<TPredicate, const TSource&> && NOTHROW)
        {
            auto future = _factory();
            while (future.Next())
                if (predicate(future.Value()))
                    return true;
            return false;
        }

    private:
        template<std::invocable<const TSource&> TSelector,
                 typename TResult = std::invoke_result_t<TSelector, const TSource&>,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TSelector, const TSource&>>
        requires (!std::is_void_v<TResult>)
        static Future<TResult, NothrowNew>
        SelectGenerator(Future<TSource, NOTHROW> source, TSelector selector) noexcept(NothrowNew)
        {
            while (source.Next())
                co_yield selector(source.Value());

            co_return;
        }

        template<std::invocable<TSource&> TTransform,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TTransform, TSource&>>
        requires std::is_void_v<std::invoke_result_t<TTransform, TSource&>>
        static Future<TSource, NothrowNew>
        TransformGenerator(Future<TSource, NOTHROW> source, TTransform transform) noexcept(NothrowNew)
        {
            while (source.Next())
            {
                TSource value = source.Value();
                transform(value);
                co_yield std::move(value);
            }

            co_return;
        }

        template<std::invocable<TSource&&> TSelector,
                 Concepts::InputIterable TCollection = std::invoke_result_t<TSelector, TSource&&>,
                 typename TResult = typename TCollection::value_type,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TSelector, TSource&&>>
        requires (!std::is_void_v<TResult>)
        static Future<TResult, NothrowNew>
        SelectManyGenerator(Future<TSource, NOTHROW> source, TSelector selector) noexcept(NothrowNew)
        {
            while (source.Next())
            {
                for (auto&& element : selector(source.Value()))
                    co_yield std::move(element);
            }

            co_return;
        }

        template<std::invocable<const TSource&> TCollectionSelector,
                 Concepts::InputIterable TCollection = std::invoke_result_t<TCollectionSelector, const TSource&>,
                 typename TCollectionValueType = typename TCollection::value_type,
                 std::invocable<TSource, TCollectionValueType&&> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, const TSource&, TCollectionValueType&&>,
                 bool NothrowNew = NOTHROW &&
                     std::is_nothrow_invocable_v<TCollectionSelector, const TSource&> &&
                     std::is_nothrow_invocable_v<TResultSelector, TCollectionValueType&&>>
        requires (!std::is_void_v<TResult>)
        static Future<TResult, NothrowNew>
        SelectManyGenerator(Future<TSource, NOTHROW> source, TCollectionSelector collectionSelector, TResultSelector resultSelector) noexcept(NothrowNew)
        {
            while (source.Next())
            {
                const TSource element = source.Value();
                for (auto&& innerElement : collectionSelector(element))
                    co_yield resultSelector(element, std::move(innerElement));
            }

            co_return;
        }

        template<Concepts::IsPredicate<TSource> TPredicate,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TPredicate&, const TSource&>>
        static Future<TSource, NothrowNew>
        WhereGenerator(FutureType source, TPredicate predicate) noexcept(NothrowNew)
        {
            while (source.Next())
            {
                if (predicate(source.Value()))
                    co_yield source.Value();
            }

            co_return;
        }

        template<Concepts::IsPredicate<TSource> TPredicate,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TPredicate&, const TSource&>>
        static Future<TSource, NothrowNew>
        RemoveWhereGenerator(FutureType source, TPredicate predicate) noexcept(NothrowNew)
        {
            while (source.Next())
            {
                if (!predicate(source.Value()))
                    co_yield source.Value();
            }

            co_return;
        }

        static Future<TSource, NOTHROW>
        OrderGenerator(FutureType source, OrderType orderType) noexcept(NOTHROW)
        requires Concepts::Comparable<TSource>
        {
            std::vector<TSource> collection;

            while (source.Next())
                collection.push_back(source.Value());

            if (collection.size() > 1)
                Sort::QuickSort(collection.data(), 0, collection.size() - 1, orderType);

            for (auto&& element : collection)
                co_yield std::move(element);

            co_return;
        }

        template<std::invocable<const TSource&> TSelector,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TSelector, const TSource&>>
        requires Concepts::Comparable<std::invoke_result_t<TSelector, const TSource&>>
        static Future<TSource, NothrowNew>
        OrderByGenerator(FutureType source, TSelector selector, OrderType orderType) noexcept(NothrowNew)
        {
            std::vector<TSource> collection;

            while (source.Next())
                collection.push_back(source.Value());

            if (collection.size() > 1)
                Sort::QuickSort(collection.data(), 0, collection.size() - 1, selector, orderType);

            for (auto&& element : collection)
                co_yield std::move(element);

            co_return;
        }

        template<Concepts::ForwardIterable TOtherCollection,
                 bool NothrowNew = NOTHROW && Concepts::IsNothrowIterable<TOtherCollection>>
        requires Concepts::Equatable<TSource> &&
                 std::convertible_to<typename TOtherCollection::value_type, TSource>
        static Future<TSource, NothrowNew>
        ExceptGenerator(FutureType source, TOtherCollection otherCollection) noexcept(NothrowNew)
        {
            std::vector<TSource> yielded;

            while (source.Next())
            {
                const TSource& element = source.Value();

                const bool excluded =
                    std::find(std::begin(otherCollection), std::end(otherCollection), element) != std::end(otherCollection);

                if (excluded)
                    continue;

                const bool alreadyYielded =
                    std::find(yielded.begin(), yielded.end(), element) != yielded.end();

                if (!alreadyYielded)
                {
                    yielded.push_back(element);
                    co_yield element;
                }
            }

            co_return;
        }

        template<Concepts::ForwardIterable TOtherCollection,
                 bool NothrowNew = NOTHROW && Concepts::IsNothrowIterable<TOtherCollection>>
        requires Concepts::Equatable<TSource> &&
                 std::convertible_to<typename TOtherCollection::value_type, TSource>
        static Future<TSource, NothrowNew>
        IntersectGenerator(FutureType source, TOtherCollection otherCollection) noexcept(NothrowNew)
        {
            std::vector<TSource> yielded;

            while (source.Next())
            {
                const TSource& element = source.Value();

                const bool found =
                    std::find(std::begin(otherCollection), std::end(otherCollection), element) != std::end(otherCollection);

                if (!found)
                    continue;

                const bool alreadyYielded =
                    std::find(yielded.begin(), yielded.end(), element) != yielded.end();

                if (!alreadyYielded)
                {
                    yielded.push_back(element);
                    co_yield element;
                }
            }

            co_return;
        }

        static Future<TSource, NOTHROW>
        DistinctGenerator(FutureType source) noexcept(NOTHROW)
        requires Concepts::Equatable<TSource>
        {
            std::vector<TSource> yielded;

            while (source.Next())
            {
                const TSource& element = source.Value();

                const bool alreadyYielded =
                    std::find(yielded.begin(), yielded.end(), element) != yielded.end();

                if (!alreadyYielded)
                {
                    yielded.push_back(element);
                    co_yield element;
                }
            }

            co_return;
        }

        template<Concepts::ForwardIterable TOtherCollection,
                 bool NothrowNew = NOTHROW && Concepts::IsNothrowIterable<TOtherCollection>>
        requires Concepts::Equatable<TSource> &&
                 std::convertible_to<typename TOtherCollection::value_type, TSource>
        static Future<TSource, NothrowNew>
        UnionGenerator(FutureType source, TOtherCollection otherCollection) noexcept(NothrowNew)
        {
            std::vector<TSource> yielded;

            while (source.Next())
            {
                const TSource& element = source.Value();

                const bool alreadyYielded =
                    std::find(yielded.begin(), yielded.end(), element) != yielded.end();

                if (!alreadyYielded)
                {
                    yielded.push_back(element);
                    co_yield element;
                }
            }

            for (const auto& element : otherCollection)
            {
                const bool alreadyYielded = std::find(yielded.begin(), yielded.end(), element) != yielded.end();

                if (!alreadyYielded)
                {
                    yielded.push_back(element);
                    co_yield element;
                }
            }

            co_return;
        }

        template<Concepts::ForwardIterable TOtherCollection,
                 std::invocable<const TSource&> TInnerKeySelector,
                 std::invocable<const typename TOtherCollection::value_type&> TOtherKeySelector,
                 std::invocable<const TSource&, const typename TOtherCollection::value_type&> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, const TSource&, const typename TOtherCollection::value_type&>,
                 bool NothrowNew =
                     NOTHROW &&
                     Concepts::IsNothrowIterable<TOtherCollection> &&
                     std::is_nothrow_invocable_v<TInnerKeySelector, const TSource&> &&
                     std::is_nothrow_invocable_v<TOtherKeySelector, const typename TOtherCollection::value_type&> &&
                     std::is_nothrow_invocable_v<TResultSelector, const TSource&, const typename TOtherCollection::value_type&>>
        requires std::same_as<std::invoke_result_t<TInnerKeySelector, const TSource&>,
                              std::invoke_result_t<TOtherKeySelector, const typename TOtherCollection::value_type&>> &&
                 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, const TSource&>>
        static Future<TResult, NothrowNew>
        JoinGenerator(
            FutureType source,
            TOtherCollection otherCollection,
            TInnerKeySelector innerKeySelector,
            TOtherKeySelector otherKeySelector,
            TResultSelector resultSelector)
        noexcept(NothrowNew)
        {
            while (source.Next())
            {
                const TSource& element = source.Value();

                for (const auto& otherElement : otherCollection)
                    if (innerKeySelector(element) == otherKeySelector(otherElement))
                        co_yield resultSelector(element, otherElement);
            }

            co_return;
        }

        template<std::invocable<const TSource&> TKeySelector,
                 typename TKey = std::invoke_result_t<TKeySelector, const TSource&>,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TKeySelector, const TSource&>>
        static Future<std::pair<TKey, std::vector<TSource>>, NothrowNew>
        GroupByGenerator(FutureType source, TKeySelector keySelector) noexcept(NothrowNew)
        {
            std::map<TKey, std::vector<TSource>> groups;

            while (source.Next())
            {
                auto element = std::move(source.Value());
                auto key = std::invoke(keySelector, element);

                groups[std::move(key)].push_back(std::move(element));
            }

            for (auto&& group : groups)
                co_yield std::move(group);

            co_return;
        }

        template<Concepts::InputIterable TOtherCollection,
                 std::invocable<const TSource&> TInnerKeySelector,
                 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, const TSource&>,
                 std::invocable<const typename TOtherCollection::value_type&> TOtherKeySelector,
                 std::invocable<const std::vector<TSource>&, const typename TOtherCollection::value_type&> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, const typename TOtherCollection::value_type&>,
                 bool NothrowNew =
                     NOTHROW &&
                     std::is_nothrow_invocable_v<TInnerKeySelector, const TSource&> &&
                     std::is_nothrow_invocable_v<TOtherKeySelector, const typename TOtherCollection::value_type&> &&
                     std::is_nothrow_invocable_v<TResultSelector, const std::vector<TSource>&, const typename TOtherCollection::value_type&> &&
                     noexcept(Concepts::IsNothrowIterable<TOtherCollection>)>
                requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, const typename TOtherCollection::value_type&>>
        static Future<TResult, NothrowNew>
        GroupJoinGenerator(
            FutureType source,
            TOtherCollection otherCollection,
            TInnerKeySelector innerKeySelector,
            TOtherKeySelector otherKeySelector,
            TResultSelector resultSelector) noexcept(NothrowNew)
        {
            auto groups = GroupByGenerator(std::move(source), std::move(innerKeySelector));

            while (groups.Next())
            {
                auto group = std::move(groups.Value());

                for (const auto& otherElement : otherCollection)
                    if (group.first == otherKeySelector(otherElement))
                        co_yield resultSelector(group.second, otherElement);
            }

            co_return;
        }

        static Future<TSource, NOTHROW>
        SkipGenerator(FutureType source, const std::size_t count) noexcept(NOTHROW)
        {
            std::size_t i = 0;

            while (i < count && source.Next())
                ++i;

            while (source.Next())
                co_yield source.Value();

            co_return;
        }

        template<Concepts::IsPredicate<const TSource&> TPredicate,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TPredicate, const TSource&>>
        static Future<TSource, NothrowNew>
        SkipWhileGenerator(FutureType source, TPredicate predicate) noexcept(NothrowNew)
        {
            while (source.Next())
            {
                TSource element = source.Value();

                if (!predicate(element))
                {
                    co_yield std::move(element);
                    break;
                }
            }

            while (source.Next())
                co_yield source.Value();

            co_return;
        }

        static Future<TSource, NOTHROW>
        TakeGenerator(FutureType source, const std::size_t count) noexcept(NOTHROW)
        {
            std::size_t i = 0;

            while (i < count && source.Next())
            {
                co_yield source.Value();
                ++i;
            }

            co_return;
        }

        template<Concepts::IsPredicate<const TSource&> TPredicate,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TPredicate, const TSource&>>
        static Future<TSource, NothrowNew>
        TakeWhileGenerator(FutureType source, TPredicate predicate) noexcept(NothrowNew)
        {
            while (source.Next())
            {
                TSource element = source.Value();

                if (!predicate(element))
                    break;

                co_yield std::move(element);
            }

            co_return;
        }

        template<Concepts::InputIterable TOtherCollection,
                 typename TOtherCollectionValueType = typename TOtherCollection::value_type,
                 bool NothrowNew = NOTHROW && Concepts::IsNothrowIterable<TOtherCollection>>
        static Future<std::pair<TSource, TOtherCollectionValueType>, NothrowNew>
        ZipGenerator(FutureType source, TOtherCollection otherCollection) noexcept(NothrowNew)
        {
            for (auto&& element : otherCollection)
            {
                if (!source.Next())
                    break;

                co_yield std::make_pair(source.Value(), std::move(element));
            }

            co_return;
        }
    };
}

#endif