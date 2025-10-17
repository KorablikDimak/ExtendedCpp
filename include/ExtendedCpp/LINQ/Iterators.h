#ifndef LINQ_Iterators_H
#define LINQ_Iterators_H

#include <concepts>
#include <optional>
#include <type_traits>
#include <utility>

#include <ExtendedCpp/LINQ/Concepts.h>

/// @brief
namespace ExtendedCpp::LINQ
{
	/// @brief
	/// @tparam TInIterator
	template<std::forward_iterator TInIterator>
	struct OptionalIterator final
	{
	private:
		std::decay_t<TInIterator> _inIterator;

	public:
		/// @brief
		using value_type = typename std::decay_t<TInIterator>::value_type;

		/// @brief
		using iterator_category = std::forward_iterator_tag;

		/// @brief
		using difference_type = std::ptrdiff_t;

		/// @brief
		using pointer = value_type*;

		/// @brief
		using reference = value_type&;

		/// @brief
		/// @param inIterator
		explicit OptionalIterator(TInIterator&& inIterator) noexcept :
			_inIterator(std::forward<TInIterator>(inIterator)) {}

		OptionalIterator(const OptionalIterator& other) noexcept = default;
		OptionalIterator(OptionalIterator&& other) noexcept = default;
		OptionalIterator& operator=(const OptionalIterator& other) noexcept = default;
		OptionalIterator& operator=(OptionalIterator&& other) noexcept = default;

		/// @brief
		/// @return
		std::optional<value_type> operator*() const
		noexcept(std::is_nothrow_invocable_v<decltype(&std::decay_t<TInIterator>::operator*)>)
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

		void operator++(int) noexcept
		{
			++_inIterator;
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
		std::decay_t<TSelector> _selector;

	public:
		/// @brief
		using value_type = TOut;

		/// @brief
		using iterator_category = std::forward_iterator_tag;

		/// @brief
		using difference_type = std::ptrdiff_t;

		/// @brief
		using pointer = value_type*;

		/// @brief
		using reference = value_type&;

		/// @brief
		/// @param inIterator
		/// @param selector
		SelectorIterator(TInIterator inIterator, TSelector&& selector) noexcept :
			_inIterator(inIterator),
			_selector(std::forward<TSelector>(selector)) {}

		/// @brief
		/// @param inIterator
		explicit SelectorIterator(TInIterator inIterator) noexcept :
			_inIterator(inIterator) {}

		/// @brief
		/// @return
		std::optional<value_type> operator*() const
		noexcept(std::is_nothrow_invocable_v<TSelector, typename TInIterator::value_type> &&
				 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
		{
			if ((*_inIterator).has_value())
				return _selector((*_inIterator).value());
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
	/// @tparam TTransform
	template<Concepts::OptionalIter TInIterator,
			 std::invocable<typename TInIterator::value_type&> TTransform>
	requires std::same_as<std::invoke_result_t<TTransform, typename TInIterator::value_type&>, void>
	struct TransformIterator final
	{
	private:
		TInIterator _inIterator;
		std::decay_t<TTransform> _transform;

	public:
		/// @brief
		using value_type = typename TInIterator::value_type;

		/// @brief
		using iterator_category = std::forward_iterator_tag;

		/// @brief
		using difference_type = std::ptrdiff_t;

		/// @brief
		using pointer = value_type*;

		/// @brief
		using reference = value_type&;

		/// @brief
		/// @param inIterator
		/// @param transform
		TransformIterator(TInIterator inIterator, TTransform&& transform) noexcept :
			_inIterator(inIterator),
			_transform(std::forward<TTransform>(transform)) {}

		/// @brief
		/// @param inIterator
		explicit TransformIterator(TInIterator inIterator) noexcept :
			_inIterator(inIterator) {}

		/// @brief
		/// @return
		std::optional<value_type> operator*() const
		noexcept(std::is_nothrow_invocable_v<TTransform, typename TInIterator::value_type&> &&
				 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
		{
			if ((*_inIterator).has_value())
			{
				auto value = (*_inIterator).value();
				_transform(value);
				return value;
			}
			return std::nullopt;
		}

		/// @brief
		/// @return
		TransformIterator& operator++() noexcept
		{
			++_inIterator;
			return *this;
		}

		/// @brief
		/// @param other
		/// @return
		bool operator!=(const TransformIterator& other) const noexcept
		{
			return _inIterator != other._inIterator;
		}

		/// @brief
		/// @param other
		/// @return
		bool operator==(const TransformIterator& other) const noexcept
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
		std::decay_t<TPredicate> _predicate;

	public:
		/// @brief
		using value_type = typename TInIterator::value_type;

		/// @brief
		using iterator_category = std::forward_iterator_tag;

		/// @brief
		using difference_type = std::ptrdiff_t;

		/// @brief
		using pointer = value_type*;

		/// @brief
		using reference = value_type&;

		/// @brief
		/// @param inIterator
		/// @param predicate
		WhereIterator(TInIterator inIterator, TPredicate&& predicate) noexcept :
			_inIterator(inIterator),
			_predicate(std::forward<TPredicate>(predicate)) {}

		/// @brief
		/// @param inIterator
		explicit WhereIterator(TInIterator inIterator) noexcept :
			_inIterator(inIterator) {}

		std::optional<value_type> operator*() const
		noexcept(std::is_nothrow_invocable_v<TPredicate, typename TInIterator::value_type> &&
				 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
		{
			if ((*_inIterator).has_value() && _predicate((*_inIterator).value()))
				return (*_inIterator).value();
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
		std::decay_t<TPredicate> _predicate;

	public:
		/// @brief
		using value_type = typename TInIterator::value_type;

		/// @brief
		using iterator_category = std::forward_iterator_tag;

		/// @brief
		using difference_type = std::ptrdiff_t;

		/// @brief
		using pointer = value_type*;

		/// @brief
		using reference = value_type&;

		/// @brief
		/// @param inIterator
		/// @param predicate
		RemoveWhereIterator(TInIterator inIterator, TPredicate&& predicate) noexcept :
			_inIterator(inIterator),
			_predicate(std::forward<TPredicate>(predicate)) {}

		/// @brief
		/// @param inIterator
		explicit RemoveWhereIterator(TInIterator inIterator) noexcept :
			_inIterator(inIterator) {}

		/// @brief
		/// @return
		std::optional<value_type> operator*() const
		noexcept(std::is_nothrow_invocable_v<TPredicate, typename TInIterator::value_type> &&
				 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
		{
			if ((*_inIterator).has_value() && !_predicate((*_inIterator).value()))
				return (*_inIterator).value();
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
			 Concepts::ForwardIterable TOtherCollection,
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
		std::decay_t<TInnerKeySelector> _innerKeySelector;
		std::decay_t<TOtherKeySelector> _otherKeySelector;
		std::decay_t<TResultSelector> _resultSelector;
		std::size_t _otherPosition = 0;

	public:
		/// @brief
		using value_type = TResult;

		/// @brief
		using iterator_category = std::forward_iterator_tag;

		/// @brief
		using difference_type = std::ptrdiff_t;

		/// @brief
		using pointer = value_type*;

		/// @brief
		using reference = value_type&;

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
		/// @param inIterator
		explicit JoinIterator(TInIterator inIterator) noexcept :
			_inIterator(inIterator) {}

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
			 Concepts::ForwardIterable TOtherCollection,
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
		using iterator_category = std::forward_iterator_tag;

		/// @brief
		using difference_type = std::ptrdiff_t;

		/// @brief
		using pointer = value_type*;

		/// @brief
		using reference = value_type&;

		/// @brief
		/// @param leftIterator
		/// @param otherCollection
		ZipIterator(TLeftIterator leftIterator, const TOtherCollection& otherCollection) noexcept :
			_leftIterator(leftIterator),
			_otherBegin(otherCollection.cbegin()),
			_otherEnd(otherCollection.cend()) {}

		/// @brief
		/// @param leftIterator
		explicit ZipIterator(TLeftIterator leftIterator) noexcept :
			_leftIterator(leftIterator) {}

		/// @brief
		/// @return
		std::optional<value_type> operator*() const
		noexcept(std::is_nothrow_invocable_v<decltype(&TLeftIterator::operator*)> &&
				 std::is_nothrow_invocable_v<decltype(&std::decay_t<TOtherCollection>::const_iterator::operator*)>)
		{
			if ((*_leftIterator).has_value() && _otherBegin + _otherPosition != _otherEnd)
				return std::make_pair((*_leftIterator).value(), *(_otherBegin + _otherPosition));
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

	/// @brief
	/// @tparam TInIterator
	template<Concepts::OptionalIter TInIterator>
	struct SkipIterator final
	{
	private:
		TInIterator _inIterator;
		mutable std::size_t _count = 0;

	public:
		/// @brief
		using value_type = typename TInIterator::value_type;

		/// @brief
		using iterator_category = std::forward_iterator_tag;

		/// @brief
		using difference_type = std::ptrdiff_t;

		/// @brief
		using pointer = value_type*;

		/// @brief
		using reference = value_type&;

		/// @brief
		/// @param inIterator
		/// @param count
		SkipIterator(TInIterator inIterator, const std::size_t count) noexcept :
			_inIterator(inIterator),
			_count(count) {}

		/// @brief
		/// @param inIterator
		explicit SkipIterator(TInIterator inIterator) noexcept :
			_inIterator(inIterator) {}

		/// @brief
		/// @return
		std::optional<value_type> operator*() const
		noexcept(std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
		{
			if (!(*_inIterator).has_value())
				return std::nullopt;

			if (_count)
			{
				--_count;
				return std::nullopt;
			}

			return (*_inIterator).value();
		}

		/// @brief
		/// @return
		SkipIterator& operator++() noexcept
		{
			++_inIterator;
			return *this;
		}

		/// @brief
		/// @param other
		/// @return
		bool operator!=(const SkipIterator& other) const noexcept
		{
			return _inIterator != other._inIterator;
		}

		/// @brief
		/// @param other
		/// @return
		bool operator==(const SkipIterator& other) const noexcept
		{
			return _inIterator == other._inIterator;
		}
	};

	/// @brief
	/// @tparam TInIterator
	/// @tparam TPredicate
	template<Concepts::OptionalIter TInIterator,
			 Concepts::IsPredicate<typename TInIterator::value_type> TPredicate>
	struct SkipWhileIterator final
	{
	private:
		TInIterator _inIterator;
		std::decay_t<TPredicate> _predicate;
		mutable bool _predicateFailed = false;

	public:
		/// @brief
		using value_type = typename TInIterator::value_type;

		/// @brief
		using iterator_category = std::forward_iterator_tag;

		/// @brief
		using difference_type = std::ptrdiff_t;

		/// @brief
		using pointer = value_type*;

		/// @brief
		using reference = value_type&;

		/// @brief
		/// @param inIterator
		/// @param predicate
		SkipWhileIterator(TInIterator inIterator, TPredicate&& predicate) noexcept :
			_inIterator(inIterator),
			_predicate(std::forward<TPredicate>(predicate)) {}

		/// @brief
		/// @param inIterator
		explicit SkipWhileIterator(TInIterator inIterator) noexcept :
			_inIterator(inIterator) {}

		/// @brief
		/// @return
		std::optional<value_type> operator*() const
		noexcept(std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)> &&
				 std::is_nothrow_invocable_v<TPredicate, value_type>)
		{
			if (!(*_inIterator).has_value())
				return std::nullopt;
			if (_predicateFailed)
				return (*_inIterator).value();

			if (_predicate((*_inIterator).value()))
				return std::nullopt;

			_predicateFailed = true;
			return (*_inIterator).value();
		}

		/// @brief
		/// @return
		SkipWhileIterator& operator++() noexcept
		{
			++_inIterator;
			return *this;
		}

		/// @brief
		/// @param other
		/// @return
		bool operator!=(const SkipWhileIterator& other) const noexcept
		{
			return _inIterator != other._inIterator;
		}

		/// @brief
		/// @param other
		/// @return
		bool operator==(const SkipWhileIterator& other) const noexcept
		{
			return _inIterator == other._inIterator;
		}
	};

	/// @brief
	/// @tparam TInIterator
	template<Concepts::OptionalIter TInIterator>
	struct TakeIterator final
	{
	private:
		TInIterator _inIterator;
		mutable std::size_t _count = 0;

	public:
		/// @brief
		using value_type = typename TInIterator::value_type;

		/// @brief
		using iterator_category = std::forward_iterator_tag;

		/// @brief
		using difference_type = std::ptrdiff_t;

		/// @brief
		using pointer = value_type*;

		/// @brief
		using reference = value_type&;

		/// @brief
		/// @param inIterator
		/// @param count
		TakeIterator(TInIterator inIterator, std::size_t count) noexcept :
			_inIterator(inIterator),
			_count(count) {}

		/// @brief
		/// @param inIterator
		explicit TakeIterator(TInIterator inIterator) noexcept :
			_inIterator(inIterator) {}

		/// @brief
		/// @return
		std::optional<value_type> operator*() const
		noexcept(std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
		{
			if (!(*_inIterator).has_value())
				return std::nullopt;

			if (_count)
			{
				--_count;
				return (*_inIterator).value();
			}
			return std::nullopt;
		}

		/// @brief
		/// @return
		TakeIterator& operator++() noexcept
		{
			++_inIterator;
			return *this;
		}

		/// @brief
		/// @param other
		/// @return
		bool operator!=(const TakeIterator& other) const noexcept
		{
			return _inIterator != other._inIterator;
		}

		/// @brief
		/// @param other
		/// @return
		bool operator==(const TakeIterator& other) const noexcept
		{
			return _inIterator == other._inIterator;
		}
	};

	/// @brief
	/// @tparam TInIterator
	/// @tparam TPredicate
	template<Concepts::OptionalIter TInIterator,
			 Concepts::IsPredicate<typename TInIterator::value_type> TPredicate>
	struct TakeWhileIterator final
	{
	private:
		TInIterator _inIterator;
		std::decay_t<TPredicate> _predicate;
		mutable bool _predicateFailed = false;

	public:
		/// @brief
		using value_type = typename TInIterator::value_type;

		/// @brief
		using iterator_category = std::forward_iterator_tag;

		/// @brief
		using difference_type = std::ptrdiff_t;

		/// @brief
		using pointer = value_type*;

		/// @brief
		using reference = value_type&;

		/// @brief
		/// @param inIterator
		/// @param predicate
		TakeWhileIterator(TInIterator inIterator, TPredicate&& predicate) noexcept :
			_inIterator(inIterator),
			_predicate(std::forward<TPredicate>(predicate)) {}

		/// @brief
		/// @param inIterator
		explicit TakeWhileIterator(TInIterator inIterator) noexcept :
			_inIterator(inIterator) {}

		/// @brief
		/// @return
		std::optional<value_type> operator*() const
		noexcept(std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)> &&
				 std::is_nothrow_invocable_v<TPredicate, value_type>)
		{
			if (!(*_inIterator).has_value() || _predicateFailed)
				return std::nullopt;

			if (_predicate((*_inIterator).value()))
				return (*_inIterator).value();

			_predicateFailed = true;
			return std::nullopt;
		}

		/// @brief
		/// @return
		TakeWhileIterator& operator++() noexcept
		{
			++_inIterator;
			return *this;
		}

		/// @brief
		/// @param other
		/// @return
		bool operator!=(const TakeWhileIterator& other) const noexcept
		{
			return _inIterator != other._inIterator;
		}

		/// @brief
		/// @param other
		/// @return
		bool operator==(const TakeWhileIterator& other) const noexcept
		{
			return _inIterator == other._inIterator;
		}
	};
}

#endif