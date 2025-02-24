#ifndef LINQ_LinqGenerator_H
#define LINQ_LinqGenerator_H

#include <array>
#include <list>
#include <forward_list>
#include <stack>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stdexcept>
#include <utility>
#include <type_traits>
#include <vector>
#include <concepts>

#include <ExtendedCpp/LINQ/YieldForeach.h>
#include <ExtendedCpp/LINQ/Aggregate.h>
#include <ExtendedCpp/LINQ/Sort.h>
#include <ExtendedCpp/LINQ/TypeTraits.h>
#include <ExtendedCpp/LINQ/Concepts.h>
#include <ExtendedCpp/LINQ/OrderType.h>
#include <ExtendedCpp/LINQ/Future.h>

/// @brief 
namespace ExtendedCpp::LINQ
{
	/// @brief 
	/// @tparam TSource 
	template<typename TSource>
	class LinqGenerator final
	{
	private:
		Future<TSource> _yieldContext;

		class Iterator final
		{
		public:
			Iterator(Future<TSource>& yieldContext, bool isEnd) noexcept :
					_yieldContext(yieldContext), _isEnd(isEnd)
			{
				if (_yieldContext)
					_value = _yieldContext.Value();
				else
					_isEnd = true;
			}

			TSource& operator*() noexcept
			{
				return _value;
			}

			const TSource& operator*() const noexcept
			{
				return _value;
			}

			Iterator& operator++() noexcept
			{
				if (_yieldContext)
					_value = _yieldContext.Next();
				else
					_isEnd = true;
				return *this;
			}

			bool operator!=(const Iterator& other) const noexcept
			{
				return _isEnd != other._isEnd;
			}

		private:
			Future<TSource>& _yieldContext;
			TSource _value;
			bool _isEnd;
		};

	public:
		/// @brief 
		using value_type = TSource;

		/// @brief 
		using iterator = Iterator;

		/// @brief 
		using promise_type = typename Future<TSource>::promise_type;

		/// @brief 
		using handle_type = typename Future<TSource>::handle_type;

		/// @brief 
		/// @tparam TGenerator 
		/// @tparam ...Args 
		/// @param generator 
		/// @param ...args 
		template<typename TGenerator, typename... Args>
		requires std::invocable<TGenerator, Args...>
		explicit LinqGenerator(TGenerator&& generator, Args... args) noexcept :
			_yieldContext(generator(std::forward<Args>(args)...)) {}

		/// @brief Copy data from vector into LINQ generator
		/// @tparam TCollection 
		/// @param collection 
		template<Concepts::ConstIterable TCollection>
		explicit LinqGenerator(const TCollection& collection) noexcept :
			_yieldContext(YieldForeach(collection)) {}

		/// @brief Move data from vector into LINQ generator
		/// @tparam TCollection 
		/// @param collection 
		template<Concepts::Iterable TCollection>
		explicit LinqGenerator(TCollection&& collection) noexcept :
			_yieldContext(YieldForeach(std::forward<TCollection>(collection))) {}

		/// @brief 
		/// @tparam TIterator 
		/// @param begin 
		/// @param end 
		template<std::forward_iterator TIterator>
		LinqGenerator(const TIterator begin, const TIterator end) noexcept :
			_yieldContext(YieldForeach(begin, end)) {}

		/// @brief Default destructor
		~LinqGenerator() = default;

		/// @brief 
		explicit operator bool() noexcept
		{
			return static_cast<bool>(_yieldContext);
		}

		/// @brief 
		/// @return 
		TSource Next() noexcept
		{
			return _yieldContext.Next();
		}

		/// @brief 
		/// @return 
		Iterator begin() noexcept
		{
			return Iterator(_yieldContext, false);
		}

		/// @brief 
		/// @return 
		Iterator end() noexcept
		{
			return Iterator(_yieldContext, true);
		}

		/// @brief 
		/// @return 
		std::vector<TSource> ToVector() noexcept
		{
			std::vector<TSource> collection;
			while (_yieldContext)
				collection.push_back(_yieldContext.Next());
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @tparam SIZE SIZE Size of returned array
		/// @return Copies of elements from 0 to min of array size or LINQ generator size
		template<std::size_t SIZE>
		requires std::is_default_constructible_v<TSource>
		std::array<TSource, SIZE> ToArray() 
		noexcept(std::is_nothrow_default_constructible_v<TSource>)
		{
			std::array<TSource, SIZE> collection{};
			for (std::size_t i = 0; i < SIZE && _yieldContext; ++i)
				collection[i] = _yieldContext.Next();
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @return Copies of elements from LINQ generator, maintaining order
		std::list<TSource> ToList() noexcept
		{
			std::list<TSource> collection;
			while (_yieldContext)
				collection.push_back(_yieldContext.Next());
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @return Copies of elements from LINQ generator, maintaining order
		std::forward_list<TSource> ToForwardList() noexcept
		{
			std::forward_list<TSource> collection;
			while (_yieldContext)
				collection.push_front(_yieldContext.Next());
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @return Copies of elements from LINQ generator, maintaining order
		std::stack<TSource> ToStack() noexcept
		{
			std::stack<TSource> collection;
			while (_yieldContext)
				collection.push(_yieldContext.Next());
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @return Copies of elements from LINQ generator, maintaining order
		std::queue<TSource> ToQueue() noexcept
		{
			std::queue<TSource> collection;
			while (_yieldContext)
				collection.push(_yieldContext.Next());
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @return Copies of elements from LINQ generator, maintaining order
		std::deque<TSource> ToDeque() noexcept
		{
			std::deque<TSource> collection;
			while (_yieldContext)
				collection.push_back(_yieldContext.Next());
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @return Copies of elements from LINQ generator, maintaining order
		std::priority_queue<TSource> ToPriorityQueue() noexcept
		{
			std::priority_queue<TSource> collection;
			while (_yieldContext)
				collection.push(_yieldContext.Next());
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @return Unique elements from LINQ generator
		std::set<TSource> ToSet() noexcept
		{
			std::set<TSource> collection;
			while (_yieldContext)
				collection.insert(_yieldContext.Next());
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @return Unique elements from LINQ generator
		std::unordered_set<TSource> ToUnorderedSet() noexcept
		{
			std::unordered_set<TSource> collection;
			while (_yieldContext)
				collection.insert(_yieldContext.Next());
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @tparam TKey 
		/// @tparam TValue 
		/// @return map<TKey, TValue> from vector<std::pair<TKey, TValue>>
		template<typename TKey = typename PairTraits<TSource>::FirstType,
				 typename TValue = typename PairTraits<TSource>::SecondType>
		requires Concepts::IsPair<TSource>
		std::map<TKey, TValue> ToMap() noexcept
		{
			std::map<TKey, TValue> collection;
			while (_yieldContext)
				collection.insert(_yieldContext.Next());
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @tparam TKey 
		/// @tparam TValue 
		/// @return unordered_map<TKey, TValue> from vector<std::pair<TKey, TValue>>
		template<typename TKey = typename PairTraits<TSource>::FirstType,
				 typename TValue = typename PairTraits<TSource>::SecondType>
		requires Concepts::IsPair<TSource>
		std::unordered_map<TKey, TValue> ToUnorderedMap() noexcept
		{
			std::unordered_map<TKey, TValue> collection;
			while (_yieldContext)
				collection.insert(_yieldContext.Next());
			return collection;
		}

		/// @brief Iterates through all elements and applies a selector to each
		/// @tparam TResult Result of selector invoke
		/// @tparam TSelector Any functional object with TSource argument
		/// @param selector Any functional object with TSource argument
		/// @return New collection LinqGenerator<TResult>
		template<std::invocable<TSource> TSelector,
				 typename TResult = std::invoke_result_t<TSelector, TSource>>
		LinqGenerator<TResult> Select(TSelector&& selector)
		noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
		{
			return LinqGenerator<TResult>([this](TSelector&& selector_)
				{ return SelectGenerator(std::forward<TSelector>(selector_)); },
						std::forward<TSelector>(selector));
		}

		/// @brief Iterates through all elements and applies a selector to each
		/// @tparam TResult value_type of selector invoke result
		/// @tparam TSelector Any functional object with TSource argument
		/// @param selector Any functional object with TSource argument
		/// @return New collection LinqGenerator<TResult>
		template<std::invocable<TSource> TSelector,
				 typename TResult = typename std::invoke_result_t<TSelector, TSource>::value_type>
		LinqGenerator<TResult> SelectMany(TSelector&& selector)
		noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
		{
			return LinqGenerator<TResult>([this](TSelector&& selector_)
				{ return SelectManyGenerator(std::forward<TSelector>(selector_)); },
						std::forward<TSelector>(selector));
		}

		/// @brief 
		/// @tparam TCollectionValueType 
		/// @tparam TResult 
		/// @tparam TCollectionSelector 
		/// @tparam TCollection 
		/// @tparam TResultSelector 
		/// @param collectionSelector 
		/// @param resultSelector 
		/// @return 
		template<std::invocable<TSource> TCollectionSelector,
				 Concepts::Iterable TCollection = std::invoke_result_t<TCollectionSelector, TSource>,
				 typename TCollectionValueType = typename TCollection::value_type,
				 std::invocable<TSource, TCollectionValueType> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, TSource, TCollectionValueType>>
		LinqGenerator<TResult> SelectMany(TCollectionSelector&& collectionSelector, TResultSelector&& resultSelector)
		noexcept(std::is_nothrow_invocable_v<TCollectionSelector, TSource> &&
				 std::is_nothrow_invocable_v<TResultSelector, TSource, TCollectionValueType>)
		{
			return LinqGenerator<TResult>([this](TCollectionSelector&& collectionSelector_, TResultSelector&& resultSelector_)
				{ return SelectManyGenerator(std::forward<TCollectionSelector>(collectionSelector_),
											 std::forward<TResultSelector>(resultSelector_)); },
						std::forward<TCollectionSelector>(collectionSelector),
						std::forward<TResultSelector>(resultSelector));
		}

		/// @brief Select elements from some set by condition
		/// @tparam TPredicate 
		/// @param predicate 
		/// @return 
		template<Concepts::IsPredicate<TSource> TPredicate>
		LinqGenerator Where(TPredicate&& predicate)
		noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
		{
			return LinqGenerator([this](TPredicate&& predicate_)
				{ return WhereGenerator(std::forward<TPredicate>(predicate_)); },
						std::forward<TPredicate>(predicate));
		}

		/// @brief Remove elements from some set by condition
		/// @tparam TPredicate 
		/// @param predicate 
		/// @return 
		template<Concepts::IsPredicate<TSource> TPredicate>
		LinqGenerator RemoveWhere(TPredicate&& predicate)
		noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
		{
			return LinqGenerator([this](TPredicate&& predicate_)
				{ return RemoveWhereGenerator(std::forward<TPredicate>(predicate_)); },
						std::forward<TPredicate>(predicate));
		}

		/// @brief Sorts the elements of a collection
		/// @param orderType 
		/// @return 
		LinqGenerator Order(OrderType orderType = OrderType::ASC) noexcept
		requires Concepts::Comparable<TSource>
		{
			std::vector<TSource> newCollection;
			while (_yieldContext)
				newCollection.push_back(_yieldContext.Next());
			if (newCollection.empty())
				return *this;

			Sort::QuickSort(newCollection.data(), 0, newCollection.size() - 1, orderType);

			return LinqGenerator([this, newCollection = std::move(newCollection)]()
				{ return Generator(newCollection); });
		}

		/// @brief Sorts the elements of a collection with selector
		/// @tparam TSelector 
		/// @param selector 
		/// @param orderType 
		/// @return 
		template<std::invocable<TSource> TSelector>
		requires Concepts::Comparable<std::invoke_result_t<TSelector, TSource>>
		LinqGenerator OrderBy(TSelector&& selector, OrderType orderType = OrderType::ASC)
		noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
		{
			std::vector<TSource> newCollection;
			while (_yieldContext)
				newCollection.push_back(_yieldContext.Next());
			if (newCollection.empty())
				return *this;

			Sort::QuickSort(newCollection.data(), 0, newCollection.size() - 1,
							std::forward<TSelector>(selector), orderType);

			return LinqGenerator([this, newCollection = std::move(newCollection)]()
				{ return Generator(newCollection); });
		}

		/// @brief Reverse the collection
		/// @return 
		LinqGenerator Reverse() noexcept
		{
			std::vector<TSource> collection;
			while (_yieldContext)
				collection.push_back(_yieldContext.Next());

			return LinqGenerator([this, collection = std::move(collection)]()
				{ return ReverseGenerator(collection); });
		}

		/// @brief Get the difference of two sequences
		/// @tparam TOtherCollection 
		/// @param otherCollection 
		/// @return 
		template<typename TOtherCollection>
		requires Concepts::ConstIterable<TOtherCollection> &&
				 Concepts::HasSize<TOtherCollection> &&
				 Concepts::Equatable<TSource> &&
				 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
		LinqGenerator Except(const TOtherCollection& otherCollection) noexcept
		{
			std::set<TSource> newCollection;

			while (_yieldContext)
			{
				auto element = _yieldContext.Next();

				std::size_t i = 0;
				for (const auto& otherElement : otherCollection)
				{
					if (element == otherElement) break;
					if (i == otherCollection.size() - 1) newCollection.insert(element);
					++i;
				}
			}

			return LinqGenerator([this, newCollection = std::move(newCollection)]()
				{ return Generator(newCollection); });
		}

		/// @brief Get the difference of two sequences
		/// @tparam TOtherCollection 
		/// @param otherCollection 
		/// @return 
		template<typename TOtherCollection>
		requires Concepts::Iterable<TOtherCollection> &&
				 Concepts::HasSize<TOtherCollection> &&
				 Concepts::Equatable<TSource> &&
				 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
		LinqGenerator Except(TOtherCollection&& otherCollection) noexcept
		{
			std::set<TSource> newCollection;

			while (_yieldContext)
			{
				auto element = _yieldContext.Next();

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

			return LinqGenerator([this, newCollection = std::move(newCollection)]()
				{ return Generator(newCollection); });
		}

		/// @brief Get the intersection of sequences
		/// @tparam TOtherCollection 
		/// @param otherCollection 
		/// @return 
		template<Concepts::ConstIterable TOtherCollection>
		requires Concepts::Equatable<TSource> &&
				 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
		LinqGenerator Intersect(const TOtherCollection& otherCollection) noexcept
		{
			std::set<TSource> newCollection;

			while (_yieldContext)
			{
				auto element = _yieldContext.Next();
				for (const auto& otherElement : otherCollection)
					if (element == otherElement)
					{
						newCollection.insert(element);
						break;
					}
			}

			return LinqGenerator([this, newCollection = std::move(newCollection)]()
				{ return Generator(newCollection); });
		}

		/// @brief Get the intersection of sequences
		/// @tparam TOtherCollection 
		/// @param otherCollection 
		/// @return 
		template<Concepts::Iterable TOtherCollection>
		requires Concepts::Equatable<TSource> &&
				 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
		LinqGenerator Intersect(TOtherCollection&& otherCollection) noexcept
		{
			std::set<TSource> newCollection;
			auto inner = std::forward<TOtherCollection>(otherCollection);

			while (_yieldContext)
			{
				auto element = _yieldContext.Next();
				for (auto&& otherElement : inner)
					if (element == otherElement)
					{
						newCollection.insert(std::move(otherElement));
						break;
					}
			}

			return LinqGenerator([this, newCollection = std::move(newCollection)]()
				{ return Generator(newCollection); });
		}

		/// @brief Remove duplicates in a set
		/// @return 
		LinqGenerator Distinct() noexcept
		requires Concepts::Equatable<TSource>
		{
			std::set<TSource> newCollection;
			while (_yieldContext)
				newCollection.insert(_yieldContext.Next());

			return LinqGenerator([this, newCollection = std::move(newCollection)]()
				{ return Generator(newCollection); });
		}

		/// @brief Join two sequences
		/// @tparam TOtherCollection 
		/// @param otherCollection 
		/// @return 
		template<Concepts::ConstIterable TOtherCollection>
		requires Concepts::Equatable<TSource> &&
				 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
		LinqGenerator Union(const TOtherCollection& otherCollection) noexcept
		{
			std::set<TSource> newCollection;

			while (_yieldContext)
				newCollection.insert(_yieldContext.Next());

			for (const auto& element : otherCollection)
				newCollection.insert(element);

			return LinqGenerator([this, newCollection = std::move(newCollection)]()
				{ return Generator(newCollection); });
		}

		/// @brief Join two sequences
		/// @tparam TOtherCollection 
		/// @param otherCollection 
		/// @return 
		template<Concepts::Iterable TOtherCollection>
		requires Concepts::Equatable<TSource> &&
				 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
		LinqGenerator Union(TOtherCollection&& otherCollection) noexcept
		{
			std::set<TSource> newCollection;

			while (_yieldContext)
				newCollection.insert(_yieldContext.Next());

			for (auto&& element : otherCollection)
				newCollection.insert(std::move(element));

			return LinqGenerator([this, newCollection = std::move(newCollection)]()
				{ return Generator(newCollection); });
		}

		/// @brief Performs a general aggregation of the elements of the collection depending on the specified expression. 
		/// After this method generator became invalid
		/// @tparam TResult 
		/// @tparam TAggregate 
		/// @param aggregateFunction 
		/// @return 
		template<typename TResult, std::invocable<TResult, TSource> TAggregate>
		TResult Aggregate(TAggregate&& aggregateFunction)
		{
			std::vector<TSource> collection;
			while (_yieldContext)
				collection.push_back(_yieldContext.Next());

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
		std::size_t Count(TPredicate&& predicate)
		noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
		{
			std::size_t result = 0;
			while (_yieldContext)
				if (predicate(_yieldContext.Next())) ++result;

			return result;
		}

		/// @brief Get the sum of values. After this method generator became invalid
		/// @return 
		TSource Sum()
		requires Concepts::Summarize<TSource>
		{
			std::vector<TSource> collection;
			while (_yieldContext)
				collection.push_back(_yieldContext.Next());

			if (collection.empty())
				throw std::out_of_range("Collection is empty");

			return Aggregate::Sum(collection.data(), 0, collection.size() - 1);
		}

		/// @brief Get the sum of values. After this method generator became invalid
		/// @tparam TSelector 
		/// @tparam TResult 
		/// @param selector 
		/// @return 
		template<std::invocable<TSource> TSelector,
				 Concepts::Summarize TResult = std::invoke_result_t<TSelector, TSource>>
		TResult Sum(TSelector&& selector)
		{
			std::vector<TSource> collection;
			while (_yieldContext)
				collection.push_back(_yieldContext.Next());

			if (collection.empty())
				throw std::out_of_range("Collection is empty");

			return Aggregate::Sum(collection.data(), 0, collection.size() - 1, std::forward<TSelector>(selector));
		}

		/// @brief Find element with the minimum value. After this method generator became invalid
		/// @return 
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
		LinqGenerator Skip(const std::size_t count) noexcept
		{
			return LinqGenerator([this](const std::size_t count_)
				{ return SkipGenerator(count_); }, count);
		}

		/// @brief Skips a chain of elements, starting with the first element, as long as they satisfy a certain condition
		/// @tparam TPredicate 
		/// @param predicate 
		/// @return 
		template<Concepts::IsPredicate<TSource> TPredicate>
		LinqGenerator SkipWhile(TPredicate&& predicate)
		noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
		{
			return LinqGenerator([this](TPredicate&& predicate_)
				{ return SkipWhileGenerator(std::forward<TPredicate>(predicate_)); },
						std::forward<TPredicate>(predicate));
		}

		/// @brief Retrieves a certain number of elements
		/// @param count 
		/// @return 
		LinqGenerator Take(const std::size_t count)
		{
			return LinqGenerator([this](const std::size_t count_)
				{ return TakeGenerator(count_); }, count);
		}

		/// @brief Retrieves a certain number of elements from the end of the collection
		/// @tparam TPredicate 
		/// @param predicate 
		/// @return 
		template<Concepts::IsPredicate<TSource> TPredicate>
		LinqGenerator TakeWhile(TPredicate&& predicate) noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
		{
			return LinqGenerator([this](TPredicate&& predicate_)
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
		LinqGenerator<std::pair<TKey, std::vector<TSource>>> GroupBy(TKeySelector&& keySelector)
		noexcept(std::is_nothrow_invocable_v<TKeySelector, TSource>)
		{
			return LinqGenerator<std::pair<TKey, std::vector<TSource>>>([this](TKeySelector&& keySelector_)
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
		template<Concepts::ConstIterable TOtherCollection,
				 std::invocable<TSource> TInnerKeySelector,
				 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
				 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, TSource, typename TOtherCollection::value_type>>
		requires std::same_as<std::invoke_result_t<TInnerKeySelector, TSource>,
							  std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
				 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, TSource>>
		LinqGenerator<TResult> Join(const TOtherCollection& otherCollection,
									TInnerKeySelector&& innerKeySelector,
									TOtherKeySelector&& otherKeySelector,
									TResultSelector&& resultSelector)
		{
			if (otherCollection.empty())
				throw std::invalid_argument("other collection must not be empty");

			return LinqGenerator<TResult>([this, otherCollection](
					TInnerKeySelector&& innerKeySelector_,
					TOtherKeySelector&& otherKeySelector_,
					TResultSelector&& resultSelector_)
				{ return JoinGenerator(otherCollection,
					std::forward<TInnerKeySelector>(innerKeySelector_),
					std::forward<TOtherKeySelector>(otherKeySelector_),
					std::forward<TResultSelector>(resultSelector_)); },
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
		template<Concepts::Iterable TOtherCollection,
				 std::invocable<TSource> TInnerKeySelector,
				 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
				 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, TSource, typename TOtherCollection::value_type>>
		requires std::same_as<std::invoke_result_t<TInnerKeySelector, TSource>,
							  std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
				 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, TSource>>
		LinqGenerator<TResult> Join(TOtherCollection&& otherCollection,
									TInnerKeySelector&& innerKeySelector,
									TOtherKeySelector&& otherKeySelector,
									TResultSelector&& resultSelector)
		{
			if (otherCollection.empty())
				throw std::invalid_argument("other collection must not be empty");

			return LinqGenerator<TResult>([this, otherCollection = std::forward<TOtherCollection>(otherCollection)](
					TInnerKeySelector&& innerKeySelector_,
					TOtherKeySelector&& otherKeySelector_,
					TResultSelector&& resultSelector_)
				{ return JoinGenerator(otherCollection,
					std::forward<TInnerKeySelector>(innerKeySelector_),
					std::forward<TOtherKeySelector>(otherKeySelector_),
					std::forward<TResultSelector>(resultSelector_)); },
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
		template<Concepts::ConstIterable TOtherCollection,
				 std::invocable<TSource> TInnerKeySelector,
				 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, TSource>,
				 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
				 std::invocable<const std::vector<TSource>&, typename TOtherCollection::value_type> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>>
		requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>>
		LinqGenerator<TResult> GroupJoin(const TOtherCollection& otherCollection,
										 TInnerKeySelector&& innerKeySelector,
										 TOtherKeySelector&& otherKeySelector,
										 TResultSelector&& resultSelector)
		{
			if (otherCollection.empty())
				throw std::invalid_argument("other collection must not be empty");

			return LinqGenerator<TResult>([this, otherCollection](
					TInnerKeySelector&& innerKeySelector_,
					TOtherKeySelector&& otherKeySelector_,
					TResultSelector&& resultSelector_)
				{ return GroupJoinGenerator(otherCollection,
					std::forward<TInnerKeySelector>(innerKeySelector_),
					std::forward<TOtherKeySelector>(otherKeySelector_),
					std::forward<TResultSelector>(resultSelector_)); },
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
		template<Concepts::Iterable TOtherCollection,
				 std::invocable<TSource> TInnerKeySelector,
				 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, TSource>,
				 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
				 std::invocable<const std::vector<TSource>&, typename TOtherCollection::value_type> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>>
		requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>>
		LinqGenerator<TResult> GroupJoin(TOtherCollection&& otherCollection,
										 TInnerKeySelector&& innerKeySelector,
										 TOtherKeySelector&& otherKeySelector,
										 TResultSelector&& resultSelector)
		{
			if (otherCollection.empty())
				throw std::invalid_argument("other collection must not be empty");

			return LinqGenerator<TResult>([this, otherCollection = std::forward<TOtherCollection>(otherCollection)](
					TInnerKeySelector&& innerKeySelector_,
					TOtherKeySelector&& otherKeySelector_,
					TResultSelector&& resultSelector_)
				{ return GroupJoinGenerator(otherCollection,
					std::forward<TInnerKeySelector>(innerKeySelector_),
					std::forward<TOtherKeySelector>(otherKeySelector_),
					std::forward<TResultSelector>(resultSelector_)); },
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
		requires Concepts::ConstIterable<TOtherCollection> && Concepts::HasSize<TOtherCollection>
		LinqGenerator<std::pair<TSource, TOtherCollectionValueType>> Zip(const TOtherCollection& otherCollection) noexcept
		{
			return LinqGenerator<std::pair<TSource, TOtherCollectionValueType>>(
					[this, otherCollection]
					{ return ZipGenerator(otherCollection); });
		}

		/// @brief Sequentially concatenates the corresponding elements of the current sequence with the second sequence
		/// @tparam TOtherCollection 
		/// @tparam TOtherCollectionValueType 
		/// @param otherCollection 
		/// @return 
		template<typename TOtherCollection,
				 typename TOtherCollectionValueType = typename TOtherCollection::value_type>
		requires Concepts::Iterable<TOtherCollection> && Concepts::HasSize<TOtherCollection>
		LinqGenerator<std::pair<TSource, TOtherCollectionValueType>> Zip(TOtherCollection&& otherCollection) noexcept
		{
			return LinqGenerator<std::pair<TSource, TOtherCollectionValueType>>(
					[this, otherCollection = std::forward<TOtherCollection>(otherCollection)]
					{ return ZipGenerator(otherCollection); });
		}

		/// @brief Checks if all elements match a condition. If all elements match the condition, then true is returned.
		/// After this method generator became invalid
		/// @tparam TPredicate 
		/// @param predicate 
		/// @return 
		template<Concepts::IsPredicate<TSource> TPredicate>
		bool All(TPredicate&& predicate) noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
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
		bool Any(TPredicate&& predicate) noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
		{
			while (_yieldContext)
				if (predicate(_yieldContext.Next()))
					return true;
			return false;
		}

	private:
		template<Concepts::ConstIterable TCollection>
		Future<TSource> Generator(TCollection collection) noexcept
		{
			for (auto&& element : collection)
				co_yield std::move(element);
		}

		template<Concepts::Iterable TCollection>
		Future<TSource> Generator(TCollection&& collection) noexcept
		{
			auto inner = std::forward<TCollection>(collection);
			for (auto&& element : inner)
				co_yield std::move(element);
		}

		template<Concepts::ConstIterable TCollection>
		Future<TSource> ReverseGenerator(TCollection collection) noexcept
		{
			for (auto it = collection.rbegin(); it != collection.rend(); ++it)
				co_yield std::move(*it);
		}

		template<Concepts::Iterable TCollection>
		Future<TSource> ReverseGenerator(TCollection&& collection) noexcept
		{
			auto inner = std::forward<TCollection>(collection);
			for (auto it = inner.rbegin(); it != inner.rend(); ++it)
				co_yield std::move(*it);
		}

		template<std::invocable<TSource> TSelector,
				 typename TResult = std::invoke_result_t<TSelector, TSource>>
		Future<TResult> SelectGenerator(TSelector&& selector)
		noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
		{
			while (_yieldContext)
				co_yield selector(_yieldContext.Next());
		}

		template<std::invocable<TSource> TSelector,
				 typename TResult = std::invoke_result_t<TSelector, TSource>::value_type>
		Future<TResult> SelectManyGenerator(TSelector&& selector)
		noexcept(std::is_nothrow_invocable_v<TSelector, TSource>)
		{
			while (_yieldContext)
			{
				std::vector<TResult> tempCollection = selector(_yieldContext.Next());
				for (auto&& tempElement : tempCollection)
					co_yield std::move(tempElement);
			}
		}

		template<std::invocable<TSource> TCollectionSelector,
				 Concepts::Iterable TCollection = std::invoke_result_t<TCollectionSelector, TSource>,
				 typename TCollectionValueType = TCollection::value_type,
				 std::invocable<TSource, TCollectionValueType> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, TSource, TCollectionValueType>>
		Future<TResult> SelectManyGenerator(TCollectionSelector&& collectionSelector, TResultSelector&& resultSelector)
		noexcept(std::is_nothrow_invocable_v<TCollectionSelector, TSource> &&
				 std::is_nothrow_invocable_v<TResultSelector, TSource, TCollectionValueType>)
		{
			while (_yieldContext)
			{
				auto element = _yieldContext.Next();
				TCollection tempCollection = collectionSelector(element);
				for (auto&& tempElement : tempCollection)
					co_yield resultSelector(element, std::move(tempElement));
			}
		}

		template<Concepts::IsPredicate<TSource> TPredicate>
		Future<TSource> WhereGenerator(TPredicate&& predicate)
		noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
		{
			while (_yieldContext)
			{
				auto element = _yieldContext.Next();
				if (predicate(element))
					co_yield element;
			}
		}

		template<Concepts::IsPredicate<TSource> TPredicate>
		Future<TSource> RemoveWhereGenerator(TPredicate&& predicate)
		noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
		{
			while (_yieldContext)
			{
				auto element = _yieldContext.Next();
				if (predicate(element))
					continue;
				else
					co_yield element;
			}
		}

		Future<TSource> SkipGenerator(const std::size_t count) noexcept
		{
			std::size_t i = 0;
			while (_yieldContext && i < count)
			{
				_yieldContext.Next();
				++i;
			}

			while (_yieldContext)
				co_yield _yieldContext.Next();
		}

		template<Concepts::IsPredicate<TSource> TPredicate>
		Future<TSource> SkipWhileGenerator(TPredicate&& predicate)
		noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
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
		}

		Future<TSource> TakeGenerator(const std::size_t count)
		{
			std::size_t i = 0;
			while (_yieldContext && i < count)
			{
				co_yield _yieldContext.Next();
				++i;
			}

			while (_yieldContext)
				_yieldContext.Next();
		}

		template<Concepts::IsPredicate<TSource> TPredicate>
		Future<TSource> TakeWhileGenerator(TPredicate&& predicate)
		noexcept(std::is_nothrow_invocable_v<TPredicate, TSource>)
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
		}

		template<std::invocable<TSource> TKeySelector,
				 typename TKey = std::invoke_result_t<TKeySelector, TSource>>
		Future<std::pair<TKey, std::vector<TSource>>> GroupByGenerator(TKeySelector&& keySelector)
		noexcept(std::is_nothrow_invocable_v<TKeySelector, TSource>)
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
		}

		template<Concepts::ConstIterable TOtherCollection,
				 std::invocable<TSource> TInnerKeySelector,
				 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
				 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, TSource, typename TOtherCollection::value_type>>
		requires std::same_as<std::invoke_result_t<TInnerKeySelector, TSource>,
							  std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
				 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, TSource>>
		Future<TResult> JoinGenerator(TOtherCollection otherCollection,
									  TInnerKeySelector&& innerKeySelector,
									  TOtherKeySelector&& otherKeySelector,
									  TResultSelector&& resultSelector)
		noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
				 std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
				 std::is_nothrow_invocable_v<TResultSelector, TSource, typename TOtherCollection::value_type>)
		{
			while (_yieldContext)
			{
				auto element = _yieldContext.Next();
				for (auto&& otherElement : otherCollection)
					if (innerKeySelector(element) == otherKeySelector(otherElement))
						co_yield resultSelector(element, std::move(otherElement));
			}
		}

		template<Concepts::Iterable TOtherCollection,
				 std::invocable<TSource> TInnerKeySelector,
				 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
				 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, TSource, typename TOtherCollection::value_type>>
		requires std::same_as<std::invoke_result_t<TInnerKeySelector, TSource>,
							  std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
				 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, TSource>>
		Future<TResult> JoinGenerator(TOtherCollection&& otherCollection,
									  TInnerKeySelector&& innerKeySelector,
									  TOtherKeySelector&& otherKeySelector,
									  TResultSelector&& resultSelector)
		noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
				 std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
				 std::is_nothrow_invocable_v<TResultSelector, TSource, typename TOtherCollection::value_type>)
		{
			auto inner = std::forward<TOtherCollection>(otherCollection);
			while (_yieldContext)
			{
				auto element = _yieldContext.Next();
				for (auto&& otherElement : inner)
					if (innerKeySelector(element) == otherKeySelector(otherElement))
						co_yield resultSelector(std::move(element), std::move(otherElement));
			}
		}

		template<Concepts::ConstIterable TOtherCollection,
				 std::invocable<TSource> TInnerKeySelector,
				 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, TSource>,
				 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
				 std::invocable<const std::vector<TSource>&, typename TOtherCollection::value_type> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>>
		requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>>
		Future<TResult> GroupJoinGenerator(TOtherCollection otherCollection,
										   TInnerKeySelector&& innerKeySelector,
										   TOtherKeySelector&& otherKeySelector,
										   TResultSelector&& resultSelector)
		noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
				 std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
				 std::is_nothrow_invocable_v<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>)
		{
			auto groups = GroupBy(std::forward<TInnerKeySelector>(innerKeySelector));

			while (groups)
			{
				auto&& [key, group] = groups.Next();
				for (auto&& element : otherCollection)
					if (std::move(key) == otherKeySelector(element))
						co_yield resultSelector(group, std::move(element));
			}
		}

		template<Concepts::Iterable TOtherCollection,
				 std::invocable<TSource> TInnerKeySelector,
				 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, TSource>,
				 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
				 std::invocable<const std::vector<TSource>&, typename TOtherCollection::value_type> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>>
		requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>>
		Future<TResult> GroupJoinGenerator(TOtherCollection&& otherCollection,
										   TInnerKeySelector&& innerKeySelector,
										   TOtherKeySelector&& otherKeySelector,
										   TResultSelector&& resultSelector)
		noexcept(std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
				 std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
				 std::is_nothrow_invocable_v<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>)
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
		}

		template<typename TOtherCollection,
				 typename TOtherCollectionValueType = typename TOtherCollection::value_type>
		requires Concepts::ConstIterable<TOtherCollection> && Concepts::HasSize<TOtherCollection>
		Future<std::pair<TSource, TOtherCollectionValueType>> ZipGenerator(TOtherCollection otherCollection) noexcept
		{
			for (auto&& element : otherCollection)
				if (_yieldContext)
					co_yield std::make_pair(_yieldContext.Next(), std::move(element));
		}

		template<typename TOtherCollection,
				 typename TOtherCollectionValueType = typename TOtherCollection::value_type>
		requires Concepts::Iterable<TOtherCollection> && Concepts::HasSize<TOtherCollection>
		Future<std::pair<TSource, TOtherCollectionValueType>> ZipGenerator(TOtherCollection&& otherCollection) noexcept
		{
			auto inner = std::forward<TOtherCollection>(otherCollection);
			for (auto&& element : inner)
				if (_yieldContext)
					co_yield std::make_pair(_yieldContext.Next(), std::move(element));
		}
	};
}

#endif