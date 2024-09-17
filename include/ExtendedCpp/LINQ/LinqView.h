#ifndef LINQ_LinqView_H
#define LINQ_LinqView_H

#include <ExtendedCpp/LINQ/Concepts.h>

namespace ExtendedCpp::LINQ
{
    template<typename TIterator>
    class LinqView final
    {
    private:
        TIterator _begin;
        TIterator _end;

        template<typename TOut, typename TInIterator, typename TSelector>
        requires Concepts::IsFunctor<TSelector, typename TInIterator::value_type> &&
                 std::same_as<TOut, typename FunctorTraits<TSelector(typename TInIterator::value_type)>::ReturnType>
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
                const noexcept(std::is_nothrow_invocable_v<TSelector, typename TInIterator::value_type>)
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

        template<typename TInIterator, typename TPredicate>
        requires Concepts::IsPredicate<TPredicate, typename TInIterator::value_type>
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
                noexcept(std::is_nothrow_invocable_v<TPredicate, typename TInIterator::value_type>)
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
        std::vector<TSource> ToVector() const noexcept
        {
            std::vector<TSource> collection;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    collection.push_back((*it).value());
            return collection;
        }

        template<std::size_t SIZE>
        std::array<TSource, SIZE> ToArray() const noexcept
        {
            std::array<TSource, SIZE> array;
            std::size_t i = 0;
            for (TIterator it = _begin; it != _end && i < SIZE; ++it, ++i)
                if ((*it).has_value())
                    array[i] = (*it).value();
            return array;
        }

        [[nodiscard]]
        std::list<TSource> ToList() const noexcept
        {
            std::list<TSource> collection;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    collection.push_back((*it).value());
            return collection;
        }

        [[nodiscard]]
        std::forward_list<TSource> ToForwardList() const noexcept
        {
            std::forward_list<TSource> collection;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    collection.push_front((*it).value());
            return collection;
        }

        [[nodiscard]]
        std::stack<TSource> ToStack() const noexcept
        {
            std::stack<TSource> stack;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    stack.push((*it).value());
            return stack;
        }

        [[nodiscard]]
        std::queue<TSource> ToQueue() const noexcept
        {
            std::queue<TSource> queue;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    queue.push((*it).value());
            return queue;
        }

        [[nodiscard]]
        std::deque<TSource> ToDeque() const noexcept
        {
            std::deque<TSource> deque;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    deque.push_back((*it).value());
            return deque;
        }

        [[nodiscard]]
        std::priority_queue<TSource> ToPriorityQueue() const noexcept
        {
            std::priority_queue<TSource> priorityQueue;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    priorityQueue.push((*it).value());
            return priorityQueue;
        }

        std::set<TSource> ToSet() const noexcept
        {
            std::set<TSource> set;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    set.insert((*it).value());
            return set;
        }

        std::unordered_set<TSource> ToUnorderedSet() const noexcept
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
        std::map<TKey, TValue> ToMap() const noexcept
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
        std::unordered_map<TKey, TValue> ToUnorderedMap() const noexcept
        {
            std::unordered_map<TKey, TValue> unorderedMap;
            for (TIterator it = _begin; it != _end; ++it)
                if ((*it).has_value())
                    unorderedMap.insert((*it).value());
            return unorderedMap;
        }

        template<typename TSelector, typename TResult = typename FunctorTraits<TSelector(TSource)>::ReturnType>
        requires Concepts::IsFunctor<TSelector, TSource>
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
        decltype(auto) RemoveWhere(TPredicate&& predicate) const noexcept
        {
            auto innerredicate =
                    [predicate = std::forward<TPredicate>(predicate)](const TSource& element)
                    { return !predicate(element); };

            return LinqView<WhereIterator<TIterator, decltype(innerredicate)>>(
                    WhereIterator<TIterator, decltype(innerredicate)>(_begin, std::move(innerredicate)),
                    WhereIterator<TIterator, decltype(innerredicate)>(_end, std::move(innerredicate)));
        }
    };
}

#endif