#ifndef LINQ_LinqView_H
#define LINQ_LinqView_H

#include <ExtendedCpp/LINQ/Iterators.h>

namespace ExtendedCpp::LINQ
{
    template<Concepts::OptionalIter TIterator>
    class LinqView final
    {
    private:
        TIterator _begin;
        TIterator _end;

    public:
        using TSource = TIterator::value_type;
        using value_type = TIterator::value_type;
        using iterator = TIterator;
        using const_iterator = TIterator;

        LinqView(const TIterator begin, const TIterator end) noexcept
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
            {
                std::optional<TSource> opt = *it;
                if (opt.has_value())
                    collection.push_back(opt.value());
            }
            return collection;
        }

        template<std::size_t SIZE>
        std::array<TSource, SIZE> ToArray()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::array<TSource, SIZE> array;
            std::size_t i = 0;
            for (TIterator it = _begin; it != _end && i < SIZE; ++it, ++i)
            {
                std::optional<TSource> opt = *it;
                if (opt.has_value())
                    array[i] = opt.value();
            }
            return array;
        }

        [[nodiscard]]
        std::list<TSource> ToList()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::list<TSource> collection;
            for (TIterator it = _begin; it != _end; ++it)
            {
                std::optional<TSource> opt = *it;
                if (opt.has_value())
                    collection.push_back(opt.value());
            }
            return collection;
        }

        [[nodiscard]]
        std::forward_list<TSource> ToForwardList()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::forward_list<TSource> collection;
            for (TIterator it = _begin; it != _end; ++it)
            {
                std::optional<TSource> opt = *it;
                if (opt.has_value())
                    collection.push_front(opt.value());
            }
            return collection;
        }

        [[nodiscard]]
        std::stack<TSource> ToStack()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::stack<TSource> stack;
            for (TIterator it = _begin; it != _end; ++it)
            {
                std::optional<TSource> opt = *it;
                if (opt.has_value())
                    stack.push(opt.value());
            }
            return stack;
        }

        [[nodiscard]]
        std::queue<TSource> ToQueue()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::queue<TSource> queue;
            for (TIterator it = _begin; it != _end; ++it)
            {
                std::optional<TSource> opt = *it;
                if (opt.has_value())
                    queue.push(opt.value());
            }
            return queue;
        }

        [[nodiscard]]
        std::deque<TSource> ToDeque()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::deque<TSource> deque;
            for (TIterator it = _begin; it != _end; ++it)
            {
                std::optional<TSource> opt = *it;
                if (opt.has_value())
                    deque.push_back(opt.value());
            }
            return deque;
        }

        [[nodiscard]]
        std::priority_queue<TSource> ToPriorityQueue()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::priority_queue<TSource> priorityQueue;
            for (TIterator it = _begin; it != _end; ++it)
            {
                std::optional<TSource> opt = *it;
                if (opt.has_value())
                    priorityQueue.push(opt.value());
            }
            return priorityQueue;
        }

        std::set<TSource> ToSet()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::set<TSource> set;
            for (TIterator it = _begin; it != _end; ++it)
            {
                std::optional<TSource> opt = *it;
                if (opt.has_value())
                    set.insert(opt.value());
            }
            return set;
        }

        std::unordered_set<TSource> ToUnorderedSet()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
        {
            std::unordered_set<TSource> unorderedSet;
            for (TIterator it = _begin; it != _end; ++it)
            {
                std::optional<TSource> opt = *it;
                if (opt.has_value())
                    unorderedSet.insert(opt.value());
            }
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
            {
                std::optional<std::pair<TKey, TValue>> opt = *it;
                if (opt.has_value())
                    map.insert(opt.value());
            }
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
            {
                std::optional<std::pair<TKey, TValue>> opt = *it;
                if (opt.has_value())
                    unorderedMap.insert(opt.value());
            }
            return unorderedMap;
        }

        template<std::invocable<TSource> TSelector, typename TResult = std::invoke_result_t<TSelector, TSource>>
        LinqView<SelectorIterator<TResult, TIterator, TSelector>> Select(TSelector&& selector) const noexcept
        {
            return LinqView<SelectorIterator<TResult, TIterator, TSelector>>(
                    SelectorIterator<TResult, TIterator, TSelector>(_begin, std::forward<TSelector>(selector)),
                    SelectorIterator<TResult, TIterator, TSelector>(_end, std::forward<TSelector>(selector)));
        }

        template<typename TPredicate>
        requires Concepts::IsPredicate<TPredicate, TSource>
        LinqView<WhereIterator<TIterator, TPredicate>> Where(TPredicate&& predicate) const noexcept
        {
            return LinqView<WhereIterator<TIterator, TPredicate>>(
                    WhereIterator<TIterator, TPredicate>(_begin, std::forward<TPredicate>(predicate)),
                    WhereIterator<TIterator, TPredicate>(_end, std::forward<TPredicate>(predicate)));
        }

        template<typename TPredicate>
        requires Concepts::IsPredicate<TPredicate, TSource>
        LinqView<RemoveWhereIterator<TIterator, TPredicate>> RemoveWhere(TPredicate&& predicate) const noexcept
        {
            return LinqView<RemoveWhereIterator<TIterator, TPredicate>>(
                    RemoveWhereIterator<TIterator, TPredicate>(_begin, std::forward<TPredicate>(predicate)),
                    RemoveWhereIterator<TIterator, TPredicate>(_end, std::forward<TPredicate>(predicate)));
        }

        template<Concepts::ConstIterable TOtherCollection,
                 std::invocable<TSource> TInnerKeySelector,
                 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
                 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector>
        requires std::same_as<std::invoke_result_t<TInnerKeySelector, TSource>,
                              std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
                 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, TSource>>
        LinqView<JoinIterator<TIterator, TOtherCollection, TInnerKeySelector,
                              TOtherKeySelector, TResultSelector>> Join(const TOtherCollection& otherCollection,
                                                                        TInnerKeySelector&& innerKeySelector,
                                                                        TOtherKeySelector&& otherKeySelector,
                                                                        TResultSelector&& resultSelector) const noexcept
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