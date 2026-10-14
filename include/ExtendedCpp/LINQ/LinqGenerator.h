#ifndef LINQ_LINQ_GENERATOR_H
#define LINQ_LINQ_GENERATOR_H

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

/// @brief
// ReSharper disable once CppInconsistentNaming
namespace ExtendedCpp::LINQ
{
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
            [[nodiscard]]
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
        Iterator begin() noexcept(NOTHROW)
        {
            return Iterator{ std::invoke(_factory) };
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
        TFactory _factory;

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
        std::vector<TSource> ToVector() noexcept(NOTHROW)
        {
            std::vector<TSource> collection;

            auto future = _factory();

            while (future.Next())
                collection.push_back(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data. After this method generator became invalid
        /// @tparam Size Size of returned array
        /// @return Copies of elements from 0 to min of array size or LINQ generator size
        template<std::size_t Size>
        requires std::is_default_constructible_v<TSource>
        std::array<TSource, Size> ToArray() noexcept(NOTHROW)
        {
            std::array<TSource, Size> collection{};
            auto future = _factory();

            for (std::size_t i = 0; i < Size && future.Next(); ++i)
                collection[i] = future.Value();

            return collection;
        }

        /// @brief Get copy of collection data. After this method generator became invalid
        /// @return Copies of elements from LINQ generator, maintaining order
        std::list<TSource> ToList() noexcept(NOTHROW)
        {
            std::list<TSource> collection;
            auto future = _factory();

            while (future.Next())
                collection.push_back(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data. After this method generator became invalid
        /// @return Copies of elements from LINQ generator, maintaining order
        std::forward_list<TSource> ToForwardList() noexcept(NOTHROW)
        {
            std::forward_list<TSource> collection;
            auto future = _factory();
            auto tail = collection.before_begin();

            while (future.Next())
                tail = collection.insert_after(tail, future.Value());

            return collection;
        }

        /// @brief Get copy of collection data. After this method generator became invalid
        /// @return Copies of elements from LINQ generator, maintaining order
        std::stack<TSource> ToStack() noexcept(NOTHROW)
        {
            std::stack<TSource> collection;
            auto future = _factory();

            while (future.Next())
                collection.push(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data. After this method generator became invalid
        /// @return Copies of elements from LINQ generator, maintaining order
        std::queue<TSource> ToQueue() noexcept(NOTHROW)
        {
            std::queue<TSource> collection;
            auto future = _factory();

            while (future.Next())
                collection.push(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data. After this method generator became invalid
        /// @return Copies of elements from LINQ generator, maintaining order
        std::deque<TSource> ToDeque() noexcept(NOTHROW)
        {
            std::deque<TSource> collection;
            auto future = _factory();

            while (future.Next())
                collection.push_back(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data. After this method generator became invalid
        /// @return Copies of elements from LINQ generator, maintaining order
        std::priority_queue<TSource> ToPriorityQueue() noexcept(NOTHROW)
        {
            std::priority_queue<TSource> collection;
            auto future = _factory();

            while (future.Next())
                collection.push(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data. After this method generator became invalid
        /// @return Unique elements from LINQ generator
        std::set<TSource> ToSet() noexcept(NOTHROW)
        {
            std::set<TSource> collection;
            auto future = _factory();

            while (future.Next())
                collection.insert(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data. After this method generator became invalid
        /// @return Unique elements from LINQ generator
        std::unordered_set<TSource> ToUnorderedSet() noexcept(NOTHROW)
        {
            std::unordered_set<TSource> collection;
            auto future = _factory();

            while (future.Next())
                collection.insert(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data. After this method generator became invalid
        /// @tparam TKey
        /// @tparam TValue
        /// @return map<TKey, TValue> from vector<std::pair<TKey, TValue>>
        template<typename TKey = typename PairTraits<TSource>::FirstType,
                 typename TValue = typename PairTraits<TSource>::SecondType>
        requires Concepts::IsPair<TSource>
        std::map<TKey, TValue> ToMap() noexcept(NOTHROW)
        {
            std::map<TKey, TValue> collection;
            auto future = _factory();

            while (future.Next())
                collection.insert(future.Value());

            return collection;
        }

        /// @brief Get copy of collection data. After this method generator became invalid
        /// @tparam TKey
        /// @tparam TValue
        /// @return unordered_map<TKey, TValue> from vector<std::pair<TKey, TValue>>
        template<typename TKey = typename PairTraits<TSource>::FirstType,
                 typename TValue = typename PairTraits<TSource>::SecondType>
        requires Concepts::IsPair<TSource>
        std::unordered_map<TKey, TValue> ToUnorderedMap() noexcept(NOTHROW)
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
        auto Select(TSelector selector) const noexcept
        {
            TFactory upstream = _factory;

            auto newFactory = [upstream = std::move(upstream), selector = std::move(selector)]
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
        auto Map(TMap mapFunction) const noexcept
        {
            auto upstream = _factory;

            auto newFactory = [upstream = std::move(upstream), mapFunction = std::move(mapFunction)]
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
        auto Transform(TTransform transform) const noexcept
        {
            auto upstream = _factory;

            auto newFactory = [upstream = std::move(upstream), transform = std::move(transform)]
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
        auto SelectMany(TSelector selector) const noexcept
        {
            auto upstream = _factory;

            auto newFactory = [upstream = std::move(upstream), selector = std::move(selector)]
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
        auto SelectMany(TCollectionSelector collectionSelector, TResultSelector resultSelector) const noexcept
        {
            auto upstream = _factory;

            auto newFactory = [upstream = std::move(upstream), collectionSelector = std::move(collectionSelector), resultSelector = std::move(resultSelector)]
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
        auto Where(TPredicate predicate) const noexcept
        {
            auto upstream = _factory;

            auto newFactory = [upstream = std::move(upstream), predicate = std::move(predicate)]
            {
                return WhereGenerator(upstream(), predicate);
            };

            return LinqGenerator<TSource, decltype(newFactory), std::integral_constant<bool, NothrowNew>>(std::move(newFactory));
        }

        /// @brief
        /// @tparam TPredicate
        /// @param predicate
        /// @return
        template<Concepts::IsPredicate<TSource> TPredicate,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TPredicate, const TSource&>>
        auto RemoveWhere(TPredicate predicate) const noexcept
        {
            auto upstream = _factory;

            auto newFactory = [upstream = std::move(upstream), predicate = std::move(predicate)]
            {
                return RemoveWhereGenerator(upstream(), predicate);
            };

            return LinqGenerator<TSource, decltype(newFactory), std::integral_constant<bool, NothrowNew>>{
                std::move(newFactory)
            };
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

        template<std::invocable<TSource> TSelector,
                 Concepts::InputIterable TCollection = std::invoke_result_t<TSelector, const TSource&>,
                 typename TResult = typename TCollection::value_type,
                 bool NothrowNew = NOTHROW && std::is_nothrow_invocable_v<TSelector, const TSource&>>
        requires (!std::is_void_v<TResult>)
        static Future<TResult, NothrowNew>
        SelectManyGenerator(Future<TSource, NOTHROW> source, TSelector selector) noexcept(NothrowNew)
        {
            while (source.Next())
            {
                const TCollection collection = selector(source.Value());
                for (const TResult& element : collection)
                    co_yield element;
            }
            co_return;
        }

        template<std::invocable<const TSource&> TCollectionSelector,
                 Concepts::InputIterable TCollection = std::invoke_result_t<TCollectionSelector, const TSource&>,
                 typename TCollectionValueType = typename TCollection::value_type,
                 std::invocable<TSource, const TCollectionValueType&> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, const TSource&, const TCollectionValueType&>,
                 bool NothrowNew = NOTHROW &&
                     std::is_nothrow_invocable_v<TCollectionSelector, const TSource&> &&
                     std::is_nothrow_invocable_v<TResultSelector, const TCollectionValueType&>>
        requires (!std::is_void_v<TResult>)
        static Future<TResult, NothrowNew>
        SelectManyGenerator(Future<TSource, NOTHROW> source, TCollectionSelector collectionSelector, TResultSelector resultSelector) noexcept(NothrowNew)
        {
            while (source.Next())
            {
                const TSource element = source.Value();
                const TCollection collection = collectionSelector(element);
                for (const TCollectionValueType& innerElement : collection)
                    co_yield resultSelector(element, innerElement);
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
    };
}

/*
        /// @brief Sorts the elements of a collection
        /// @param orderType
        /// @return
        auto Order(OrderType orderType = OrderType::ASC) noexcept(IsNothrowCoroutine)
        requires Concepts::Comparable<TSource>
        {
            std::vector<TSource> newCollection;
            while (_yieldContext.top())
                newCollection.push_back(_yieldContext.top().Next());
            if (newCollection.empty())
                return std::move(*this);

            Sort::QuickSort(newCollection.data(), 0, newCollection.size() - 1, orderType);

            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(Generator(std::move(newCollection)))>;

            return LinqGenerator<TSource, NothrowNewCoroutine>([this]
                (std::vector<TSource>&& newCollection_) noexcept
                { return Generator(std::move(newCollection_)); },
                        std::move(newCollection));
        }

        /// @brief Sorts the elements of a collection with selector
        /// @tparam TSelector
        /// @param selector
        /// @param orderType
        /// @return
        template<std::invocable<TSource> TSelector>
        requires Concepts::Comparable<std::invoke_result_t<TSelector, TSource>>
        auto OrderBy(TSelector&& selector, OrderType orderType = OrderType::ASC)
        noexcept(std::is_nothrow_invocable_v<TSelector, TSource> && IsNothrowCoroutine)
        {
            std::vector<TSource> newCollection;
            while (_yieldContext.top())
                newCollection.push_back(_yieldContext.top().Next());
            if (newCollection.empty())
                return std::move(*this);

            Sort::QuickSort(newCollection.data(), 0, newCollection.size() - 1,
                            std::forward<TSelector>(selector), orderType);

            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(Generator(std::move(newCollection)))>;

            return LinqGenerator<TSource, NothrowNewCoroutine>([this]
                (std::vector<TSource>&& newCollection_) noexcept
                { return Generator(std::move(newCollection_)); },
                        std::move(newCollection));
        }

        /// @brief Reverse the collection
        /// @return
        auto Reverse() noexcept(IsNothrowCoroutine)
        {
            std::vector<TSource> collection;
            while (_yieldContext.top())
                collection.push_back(_yieldContext.top().Next());

            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(ReverseGenerator(std::move(collection)))>;

            return LinqGenerator<TSource, NothrowNewCoroutine>([this]
                (std::vector<TSource>&& collection_) noexcept
                { return ReverseGenerator(std::move(collection_)); },
                        std::move(collection));
        }

        /// @brief Get the difference of two sequences
        /// @tparam TOtherCollection
        /// @param otherCollection
        /// @return
        template<typename TOtherCollection>
        requires Concepts::ForwardIterable<TOtherCollection> &&
                 Concepts::HasSize<TOtherCollection> &&
                 Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
        auto Except(const TOtherCollection& otherCollection) noexcept(IsNothrowCoroutine)
        {
            std::set<TSource> newCollection;

            while (_yieldContext.top())
            {
                auto element = _yieldContext.top().Next();

                std::size_t i = 0;
                for (const auto& otherElement : otherCollection)
                {
                    if (element == otherElement) break;
                    if (i == otherCollection.size() - 1) newCollection.insert(element);
                    ++i;
                }
            }

            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(Generator(std::move(newCollection)))>;

            return LinqGenerator<TSource, NothrowNewCoroutine>([this]
                (std::set<TSource>&& newCollection_) noexcept
                { return Generator(std::move(newCollection_)); },
                        std::move(newCollection));
        }

        /// @brief Get the difference of two sequences
        /// @tparam TOtherCollection
        /// @param otherCollection
        /// @return
        template<typename TOtherCollection>
        requires Concepts::InputIterable<TOtherCollection> &&
                 Concepts::HasSize<TOtherCollection> &&
                 Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
        auto Except(TOtherCollection otherCollection) noexcept(IsNothrowCoroutine)
        {
            std::set<TSource> newCollection;

            while (_yieldContext.top())
            {
                auto element = _yieldContext.top().Next();

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

            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(Generator(std::move(newCollection)))>;

            return LinqGenerator<TSource, NothrowNewCoroutine>([this]
                (std::set<TSource>&& newCollection_) noexcept
                { return Generator(std::move(newCollection_)); },
                        std::move(newCollection));
        }

        /// @brief Get the intersection of sequences
        /// @tparam TOtherCollection
        /// @param otherCollection
        /// @return
        template<Concepts::ForwardIterable TOtherCollection>
        requires Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
        auto Intersect(const TOtherCollection& otherCollection) noexcept(IsNothrowCoroutine)
        {
            std::set<TSource> newCollection;

            while (_yieldContext.top())
            {
                auto element = _yieldContext.top().Next();
                for (const auto& otherElement : otherCollection)
                    if (element == otherElement)
                    {
                        newCollection.insert(element);
                        break;
                    }
            }

            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(Generator(std::move(newCollection)))>;

            return LinqGenerator<TSource, NothrowNewCoroutine>([this]
                (std::set<TSource>&& newCollection_) noexcept
                { return Generator(std::move(newCollection_)); },
                        std::move(newCollection));
        }

        /// @brief Get the intersection of sequences
        /// @tparam TOtherCollection
        /// @param otherCollection
        /// @return
        template<Concepts::InputIterable TOtherCollection>
        requires Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
        auto Intersect(TOtherCollection&& otherCollection) noexcept(IsNothrowCoroutine)
        {
            std::set<TSource> newCollection;
            auto inner = std::forward<TOtherCollection>(otherCollection);

            while (_yieldContext.top())
            {
                auto element = _yieldContext.top().Next();
                for (auto&& otherElement : inner)
                    if (element == otherElement)
                    {
                        newCollection.insert(std::move(otherElement));
                        break;
                    }
            }

            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(Generator(std::move(newCollection)))>;

            return LinqGenerator<TSource, NothrowNewCoroutine>([this]
                (std::set<TSource>&& newCollection_) noexcept
                { return Generator(std::move(newCollection_)); },
                        std::move(newCollection));
        }

        /// @brief Remove duplicates in a set
        /// @return
        auto Distinct() noexcept(IsNothrowCoroutine)
        requires Concepts::Equatable<TSource>
        {
            std::set<TSource> newCollection;
            while (_yieldContext.top())
                newCollection.insert(_yieldContext.top().Next());

            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(Generator(std::move(newCollection)))>;

            return LinqGenerator<TSource, NothrowNewCoroutine>([this]
                (std::set<TSource>&& newCollection_) noexcept
                { return Generator(std::move(newCollection_)); },
                        std::move(newCollection));
        }

        /// @brief Join two sequences
        /// @tparam TOtherCollection
        /// @param otherCollection
        /// @return
        template<Concepts::ForwardIterable TOtherCollection>
        requires Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
        auto Union(const TOtherCollection& otherCollection) noexcept(IsNothrowCoroutine)
        {
            std::set<TSource> newCollection;

            while (_yieldContext.top())
                newCollection.insert(_yieldContext.top().Next());

            for (const auto& element : otherCollection)
                newCollection.insert(element);

            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(Generator(std::move(newCollection)))>;

            return LinqGenerator<TSource, NothrowNewCoroutine>([this]
                (std::set<TSource>&& newCollection_) noexcept
                { return Generator(std::move(newCollection_)); },
                        std::move(newCollection));
        }

        /// @brief Join two sequences
        /// @tparam TOtherCollection
        /// @param otherCollection
        /// @return
        template<Concepts::InputIterable TOtherCollection>
        requires Concepts::Equatable<TSource> &&
                 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
        auto Union(TOtherCollection otherCollection) noexcept(IsNothrowCoroutine)
        {
            std::set<TSource> newCollection;

            while (_yieldContext.top())
                newCollection.insert(_yieldContext.top().Next());

            for (auto&& element : otherCollection)
                newCollection.insert(std::move(element));

            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(Generator(std::move(newCollection)))>;

            return LinqGenerator<TSource, NothrowNewCoroutine>([this]
                (std::set<TSource>&& newCollection_) noexcept
                { return Generator(std::move(newCollection_)); },
                    std::move(newCollection));
        }

        /// @brief Performs a general aggregation of the elements of the collection depending on the specified expression.
        /// After this method generator became invalid
        /// @tparam TResult
        /// @tparam TAggregate
        /// @param aggregateFunction
        /// @return
        /// @throw std::out_of_range
        template<typename TResult, std::invocable<TResult, TSource> TAggregate>
        TResult Aggregate(TAggregate&& aggregateFunction)
        {
            std::vector<TSource> collection;
            while (_yieldContext.top())
                collection.push_back(_yieldContext.top().Next());

            if (collection.empty())
                throw std::out_of_range("Collection is empty");

            return Aggregate::Aggregate<TResult>(collection.data(), 0, collection.size() - 1,
                std::forward<TAggregate>(aggregateFunction));
        }

        /// @brief Get the number of elements. After this method generator became invalid
        /// @tparam TPredicate
        /// @param predicate
        /// @return
        template<Concepts::IsPredicate<TSource> TPredicate>
        std::size_t Count(TPredicate predicate)
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource> && IsNothrowCoroutine)
        {
            std::size_t result = 0;
            while (_yieldContext.top())
                if (predicate(_yieldContext.top().Next()))
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
            while (_yieldContext.top())
                collection.push_back(_yieldContext.top().Next());

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
        template<std::invocable<TSource> TSelector,
                 Concepts::Addable TResult = std::invoke_result_t<TSelector, TSource>>
        TResult Sum(TSelector&& selector)
        {
            std::vector<TSource> collection;
            while (_yieldContext.top())
                collection.push_back(_yieldContext.Next());

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
            while (_yieldContext)
                collection.push_back(_yieldContext.Next());

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

        /// @brief Find element with the maximum value. After this method generator became invalid
        /// @return
        /// @throw std::out_of_range
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

        /// @brief Find element with the maximum value. After this method generator became invalid
        /// @tparam TSelector
        /// @tparam TResult
        /// @param selector
        /// @return
        /// @throw std::out_of_range
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

        /// @brief Find the average value of the collection. After this method generator became invalid
        /// @return
        /// @throw std::out_of_range
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

        /// @brief Find the average value of the collection. After this method generator became invalid
        /// @tparam TSelector
        /// @tparam TResult
        /// @param selector
        /// @return
        /// @throw std::out_of_range
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

        /// @brief Skips a certain number of elements
        /// @param count
        /// @return
        auto Skip(const std::size_t count) noexcept
        {
            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(SkipGenerator(count))>;

            return LinqGenerator<TSource, NothrowNewCoroutine>([this]
                (const std::size_t count_) noexcept
                { return SkipGenerator(count_); }, count);
        }

        /// @brief Skips a chain of elements, starting with the first element, as long as they satisfy a certain condition
        /// @tparam TPredicate
        /// @param predicate
        /// @return
        template<Concepts::IsPredicate<TSource> TPredicate>
        auto SkipWhile(TPredicate&& predicate) noexcept
        {
            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(SkipWhileGenerator(std::forward<TPredicate>(predicate)))>;

            return LinqGenerator<TSource, NothrowNewCoroutine>([this]
                (TPredicate&& predicate_) noexcept
                { return SkipWhileGenerator(std::forward<TPredicate>(predicate_)); },
                        std::forward<TPredicate>(predicate));
        }

        /// @brief Retrieves a certain number of elements
        /// @param count
        /// @return
        auto Take(const std::size_t count) noexcept
        {
            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(TakeGenerator(count))>;

            return LinqGenerator<TSource, NothrowNewCoroutine>([this]
                (const std::size_t count_) noexcept
                { return TakeGenerator(count_); }, count);
        }

        /// @brief Retrieves a certain number of elements from the end of the collection
        /// @tparam TPredicate
        /// @param predicate
        /// @return
        template<Concepts::IsPredicate<TSource> TPredicate>
        auto TakeWhile(TPredicate&& predicate) noexcept
        {
            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(TakeWhileGenerator(std::forward<TPredicate>(predicate)))>;

            return LinqGenerator<TSource, NothrowNewCoroutine>([this]
                (TPredicate&& predicate_) noexcept
                { return TakeWhileGenerator(std::forward<TPredicate>(predicate_)); },
                        std::forward<TPredicate>(predicate));
        }

        /// @brief Group data by certain parameters
        /// @tparam TKey
        /// @tparam TKeySelector
        /// @param keySelector
        /// @return
        template<std::invocable<TSource> TKeySelector,
                 typename TKey = std::invoke_result_t<TKeySelector, TSource>>
        auto GroupBy(TKeySelector&& keySelector) noexcept
        {
            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(GroupByGenerator(std::forward<TKeySelector>(keySelector)))>;

            return LinqGenerator<std::pair<TKey, std::vector<TSource>>, NothrowNewCoroutine>([this]
                (TKeySelector&& keySelector_) noexcept
                { return GroupByGenerator(std::forward<TKeySelector>(keySelector_)); },
                        std::forward<TKeySelector>(keySelector));
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
        /// @throw std::invalid_argument
        template<Concepts::ForwardIterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, TSource, typename TOtherCollection::value_type>>
        requires std::same_as<std::invoke_result_t<TInnerKeySelector, TSource>,
                              std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
                 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, TSource>>
        auto Join(const TOtherCollection& otherCollection,
                  TInnerKeySelector&& innerKeySelector,
                  TOtherKeySelector&& otherKeySelector,
                  TResultSelector&& resultSelector)
        {
            if (otherCollection.empty())
                throw std::invalid_argument("other collection must not be empty");

            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(JoinGenerator(
                    std::declval<TOtherCollection>(),
                    std::forward<TInnerKeySelector>(innerKeySelector),
                    std::forward<TOtherKeySelector>(otherKeySelector),
                    std::forward<TResultSelector>(resultSelector)))>;

            return LinqGenerator<TResult, NothrowNewCoroutine>([this](
                    TOtherCollection otherCollection_,
                    TInnerKeySelector&& innerKeySelector_,
                    TOtherKeySelector&& otherKeySelector_,
                    TResultSelector&& resultSelector_) noexcept
                { return JoinGenerator(
                    std::move(otherCollection_),
                    std::forward<TInnerKeySelector>(innerKeySelector_),
                    std::forward<TOtherKeySelector>(otherKeySelector_),
                    std::forward<TResultSelector>(resultSelector_)); },
                otherCollection,
                std::forward<TInnerKeySelector>(innerKeySelector),
                std::forward<TOtherKeySelector>(otherKeySelector),
                std::forward<TResultSelector>(resultSelector));
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
        /// @throw std::invalid_argument
        template<Concepts::InputIterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, TSource, typename TOtherCollection::value_type>>
        requires std::same_as<std::invoke_result_t<TInnerKeySelector, TSource>,
                              std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
                 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, TSource>>
        auto Join(TOtherCollection&& otherCollection,
                  TInnerKeySelector&& innerKeySelector,
                  TOtherKeySelector&& otherKeySelector,
                  TResultSelector&& resultSelector)
        {
            if (otherCollection.empty())
                throw std::invalid_argument("other collection must not be empty");

            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(JoinGenerator(
                    std::declval<TOtherCollection>(),
                    std::forward<TInnerKeySelector>(innerKeySelector),
                    std::forward<TOtherKeySelector>(otherKeySelector),
                    std::forward<TResultSelector>(resultSelector)))>;

            return LinqGenerator<TResult, NothrowNewCoroutine>([this](
                    TOtherCollection&& otherCollection_,
                    TInnerKeySelector&& innerKeySelector_,
                    TOtherKeySelector&& otherKeySelector_,
                    TResultSelector&& resultSelector_) noexcept
                { return JoinGenerator(
                    std::forward<TOtherCollection>(otherCollection_),
                    std::forward<TInnerKeySelector>(innerKeySelector_),
                    std::forward<TOtherKeySelector>(otherKeySelector_),
                    std::forward<TResultSelector>(resultSelector_)); },
                std::forward<TOtherCollection>(otherCollection),
                std::forward<TInnerKeySelector>(innerKeySelector),
                std::forward<TOtherKeySelector>(otherKeySelector),
                std::forward<TResultSelector>(resultSelector));
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
        /// @throw std::invalid_argument
        template<Concepts::ForwardIterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, TSource>,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<const std::vector<TSource>&, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>>
        requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>>
        auto GroupJoin(const TOtherCollection& otherCollection,
                       TInnerKeySelector&& innerKeySelector,
                       TOtherKeySelector&& otherKeySelector,
                       TResultSelector&& resultSelector)
        {
            if (otherCollection.empty())
                throw std::invalid_argument("other collection must not be empty");

            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(GroupJoinGenerator(
                    std::declval<TOtherCollection>(),
                    std::forward<TInnerKeySelector>(innerKeySelector),
                    std::forward<TOtherKeySelector>(otherKeySelector),
                    std::forward<TResultSelector>(resultSelector)))>;

            return LinqGenerator<TResult, NothrowNewCoroutine>([this](
                    TOtherCollection otherCollection_,
                    TInnerKeySelector&& innerKeySelector_,
                    TOtherKeySelector&& otherKeySelector_,
                    TResultSelector&& resultSelector_) noexcept
                { return GroupJoinGenerator(
                    std::move(otherCollection_),
                    std::forward<TInnerKeySelector>(innerKeySelector_),
                    std::forward<TOtherKeySelector>(otherKeySelector_),
                    std::forward<TResultSelector>(resultSelector_)); },
                otherCollection,
                std::forward<TInnerKeySelector>(innerKeySelector),
                std::forward<TOtherKeySelector>(otherKeySelector),
                std::forward<TResultSelector>(resultSelector));
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
        /// @throw std::invalid_argument
        template<Concepts::InputIterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, TSource>,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<const std::vector<TSource>&, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>>
        requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>>
        auto GroupJoin(TOtherCollection&& otherCollection,
                       TInnerKeySelector&& innerKeySelector,
                       TOtherKeySelector&& otherKeySelector,
                       TResultSelector&& resultSelector)
        {
            if (otherCollection.empty())
                throw std::invalid_argument("other collection must not be empty");

            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(GroupJoinGenerator(
                    std::declval<TOtherCollection>(),
                    std::forward<TInnerKeySelector>(innerKeySelector),
                    std::forward<TOtherKeySelector>(otherKeySelector),
                    std::forward<TResultSelector>(resultSelector)))>;

            return LinqGenerator<TResult, NothrowNewCoroutine>([this](
                    TOtherCollection&& otherCollection_,
                    TInnerKeySelector&& innerKeySelector_,
                    TOtherKeySelector&& otherKeySelector_,
                    TResultSelector&& resultSelector_) noexcept
                { return GroupJoinGenerator(
                    std::forward<TOtherCollection>(otherCollection_),
                    std::forward<TInnerKeySelector>(innerKeySelector_),
                    std::forward<TOtherKeySelector>(otherKeySelector_),
                    std::forward<TResultSelector>(resultSelector_)); },
                std::forward<TOtherCollection>(otherCollection),
                std::forward<TInnerKeySelector>(innerKeySelector),
                std::forward<TOtherKeySelector>(otherKeySelector),
                std::forward<TResultSelector>(resultSelector));
        }

        /// @brief Sequentially concatenates the corresponding elements of the current sequence with the second sequence
        /// @tparam TOtherCollection
        /// @tparam TOtherCollectionValueType
        /// @param otherCollection
        /// @return
        template<typename TOtherCollection,
                 typename TOtherCollectionValueType = typename TOtherCollection::value_type>
        requires Concepts::ForwardIterable<TOtherCollection> && Concepts::HasSize<TOtherCollection>
        auto Zip(const TOtherCollection& otherCollection) noexcept
        {
            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(ZipGenerator(std::declval<TOtherCollection>()))>;

            return LinqGenerator<std::pair<TSource, TOtherCollectionValueType>, NothrowNewCoroutine>([this]
                (TOtherCollection otherCollection_) noexcept
                { return ZipGenerator(std::move(otherCollection_)); },
                    otherCollection);
        }

        /// @brief Sequentially concatenates the corresponding elements of the current sequence with the second sequence
        /// @tparam TOtherCollection
        /// @tparam TOtherCollectionValueType
        /// @param otherCollection
        /// @return
        template<typename TOtherCollection,
                 typename TOtherCollectionValueType = typename TOtherCollection::value_type>
        requires Concepts::InputIterable<TOtherCollection> && Concepts::HasSize<TOtherCollection>
        auto Zip(TOtherCollection&& otherCollection) noexcept
        {
            using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
                noexcept(ZipGenerator(std::declval<TOtherCollection>()))>;

            return LinqGenerator<std::pair<TSource, TOtherCollectionValueType>, NothrowNewCoroutine>([this]
                (TOtherCollection&& otherCollection_) noexcept
                { return ZipGenerator(std::forward<TOtherCollection>(otherCollection_)); },
                    std::forward<TOtherCollection>(otherCollection));
        }

        /// @brief Checks if all elements match a condition. If all elements match the condition, then true is returned.
        /// After this method generator became invalid
        /// @tparam TPredicate
        /// @param predicate
        /// @return
        template<Concepts::IsPredicate<TSource> TPredicate>
        bool All(TPredicate predicate)
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource> && IsNothrowCoroutine)
        {
            while (_yieldContext)
                if (!predicate(_yieldContext.Next()))
                    return false;
            return true;
        }

        /// @brief Returns true if at least one element of the collection meets a certain condition.
        /// After this method generator became invalid
        /// @tparam TPredicate
        /// @param predicate
        /// @return
        template<Concepts::IsPredicate<TSource> TPredicate>
        bool Any(TPredicate predicate)
        noexcept(std::is_nothrow_invocable_v<TPredicate, TSource> && IsNothrowCoroutine)
        {
            while (_yieldContext)
                if (predicate(_yieldContext.Next()))
                    return true;
            return false;
        }

    private:
        Future<TSource, std::integral_constant<bool, IsNothrowCoroutine>>
        SkipGenerator(const std::size_t count) noexcept(IsNothrowCoroutine)
        {
            std::size_t i = 0;
            while (_yieldContext && i < count)
            {
                _yieldContext.Next();
                ++i;
            }

            while (_yieldContext)
                co_yield _yieldContext.Next();
            co_return;
        }

        template<Concepts::IsPredicate<TSource> TPredicate,
                 bool IsNothrowNewCoroutine = IsNothrowCoroutine && std::is_nothrow_invocable_v<TPredicate, TSource>>
        Future<TSource, std::integral_constant<bool, IsNothrowNewCoroutine>>
        SkipWhileGenerator(TPredicate predicate) noexcept(IsNothrowNewCoroutine)
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
            co_return;
        }

        Future<TSource, std::integral_constant<bool, IsNothrowCoroutine>>
        TakeGenerator(const std::size_t count) noexcept(IsNothrowCoroutine)
        {
            std::size_t i = 0;
            while (_yieldContext && i < count)
            {
                co_yield _yieldContext.Next();
                ++i;
            }

            while (_yieldContext)
                _yieldContext.Next();
            co_return;
        }

        template<Concepts::IsPredicate<TSource> TPredicate,
                 bool IsNothrowNewCoroutine = IsNothrowCoroutine && std::is_nothrow_invocable_v<TPredicate, TSource>>
        Future<TSource, std::integral_constant<bool, IsNothrowNewCoroutine>>
        TakeWhileGenerator(TPredicate predicate) noexcept(IsNothrowNewCoroutine)
        {
            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                if (!predicate(element))
                    break;
                co_yield element;
            }

            while (_yieldContext)
                _yieldContext.Next();
            co_return;
        }

        template<std::invocable<TSource> TKeySelector,
                 typename TKey = std::invoke_result_t<TKeySelector, TSource>,
                 bool IsNothrowNewCoroutine = IsNothrowCoroutine && std::is_nothrow_invocable_v<TKeySelector, TSource>>
        Future<std::pair<TKey, std::vector<TSource>>, std::integral_constant<bool, IsNothrowNewCoroutine>>
        GroupByGenerator(TKeySelector keySelector) noexcept(IsNothrowNewCoroutine)
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
            co_return;
        }

        template<Concepts::InputIterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, TSource, typename TOtherCollection::value_type>,
                 bool IsNothrowNewCoroutine =
                        std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
                        std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
                        std::is_nothrow_invocable_v<TResultSelector, TSource, typename TOtherCollection::value_type> &&
                        IsNothrowCoroutine && noexcept(++std::declval<TOtherCollection>().begin())>
        requires std::same_as<std::invoke_result_t<TInnerKeySelector, TSource>,
                              std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
                 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, TSource>>
        Future<TResult, std::integral_constant<bool, IsNothrowNewCoroutine>>
        JoinGenerator(TOtherCollection&& otherCollection,
                      TInnerKeySelector innerKeySelector,
                      TOtherKeySelector otherKeySelector,
                      TResultSelector resultSelector) noexcept(IsNothrowNewCoroutine)
        {
            auto inner = std::forward<TOtherCollection>(otherCollection);
            while (_yieldContext)
            {
                auto element = _yieldContext.Next();
                for (auto&& otherElement : inner)
                    if (innerKeySelector(element) == otherKeySelector(otherElement))
                        co_yield resultSelector(std::move(element), std::move(otherElement));
            }
            co_return;
        }

        template<Concepts::InputIterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, TSource>,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<const std::vector<TSource>&, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>,
                 bool IsNothrowNewCoroutine =
                        std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
                        std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
                        std::is_nothrow_invocable_v<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type> &&
                        IsNothrowCoroutine && noexcept(++std::declval<TOtherCollection>().begin())>
        requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>>
        Future<TResult, std::integral_constant<bool, IsNothrowNewCoroutine>>
        GroupJoinGenerator(TOtherCollection&& otherCollection,
                           TInnerKeySelector&& innerKeySelector,
                           TOtherKeySelector otherKeySelector,
                           TResultSelector resultSelector) noexcept(IsNothrowNewCoroutine)
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
            co_return;
        }

        template<Concepts::ForwardIterable TOtherCollection,
                 typename TOtherCollectionValueType = typename TOtherCollection::value_type,
                 bool IsNothrowNewCoroutine = IsNothrowCoroutine && noexcept(++std::declval<TOtherCollection>().begin())>
        requires Concepts::InputIterable<TOtherCollection> && Concepts::HasSize<TOtherCollection>
        Future<std::pair<TSource, TOtherCollectionValueType>, std::integral_constant<bool, IsNothrowNewCoroutine>>
        ZipGenerator(TOtherCollection&& otherCollection) noexcept(IsNothrowNewCoroutine)
        {
            auto inner = std::forward<TOtherCollection>(otherCollection);
            for (auto&& element : inner)
                if (_yieldContext)
                    co_yield std::make_pair(_yieldContext.Next(), std::move(element));
            co_return;
        }
    };
}
*/

#endif