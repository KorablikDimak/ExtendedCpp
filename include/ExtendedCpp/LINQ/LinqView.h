#ifndef LINQ_LinqView_H
#define LINQ_LinqView_H

#include <array>
#include <vector>
#include <list>
#include <forward_list>
#include <stack>
#include <queue>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <deque>
#include <concepts>
#include <optional>
#include <type_traits>
#include <utility>

#include <ExtendedCpp/LINQ/Iterators.h>
#include <ExtendedCpp/LINQ/Concepts.h>
#include <ExtendedCpp/LINQ/TypeTraits.h>

/// @brief 
namespace ExtendedCpp::LINQ
{
	/// @brief 
	/// @tparam TIterator 
	template<Concepts::OptionalIter TIterator>
	class LinqView final
	{
	private:
		TIterator _begin;
		TIterator _end;

	public:
		/// @brief 
		using value_type = TIterator::value_type;

		/// @brief 
		using iterator = TIterator;

		/// @brief 
		using const_iterator = TIterator;

		/// @brief 
		/// @param begin 
		/// @param end 
		LinqView(const TIterator begin, const TIterator end) noexcept : 
			_begin(begin), _end(end) {}

		/// @brief 
		/// @return 
		TIterator begin() const noexcept
		{
			return _begin;
		}

		/// @brief 
		/// @return 
		TIterator cbegin() const noexcept
		{
			return _begin;
		}

		/// @brief 
		/// @return 
		TIterator end() const noexcept
		{
			return _end;
		}

		/// @brief 
		/// @return 
		TIterator cend() const noexcept
		{
			return _end;
		}

		/// @brief 
		/// @return 
		[[nodiscard]]
		std::vector<value_type> ToVector() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
		{
			std::vector<value_type> collection;
			for (TIterator it = _begin; it != _end; ++it)
			{
				std::optional<value_type> opt = *it;
				if (opt.has_value())
					collection.push_back(opt.value());
			}
			return collection;
		}

		/// @brief 
		/// @tparam SIZE 
		/// @return 
		template<std::size_t SIZE>
		std::array<value_type, SIZE> ToArray() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
		{
			std::array<value_type, SIZE> array;
			std::size_t i = 0;
			for (TIterator it = _begin; it != _end && i < SIZE; ++it, ++i)
			{
				std::optional<value_type> opt = *it;
				if (opt.has_value())
					array[i] = opt.value();
			}
			return array;
		}

		/// @brief 
		/// @return 
		[[nodiscard]]
		std::list<value_type> ToList() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
		{
			std::list<value_type> collection;
			for (TIterator it = _begin; it != _end; ++it)
			{
				std::optional<value_type> opt = *it;
				if (opt.has_value())
					collection.push_back(opt.value());
			}
			return collection;
		}

		/// @brief 
		/// @return 
		[[nodiscard]]
		std::forward_list<value_type> ToForwardList() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
		{
			std::forward_list<value_type> collection;
			for (TIterator it = _begin; it != _end; ++it)
			{
				std::optional<value_type> opt = *it;
				if (opt.has_value())
					collection.push_front(opt.value());
			}
			return collection;
		}

		/// @brief 
		/// @return 
		[[nodiscard]]
		std::stack<value_type> ToStack() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
		{
			std::stack<value_type> stack;
			for (TIterator it = _begin; it != _end; ++it)
			{
				std::optional<value_type> opt = *it;
				if (opt.has_value())
					stack.push(opt.value());
			}
			return stack;
		}

		/// @brief 
		/// @return 
		[[nodiscard]]
		std::queue<value_type> ToQueue() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
		{
			std::queue<value_type> queue;
			for (TIterator it = _begin; it != _end; ++it)
			{
				std::optional<value_type> opt = *it;
				if (opt.has_value())
					queue.push(opt.value());
			}
			return queue;
		}

		/// @brief 
		/// @return 
		[[nodiscard]]
		std::deque<value_type> ToDeque() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
		{
			std::deque<value_type> deque;
			for (TIterator it = _begin; it != _end; ++it)
			{
				std::optional<value_type> opt = *it;
				if (opt.has_value())
					deque.push_back(opt.value());
			}
			return deque;
		}

		/// @brief 
		/// @return 
		[[nodiscard]]
		std::priority_queue<value_type> ToPriorityQueue() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
		{
			std::priority_queue<value_type> priorityQueue;
			for (TIterator it = _begin; it != _end; ++it)
			{
				std::optional<value_type> opt = *it;
				if (opt.has_value())
					priorityQueue.push(opt.value());
			}
			return priorityQueue;
		}

		/// @brief 
		/// @return 
		std::set<value_type> ToSet() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
		{
			std::set<value_type> set;
			for (TIterator it = _begin; it != _end; ++it)
			{
				std::optional<value_type> opt = *it;
				if (opt.has_value())
					set.insert(opt.value());
			}
			return set;
		}

		/// @brief 
		/// @return 
		std::unordered_set<value_type> ToUnorderedSet() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
		{
			std::unordered_set<value_type> unorderedSet;
			for (TIterator it = _begin; it != _end; ++it)
			{
				std::optional<value_type> opt = *it;
				if (opt.has_value())
					unorderedSet.insert(opt.value());
			}
			return unorderedSet;
		}

		/// @brief 
		/// @tparam TKey 
		/// @tparam TValue 
		/// @return 
		template<typename TKey = typename PairTraits<value_type>::FirstType,
				 typename TValue = typename PairTraits<value_type>::SecondType>
		requires Concepts::IsPair<value_type>
		std::map<TKey, TValue> ToMap() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
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

		/// @brief 
		/// @tparam TKey 
		/// @tparam TValue 
		/// @return 
		template<typename TKey = typename PairTraits<value_type>::FirstType,
				 typename TValue = typename PairTraits<value_type>::SecondType>
		requires Concepts::IsPair<value_type>
		std::unordered_map<TKey, TValue> ToUnorderedMap() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&TIterator::operator*)>)
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

		/// @brief
		/// @tparam TResult 
		/// @tparam TSelector 
		/// @param selector 
		/// @return 
		template<std::invocable<value_type> TSelector, typename TResult = std::invoke_result_t<TSelector, value_type>>
		LinqView<SelectorIterator<TResult, TIterator, TSelector>> Select(TSelector&& selector) const noexcept
		{
			return LinqView<SelectorIterator<TResult, TIterator, TSelector>>(
				SelectorIterator<TResult, TIterator, TSelector>(_begin, std::forward<TSelector>(selector)),
				SelectorIterator<TResult, TIterator, TSelector>(_end));
		}

		/// @brief 
		/// @tparam TPredicate 
		/// @param predicate 
		/// @return 
		template<Concepts::IsPredicate<value_type> TPredicate>
		LinqView<WhereIterator<TIterator, TPredicate>> Where(TPredicate&& predicate) const noexcept
		{
			return LinqView<WhereIterator<TIterator, TPredicate>>(
				WhereIterator<TIterator, TPredicate>(_begin, std::forward<TPredicate>(predicate)),
				WhereIterator<TIterator, TPredicate>(_end));
		}

		/// @brief 
		/// @tparam TPredicate 
		/// @param predicate 
		/// @return 
		template<Concepts::IsPredicate<value_type> TPredicate>
		LinqView<RemoveWhereIterator<TIterator, TPredicate>> RemoveWhere(TPredicate&& predicate) const noexcept
		{
			return LinqView<RemoveWhereIterator<TIterator, TPredicate>>(
				RemoveWhereIterator<TIterator, TPredicate>(_begin, std::forward<TPredicate>(predicate)),
				RemoveWhereIterator<TIterator, TPredicate>(_end));
		}

		/// @brief 
		/// @tparam TKey 
		/// @tparam TKeySelector 
		/// @param keySelector 
		/// @return 
		template<std::invocable<value_type> TKeySelector,
				 typename TKey = std::invoke_result_t<TKeySelector, value_type>>
		std::map<TKey, std::vector<value_type>> GroupBy(TKeySelector&& keySelector) const 
		noexcept(std::is_nothrow_invocable_v<TKeySelector, value_type>)
		{
			std::map<TKey, std::vector<value_type>> result;

			for (TIterator it = _begin; it != _end; ++it)
			{
				std::optional<value_type> opt = *it;
				if (opt.has_value())
				{
					value_type value = opt.value();
					if (!result.contains(keySelector(value)))
						result.insert(std::pair<TKey, std::vector<value_type>>(keySelector(value), std::vector<value_type>()));
					result.at(keySelector(value)).push_back(value);
				}
			}

			return result;
		}

		/// @brief 
		/// @tparam TOtherCollection 
		/// @tparam TInnerKeySelector 
		/// @tparam TOtherKeySelector 
		/// @tparam TResultSelector 
		/// @param otherCollection 
		/// @param innerKeySelector 
		/// @param otherKeySelector 
		/// @param resultSelector 
		/// @return 
		template<Concepts::ConstIterable TOtherCollection,
				 std::invocable<value_type> TInnerKeySelector,
				 std::invocable<typename std::decay_t<TOtherCollection>::value_type> TOtherKeySelector,
				 std::invocable<value_type, typename std::decay_t<TOtherCollection>::value_type> TResultSelector>
		requires std::same_as<std::invoke_result_t<TInnerKeySelector, value_type>,
							  std::invoke_result_t<TOtherKeySelector, typename std::decay_t<TOtherCollection>::value_type>> &&
				 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, value_type>>
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
				JoinIterator<TIterator, TOtherCollection, TInnerKeySelector, TOtherKeySelector, TResultSelector>(_end));
		}

		/// @brief 
		/// @tparam TOtherCollection 
		/// @param otherCollection 
		/// @return 
		template<Concepts::ConstIterable TOtherCollection>
		LinqView<ZipIterator<TIterator, TOtherCollection>> Zip(const TOtherCollection& otherCollection) const noexcept
		{
			return LinqView<ZipIterator<TIterator, TOtherCollection>>(
				ZipIterator<TIterator, TOtherCollection>(_begin, otherCollection),
				ZipIterator<TIterator, TOtherCollection>(_end));
		}

		/// @brief 
		/// @param count 
		/// @return 
		LinqView<SkipIterator<TIterator>> Skip(std::size_t count) const noexcept
		{
			return LinqView<SkipIterator<TIterator>>(
				SkipIterator<TIterator>(_begin, count),
				SkipIterator<TIterator>(_end));
		}

		/// @brief 
		/// @tparam TPredicate 
		/// @param predicate 
		/// @return 
		template<Concepts::IsPredicate<value_type> TPredicate>
		LinqView<SkipWhileIterator<TIterator, TPredicate>> SkipWhile(TPredicate&& predicate) const noexcept
		{
			return LinqView<SkipWhileIterator<TIterator, TPredicate>>(
				SkipWhileIterator<TIterator, TPredicate>(_begin, std::forward<TPredicate>(predicate)),
				SkipWhileIterator<TIterator, TPredicate>(_end));
		}

		/// @brief 
		/// @param count 
		/// @return 
		LinqView<TakeIterator<TIterator>> Take(std::size_t count) const noexcept
		{
			return LinqView<TakeIterator<TIterator>>(
				TakeIterator<TIterator>(_begin, count),
				TakeIterator<TIterator>(_end));
		}

		/// @brief 
		/// @tparam TPredicate 
		/// @param predicate 
		/// @return 
		template<Concepts::IsPredicate<value_type> TPredicate>
		LinqView<TakeWhileIterator<TIterator, TPredicate>> TakeWhile(TPredicate&& predicate) const noexcept
		{
			return LinqView<TakeWhileIterator<TIterator, TPredicate>>(
				TakeWhileIterator<TIterator, TPredicate>(_begin, std::forward<TPredicate>(predicate)),
				TakeWhileIterator<TIterator, TPredicate>(_end));
		}
	};
}

#endif