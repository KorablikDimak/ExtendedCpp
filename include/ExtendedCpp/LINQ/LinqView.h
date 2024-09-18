#ifndef LINQ_LinqView_H
#define LINQ_LinqView_H

#include <ExtendedCpp/LINQ/Concepts.h>

namespace ExtendedCpp::LINQ
{
    template<Concepts::OptionalIter TIterator>
    class LinqView final
    {
    private:
        TIterator _begin;
        TIterator _end;

        template<typename TOut, Concepts::OptionalIter TInIterator, std::invocable<typename TInIterator::value_type> TSelector>
        requires std::same_as<TOut, typename FunctorTraits<TSelector(typename TInIterator::value_type)>::ReturnType>
        struct SelectorIterator
        {
        private:
            TInIterator _inIterator;
            TSelector _selector;

        public:
            using value_type = TOut;

            SelectorIterator(TInIterator inIterator, TSelector&& selector) noexcept :
                _inIterator(inIterator),
                _selector(std::forward<TSelector>(selector)) {}

            std::optional<TOut> operator*()
            const noexcept(std::is_nothrow_invocable_v<TSelector, typename TInIterator::value_type> &&
                           std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
            {
                if ((*_inIterator).has_value())
                    return _selector((*_inIterator).value());
                else
                    return std::nullopt;
            }

            SelectorIterator& operator++() noexcept
            {
                ++_inIterator;
                return *this;
            }

            bool operator!=(const SelectorIterator& other) const noexcept
            {
                return _inIterator != other._inIterator;
            }
        };

        template<Concepts::OptionalIter TInIterator, Concepts::IsPredicate<typename TInIterator::value_type> TPredicate>
        struct WhereIterator
        {
        private:
            TInIterator _inIterator;
            TPredicate _predicate;

        public:
            using value_type = TInIterator::value_type;

            WhereIterator(TInIterator inIterator, TPredicate&& predicate) noexcept :
                _inIterator(inIterator),
                _predicate(std::forward<TPredicate>(predicate)) {}

            std::optional<value_type> operator*()
            const noexcept(std::is_nothrow_invocable_v<TPredicate, typename TInIterator::value_type> &&
                           std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
            {
                if ((*_inIterator).has_value() && _predicate((*_inIterator).value()))
                    return (*_inIterator).value();
                else
                    return std::nullopt;
            }

            WhereIterator& operator++() noexcept
            {
                ++_inIterator;
                return *this;
            }

            bool operator!=(const WhereIterator& other) const noexcept
            {
                return _inIterator != other._inIterator;
            }
        };

        template<Concepts::OptionalIter TInIterator, Concepts::IsPredicate<typename TInIterator::value_type> TPredicate>
        struct RemoveWhereIterator
        {
        private:
            TInIterator _inIterator;
            TPredicate _predicate;

        public:
            using value_type = TInIterator::value_type;

            RemoveWhereIterator(TInIterator inIterator, TPredicate&& predicate) noexcept :
                _inIterator(inIterator),
                _predicate(std::forward<TPredicate>(predicate)) {}

            std::optional<value_type> operator*()
            const noexcept(std::is_nothrow_invocable_v<TPredicate, typename TInIterator::value_type> &&
                           std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
            {
                if ((*_inIterator).has_value() && !_predicate((*_inIterator).value()))
                    return (*_inIterator).value();
                else
                    return std::nullopt;
            }

            RemoveWhereIterator& operator++() noexcept
            {
                ++_inIterator;
                return *this;
            }

            bool operator!=(const RemoveWhereIterator& other) const noexcept
            {
                return _inIterator != other._inIterator;
            }
        };

        template<Concepts::OptionalIter TInIterator,
                 Concepts::ConstIterable TOtherCollection,
                 std::invocable<typename TInIterator::value_type> TInnerKeySelector,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<typename TInIterator::value_type, typename TOtherCollection::value_type> TResultSelector,
                 typename TResult = typename FunctorTraits<TResultSelector(typename TInIterator::value_type,
                                                                           typename TOtherCollection::value_type)>::ReturnType>
        requires std::same_as<typename FunctorTraits<TInnerKeySelector(typename TInIterator::value_type)>::ReturnType,
                              typename FunctorTraits<TOtherKeySelector(typename TOtherCollection::value_type)>::ReturnType> &&
                 Concepts::Equatable<typename FunctorTraits<TInnerKeySelector(typename TInIterator::value_type)>::ReturnType>
        struct JoinIterator
        {
        private:
            TInIterator _inIterator;
            TOtherCollection _otherCollection;
            using OtherIterator = TOtherCollection::iterator;
            OtherIterator _otherIterator;
            TInnerKeySelector _innerKeySelector;
            TOtherKeySelector _otherKeySelector;
            TResultSelector _resultSelector;

        public:
            using value_type = TResult;

            JoinIterator(TInIterator inIterator,
                         const TOtherCollection& otherCollection,
                         TInnerKeySelector&& innerKeySelector,
                         TOtherKeySelector&& otherKeySelector,
                         TResultSelector&& resultSelector) noexcept :
                _inIterator(inIterator),
                _otherCollection(otherCollection),
                _otherIterator(_otherCollection.begin()),
                _innerKeySelector(std::forward<TInnerKeySelector>(innerKeySelector)),
                _otherKeySelector(std::forward<TOtherKeySelector>(otherKeySelector)),
                _resultSelector(std::forward<TResultSelector>(resultSelector)) {}

            std::optional<TResult> operator*()
            const noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, typename TInIterator::value_type> &&
                           std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
                           std::is_nothrow_invocable_v<TResultSelector, typename TInIterator::value_type,
                                                                        typename TOtherCollection::value_type> &&
                           std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
            {
                if ((*_inIterator).has_value())
                {
                    if (_innerKeySelector((*_inIterator).value()) == _otherKeySelector(*_otherIterator))
                        return _resultSelector((*_inIterator).value(), *_otherIterator);
                    else
                        return std::nullopt;
                }
                else
                    return std::nullopt;
            }

            JoinIterator& operator++() noexcept
            {
                if (_otherIterator != _otherCollection.end())
                {
                    ++_otherIterator;
                    return *this;
                }
                else
                {
                    _otherIterator = _otherCollection.begin();
                    ++_inIterator;
                    return *this;
                }
            }

            bool operator!=(const JoinIterator& other) const noexcept
            {
                return _inIterator != other._inIterator;
            }
        };

    public:
        using TSource = TIterator::value_type;
        using value_type = TIterator::value_type;
        using iterator = TIterator;
        using const_iterator = TIterator;

        LinqView(TIterator begin, TIterator end) noexcept
            : _begin(begin), _end(end) {}

        TIterator begin() const noexcept
        {
            return _begin;
        }

        TIterator cbegin() const noexcept
        {
            return _begin;
        }

        TIterator end() const noexcept
        {
            return _end;
        }

        TIterator cend() const noexcept
        {
            return _end;
        }

        [[nodiscard]]
        std::vector<TSource> ToVector()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::vector<TSource> collection;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    collection.push_back((*it).value());
            return collection;
        }

        template<std::size_t SIZE>
        std::array<TSource, SIZE> ToArray()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::array<TSource, SIZE> array;
            std::size_t i = 0;
            for (TIterator it = _begin; it != _end && i < SIZE; ++it, ++i)
                if ((*it).has_value())
                    array[i] = (*it).value();
            return array;
        }

        [[nodiscard]]
        std::list<TSource> ToList()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::list<TSource> collection;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    collection.push_back((*it).value());
            return collection;
        }

        [[nodiscard]]
        std::forward_list<TSource> ToForwardList()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::forward_list<TSource> collection;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    collection.push_front((*it).value());
            return collection;
        }

        [[nodiscard]]
        std::stack<TSource> ToStack()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::stack<TSource> stack;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    stack.push((*it).value());
            return stack;
        }

        [[nodiscard]]
        std::queue<TSource> ToQueue()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::queue<TSource> queue;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    queue.push((*it).value());
            return queue;
        }

        [[nodiscard]]
        std::deque<TSource> ToDeque()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::deque<TSource> deque;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    deque.push_back((*it).value());
            return deque;
        }

        [[nodiscard]]
        std::priority_queue<TSource> ToPriorityQueue()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::priority_queue<TSource> priorityQueue;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    priorityQueue.push((*it).value());
            return priorityQueue;
        }

        std::set<TSource> ToSet()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::set<TSource> set;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    set.insert((*it).value());
            return set;
        }

        std::unordered_set<TSource> ToUnorderedSet()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::unordered_set<TSource> unorderedSet;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    unorderedSet.insert((*it).value());
            return unorderedSet;
        }

        template<typename TKey = typename PairTraits<TSource>::FirstType,
                 typename TValue = typename PairTraits<TSource>::SecondType>
        requires Concepts::IsPair<TSource>
        std::map<TKey, TValue> ToMap()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::map<TKey, TValue> map;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    map.insert((*it).value());
            return map;
        }

        template<typename TKey = typename PairTraits<TSource>::FirstType,
                 typename TValue = typename PairTraits<TSource>::SecondType>
        requires Concepts::IsPair<TSource>
        std::unordered_map<TKey, TValue> ToUnorderedMap()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::unordered_map<TKey, TValue> unorderedMap;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    unorderedMap.insert((*it).value());
            return unorderedMap;
        }

        template<std::invocable<TSource> TSelector, typename TResult = typename FunctorTraits<TSelector(TSource)>::ReturnType>
        LinqView<SelectorIterator<TResult, TIterator, TSelector>> Select(TSelector&& selector)
        const noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
        {
            return LinqView<SelectorIterator<TResult, TIterator, TSelector>>(
                    SelectorIterator<TResult, TIterator, TSelector>(_begin, std::forward<TSelector>(selector)),
                    SelectorIterator<TResult, TIterator, TSelector>(_end, std::forward<TSelector>(selector)));
        }

        template<typename TPredicate>
        requires Concepts::IsPredicate<TPredicate, TSource>
        LinqView<WhereIterator<TIterator, TPredicate>> Where(TPredicate&& predicate)
        const noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            return LinqView<WhereIterator<TIterator, TPredicate>>(
                    WhereIterator<TIterator, TPredicate>(_begin, std::forward<TPredicate>(predicate)),
                    WhereIterator<TIterator, TPredicate>(_end, std::forward<TPredicate>(predicate)));
        }

        template<typename TPredicate>
        requires Concepts::IsPredicate<TPredicate, TSource>
        LinqView<RemoveWhereIterator<TIterator, TPredicate>> RemoveWhere(TPredicate&& predicate)
        const noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
        {
            return LinqView<RemoveWhereIterator<TIterator, TPredicate>>(
                    RemoveWhereIterator<TIterator, TPredicate>(_begin, std::forward<TPredicate>(predicate)),
                    RemoveWhereIterator<TIterator, TPredicate>(_end, std::forward<TPredicate>(predicate)));
        }

        template<Concepts::ConstIterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector>
        requires std::same_as<typename FunctorTraits<TInnerKeySelector(TSource)>::ReturnType,
                              typename FunctorTraits<TOtherKeySelector(typename TOtherCollection::value_type)>::ReturnType> &&
                 Concepts::Equatable<typename FunctorTraits<TInnerKeySelector(TSource)>::ReturnType>
        LinqView<JoinIterator<TIterator, TOtherCollection, TInnerKeySelector,
                              TOtherKeySelector, TResultSelector>> Join(const TOtherCollection& otherCollection,
                                                                                 TInnerKeySelector&& innerKeySelector,
                                                                                 TOtherKeySelector&& otherKeySelector,
                                                                                 TResultSelector&& resultSelector)
        const noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
                       std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
                       std::is_nothrow_invocable_v<TResultSelector, TSource, typename TOtherCollection::value_type>)
        {
            return LinqView<JoinIterator<TIterator, TOtherCollection, TInnerKeySelector,
                                         TOtherKeySelector, TResultSelector>>(
                    JoinIterator<TIterator, TOtherCollection, TInnerKeySelector, TOtherKeySelector, TResultSelector>(
                            _begin,
                            otherCollection,
                            std::forward<TInnerKeySelector>(innerKeySelector),
                            std::forward<TOtherKeySelector>(otherKeySelector),
                            std::forward<TResultSelector>(resultSelector)),
                    JoinIterator<TIterator, TOtherCollection, TInnerKeySelector, TOtherKeySelector, TResultSelector>(
                            _end,
                            otherCollection,
                            std::forward<TInnerKeySelector>(innerKeySelector),
                            std::forward<TOtherKeySelector>(otherKeySelector),
                            std::forward<TResultSelector>(resultSelector)));
        }
    };
}

#endif