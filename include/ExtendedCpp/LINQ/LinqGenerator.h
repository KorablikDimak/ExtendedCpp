#ifndef LINQ_LinqGenerator_H
#define LINQ_LinqGenerator_H

#include <vector>
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
	/// @tparam NothrowCoroutine
	template<typename TSource, typename NothrowCoroutine = std::false_type>
	class LinqGenerator final
	{
	private:
		Future<TSource, NothrowCoroutine> _yieldContext;
		static constexpr bool IsNothrowCoroutine = std::same_as<NothrowCoroutine, std::true_type>;

	public:
		/// @brief
		struct Iterator final
		{
			/// @brief
			using value_type = TSource;

			/// @brief
			using difference_type = std::ptrdiff_t;

			/// @brief
			/// @param yieldContext
			/// @param isEnd
			Iterator(Future<TSource, NothrowCoroutine>& yieldContext, const bool isEnd) noexcept :
				_yieldContext(yieldContext), _isEnd(isEnd)
			{
				if (_yieldContext)
					_value = _yieldContext.Value();
				else
					_isEnd = true;
			}

			/// @brief
			/// @param other
			Iterator(const Iterator& other) noexcept = default;

			/// @brief
			/// @param other
			Iterator(Iterator&& other) noexcept = default;

			/// @brief
			/// @param other
			/// @return
			Iterator& operator=(const Iterator& other) noexcept = default;

			/// @brief
			/// @param other
			/// @return
			Iterator& operator=(Iterator&& other) noexcept = default;

			/// @brief
			/// @return
			TSource& operator*() noexcept
			{
				return _value;
			}

			/// @brief
			/// @return
			const TSource& operator*() const noexcept
			{
				return _value;
			}

			/// @brief
			/// @return
			Iterator& operator++()
			noexcept(IsNothrowCoroutine)
			{
				if (_yieldContext)
					_value = _yieldContext.Next();
				else
					_isEnd = true;
				return *this;
			}

			/// @brief
			/// @param other
			/// @return
			bool operator==(const Iterator& other) const noexcept
			{
				return _isEnd == other._isEnd;
			}

			/// @brief
			/// @param other
			/// @return
			bool operator!=(const Iterator& other) const noexcept
			{
				return _isEnd != other._isEnd;
			}

		private:
			Future<TSource, NothrowCoroutine>& _yieldContext;
			TSource _value;
			bool _isEnd;
		};

		/// @brief 
		using value_type = TSource;

		/// @brief 
		using iterator = Iterator;

		/// @brief 
		using promise_type = typename Future<TSource, NothrowCoroutine>::promise_type;

		/// @brief 
		using handle_type = typename Future<TSource, NothrowCoroutine>::handle_type;

		/// @brief
		static constexpr bool IsLinqCollection = true;

		/// @brief 
		/// @tparam TGenerator 
		/// @tparam Args
		/// @param generator 
		/// @param args
		template<typename TGenerator, typename... Args>
		requires std::same_as<std::invoke_result_t<TGenerator, Args...>, Future<TSource, NothrowCoroutine>>
		explicit LinqGenerator(TGenerator&& generator, Args&&... args)
		noexcept(std::is_nothrow_invocable_v<TGenerator, Args...>) :
			_yieldContext(generator(std::forward<Args>(args)...)) {}

		/// @brief Move data from vector into LINQ generator
		/// @tparam TCollection 
		/// @param collection 
		template<Concepts::InputIterable TCollection>
		explicit LinqGenerator(TCollection&& collection) noexcept :
			_yieldContext(YieldForeach(std::forward<TCollection>(collection))) {}

		/// @brief 
		/// @tparam TIterator 
		/// @param begin 
		/// @param end 
		template<std::input_iterator TIterator>
		LinqGenerator(TIterator&& begin, TIterator&& end) noexcept :
			_yieldContext(YieldForeach(std::forward<TIterator>(begin), std::forward<TIterator>(end))) {}

		/// @brief
		/// @param other
		LinqGenerator(const LinqGenerator& other) noexcept = default;

		/// @brief
		/// @param other
		LinqGenerator(LinqGenerator&& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		LinqGenerator& operator=(const LinqGenerator& other) noexcept = default;

		/// @brief
		/// @param other
		/// @return
		LinqGenerator& operator=(LinqGenerator&& other) noexcept = default;

		/// @brief 
		explicit operator bool() noexcept
		{
			return static_cast<bool>(_yieldContext);
		}

		/// @brief 
		/// @return 
		TSource Next() noexcept(IsNothrowCoroutine)
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
		std::vector<TSource> ToVector() noexcept(IsNothrowCoroutine)
		{
			std::vector<TSource> collection;
			while (_yieldContext)
				collection.push_back(_yieldContext.Next());
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @tparam SIZE Size of returned array
		/// @return Copies of elements from 0 to min of array size or LINQ generator size
		template<std::size_t SIZE>
		requires std::is_default_constructible_v<TSource>
		std::array<TSource, SIZE> ToArray() noexcept(IsNothrowCoroutine)
		{
			std::array<TSource, SIZE> collection{};
			for (std::size_t i = 0; i < SIZE && _yieldContext; ++i)
				collection[i] = _yieldContext.Next();
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @return Copies of elements from LINQ generator, maintaining order
		std::list<TSource> ToList() noexcept(IsNothrowCoroutine)
		{
			std::list<TSource> collection;
			while (_yieldContext)
				collection.push_back(_yieldContext.Next());
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @return Copies of elements from LINQ generator, maintaining order
		std::forward_list<TSource> ToForwardList() noexcept(IsNothrowCoroutine)
		{
			const std::vector<TSource> collection = ToVector();
			return std::forward_list<TSource>(collection.cbegin(), collection.cend());
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @return Copies of elements from LINQ generator, maintaining order
		std::stack<TSource> ToStack() noexcept(IsNothrowCoroutine)
		{
			std::stack<TSource> collection;
			while (_yieldContext)
				collection.push(_yieldContext.Next());
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @return Copies of elements from LINQ generator, maintaining order
		std::queue<TSource> ToQueue() noexcept(IsNothrowCoroutine)
		{
			std::queue<TSource> collection;
			while (_yieldContext)
				collection.push(_yieldContext.Next());
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @return Copies of elements from LINQ generator, maintaining order
		std::deque<TSource> ToDeque() noexcept(IsNothrowCoroutine)
		{
			std::deque<TSource> collection;
			while (_yieldContext)
				collection.push_back(_yieldContext.Next());
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @return Copies of elements from LINQ generator, maintaining order
		std::priority_queue<TSource> ToPriorityQueue() noexcept(IsNothrowCoroutine)
		{
			std::priority_queue<TSource> collection;
			while (_yieldContext)
				collection.push(_yieldContext.Next());
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @return Unique elements from LINQ generator
		std::set<TSource> ToSet() noexcept(IsNothrowCoroutine)
		{
			std::set<TSource> collection;
			while (_yieldContext)
				collection.insert(_yieldContext.Next());
			return collection;
		}

		/// @brief Get copy of collection data. After this method generator became invalid
		/// @return Unique elements from LINQ generator
		std::unordered_set<TSource> ToUnorderedSet() noexcept(IsNothrowCoroutine)
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
		std::map<TKey, TValue> ToMap() noexcept(IsNothrowCoroutine)
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
		std::unordered_map<TKey, TValue> ToUnorderedMap() noexcept(IsNothrowCoroutine)
		{
			std::unordered_map<TKey, TValue> collection;
			while (_yieldContext)
				collection.insert(_yieldContext.Next());
			return collection;
		}

		/// @brief
		/// @tparam TMap
		/// @param mapFunction
		/// @return
		template<std::invocable<TSource> TMap>
        requires std::same_as<std::invoke_result_t<TMap, TSource>, TSource>
        auto Map(TMap&& mapFunction) noexcept
		{
			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(SelectGenerator(mapFunction))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this](TMap&& mapFunction_) noexcept
				{ return SelectGenerator(std::forward<TMap>(mapFunction_)); },
						std::forward<TMap>(mapFunction));
		}

		/// @brief
		/// @tparam TTransform
		/// @param transform
		/// @return
		template<std::invocable<TSource&> TTransform>
        requires std::same_as<std::invoke_result_t<TTransform, TSource&>, void>
        auto Transform(TTransform&& transform) noexcept
		{
			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(TransformGenerator(transform))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this](TTransform&& transform_) noexcept
				{ return TransformGenerator(std::forward<TTransform>(transform_)); },
						std::forward<TTransform>(transform));
		}

		/// @brief Iterates through all elements and applies a selector to each
		/// @tparam TResult Result of selector invoke
		/// @tparam TSelector Any functional object with TSource argument
		/// @param selector Any functional object with TSource argument
		/// @return New collection LinqGenerator<TResult>
		template<std::invocable<TSource> TSelector,
				 typename TResult = std::invoke_result_t<TSelector, TSource>>
		auto Select(TSelector&& selector) noexcept
		{
			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(SelectGenerator(selector))>;

			return LinqGenerator<TResult, NothrowNewCoroutine>(
				[this](TSelector&& selector_) noexcept
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
		auto SelectMany(TSelector&& selector) noexcept
		{
			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(SelectManyGenerator(selector))>;

			return LinqGenerator<TResult, NothrowNewCoroutine>(
				[this](TSelector&& selector_) noexcept
				{ return SelectManyGenerator(std::forward<TSelector>(selector_)); },
						std::forward<TSelector>(selector));
		}

		/// @brief 
		/// @tparam TCollectionValueType
		/// @tparam TCollectionSelector 
		/// @tparam TCollection 
		/// @tparam TResultSelector 
		/// @param collectionSelector 
		/// @param resultSelector 
		/// @return 
		template<std::invocable<TSource> TCollectionSelector,
				 Concepts::InputIterable TCollection = std::invoke_result_t<TCollectionSelector, TSource>,
				 typename TCollectionValueType = typename TCollection::value_type,
				 std::invocable<TSource, TCollectionValueType> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, TSource, TCollectionValueType>>
		auto SelectMany(TCollectionSelector&& collectionSelector, TResultSelector&& resultSelector) noexcept
		{
			using NothrowNewCoroutine = std::integral_constant<bool,
				IsNothrowCoroutine && noexcept(SelectManyGenerator(collectionSelector, resultSelector))>;

			return LinqGenerator<TResult, NothrowNewCoroutine>(
				[this](TCollectionSelector&& collectionSelector_, TResultSelector&& resultSelector_) noexcept
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
		auto Where(TPredicate&& predicate) noexcept
		{
			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(WhereGenerator(predicate))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this](TPredicate&& predicate_) noexcept
				{ return WhereGenerator(std::forward<TPredicate>(predicate_)); },
						std::forward<TPredicate>(predicate));
		}

		/// @brief Remove elements from some set by condition
		/// @tparam TPredicate 
		/// @param predicate 
		/// @return 
		template<Concepts::IsPredicate<TSource> TPredicate>
		auto RemoveWhere(TPredicate&& predicate) noexcept
		{
			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(RemoveWhereGenerator(predicate))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this](TPredicate&& predicate_) noexcept
				{ return RemoveWhereGenerator(std::forward<TPredicate>(predicate_)); },
						std::forward<TPredicate>(predicate));
		}

		/// @brief Sorts the elements of a collection
		/// @param orderType 
		/// @return 
		auto Order(OrderType orderType = OrderType::ASC) noexcept(IsNothrowCoroutine)
		requires Concepts::Comparable<TSource>
		{
			std::vector<TSource> newCollection;
			while (_yieldContext)
				newCollection.push_back(_yieldContext.Next());
			if (newCollection.empty())
				return std::move(*this);

			Sort::QuickSort(newCollection.data(), 0, newCollection.size() - 1, orderType);

			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(Generator(newCollection))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this, newCollection = std::move(newCollection)]() noexcept
				{ return Generator(newCollection); });
		}

		/// @brief Sorts the elements of a collection with selector
		/// @tparam TSelector 
		/// @param selector 
		/// @param orderType 
		/// @return 
		template<std::invocable<TSource> TSelector>
		requires Concepts::Comparable<std::invoke_result_t<TSelector, TSource>>
		auto OrderBy(TSelector&& selector, OrderType orderType = OrderType::ASC)
		noexcept(std::is_nothrow_invocable_v<TSelector, TSource> && IsNothrowCoroutine)
		{
			std::vector<TSource> newCollection;
			while (_yieldContext)
				newCollection.push_back(_yieldContext.Next());
			if (newCollection.empty())
				return std::move(*this);

			Sort::QuickSort(newCollection.data(), 0, newCollection.size() - 1,
							std::forward<TSelector>(selector), orderType);

			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(Generator(newCollection))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this, newCollection = std::move(newCollection)]() noexcept
				{ return Generator(newCollection); });
		}

		/// @brief Reverse the collection
		/// @return 
		auto Reverse() noexcept(IsNothrowCoroutine)
		{
			std::vector<TSource> collection;
			while (_yieldContext)
				collection.push_back(_yieldContext.Next());

			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(ReverseGenerator(collection))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this, collection = std::move(collection)]() noexcept
				{ return ReverseGenerator(collection); });
		}

		/// @brief Get the difference of two sequences
		/// @tparam TOtherCollection 
		/// @param otherCollection 
		/// @return 
		template<typename TOtherCollection>
		requires Concepts::ForwardIterable<TOtherCollection> &&
				 Concepts::HasSize<TOtherCollection> &&
				 Concepts::Equatable<TSource> &&
				 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
		auto Except(const TOtherCollection& otherCollection) noexcept(IsNothrowCoroutine)
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

			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(Generator(newCollection))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this, newCollection = std::move(newCollection)]() noexcept
				{ return Generator(newCollection); });
		}

		/// @brief Get the difference of two sequences
		/// @tparam TOtherCollection 
		/// @param otherCollection 
		/// @return 
		template<typename TOtherCollection>
		requires Concepts::InputIterable<TOtherCollection> &&
				 Concepts::HasSize<TOtherCollection> &&
				 Concepts::Equatable<TSource> &&
				 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
		auto Except(TOtherCollection&& otherCollection) noexcept(IsNothrowCoroutine)
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

			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(Generator(newCollection))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this, newCollection = std::move(newCollection)]() noexcept
				{ return Generator(newCollection); });
		}

		/// @brief Get the intersection of sequences
		/// @tparam TOtherCollection 
		/// @param otherCollection 
		/// @return 
		template<Concepts::ForwardIterable TOtherCollection>
		requires Concepts::Equatable<TSource> &&
				 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
		auto Intersect(const TOtherCollection& otherCollection) noexcept(IsNothrowCoroutine)
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

			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(Generator(newCollection))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this, newCollection = std::move(newCollection)]() noexcept
				{ return Generator(newCollection); });
		}

		/// @brief Get the intersection of sequences
		/// @tparam TOtherCollection 
		/// @param otherCollection 
		/// @return 
		template<Concepts::InputIterable TOtherCollection>
		requires Concepts::Equatable<TSource> &&
				 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
		auto Intersect(TOtherCollection&& otherCollection) noexcept(IsNothrowCoroutine)
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

			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(Generator(newCollection))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this, newCollection = std::move(newCollection)]() noexcept
				{ return Generator(newCollection); });
		}

		/// @brief Remove duplicates in a set
		/// @return 
		auto Distinct() noexcept(IsNothrowCoroutine)
		requires Concepts::Equatable<TSource>
		{
			std::set<TSource> newCollection;
			while (_yieldContext)
				newCollection.insert(_yieldContext.Next());

			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(Generator(newCollection))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this, newCollection = std::move(newCollection)]() noexcept
				{ return Generator(newCollection); });
		}

		/// @brief Join two sequences
		/// @tparam TOtherCollection 
		/// @param otherCollection 
		/// @return 
		template<Concepts::ForwardIterable TOtherCollection>
		requires Concepts::Equatable<TSource> &&
				 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
		auto Union(const TOtherCollection& otherCollection) noexcept(IsNothrowCoroutine)
		{
			std::set<TSource> newCollection;

			while (_yieldContext)
				newCollection.insert(_yieldContext.Next());

			for (const auto& element : otherCollection)
				newCollection.insert(element);

			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(Generator(newCollection))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this, newCollection = std::move(newCollection)]() noexcept
				{ return Generator(newCollection); });
		}

		/// @brief Join two sequences
		/// @tparam TOtherCollection 
		/// @param otherCollection 
		/// @return 
		template<Concepts::InputIterable TOtherCollection>
		requires Concepts::Equatable<TSource> &&
				 std::same_as<typename std::decay_t<TOtherCollection>::value_type, TSource>
		auto Union(TOtherCollection&& otherCollection) noexcept(IsNothrowCoroutine)
		{
			std::set<TSource> newCollection;

			while (_yieldContext)
				newCollection.insert(_yieldContext.Next());

			for (auto&& element : otherCollection)
				newCollection.insert(std::move(element));

			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(Generator(newCollection))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this, newCollection = std::move(newCollection)]() noexcept
				{ return Generator(newCollection); });
		}

		/// @brief Performs a general aggregation of the elements of the collection depending on the specified expression. 
		/// After this method generator became invalid
		/// @tparam TResult 
		/// @tparam TAggregate 
		/// @param aggregateFunction 
		/// @return
		/// @throw std::out_of_range
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
		noexcept(std::is_nothrow_invocable_v<TPredicate, TSource> && IsNothrowCoroutine)
		{
			std::size_t result = 0;
			while (_yieldContext)
				if (predicate(_yieldContext.Next()))
					++result;

			return result;
		}

		/// @brief Get the sum of values. After this method generator became invalid
		/// @return
		/// @throw std::out_of_range
		TSource Sum()
		requires Concepts::Addable<TSource>
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
		/// @throw std::out_of_range
		template<std::invocable<TSource> TSelector,
				 Concepts::Addable TResult = std::invoke_result_t<TSelector, TSource>>
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
		/// @throw std::out_of_range
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
		/// @throw std::out_of_range
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
		/// @throw std::out_of_range
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
		/// @throw std::out_of_range
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
		/// @throw std::out_of_range
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
		/// @throw std::out_of_range
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
		auto Skip(const std::size_t count) noexcept
		{
			using NothrowNewCoroutine =
					std::integral_constant<bool, IsNothrowCoroutine && noexcept(SkipGenerator(count))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this](const std::size_t count_) noexcept
				{ return SkipGenerator(count_); }, count);
		}

		/// @brief Skips a chain of elements, starting with the first element, as long as they satisfy a certain condition
		/// @tparam TPredicate 
		/// @param predicate 
		/// @return 
		template<Concepts::IsPredicate<TSource> TPredicate>
		auto SkipWhile(TPredicate&& predicate) noexcept
		{
			using NothrowNewCoroutine =
					std::integral_constant<bool, IsNothrowCoroutine && noexcept(SkipWhileGenerator(predicate))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this](TPredicate&& predicate_) noexcept
				{ return SkipWhileGenerator(std::forward<TPredicate>(predicate_)); },
						std::forward<TPredicate>(predicate));
		}

		/// @brief Retrieves a certain number of elements
		/// @param count 
		/// @return 
		auto Take(const std::size_t count) noexcept
		{
			using NothrowNewCoroutine =
					std::integral_constant<bool, IsNothrowCoroutine && noexcept(TakeGenerator(count))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this](const std::size_t count_) noexcept
				{ return TakeGenerator(count_); }, count);
		}

		/// @brief Retrieves a certain number of elements from the end of the collection
		/// @tparam TPredicate 
		/// @param predicate 
		/// @return 
		template<Concepts::IsPredicate<TSource> TPredicate>
		auto TakeWhile(TPredicate&& predicate) noexcept
		{
			using NothrowNewCoroutine =
					std::integral_constant<bool, IsNothrowCoroutine && noexcept(TakeWhileGenerator(predicate))>;

			return LinqGenerator<TSource, NothrowNewCoroutine>(
				[this](TPredicate&& predicate_) noexcept
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
		auto GroupBy(TKeySelector&& keySelector) noexcept
		{
			using NothrowNewCoroutine =
					std::integral_constant<bool, IsNothrowCoroutine && noexcept(GroupByGenerator(keySelector))>;

			return LinqGenerator<std::pair<TKey, std::vector<TSource>>, NothrowNewCoroutine>(
				[this](TKeySelector&& keySelector_) noexcept
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
		/// @throw std::invalid_argument
		template<Concepts::ForwardIterable TOtherCollection,
				 std::invocable<TSource> TInnerKeySelector,
				 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
				 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, TSource, typename TOtherCollection::value_type>>
		requires std::same_as<std::invoke_result_t<TInnerKeySelector, TSource>,
							  std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
				 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, TSource>>
		auto Join(const TOtherCollection& otherCollection,
				  TInnerKeySelector&& innerKeySelector,
				  TOtherKeySelector&& otherKeySelector,
				  TResultSelector&& resultSelector)
		{
			if (otherCollection.empty())
				throw std::invalid_argument("other collection must not be empty");

			using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
				noexcept(JoinGenerator(std::declval<TOtherCollection>(), innerKeySelector, otherKeySelector, resultSelector))>;

			return LinqGenerator<TResult, NothrowNewCoroutine>([this](
					TOtherCollection otherCollection_,
					TInnerKeySelector&& innerKeySelector_,
					TOtherKeySelector&& otherKeySelector_,
					TResultSelector&& resultSelector_) noexcept
				{ return JoinGenerator(std::move(otherCollection_),
					std::forward<TInnerKeySelector>(innerKeySelector_),
					std::forward<TOtherKeySelector>(otherKeySelector_),
					std::forward<TResultSelector>(resultSelector_)); },
				otherCollection,
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
		/// @throw std::invalid_argument
		template<Concepts::InputIterable TOtherCollection,
				 std::invocable<TSource> TInnerKeySelector,
				 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
				 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, TSource, typename TOtherCollection::value_type>>
		requires std::same_as<std::invoke_result_t<TInnerKeySelector, TSource>,
							  std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
				 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, TSource>>
		auto Join(TOtherCollection&& otherCollection,
				  TInnerKeySelector&& innerKeySelector,
				  TOtherKeySelector&& otherKeySelector,
				  TResultSelector&& resultSelector)
		{
			if (otherCollection.empty())
				throw std::invalid_argument("other collection must not be empty");

			using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
				noexcept(JoinGenerator(std::declval<TOtherCollection>(), innerKeySelector, otherKeySelector, resultSelector))>;

			return LinqGenerator<TResult, NothrowNewCoroutine>([this](
					TOtherCollection&& otherCollection_,
					TInnerKeySelector&& innerKeySelector_,
					TOtherKeySelector&& otherKeySelector_,
					TResultSelector&& resultSelector_) noexcept
				{ return JoinGenerator(std::forward<TOtherCollection>(otherCollection_),
					std::forward<TInnerKeySelector>(innerKeySelector_),
					std::forward<TOtherKeySelector>(otherKeySelector_),
					std::forward<TResultSelector>(resultSelector_)); },
				std::forward<TOtherCollection>(otherCollection),
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
		/// @throw std::invalid_argument
		template<Concepts::ForwardIterable TOtherCollection,
				 std::invocable<TSource> TInnerKeySelector,
				 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, TSource>,
				 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
				 std::invocable<const std::vector<TSource>&, typename TOtherCollection::value_type> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>>
		requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>>
		auto GroupJoin(const TOtherCollection& otherCollection,
					   TInnerKeySelector&& innerKeySelector,
					   TOtherKeySelector&& otherKeySelector,
					   TResultSelector&& resultSelector)
		{
			if (otherCollection.empty())
				throw std::invalid_argument("other collection must not be empty");

			using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
				noexcept(GroupJoinGenerator(std::declval<TOtherCollection>(), innerKeySelector, otherKeySelector, resultSelector))>;

			return LinqGenerator<TResult, NothrowNewCoroutine>([this](
					TOtherCollection otherCollection_,
					TInnerKeySelector&& innerKeySelector_,
					TOtherKeySelector&& otherKeySelector_,
					TResultSelector&& resultSelector_) noexcept
				{ return GroupJoinGenerator(std::move(otherCollection_),
					std::forward<TInnerKeySelector>(innerKeySelector_),
					std::forward<TOtherKeySelector>(otherKeySelector_),
					std::forward<TResultSelector>(resultSelector_)); },
				otherCollection,
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
		/// @throw std::invalid_argument
		template<Concepts::InputIterable TOtherCollection,
				 std::invocable<TSource> TInnerKeySelector,
				 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, TSource>,
				 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
				 std::invocable<const std::vector<TSource>&, typename TOtherCollection::value_type> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>>
		requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>>
		auto GroupJoin(TOtherCollection&& otherCollection,
					   TInnerKeySelector&& innerKeySelector,
					   TOtherKeySelector&& otherKeySelector,
					   TResultSelector&& resultSelector)
		{
			if (otherCollection.empty())
				throw std::invalid_argument("other collection must not be empty");

			using NothrowNewCoroutine = std::integral_constant<bool, IsNothrowCoroutine &&
				noexcept(GroupJoinGenerator(std::declval<TOtherCollection>(), innerKeySelector, otherKeySelector, resultSelector))>;

			return LinqGenerator<TResult, NothrowNewCoroutine>([this](
					TOtherCollection&& otherCollection_,
					TInnerKeySelector&& innerKeySelector_,
					TOtherKeySelector&& otherKeySelector_,
					TResultSelector&& resultSelector_) noexcept
				{ return GroupJoinGenerator(std::forward<TOtherCollection>(otherCollection_),
					std::forward<TInnerKeySelector>(innerKeySelector_),
					std::forward<TOtherKeySelector>(otherKeySelector_),
					std::forward<TResultSelector>(resultSelector_)); },
				std::forward<TOtherCollection>(otherCollection),
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
		requires Concepts::ForwardIterable<TOtherCollection> && Concepts::HasSize<TOtherCollection>
		auto Zip(const TOtherCollection& otherCollection) noexcept
		{
			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(ZipGenerator(std::declval<TOtherCollection>()))>;

			return LinqGenerator<std::pair<TSource, TOtherCollectionValueType>, NothrowNewCoroutine>([this]
				(TOtherCollection otherCollection_) noexcept
				{ return ZipGenerator(std::move(otherCollection_)); },
					otherCollection);
		}

		/// @brief Sequentially concatenates the corresponding elements of the current sequence with the second sequence
		/// @tparam TOtherCollection 
		/// @tparam TOtherCollectionValueType 
		/// @param otherCollection 
		/// @return 
		template<typename TOtherCollection,
				 typename TOtherCollectionValueType = typename TOtherCollection::value_type>
		requires Concepts::InputIterable<TOtherCollection> && Concepts::HasSize<TOtherCollection>
		auto Zip(TOtherCollection&& otherCollection) noexcept
		{
			using NothrowNewCoroutine =
				std::integral_constant<bool, IsNothrowCoroutine && noexcept(ZipGenerator(std::declval<TOtherCollection>()))>;

			return LinqGenerator<std::pair<TSource, TOtherCollectionValueType>, NothrowNewCoroutine>([this]
				(TOtherCollection&& otherCollection_) noexcept
				{ return ZipGenerator(std::forward<TOtherCollection>(otherCollection_)); },
					std::forward<TOtherCollection>(otherCollection));
		}

		/// @brief Checks if all elements match a condition. If all elements match the condition, then true is returned.
		/// After this method generator became invalid
		/// @tparam TPredicate 
		/// @param predicate 
		/// @return 
		template<Concepts::IsPredicate<TSource> TPredicate>
		bool All(TPredicate&& predicate)
		noexcept(std::is_nothrow_invocable_v<TPredicate, TSource> && IsNothrowCoroutine)
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
		bool Any(TPredicate&& predicate)
		noexcept(std::is_nothrow_invocable_v<TPredicate, TSource> && IsNothrowCoroutine)
		{
			while (_yieldContext)
				if (predicate(_yieldContext.Next()))
					return true;
			return false;
		}

	private:
		template<Concepts::InputIterable TCollection,
				 bool NothrowIterable = IsNothrowCoroutine && noexcept(++std::declval<TCollection>().begin())>
		Future<TSource, std::integral_constant<bool, NothrowIterable>>
		Generator(TCollection&& collection) noexcept(NothrowIterable)
		{
			auto inner = std::forward<TCollection>(collection);
			for (auto&& element : inner)
				co_yield std::move(element);
		}

		template<Concepts::InputIterable TCollection,
				 bool NothrowIterable = IsNothrowCoroutine && noexcept(++std::declval<TCollection>().rbegin())>
		Future<TSource, std::integral_constant<bool, NothrowIterable>>
		ReverseGenerator(TCollection&& collection) noexcept(NothrowIterable)
		{
			auto inner = std::forward<TCollection>(collection);
			for (auto it = inner.rbegin(); it != inner.rend(); ++it)
				co_yield std::move(*it);
		}

		template<std::invocable<TSource&> TTransform,
				 bool NothrowNewCoroutine = IsNothrowCoroutine && std::is_nothrow_invocable_v<TTransform, TSource&>>
        requires std::same_as<std::invoke_result_t<TTransform, TSource&>, void>
        Future<TSource, std::integral_constant<bool, NothrowNewCoroutine>>
		TransformGenerator(TTransform&& transform) noexcept(NothrowNewCoroutine)
		{
			while (_yieldContext)
			{
				auto value = _yieldContext.Next();
				transform(value);
				co_yield std::move(value);
			}
		}

		template<std::invocable<TSource> TSelector,
				 typename TResult = std::invoke_result_t<TSelector, TSource>,
				 bool NothrowNewCoroutine = IsNothrowCoroutine && std::is_nothrow_invocable_v<TSelector, TSource&>>
		Future<TResult, std::integral_constant<bool, NothrowNewCoroutine>>
		SelectGenerator(TSelector&& selector) noexcept(NothrowNewCoroutine)
		{
			while (_yieldContext)
				co_yield selector(_yieldContext.Next());
		}

		template<std::invocable<TSource> TSelector,
			 typename TResult = typename std::invoke_result_t<TSelector, TSource>::value_type,
			 bool NothrowNewCoroutine = IsNothrowCoroutine && std::is_nothrow_invocable_v<TSelector, TSource&>>
		Future<TResult, std::integral_constant<bool, NothrowNewCoroutine>>
		SelectManyGenerator(TSelector&& selector) noexcept(NothrowNewCoroutine)
		{
			while (_yieldContext)
			{
				std::vector<TResult> tempCollection = selector(_yieldContext.Next());
				for (auto&& tempElement : tempCollection)
					co_yield std::move(tempElement);
			}
		}

		template<std::invocable<TSource> TCollectionSelector,
				 Concepts::InputIterable TCollection = std::invoke_result_t<TCollectionSelector, TSource>,
				 typename TCollectionValueType = typename TCollection::value_type,
				 std::invocable<TSource, TCollectionValueType> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, TSource, TCollectionValueType>,
				 bool NothrowNewCoroutine = std::is_nothrow_invocable_v<TCollectionSelector, TSource> &&
						std::is_nothrow_invocable_v<TResultSelector, TSource, TCollectionValueType> &&
						IsNothrowCoroutine>
		Future<TResult, std::integral_constant<bool, NothrowNewCoroutine>>
		SelectManyGenerator(TCollectionSelector&& collectionSelector, TResultSelector&& resultSelector) noexcept(NothrowNewCoroutine)
		{
			while (_yieldContext)
			{
				auto element = _yieldContext.Next();
				TCollection tempCollection = collectionSelector(element);
				for (auto&& tempElement : tempCollection)
					co_yield resultSelector(element, std::move(tempElement));
			}
		}

		template<Concepts::IsPredicate<TSource> TPredicate,
				 bool NothrowNewCoroutine = IsNothrowCoroutine && std::is_nothrow_invocable_v<TPredicate, TSource>>
		Future<TSource, std::integral_constant<bool, NothrowNewCoroutine>>
		WhereGenerator(TPredicate&& predicate) noexcept(NothrowNewCoroutine)
		{
			while (_yieldContext)
			{
				auto element = _yieldContext.Next();
				if (predicate(element))
					co_yield element;
			}
		}

		template<Concepts::IsPredicate<TSource> TPredicate,
				 bool NothrowNewCoroutine = IsNothrowCoroutine && std::is_nothrow_invocable_v<TPredicate, TSource>>
		Future<TSource, std::integral_constant<bool, NothrowNewCoroutine>>
		RemoveWhereGenerator(TPredicate&& predicate) noexcept(NothrowNewCoroutine)
		{
			while (_yieldContext)
			{
				auto element = _yieldContext.Next();
				if (predicate(element))
					continue;
				co_yield element;
			}
		}

		Future<TSource, std::integral_constant<bool, IsNothrowCoroutine>>
		SkipGenerator(const std::size_t count) noexcept(IsNothrowCoroutine)
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

		template<Concepts::IsPredicate<TSource> TPredicate,
				 bool IsNothrowNewCoroutine = IsNothrowCoroutine && std::is_nothrow_invocable_v<TPredicate, TSource>>
		Future<TSource, std::integral_constant<bool, IsNothrowNewCoroutine>>
		SkipWhileGenerator(TPredicate&& predicate) noexcept(IsNothrowNewCoroutine)
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

		Future<TSource, std::integral_constant<bool, IsNothrowCoroutine>>
		TakeGenerator(const std::size_t count) noexcept(IsNothrowCoroutine)
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

		template<Concepts::IsPredicate<TSource> TPredicate,
				 bool IsNothrowNewCoroutine = IsNothrowCoroutine && std::is_nothrow_invocable_v<TPredicate, TSource>>
		Future<TSource, std::integral_constant<bool, IsNothrowNewCoroutine>>
		TakeWhileGenerator(TPredicate&& predicate) noexcept(IsNothrowNewCoroutine)
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
				 typename TKey = std::invoke_result_t<TKeySelector, TSource>,
				 bool IsNothrowNewCoroutine = IsNothrowCoroutine && std::is_nothrow_invocable_v<TKeySelector, TSource>>
		Future<std::pair<TKey, std::vector<TSource>>, std::integral_constant<bool, IsNothrowNewCoroutine>>
		GroupByGenerator(TKeySelector&& keySelector) noexcept(IsNothrowNewCoroutine)
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

		template<Concepts::InputIterable TOtherCollection,
				 std::invocable<TSource> TInnerKeySelector,
				 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
				 std::invocable<TSource, typename TOtherCollection::value_type> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, TSource, typename TOtherCollection::value_type>,
				 bool IsNothrowNewCoroutine =
				 		std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
						std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
						std::is_nothrow_invocable_v<TResultSelector, TSource, typename TOtherCollection::value_type> &&
						IsNothrowCoroutine && noexcept(++std::declval<TOtherCollection>().begin())>
		requires std::same_as<std::invoke_result_t<TInnerKeySelector, TSource>,
							  std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>> &&
				 Concepts::Equatable<std::invoke_result_t<TInnerKeySelector, TSource>>
		Future<TResult, std::integral_constant<bool, IsNothrowNewCoroutine>>
		JoinGenerator(TOtherCollection&& otherCollection,
					  TInnerKeySelector&& innerKeySelector,
					  TOtherKeySelector&& otherKeySelector,
					  TResultSelector&& resultSelector) noexcept(IsNothrowNewCoroutine)
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

		template<Concepts::InputIterable TOtherCollection,
				 std::invocable<TSource> TInnerKeySelector,
				 Concepts::Equatable TKey = std::invoke_result_t<TInnerKeySelector, TSource>,
				 std::invocable<typename TOtherCollection::value_type> TOtherKeySelector,
				 std::invocable<const std::vector<TSource>&, typename TOtherCollection::value_type> TResultSelector,
				 typename TResult = std::invoke_result_t<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type>,
				 bool IsNothrowNewCoroutine =
				 		std::is_nothrow_invocable_v<TInnerKeySelector, TSource> &&
				 		std::is_nothrow_invocable_v<TOtherKeySelector, typename TOtherCollection::value_type> &&
				 		std::is_nothrow_invocable_v<TResultSelector, const std::vector<TSource>&, typename TOtherCollection::value_type> &&
				 		IsNothrowCoroutine && noexcept(++std::declval<TOtherCollection>().begin())>
		requires std::same_as<TKey, std::invoke_result_t<TOtherKeySelector, typename TOtherCollection::value_type>>
		Future<TResult, std::integral_constant<bool, IsNothrowNewCoroutine>>
		GroupJoinGenerator(TOtherCollection&& otherCollection,
						   TInnerKeySelector&& innerKeySelector,
						   TOtherKeySelector&& otherKeySelector,
						   TResultSelector&& resultSelector) noexcept(IsNothrowNewCoroutine)
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

		template<Concepts::ForwardIterable TOtherCollection,
				 typename TOtherCollectionValueType = typename TOtherCollection::value_type,
				 bool IsNothrowNewCoroutine = IsNothrowCoroutine && noexcept(++std::declval<TOtherCollection>().begin())>
		requires Concepts::InputIterable<TOtherCollection> && Concepts::HasSize<TOtherCollection>
		Future<std::pair<TSource, TOtherCollectionValueType>, std::integral_constant<bool, IsNothrowNewCoroutine>>
		ZipGenerator(TOtherCollection&& otherCollection) noexcept(IsNothrowNewCoroutine)
		{
			auto inner = std::forward<TOtherCollection>(otherCollection);
			for (auto&& element : inner)
				if (_yieldContext)
					co_yield std::make_pair(_yieldContext.Next(), std::move(element));
		}
	};
}

#endif