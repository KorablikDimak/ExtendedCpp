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

#include <ExtendedCpp/LINQ/Iterators.h>
#include <ExtendedCpp/LINQ/TypeTraits.h>

/// @brief 
namespace ExtendedCpp::LINQ
{
	/// @brief
	/// @tparam T
	template<typename T>
	class LinqView;

	/// @brief
	/// @tparam TIterator 
	template<std::forward_iterator TIterator>
	class LinqView<TIterator> final
	{
	public:
		/// @brief
		using iterator = std::decay_t<TIterator>;

		/// @brief
		using const_iterator = std::remove_reference_t<TIterator>;

		/// @brief
		using value_type = typename iterator::value_type;

		/// @brief
		static constexpr bool IsLinqCollection = true;

	private:
		iterator _begin;
		iterator _end;

	public:
		/// @brief 
		/// @param begin 
		/// @param end 
		LinqView(TIterator&& begin, TIterator&& end) noexcept :
			_begin(std::forward<TIterator>(begin)), _end(std::forward<TIterator>(end)) {}

		/// @brief 
		/// @return 
		iterator begin() const noexcept
		{
			return _begin;
		}

		/// @brief 
		/// @return 
		const_iterator cbegin() const noexcept
		{
			return _begin;
		}

		/// @brief 
		/// @return 
		iterator end() const noexcept
		{
			return _end;
		}

		/// @brief 
		/// @return 
		const_iterator cend() const noexcept
		{
			return _end;
		}

		/// @brief
		/// @return 
		[[nodiscard]]
		std::vector<value_type> ToVector() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&const_iterator::operator*)>)
		{
			std::vector<value_type> collection;
			for (TIterator it = _begin; it != _end; ++it)
				collection.push_back(*it);
			return collection;
		}

		/// @brief 
		/// @tparam SIZE 
		/// @return 
		template<std::size_t SIZE>
		std::array<value_type, SIZE> ToArray() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&const_iterator::operator*)>)
		{
			std::array<value_type, SIZE> array;
			std::size_t i = 0;
			for (TIterator it = _begin; it != _end && i < SIZE; ++it, ++i)
				array[i] = *it;
			return array;
		}

		/// @brief 
		/// @return 
		[[nodiscard]]
		std::list<value_type> ToList() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&const_iterator::operator*)>)
		{
			std::list<value_type> collection;
			for (TIterator it = _begin; it != _end; ++it)
				collection.push_back(*it);
			return collection;
		}

		/// @brief 
		/// @return 
		[[nodiscard]]
		std::forward_list<value_type> ToForwardList() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&const_iterator::operator*)>)
		{
			const std::vector<value_type> collection = ToVector();
			return std::forward_list<value_type>(collection.cbegin(), collection.cend());
		}

		/// @brief 
		/// @return 
		[[nodiscard]]
		std::stack<value_type> ToStack() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&const_iterator::operator*)>)
		{
			std::stack<value_type> stack;
			for (TIterator it = _begin; it != _end; ++it)
				stack.push(*it);
			return stack;
		}

		/// @brief 
		/// @return 
		[[nodiscard]]
		std::queue<value_type> ToQueue() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&const_iterator::operator*)>)
		{
			std::queue<value_type> queue;
			for (TIterator it = _begin; it != _end; ++it)
				queue.push(*it);
			return queue;
		}

		/// @brief 
		/// @return 
		[[nodiscard]]
		std::deque<value_type> ToDeque() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&const_iterator::operator*)>)
		{
			std::deque<value_type> deque;
			for (TIterator it = _begin; it != _end; ++it)
				deque.push_back(*it);
			return deque;
		}

		/// @brief 
		/// @return 
		[[nodiscard]]
		std::priority_queue<value_type> ToPriorityQueue() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&const_iterator::operator*)>)
		{
			std::priority_queue<value_type> priorityQueue;
			for (TIterator it = _begin; it != _end; ++it)
				priorityQueue.push(*it);
			return priorityQueue;
		}

		/// @brief 
		/// @return 
		std::set<value_type> ToSet() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&const_iterator::operator*)>)
		{
			std::set<value_type> set;
			for (TIterator it = _begin; it != _end; ++it)
				set.insert(*it);
			return set;
		}

		/// @brief 
		/// @return 
		std::unordered_set<value_type> ToUnorderedSet() const 
		noexcept(std::is_nothrow_invocable_v<decltype(&const_iterator::operator*)>)
		{
			std::unordered_set<value_type> unorderedSet;
			for (TIterator it = _begin; it != _end; ++it)
				unorderedSet.insert(*it);
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
		noexcept(std::is_nothrow_invocable_v<decltype(&const_iterator::operator*)>)
		{
			std::map<TKey, TValue> map;
			for (TIterator it = _begin; it != _end; ++it)
				map.insert(*it);
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
		noexcept(std::is_nothrow_invocable_v<decltype(&const_iterator::operator*)>)
		{
			std::unordered_map<TKey, TValue> unorderedMap;
			for (TIterator it = _begin; it != _end; ++it)
				unorderedMap.insert(*it);
			return unorderedMap;
		}

		/// @brief 
		/// @tparam TMap 
		/// @param mapFunction 
		/// @return 
		template<std::invocable<value_type> TMap>
		requires std::same_as<std::invoke_result_t<TMap, value_type>, value_type>
		LinqView<SelectorIterator<value_type, TIterator, TMap>> Map(TMap&& mapFunction) const noexcept
		{
			return LinqView<SelectorIterator<value_type, TIterator, TMap>>(
				SelectorIterator<value_type, TIterator, TMap>(_begin, std::forward<TMap>(mapFunction)),
				SelectorIterator<value_type, TIterator, TMap>(_end));
		}

		/// @brief 
		/// @tparam TTransform 
		/// @param transform 
		/// @return 
		template<std::invocable<value_type&> TTransform>
		requires std::same_as<std::invoke_result_t<TTransform, value_type&>, void>
		LinqView<TransformIterator<TIterator, TTransform>> Transform(TTransform&& transform) const noexcept
		{
			return LinqView<TransformIterator<TIterator, TTransform>>(
				TransformIterator<TIterator, TTransform>(_begin, std::forward<TTransform>(transform)),
				TransformIterator<TIterator, TTransform>(_end));
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
		LinqView<WhereIterator<TIterator, TPredicate>> Where(TPredicate&& predicate) const
		noexcept(std::is_nothrow_constructible_v<WhereIterator<TIterator, TPredicate>, TIterator, TPredicate>)
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
		LinqView<RemoveWhereIterator<TIterator, TPredicate>> RemoveWhere(TPredicate&& predicate) const
		noexcept(std::is_nothrow_constructible_v<WhereIterator<TIterator, TPredicate>, TIterator, TPredicate>)
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
				value_type value = *it;
				if (!result.contains(keySelector(value)))
					result.insert(std::pair<TKey, std::vector<value_type>>(keySelector(value), std::vector<value_type>()));
				result.at(keySelector(value)).push_back(std::move(value));
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
		template<Concepts::ForwardIterable TOtherCollection,
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
																		TResultSelector&& resultSelector) const
		noexcept(std::is_nothrow_constructible_v<JoinIterator<TIterator, TOtherCollection, TInnerKeySelector, TOtherKeySelector, TResultSelector>,
												 TOtherCollection, TInnerKeySelector, TOtherKeySelector, TResultSelector>)
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
		template<Concepts::ForwardIterable TOtherCollection>
		LinqView<ZipIterator<TIterator, TOtherCollection>> Zip(const TOtherCollection& otherCollection) const noexcept
		{
			return LinqView<ZipIterator<TIterator, TOtherCollection>>(
				ZipIterator<TIterator, TOtherCollection>(_begin, otherCollection),
				ZipIterator<TIterator, TOtherCollection>(_end));
		}

		/// @brief 
		/// @param count 
		/// @return 
		LinqView<SkipIterator<TIterator>> Skip(const std::size_t count) const
		noexcept(std::is_nothrow_constructible_v<SkipIterator<TIterator>, TIterator, std::size_t>)
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
		LinqView<SkipWhileIterator<TIterator, TPredicate>> SkipWhile(TPredicate&& predicate) const
		noexcept(std::is_nothrow_constructible_v<SkipWhileIterator<TIterator, TPredicate>, TIterator, TPredicate>)
		{
			return LinqView<SkipWhileIterator<TIterator, TPredicate>>(
				SkipWhileIterator<TIterator, TPredicate>(_begin, std::forward<TPredicate>(predicate)),
				SkipWhileIterator<TIterator, TPredicate>(_end));
		}

		/// @brief 
		/// @param count 
		/// @return 
		LinqView<TakeIterator<TIterator>> Take(const std::size_t count) const
		noexcept(std::is_nothrow_constructible_v<TakeIterator<TIterator>, TIterator, std::size_t>)
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
		LinqView<TakeWhileIterator<TIterator, TPredicate>> TakeWhile(TPredicate&& predicate) const
		noexcept(std::is_nothrow_constructible_v<TakeWhileIterator<TIterator, TPredicate>, TIterator, TPredicate>)
		{
			return LinqView<TakeWhileIterator<TIterator, TPredicate>>(
				TakeWhileIterator<TIterator, TPredicate>(_begin, std::forward<TPredicate>(predicate)),
				TakeWhileIterator<TIterator, TPredicate>(_end));
		}
	};
}

#endif