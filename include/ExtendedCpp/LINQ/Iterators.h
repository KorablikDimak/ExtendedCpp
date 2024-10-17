#ifndef LINQ_Iterators_H
#define LINQ_Iterators_H

#include <ExtendedCpp/LINQ/Concepts.h>
#include <ExtendedCpp/LINQ/TypeTraits.h>

namespace ExtendedCpp::LINQ
{
    template<typename TInIterator>
    struct OptionalIterator final
    {
    private:
        TInIterator _inIterator;

    public:
        using value_type = TInIterator::value_type;

        explicit OptionalIterator(TInIterator inIterator) noexcept :
                _inIterator(inIterator) {}

        std::optional<value_type> operator*()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
        {
            return *_inIterator;
        }

        OptionalIterator& operator++() noexcept
        {
            ++_inIterator;
            return *this;
        }

        bool operator!=(const OptionalIterator& other) const noexcept
        {
            return _inIterator != other._inIterator;
        }

        bool operator==(const OptionalIterator& other) const noexcept
        {
            return _inIterator == other._inIterator;
        }
    };

    template<typename TOut,
             Concepts::OptionalIter TInIterator,
             std::invocable<typename TInIterator::value_type> TSelector>
    requires std::same_as<TOut, std::invoke_result_t<TSelector, typename TInIterator::value_type>>
    struct SelectorIterator final
    {
    private:
        TInIterator _inIterator;
        TSelector _selector;

    public:
        using value_type = TOut;

        SelectorIterator(TInIterator inIterator, TSelector&& selector) noexcept :
                _inIterator(inIterator),
                _selector(std::forward<TSelector>(selector)) {}

        std::optional<value_type> operator*()
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

        bool operator==(const SelectorIterator& other) const noexcept
        {
            return _inIterator == other._inIterator;
        }
    };

    template<Concepts::OptionalIter TInIterator,
             Concepts::IsPredicate<typename TInIterator::value_type> TPredicate>
    struct WhereIterator final
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

        bool operator==(const WhereIterator& other) const noexcept
        {
            return _inIterator == other._inIterator;
        }
    };

    template<Concepts::OptionalIter TInIterator,
             Concepts::IsPredicate<typename TInIterator::value_type> TPredicate>
    struct RemoveWhereIterator final
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

        bool operator==(const RemoveWhereIterator& other) const noexcept
        {
            return _inIterator == other._inIterator;
        }
    };

    template<Concepts::OptionalIter TInIterator,
             typename TKeySelector,
             typename TValue = TInIterator::value_type,
             typename TKey = std::invoke_result_t<TKeySelector, TValue>>
    requires std::invocable<TKeySelector, TValue>
    struct GroupByIterator final
    {
    private:


    public:
        using value_type = std::pair<std::invoke_result_t<TKeySelector, TValue>, std::vector<TValue>>;
    };


    template<Concepts::OptionalIter TInIterator,
             Concepts::ConstIterable TOtherCollection,
             std::invocable<typename TInIterator::value_type> TInnerKeySelector,
             std::invocable<typename std::decay_t<TOtherCollection>::value_type> TOtherKeySelector,
             std::invocable<typename TInIterator::value_type,
                            typename std::decay_t<TOtherCollection>::value_type> TResultSelector,
             typename TResult = std::invoke_result_t<TResultSelector,
                                                     typename TInIterator::value_type,
                                                     typename std::decay_t<TOtherCollection>::value_type>>
    requires std::same_as<std::invoke_result_t<TInnerKeySelector, typename TInIterator::value_type>,
                          std::invoke_result_t<TOtherKeySelector, typename std::decay_t<TOtherCollection>::value_type>> &&
             Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, typename TInIterator::value_type>>
    struct JoinIterator final
    {
    private:
        TInIterator _inIterator;
        TOtherCollection _otherCollection;
        TInnerKeySelector _innerKeySelector;
        TOtherKeySelector _otherKeySelector;
        TResultSelector _resultSelector;
        std::size_t _otherPosition = 0;

    public:
        using value_type = TResult;

        JoinIterator(TInIterator inIterator,
                     const TOtherCollection& otherCollection,
                     TInnerKeySelector&& innerKeySelector,
                     TOtherKeySelector&& otherKeySelector,
                     TResultSelector&& resultSelector) noexcept :
                _inIterator(inIterator),
                _otherCollection(otherCollection),
                _innerKeySelector(std::forward<TInnerKeySelector>(innerKeySelector)),
                _otherKeySelector(std::forward<TOtherKeySelector>(otherKeySelector)),
                _resultSelector(std::forward<TResultSelector>(resultSelector)) {}

        std::optional<value_type> operator*()
        const noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, typename TInIterator::value_type> &&
                       std::is_nothrow_invocable_v<TOtherKeySelector, typename std::decay_t<TOtherCollection>::value_type> &&
                       std::is_nothrow_invocable_v<TResultSelector, typename TInIterator::value_type,
                                                                    typename std::decay_t<TOtherCollection>::value_type> &&
                       std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
        {
            const auto& opt = *_inIterator;
            if (!opt.has_value() || (_otherCollection.cbegin() + _otherPosition) == _otherCollection.cend())
                return std::nullopt;
            if (_innerKeySelector(opt.value()) == _otherKeySelector(*(_otherCollection.cbegin() + _otherPosition)))
                return _resultSelector(opt.value(), *(_otherCollection.cbegin() + _otherPosition));
            else
                return std::nullopt;
        }

        JoinIterator& operator++() noexcept
        {
            if ((_otherCollection.cbegin() + _otherPosition) != _otherCollection.cend())
                ++_otherPosition;
            if ((_otherCollection.cbegin() + _otherPosition) == _otherCollection.cend())
            {
                _otherPosition = 0;
                ++_inIterator;
                return *this;
            }
            else
                return *this;
        }

        bool operator!=(const JoinIterator& other) const noexcept
        {
            return _inIterator != other._inIterator;
        }

        bool operator==(const JoinIterator& other) const noexcept
        {
            return _inIterator == other._inIterator;
        }
    };

    template<Concepts::OptionalIter TLeftIterator,
             Concepts::ConstIterable TOtherCollection,
             typename TLeft = typename std::decay_t<TLeftIterator>::value_type,
             typename TRight = typename std::decay_t<TOtherCollection>::value_type>
    struct ZipIterator final
    {
    private:
        TLeftIterator _leftIterator;
        TOtherCollection _otherCollection;
        std::size_t _otherPosition = 0;

    public:
        using value_type = std::pair<TLeft, TRight>;

        ZipIterator(TLeftIterator leftIterator, const TOtherCollection& otherCollection) noexcept :
            _leftIterator(leftIterator), _otherCollection(otherCollection) {}

        std::optional<value_type> operator*()
        const noexcept(std::is_nothrow_invocable_v<decltype(&TLeftIterator::operator*)> &&
                       std::is_nothrow_invocable_v<decltype(&std::decay_t<TOtherCollection>::const_iterator::operator*)>)
        {
            if ((*_leftIterator).has_value() && _otherCollection.cbegin() + _otherPosition != _otherCollection.cend())
                return std::make_pair((*_leftIterator).value(), *(_otherCollection.cbegin() + _otherPosition));
            else
                return std::nullopt;
        }

        ZipIterator& operator++() noexcept
        {
            ++_leftIterator;
            ++_otherPosition;
            return *this;
        }

        bool operator!=(const ZipIterator& other) const noexcept
        {
            return _leftIterator != other._leftIterator;
        }

        bool operator==(const ZipIterator& other) const noexcept
        {
            return _leftIterator == other._leftIterator;
        }
    };
}

#endif