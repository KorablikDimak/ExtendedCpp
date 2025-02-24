#ifndef LINQ_Iterators_H
#define LINQ_Iterators_H

#include <ExtendedCpp/LINQ/Concepts.h>
#include <ExtendedCpp/LINQ/TypeTraits.h>

/// @brief
namespace ExtendedCpp::LINQ
{
    /// @brief 
    /// @tparam TInIterator 
    template<std::forward_iterator TInIterator>
    struct OptionalIterator final
    {
    private:
        TInIterator _inIterator;

    public:
        /// @brief 
        using value_type = TInIterator::value_type;

        /// @brief 
        /// @param inIterator 
        explicit OptionalIterator(TInIterator inIterator) noexcept :
            _inIterator(inIterator) {}

        /// @brief 
        /// @return 
        std::optional<value_type> operator*() const 
        noexcept(std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
        {
            return *_inIterator;
        }

        /// @brief 
        /// @return 
        OptionalIterator& operator++() noexcept
        {
            ++_inIterator;
            return *this;
        }

        /// @brief 
        /// @param other 
        /// @return 
        bool operator!=(const OptionalIterator& other) const noexcept
        {
            return _inIterator != other._inIterator;
        }

        /// @brief 
        /// @param other 
        /// @return 
        bool operator==(const OptionalIterator& other) const noexcept
        {
            return _inIterator == other._inIterator;
        }
    };

    /// @brief 
    /// @tparam TOut 
    /// @tparam TInIterator 
    /// @tparam TSelector 
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
        /// @brief 
        using value_type = TOut;

        /// @brief 
        /// @param inIterator 
        /// @param selector 
        SelectorIterator(TInIterator inIterator, TSelector&& selector) noexcept :
            _inIterator(inIterator),
            _selector(std::forward<TSelector>(selector)) {}

        /// @brief 
        /// @return 
        std::optional<value_type> operator*() const 
        noexcept(std::is_nothrow_invocable_v<TSelector, typename TInIterator::value_type> &&
                 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
        {
            if ((*_inIterator).has_value())
                return _selector((*_inIterator).value());
            else
                return std::nullopt;
        }

        /// @brief 
        /// @return 
        SelectorIterator& operator++() noexcept
        {
            ++_inIterator;
            return *this;
        }

        /// @brief 
        /// @param other 
        /// @return 
        bool operator!=(const SelectorIterator& other) const noexcept
        {
            return _inIterator != other._inIterator;
        }

        /// @brief 
        /// @param other 
        /// @return 
        bool operator==(const SelectorIterator& other) const noexcept
        {
            return _inIterator == other._inIterator;
        }
    };

    /// @brief 
    /// @tparam TInIterator 
    /// @tparam TPredicate 
    template<Concepts::OptionalIter TInIterator,
             Concepts::IsPredicate<typename TInIterator::value_type> TPredicate>
    struct WhereIterator final
    {
    private:
        TInIterator _inIterator;
        TPredicate _predicate;

    public:
        /// @brief 
        using value_type = TInIterator::value_type;

        /// @brief 
        /// @param inIterator 
        /// @param predicate 
        WhereIterator(TInIterator inIterator, TPredicate&& predicate) noexcept :
            _inIterator(inIterator),
            _predicate(std::forward<TPredicate>(predicate)) {}

        std::optional<value_type> operator*() const 
        noexcept(std::is_nothrow_invocable_v<TPredicate, typename TInIterator::value_type> &&
                 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
        {
            if ((*_inIterator).has_value() && _predicate((*_inIterator).value()))
                return (*_inIterator).value();
            else
                return std::nullopt;
        }

        /// @brief 
        /// @return 
        WhereIterator& operator++() noexcept
        {
            ++_inIterator;
            return *this;
        }

        /// @brief 
        /// @param other 
        /// @return 
        bool operator!=(const WhereIterator& other) const noexcept
        {
            return _inIterator != other._inIterator;
        }

        /// @brief 
        /// @param other 
        /// @return 
        bool operator==(const WhereIterator& other) const noexcept
        {
            return _inIterator == other._inIterator;
        }
    };

    /// @brief 
    /// @tparam TInIterator 
    /// @tparam TPredicate 
    template<Concepts::OptionalIter TInIterator,
             Concepts::IsPredicate<typename TInIterator::value_type> TPredicate>
    struct RemoveWhereIterator final
    {
    private:
        TInIterator _inIterator;
        TPredicate _predicate;

    public:
        /// @brief 
        using value_type = TInIterator::value_type;

        /// @brief 
        /// @param inIterator 
        /// @param predicate 
        RemoveWhereIterator(TInIterator inIterator, TPredicate&& predicate) noexcept :
            _inIterator(inIterator),
            _predicate(std::forward<TPredicate>(predicate)) {}

        /// @brief 
        /// @return 
        std::optional<value_type> operator*() const 
        noexcept(std::is_nothrow_invocable_v<TPredicate, typename TInIterator::value_type> &&
                 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
        {
            if ((*_inIterator).has_value() && !_predicate((*_inIterator).value()))
                return (*_inIterator).value();
            else
                return std::nullopt;
        }

        /// @brief 
        /// @return 
        RemoveWhereIterator& operator++() noexcept
        {
            ++_inIterator;
            return *this;
        }

        /// @brief 
        /// @param other 
        /// @return 
        bool operator!=(const RemoveWhereIterator& other) const noexcept
        {
            return _inIterator != other._inIterator;
        }

        /// @brief 
        /// @param other 
        /// @return 
        bool operator==(const RemoveWhereIterator& other) const noexcept
        {
            return _inIterator == other._inIterator;
        }
    };

    /// @brief 
    /// @tparam TResult 
    /// @tparam TInIterator 
    /// @tparam TOtherCollection 
    /// @tparam TInnerKeySelector 
    /// @tparam TOtherKeySelector 
    /// @tparam TResultSelector 
    template<Concepts::OptionalIter TInIterator,
             Concepts::ConstIterable TOtherCollection,
             std::invocable<typename TInIterator::value_type> TInnerKeySelector,
             std::invocable<typename std::decay_t<TOtherCollection>::value_type> TOtherKeySelector,
             std::invocable<typename TInIterator::value_type,
                            typename std::decay_t<TOtherCollection>::value_type> TResultSelector,
             typename TResult = std::invoke_result_t<TResultSelector,
                                                     typename TInIterator::value_type,
                                                     typename std::decay_t<TOtherCollection>::value_type>,
             std::forward_iterator TOtherIterator = typename TOtherCollection::const_iterator>
    requires std::same_as<std::invoke_result_t<TInnerKeySelector, typename TInIterator::value_type>,
                          std::invoke_result_t<TOtherKeySelector, typename std::decay_t<TOtherCollection>::value_type>> &&
             Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, typename TInIterator::value_type>>
    struct JoinIterator final
    {
    private:
        TInIterator _inIterator;
        TOtherIterator _otherBegin;
        TOtherIterator _otherEnd;
        TInnerKeySelector _innerKeySelector;
        TOtherKeySelector _otherKeySelector;
        TResultSelector _resultSelector;
        std::size_t _otherPosition = 0;

    public:
        /// @brief 
        using value_type = TResult;

        /// @brief 
        /// @param inIterator 
        /// @param otherCollection 
        /// @param innerKeySelector 
        /// @param otherKeySelector 
        /// @param resultSelector 
        JoinIterator(TInIterator inIterator,
                     const TOtherCollection& otherCollection,
                     TInnerKeySelector&& innerKeySelector,
                     TOtherKeySelector&& otherKeySelector,
                     TResultSelector&& resultSelector) noexcept :
            _inIterator(inIterator),
            _otherBegin(otherCollection.cbegin()),
            _otherEnd(otherCollection.cend()),
            _innerKeySelector(std::forward<TInnerKeySelector>(innerKeySelector)),
            _otherKeySelector(std::forward<TOtherKeySelector>(otherKeySelector)),
            _resultSelector(std::forward<TResultSelector>(resultSelector)) {}

        /// @brief 
        /// @return 
        std::optional<value_type> operator*() const 
        noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, typename TInIterator::value_type> &&
                 std::is_nothrow_invocable_v<TOtherKeySelector, typename std::decay_t<TOtherCollection>::value_type> &&
                 std::is_nothrow_invocable_v<TResultSelector, typename TInIterator::value_type,
                                             typename std::decay_t<TOtherCollection>::value_type> && 
                 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
        {
            const auto& opt = *_inIterator;
            if (!opt.has_value() || (_otherBegin + _otherPosition) == _otherEnd)
                return std::nullopt;
            if (_innerKeySelector(opt.value()) == _otherKeySelector(*(_otherBegin + _otherPosition)))
                return _resultSelector(opt.value(), *(_otherBegin + _otherPosition));
            else
                return std::nullopt;
        }

        /// @brief 
        /// @return 
        JoinIterator& operator++() noexcept
        {
            if ((_otherBegin + _otherPosition) != _otherEnd)
                ++_otherPosition;
            if ((_otherBegin + _otherPosition) == _otherEnd)
            {
                _otherPosition = 0;
                ++_inIterator;
                return *this;
            }
            else
                return *this;
        }

        /// @brief 
        /// @param other 
        /// @return 
        bool operator!=(const JoinIterator& other) const noexcept
        {
            return _inIterator != other._inIterator;
        }

        /// @brief 
        /// @param other 
        /// @return 
        bool operator==(const JoinIterator& other) const noexcept
        {
            return _inIterator == other._inIterator;
        }
    };

    /// @brief 
    /// @tparam TLeft 
    /// @tparam TRight 
    /// @tparam TLeftIterator 
    /// @tparam TOtherCollection 
    template<Concepts::OptionalIter TLeftIterator,
             Concepts::ConstIterable TOtherCollection,
             typename TLeft = typename std::decay_t<TLeftIterator>::value_type,
             typename TRight = typename std::decay_t<TOtherCollection>::value_type,
             std::forward_iterator TOtherIterator = typename TOtherCollection::const_iterator>
    struct ZipIterator final
    {
    private:
        TLeftIterator _leftIterator;
        TOtherIterator _otherBegin;
        TOtherIterator _otherEnd;
        std::size_t _otherPosition = 0;

    public:
        /// @brief 
        using value_type = std::pair<TLeft, TRight>;

        /// @brief 
        /// @param leftIterator 
        /// @param otherCollection 
        ZipIterator(TLeftIterator leftIterator, const TOtherCollection& otherCollection) noexcept :
            _leftIterator(leftIterator), 
            _otherBegin(otherCollection.cbegin()),
            _otherEnd(otherCollection.cend()) {}

        /// @brief 
        /// @return 
        std::optional<value_type> operator*() const 
        noexcept(std::is_nothrow_invocable_v<decltype(&TLeftIterator::operator*)> &&
                 std::is_nothrow_invocable_v<decltype(&std::decay_t<TOtherCollection>::const_iterator::operator*)>)
        {
            if ((*_leftIterator).has_value() && _otherBegin + _otherPosition != _otherEnd)
                return std::make_pair((*_leftIterator).value(), *(_otherBegin + _otherPosition));
            else
                return std::nullopt;
        }

        /// @brief 
        /// @return 
        ZipIterator& operator++() noexcept
        {
            ++_leftIterator;
            ++_otherPosition;
            return *this;
        }

        /// @brief 
        /// @param other 
        /// @return 
        bool operator!=(const ZipIterator& other) const noexcept
        {
            return _leftIterator != other._leftIterator;
        }

        /// @brief 
        /// @param other 
        /// @return 
        bool operator==(const ZipIterator& other) const noexcept
        {
            return _leftIterator == other._leftIterator;
        }
    };
}

#endif