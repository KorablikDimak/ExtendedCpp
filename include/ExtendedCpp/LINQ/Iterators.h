#ifndef LINQ_Iterators_H
#define LINQ_Iterators_H

#include <ExtendedCpp/LINQ/Concepts.h>

/// @brief
namespace ExtendedCpp::LINQ
{
	/// @brief
	/// @tparam TInIterator
	template<std::forward_iterator TInIterator>
	struct IteratorWrapper final
	{
	private:
		TInIterator _inIterator;
		TInIterator _endIterator;

	public:
		/// @brief
		using value_type = typename TInIterator::value_type;

		/// @brief
		using difference_type = std::ptrdiff_t;

		/// @brief
		/// @param inIterator
		/// @param endIterator
		IteratorWrapper(TInIterator inIterator, TInIterator endIterator) noexcept :
			_inIterator(std::move(inIterator)),
			_endIterator(std::move(endIterator)) {}

		/// @brief
		IteratorWrapper() noexcept = default;

		/// @brief
		/// @param other
		IteratorWrapper(const IteratorWrapper& other) noexcept = default;

		/// @brief
		/// @param other
		IteratorWrapper(IteratorWrapper&& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		IteratorWrapper& operator=(const IteratorWrapper& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		IteratorWrapper& operator=(IteratorWrapper&& other) noexcept = default;

		/// @brief
		/// @return
		value_type operator*() const
		noexcept(std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
		{
			return *_inIterator;
		}

		/// @brief
		/// @return
		IteratorWrapper& operator++()
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TInIterator>().operator++())>)
		{
			++_inIterator;
			return *this;
		}

		/// @brief
		/// @return
		IteratorWrapper operator++(int)
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<IteratorWrapper>().operator++())>)
		{
			IteratorWrapper temp = *this;
			++*this;
			return temp;
		}

		/// @brief
		/// @param other
		/// @return
		bool operator!=(const IteratorWrapper& other) const noexcept
		{
			return _inIterator != other._inIterator;
		}

		/// @brief
		/// @param other
		/// @return
		bool operator==(const IteratorWrapper& other) const noexcept
		{
			return _inIterator == other._inIterator;
		}

		/// @brief
		explicit operator bool() const noexcept
		{
			return _inIterator != _endIterator;
		}
	};

	/// @brief
	/// @tparam TOut
	/// @tparam TInIterator
	/// @tparam TSelector
	template<typename TOut,
			 std::forward_iterator TInIterator,
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
		using difference_type = std::ptrdiff_t;

		/// @brief
		/// @param inIterator
		/// @param selector
		SelectorIterator(TInIterator inIterator, TSelector&& selector) noexcept :
			_inIterator(std::move(inIterator)),
			_selector(std::forward<TSelector>(selector)) {}

		/// @brief
		/// @param inIterator
		explicit SelectorIterator(TInIterator inIterator) noexcept :
			_inIterator(std::move(inIterator)) {}

		/// @brief
		SelectorIterator() noexcept = default;

		/// @brief
		/// @param other
		SelectorIterator(const SelectorIterator& other) noexcept = default;

		/// @brief
		/// @param other
		SelectorIterator(SelectorIterator&& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		SelectorIterator& operator=(const SelectorIterator& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		SelectorIterator& operator=(SelectorIterator&& other) noexcept = default;

		/// @brief
		/// @return
		value_type operator*() const
		noexcept(std::is_nothrow_invocable_v<TSelector, typename TInIterator::value_type> &&
				 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
		{
			return _selector(*_inIterator);
		}

		/// @brief
		/// @return
		SelectorIterator& operator++()
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TInIterator>().operator++())>)
		{
			++_inIterator;
			return *this;
		}

		/// @brief
		/// @return
		SelectorIterator operator++(int)
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<SelectorIterator>().operator++())>)
		{
			SelectorIterator temp = *this;
			++*this;
			return temp;
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

		/// @brief
		explicit operator bool() const noexcept
		{
			return _inIterator.operator bool();
		}
	};

	/// @brief
	/// @tparam TInIterator
	/// @tparam TTransform
	template<std::forward_iterator TInIterator,
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
		using difference_type = std::ptrdiff_t;

		/// @brief
		/// @param inIterator
		/// @param transform
		TransformIterator(TInIterator inIterator, TTransform&& transform) noexcept :
			_inIterator(std::move(inIterator)),
			_transform(std::forward<TTransform>(transform)) {}

		/// @brief
		/// @param inIterator
		explicit TransformIterator(TInIterator inIterator) noexcept :
			_inIterator(std::move(inIterator)) {}

		/// @brief
		TransformIterator() noexcept = default;

		/// @brief
		/// @param other
		TransformIterator(const TransformIterator& other) noexcept = default;

		/// @brief
		/// @param other
		TransformIterator(TransformIterator&& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		TransformIterator& operator=(const TransformIterator& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		TransformIterator& operator=(TransformIterator&& other) noexcept = default;

		/// @brief
		/// @return
		value_type operator*() const
		noexcept(std::is_nothrow_copy_assignable_v<value_type> &&
				 std::is_nothrow_invocable_v<TTransform, typename TInIterator::value_type&> &&
				 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
		{
			auto value = *_inIterator;
			_transform(value);
			return value;
		}

		/// @brief
		/// @return
		TransformIterator& operator++()
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TInIterator>().operator++())>)
		{
			++_inIterator;
			return *this;
		}

		/// @brief
		/// @return
		TransformIterator operator++(int)
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TransformIterator>().operator++())>)
		{
			TransformIterator temp = *this;
			++*this;
			return temp;
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

		/// @brief
		explicit operator bool() const noexcept
		{
			return _inIterator.operator bool();
		}
	};

	/// @brief
	/// @tparam TInIterator
	/// @tparam TPredicate
	template<std::forward_iterator TInIterator,
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
		using difference_type = std::ptrdiff_t;

		/// @brief
		/// @param inIterator
		/// @param predicate
		WhereIterator(TInIterator inIterator, TPredicate&& predicate)
		noexcept(std::is_nothrow_invocable_v<TPredicate, typename TInIterator::value_type> &&
				 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)> &&
				 std::is_nothrow_invocable_v<decltype(std::declval<TInIterator>().operator++())>) :
			_inIterator(std::move(inIterator)),
			_predicate(std::forward<TPredicate>(predicate))
		{
			while (_inIterator)
			{
				if (_predicate(*_inIterator))
					break;
				++_inIterator;
			}
		}

		/// @brief
		/// @param inIterator
		explicit WhereIterator(TInIterator inIterator) noexcept :
			_inIterator(std::move(inIterator)) {}

		/// @brief
		WhereIterator() noexcept = default;

		/// @brief
		/// @param other
		WhereIterator(const WhereIterator& other) noexcept = default;

		/// @brief
		/// @param other
		WhereIterator(WhereIterator&& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		WhereIterator& operator=(const WhereIterator& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		WhereIterator& operator=(WhereIterator&& other) noexcept = default;

		/// @brief
		/// @return
		value_type operator*() const
		noexcept(std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
		{
			return *_inIterator;
		}

		/// @brief
		/// @return
		WhereIterator& operator++()
		noexcept(std::is_nothrow_invocable_v<TPredicate, typename TInIterator::value_type> &&
				 std::is_nothrow_invocable_v<decltype(std::declval<TInIterator>().operator++())> &&
				 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
		{
			++_inIterator;
			while (_inIterator)
			{
				if (_predicate(*_inIterator))
					break;
				++_inIterator;
			}
			return *this;
		}

		/// @brief
		/// @return
		WhereIterator operator++(int)
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<WhereIterator>().operator++())>)
		{
			WhereIterator temp = *this;
			++*this;
			return temp;
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

		/// @brief
		explicit operator bool() const noexcept
		{
			return _inIterator.operator bool();
		}
	};

	/// @brief
	/// @tparam TInIterator
	/// @tparam TPredicate
	template<std::forward_iterator TInIterator,
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
		using difference_type = std::ptrdiff_t;

		/// @brief
		/// @param inIterator
		/// @param predicate
		RemoveWhereIterator(TInIterator inIterator, TPredicate&& predicate)
		noexcept(std::is_nothrow_invocable_v<TPredicate, typename TInIterator::value_type> &&
				 std::is_nothrow_invocable_v<decltype(std::declval<TInIterator>().operator++())> &&
				 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>) :
			_inIterator(std::move(inIterator)),
			_predicate(std::forward<TPredicate>(predicate))
		{
			while (_inIterator)
			{
				if (!_predicate(*_inIterator))
					break;
				++_inIterator;
			}
		}

		/// @brief
		/// @param inIterator
		explicit RemoveWhereIterator(TInIterator inIterator) noexcept :
			_inIterator(std::move(inIterator)) {}

		/// @brief
		RemoveWhereIterator() noexcept = default;

		/// @brief
		/// @param other
		RemoveWhereIterator(const RemoveWhereIterator& other) noexcept = default;

		/// @brief
		/// @param other
		RemoveWhereIterator(RemoveWhereIterator&& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		RemoveWhereIterator& operator=(const RemoveWhereIterator& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		RemoveWhereIterator& operator=(RemoveWhereIterator&& other) noexcept = default;

		/// @brief
		/// @return
		value_type operator*() const
		noexcept(std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
		{
			return *_inIterator;
		}

		/// @brief
		/// @return
		RemoveWhereIterator& operator++()
		noexcept(std::is_nothrow_invocable_v<TPredicate, typename TInIterator::value_type> &&
				 std::is_nothrow_invocable_v<decltype(std::declval<TInIterator>().operator++())> &&
				 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
		{
			++_inIterator;
			while (_inIterator)
			{
				if (!_predicate(*_inIterator))
					break;
				++_inIterator;
			}
			return *this;
		}

		/// @brief
		/// @return
		RemoveWhereIterator operator++(int)
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<RemoveWhereIterator>().operator++())>)
		{
			WhereIterator temp = *this;
			++*this;
			return temp;
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

		/// @brief
		explicit operator bool() const noexcept
		{
			return _inIterator.operator bool();
		}
	};

	/// @brief
	/// @tparam TResult
	/// @tparam TInIterator
	/// @tparam TOtherCollection
	/// @tparam TInnerKeySelector
	/// @tparam TOtherKeySelector
	/// @tparam TResultSelector
	template<std::forward_iterator TInIterator,
			 Concepts::ForwardIterable TOtherCollection,
			 std::invocable<typename TInIterator::value_type> TInnerKeySelector,
			 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
			 std::invocable<typename TInIterator::value_type,
							typename TOtherCollection::value_type> TResultSelector,
			 typename TResult = std::invoke_result_t<TResultSelector,
													 typename TInIterator::value_type,
													 typename TOtherCollection::value_type>,
			 std::forward_iterator TOtherIterator = typename TOtherCollection::const_iterator>
	requires std::same_as<std::invoke_result_t<TInnerKeySelector, typename TInIterator::value_type>,
						  std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
			 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, typename TInIterator::value_type>>
	struct JoinIterator final
	{
	private:
		TInIterator _inIterator;
		TOtherIterator _otherBegin;
		TOtherIterator _otherEnd;
		TOtherIterator _currentOther;
		std::decay_t<TInnerKeySelector> _innerKeySelector;
		std::decay_t<TOtherKeySelector> _otherKeySelector;
		std::decay_t<TResultSelector> _resultSelector;

	public:
		/// @brief
		using value_type = TResult;

		/// @brief
		using difference_type = std::ptrdiff_t;

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
					 TResultSelector&& resultSelector)
		noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, typename TInIterator::value_type> &&
				 std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
				 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)> &&
				 std::is_nothrow_invocable_v<decltype(&TOtherIterator::operator*)> &&
				 std::is_nothrow_invocable_v<decltype(std::declval<TInIterator>().operator++())> &&
				 std::is_nothrow_invocable_v<decltype(std::declval<TOtherIterator>().operator++())>) :
			_inIterator(std::move(inIterator)),
			_otherBegin(otherCollection.begin()),
			_otherEnd(otherCollection.end()),
			_innerKeySelector(std::forward<TInnerKeySelector>(innerKeySelector)),
			_otherKeySelector(std::forward<TOtherKeySelector>(otherKeySelector)),
			_resultSelector(std::forward<TResultSelector>(resultSelector))
		{
			while (_inIterator)
			{
				_currentOther = _otherBegin;
				while (_currentOther != _otherEnd)
				{
					if (_innerKeySelector(*_inIterator) == _otherKeySelector(*_currentOther))
						return;
					++_currentOther;
				}
				++_inIterator;
			}
		}

		/// @brief
		/// @param inIterator
		explicit JoinIterator(TInIterator inIterator) noexcept :
			_inIterator(std::move(inIterator)) {}

		/// @brief
		JoinIterator() noexcept = default;

		/// @brief
		/// @param other
		JoinIterator(const JoinIterator& other) noexcept = default;

		/// @brief
		/// @param other
		JoinIterator(JoinIterator&& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		JoinIterator& operator=(const JoinIterator& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		JoinIterator& operator=(JoinIterator&& other) noexcept = default;

		/// @brief
		/// @return
		value_type operator*() const
		noexcept(std::is_nothrow_invocable_v<TResultSelector, typename TInIterator::value_type, typename TOtherCollection::value_type> &&
				 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)> &&
				 std::is_nothrow_invocable_v<decltype(&TOtherIterator::operator*)>)
		{
			return _resultSelector(*_inIterator, *_currentOther);
		}

		/// @brief
		/// @return
		JoinIterator& operator++()
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TInIterator>().operator++())> &&
				 std::is_nothrow_invocable_v<decltype(std::declval<TOtherIterator>().operator++())>)
		{
			if (_currentOther != _otherEnd)
				++_currentOther;
			else
			{
				_currentOther = _otherBegin;
				++_inIterator;
			}

			while (_inIterator)
			{
				while (_currentOther != _otherEnd)
				{
					if (_innerKeySelector(*_inIterator) == _otherKeySelector(*_currentOther))
						return *this;
					++_currentOther;
				}
				_currentOther = _otherBegin;
				++_inIterator;
			}
			return *this;
		}

		/// @brief
		/// @return
		JoinIterator operator++(int)
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<JoinIterator>().operator++())>)
		{
			JoinIterator temp = *this;
			++*this;
			return temp;
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

		/// @brief
		explicit operator bool() const noexcept
		{
			return _inIterator.operator bool();
		}
	};

	/// @brief
	/// @tparam TLeft
	/// @tparam TRight
	/// @tparam TLeftIterator
	/// @tparam TOtherCollection
	template<std::forward_iterator TLeftIterator,
			 Concepts::ForwardIterable TOtherCollection,
			 typename TLeft = typename TLeftIterator::value_type,
			 typename TRight = typename TOtherCollection::value_type,
			 std::forward_iterator TRightIterator = typename TOtherCollection::const_iterator>
	struct ZipIterator final
	{
	private:
		TLeftIterator _leftIterator;
		TRightIterator _rightIterator;
		TRightIterator _rightEnd;

	public:
		/// @brief
		using value_type = std::pair<TLeft, TRight>;

		/// @brief
		using difference_type = std::ptrdiff_t;

		/// @brief
		/// @param leftIterator
		/// @param otherCollection
		ZipIterator(TLeftIterator leftIterator, const TOtherCollection& otherCollection) noexcept :
			_leftIterator(std::move(leftIterator)),
			_rightIterator(otherCollection.begin()),
			_rightEnd(otherCollection.end()) {}

		/// @brief
		/// @param leftIterator
		explicit ZipIterator(TLeftIterator leftIterator) noexcept :
			_leftIterator(std::move(leftIterator)) {}

		/// @brief
		ZipIterator() noexcept = default;

		/// @brief
		/// @param other
		ZipIterator(const ZipIterator& other) noexcept = default;

		/// @brief
		/// @param other
		ZipIterator(ZipIterator&& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		ZipIterator& operator=(const ZipIterator& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		ZipIterator& operator=(ZipIterator&& other) noexcept = default;

		/// @brief
		/// @return
		value_type operator*() const
		noexcept(std::is_nothrow_invocable_v<decltype(&TLeftIterator::operator*)> &&
				 std::is_nothrow_invocable_v<decltype(&TRightIterator::operator*)> &&
				 std::is_nothrow_move_constructible_v<TLeft> &&
				 std::is_nothrow_move_constructible_v<TRight>)
		{
			return std::make_pair(*_leftIterator, *_rightIterator);
		}

		/// @brief
		/// @return
		ZipIterator& operator++()
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TLeftIterator>().operator++())> &&
				 std::is_nothrow_invocable_v<decltype(std::declval<TRightIterator>().operator++())>)
		{
			if (_leftIterator && _rightIterator != _rightEnd)
			{
				++_leftIterator;
				++_rightIterator;
			}
			return *this;
		}

		/// @brief
		/// @return
		ZipIterator operator++(int)
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<ZipIterator>().operator++())>)
		{
			JoinIterator temp = *this;
			++*this;
			return temp;
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

		/// @brief
		explicit operator bool() const noexcept
		{
			return _leftIterator.operator bool();
		}
	};

	/// @brief
	/// @tparam TInIterator
	template<std::forward_iterator TInIterator>
	struct SkipIterator final
	{
	private:
		TInIterator _inIterator;

	public:
		/// @brief
		using value_type = typename TInIterator::value_type;

		/// @brief
		using difference_type = std::ptrdiff_t;

		/// @brief
		/// @param inIterator
		/// @param count
		SkipIterator(TInIterator inIterator, const std::size_t count)
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TInIterator>().operator++())>) :
			_inIterator(std::move(inIterator))
		{
			for (std::size_t i = 0; i < count && _inIterator; ++i)
				++_inIterator;
		}

		/// @brief
		/// @param inIterator
		explicit SkipIterator(TInIterator inIterator) noexcept :
			_inIterator(inIterator) {}

		/// @brief
		SkipIterator() noexcept = default;

		/// @brief
		/// @param other
		SkipIterator(const SkipIterator& other) noexcept = default;

		/// @brief
		/// @param other
		SkipIterator(SkipIterator&& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		SkipIterator& operator=(const SkipIterator& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		SkipIterator& operator=(SkipIterator&& other) noexcept = default;

		/// @brief
		/// @return
		value_type operator*() const
		noexcept(std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
		{
			return *_inIterator;
		}

		/// @brief
		/// @return
		SkipIterator& operator++()
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TInIterator>().operator++())>)
		{
			++_inIterator;
			return *this;
		}

		/// @brief
		/// @return
		SkipIterator operator++(int)
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<SkipIterator>().operator++())>)
		{
			SkipIterator temp = *this;
			++*this;
			return temp;
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

		/// @brief
		explicit operator bool() const noexcept
		{
			return _inIterator.operator bool();
		}
	};

	/// @brief
	/// @tparam TInIterator
	/// @tparam TPredicate
	template<std::forward_iterator TInIterator,
			 Concepts::IsPredicate<typename TInIterator::value_type> TPredicate>
	struct SkipWhileIterator final
	{
	private:
		TInIterator _inIterator;
		std::decay_t<TPredicate> _predicate;

	public:
		/// @brief
		using value_type = typename TInIterator::value_type;

		/// @brief
		using difference_type = std::ptrdiff_t;

		/// @brief
		/// @param inIterator
		/// @param predicate
		SkipWhileIterator(TInIterator inIterator, TPredicate&& predicate)
		noexcept(std::is_nothrow_invocable_v<TPredicate, value_type> &&
				 std::is_nothrow_invocable_v<decltype(std::declval<TInIterator>().operator++())> &&
				 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>) :
			_inIterator(std::move(inIterator)),
			_predicate(std::forward<TPredicate>(predicate))
		{
			while (_inIterator)
			{
				if (!_predicate(*_inIterator))
					break;
				++_inIterator;
			}
		}

		/// @brief
		/// @param inIterator
		explicit SkipWhileIterator(TInIterator inIterator) noexcept :
			_inIterator(std::move(inIterator)) {}

		/// @brief
		SkipWhileIterator() noexcept = default;

		/// @brief
		/// @param other
		SkipWhileIterator(const SkipWhileIterator& other) noexcept = default;

		/// @brief
		/// @param other
		SkipWhileIterator(SkipWhileIterator&& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		SkipWhileIterator& operator=(const SkipWhileIterator& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		SkipWhileIterator& operator=(SkipWhileIterator&& other) noexcept = default;

		/// @brief
		/// @return
		value_type operator*() const
		noexcept(std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
		{
			return *_inIterator;
		}

		/// @brief
		/// @return
		SkipWhileIterator& operator++()
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TInIterator>().operator++())>)
		{
			++_inIterator;
			return *this;
		}

		/// @brief
		/// @return
		SkipWhileIterator operator++(int)
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<SkipWhileIterator>().operator++())>)
		{
			SkipWhileIterator temp = *this;
			++*this;
			return temp;
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

		/// @brief
		/// @return
		explicit operator bool() const noexcept
		{
			return _inIterator.operator bool();
		}
	};

	/// @brief
	/// @tparam TInIterator
	template<std::forward_iterator TInIterator>
	struct TakeIterator final
	{
	private:
		TInIterator _inIterator;
		std::size_t _count = 0;

	public:
		/// @brief
		using value_type = typename TInIterator::value_type;

		/// @brief
		using difference_type = std::ptrdiff_t;

		/// @brief
		/// @param inIterator
		/// @param count
		TakeIterator(TInIterator inIterator, const std::size_t count) noexcept :
			_inIterator(std::move(inIterator)),
			_count(count)
		{
			if (!count)
				while (_inIterator)
					++_inIterator;
		}

		/// @brief
		/// @param inIterator
		explicit TakeIterator(TInIterator inIterator) noexcept :
			_inIterator(std::move(inIterator)) {}

		/// @brief
		TakeIterator() noexcept = default;

		/// @brief
		/// @param other
		TakeIterator(const TakeIterator& other) noexcept = default;

		/// @brief
		/// @param other
		TakeIterator(TakeIterator&& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		TakeIterator& operator=(const TakeIterator& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		TakeIterator& operator=(TakeIterator&& other) noexcept = default;

		/// @brief
		/// @return
		value_type operator*() const
		noexcept(std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
		{
			return *_inIterator;
		}

		/// @brief
		/// @return
		TakeIterator& operator++()
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TInIterator>().operator++())>)
		{
			++_inIterator;

			if (_count)
			{
				--_count;
				if (!_count)
					while (_inIterator)
						++_inIterator;
			}

			return *this;
		}

		/// @brief
		/// @return
		TakeIterator operator++(int)
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TakeIterator>().operator++())>)
		{
			TakeIterator temp = *this;
			++*this;
			return temp;
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

		/// @brief
		/// @return
		explicit operator bool() const noexcept
		{
			return _inIterator.operator bool();
		}
	};

	/// @brief
	/// @tparam TInIterator
	/// @tparam TPredicate
	template<std::forward_iterator TInIterator,
			 Concepts::IsPredicate<typename TInIterator::value_type> TPredicate>
	struct TakeWhileIterator final
	{
	private:
		TInIterator _inIterator;
		std::decay_t<TPredicate> _predicate;

	public:
		/// @brief
		using value_type = typename TInIterator::value_type;

		/// @brief
		using difference_type = std::ptrdiff_t;

		/// @brief
		/// @param inIterator
		/// @param predicate
		TakeWhileIterator(TInIterator inIterator, TPredicate&& predicate)
		noexcept(std::is_nothrow_invocable_v<TPredicate, value_type> &&
				 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)> &&
				 std::is_nothrow_invocable_v<decltype(std::declval<TInIterator>().operator++())>) :
			_inIterator(std::move(inIterator)),
			_predicate(std::forward<TPredicate>(predicate))
		{
			if (_inIterator)
				if (!_predicate(*_inIterator))
				{
					while (_inIterator)
						++_inIterator;
				}
		}

		/// @brief
		/// @param inIterator
		explicit TakeWhileIterator(TInIterator inIterator) noexcept :
			_inIterator(std::move(inIterator)) {}

		/// @brief
		TakeWhileIterator() noexcept = default;

		/// @brief
		/// @param other
		TakeWhileIterator(const TakeWhileIterator& other) noexcept = default;

		/// @brief
		/// @param other
		TakeWhileIterator(TakeWhileIterator&& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		TakeWhileIterator& operator=(const TakeWhileIterator& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		TakeWhileIterator& operator=(TakeWhileIterator&& other) noexcept = default;

		/// @brief
		/// @return
		value_type operator*() const
		noexcept(std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)>)
		{
			return *_inIterator;
		}

		/// @brief
		/// @return
		TakeWhileIterator& operator++()
		noexcept(std::is_nothrow_invocable_v<TPredicate, value_type> &&
				 std::is_nothrow_invocable_v<decltype(&TInIterator::operator*)> &&
				 std::is_nothrow_invocable_v<decltype(std::declval<TInIterator>().operator++())>)
		{
			++_inIterator;
			if (_inIterator && _predicate(*_inIterator))
				return *this;

			while (_inIterator)
				++_inIterator;
			return *this;
		}

		/// @brief
		/// @return
		TakeWhileIterator operator++(int)
		noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TakeWhileIterator>().operator++())>)
		{
			TakeWhileIterator temp = *this;
			++*this;
			return temp;
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

		/// @brief
		/// @return
		explicit operator bool() const noexcept
		{
			return _inIterator.operator bool();
		}
	};
}

#endif