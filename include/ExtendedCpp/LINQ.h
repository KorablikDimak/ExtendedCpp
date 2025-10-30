#ifndef LINQ_LINQ_H
#define LINQ_LINQ_H

#include <ExtendedCpp/LINQ/LinqContainer.h>
#include <ExtendedCpp/LINQ/LinqGenerator.h>
#include <ExtendedCpp/LINQ/LinqView.h>

/// @brief 
namespace ExtendedCpp::LINQ
{
    /// @brief 
    /// @tparam TSource 
    /// @tparam TCollection 
    /// @param collection 
    /// @return 
    template<Concepts::InputIterable TCollection, typename TSource = typename std::decay_t<TCollection>::value_type>
    LinqContainer<TSource> From(TCollection&& collection) noexcept
    {
        return LinqContainer<TSource>
            (std::vector<TSource>(collection.begin(), collection.end()));
    }

    /// @brief 
    /// @tparam TSource 
    /// @tparam TIterator 
    /// @param begin 
    /// @param end 
    /// @return 
    template<std::input_iterator TIterator, typename TSource = typename std::iterator_traits<TIterator>::value_type>
    LinqContainer<TSource> From(TIterator&& begin, TIterator&& end) noexcept
    {
        return LinqContainer<TSource>(std::vector<TSource>
            (std::forward<TIterator>(begin), std::forward<TIterator>(end)));
    }

    /// @brief
    /// @tparam TSource
    /// @param collection
    /// @return
    template<typename TSource>
    LinqContainer<TSource> From(std::vector<TSource>&& collection) noexcept
    {
        return LinqContainer<TSource>(std::move(collection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqContainer<TSource> From(const std::stack<TSource>& collection) noexcept
    {
        std::stack<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.top());
            copy.pop();
        }

        return LinqContainer<TSource>(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqContainer<TSource> From(std::stack<TSource>&& collection) noexcept
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(collection.top());
            collection.pop();
        }

        return LinqContainer<TSource>(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqContainer<TSource> From(const std::queue<TSource>& collection) noexcept
    {
        std::queue<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.front());
            copy.pop();
        }

        return LinqContainer<TSource>(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqContainer<TSource> From(std::queue<TSource>&& collection) noexcept
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(collection.front());
            collection.pop();
        }

        return LinqContainer<TSource>(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqContainer<TSource> From(const std::priority_queue<TSource>& collection) noexcept
    {
        std::priority_queue<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.top());
            copy.pop();
        }

        return LinqContainer<TSource>(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqContainer<TSource> From(std::priority_queue<TSource>&& collection) noexcept
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(collection.top());
            collection.pop();
        }

        return LinqContainer<TSource>(std::move(vectorCollection));
    }

    /// @brief
    /// @tparam TKey
    /// @tparam TValue
    /// @param collection
    /// @return
    template<typename TKey, typename TValue>
    LinqContainer<std::pair<TKey, TValue>> From(const std::map<TKey, TValue>& collection) noexcept
    {
        return LinqContainer<std::pair<TKey, TValue>>
            (std::vector<std::pair<TKey, TValue>>(collection.cbegin(), collection.cend()));
    }

    /// @brief
    /// @tparam TKey
    /// @tparam TValue
    /// @param collection
    /// @return
    template<typename TKey, typename TValue>
    LinqContainer<std::pair<TKey, TValue>> From(std::map<TKey, TValue>&& collection) noexcept
    {
        return LinqContainer<std::pair<TKey, TValue>>
            (std::vector<std::pair<TKey, TValue>>(collection.begin(), collection.end()));
    }

    /// @brief
    /// @tparam TKey
    /// @tparam TValue
    /// @param collection
    /// @return
    template<typename TKey, typename TValue>
    LinqContainer<std::pair<TKey, TValue>> From(const std::unordered_map<TKey, TValue>& collection) noexcept
    {
        return LinqContainer<std::pair<TKey, TValue>>
            (std::vector<std::pair<TKey, TValue>>(collection.cbegin(), collection.cend()));
    }

    /// @brief
    /// @tparam TKey
    /// @tparam TValue
    /// @param collection
    /// @return
    template<typename TKey, typename TValue>
    LinqContainer<std::pair<TKey, TValue>> From(std::unordered_map<TKey, TValue>&& collection) noexcept
    {
        return LinqContainer<std::pair<TKey, TValue>>
            (std::vector<std::pair<TKey, TValue>>(collection.begin(), collection.end()));
    }

    /// @brief 
    /// @tparam TSource 
    /// @tparam TCollection 
    /// @param collection 
    /// @return 
    template<Concepts::InputIterable TCollection, typename TSource = typename std::decay_t<TCollection>::value_type>
    requires (!Concepts::IsPair<TSource>)
    LinqGenerator<TSource, std::integral_constant<bool, Concepts::IsNothrowIterable<TCollection>>>
    Generator(TCollection&& collection) noexcept
    {
        return LinqGenerator<TSource, std::integral_constant<bool, Concepts::IsNothrowIterable<TCollection>>>
            (std::forward<TCollection>(collection));
    }

    template<Concepts::InputIterable TCollection,
             Concepts::IsPair TSource = typename std::decay_t<TCollection>::value_type,
             typename TKey = std::remove_const_t<typename PairTraits<TSource>::FirstType>,
             typename TValue = typename PairTraits<TSource>::SecondType>
    LinqGenerator<std::pair<TKey, TValue>, std::integral_constant<bool, Concepts::IsNothrowIterable<TCollection>>>
    Generator(TCollection&& collection) noexcept
    {
        return LinqGenerator<std::pair<TKey, TValue>, std::integral_constant<bool, Concepts::IsNothrowIterable<TCollection>>>
            (std::forward<TCollection>(collection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @tparam TIterator 
    /// @param begin 
    /// @param end 
    /// @return 
    template<std::input_iterator TIterator,
             typename TSource = typename std::iterator_traits<TIterator>::value_type,
             bool NothrowIterable = ++std::declval<TIterator>()>
    LinqGenerator<TSource, std::integral_constant<bool, NothrowIterable>>
    Generator(TIterator&& begin, TIterator&& end) noexcept
    {
        return LinqGenerator<TSource, std::integral_constant<bool, NothrowIterable>>
            (std::forward<TIterator>(begin), std::forward<TIterator>(end));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqGenerator<TSource, std::true_type>
    Generator(const std::stack<TSource>& collection) noexcept
    {
        std::stack<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.top());
            copy.pop();
        }

        return LinqGenerator<TSource, std::true_type>(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqGenerator<TSource, std::true_type>
    Generator(std::stack<TSource>&& collection) noexcept
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(collection.top());
            collection.pop();
        }

        return LinqGenerator<TSource, std::true_type>(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqGenerator<TSource, std::true_type>
    Generator(const std::queue<TSource>& collection) noexcept
    {
        std::queue<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.front());
            copy.pop();
        }

        return LinqGenerator<TSource, std::true_type>(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqGenerator<TSource, std::true_type>
    Generator(std::queue<TSource>&& collection) noexcept
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = collection.front();
            collection.pop();
        }

        return LinqGenerator<TSource, std::true_type>(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqGenerator<TSource, std::true_type>
    Generator(const std::priority_queue<TSource>& collection) noexcept
    {
        std::priority_queue<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.top());
            copy.pop();
        }

        return LinqGenerator<TSource, std::true_type>(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqGenerator<TSource, std::true_type>
    Generator(std::priority_queue<TSource>&& collection) noexcept
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(collection.top());
            collection.pop();
        }

        return LinqGenerator<TSource, std::true_type>(std::move(vectorCollection));
    }

    /// @brief
    /// @tparam TIterator
    /// @tparam TCollection
    /// @param collection
    /// @return
    template<Concepts::ForwardIterable TCollection, std::forward_iterator TIterator = typename std::decay_t<TCollection>::const_iterator>
    LinqView<IteratorWrapper<TIterator>> View(const TCollection& collection) noexcept
    {
        return LinqView<IteratorWrapper<TIterator>>(
            IteratorWrapper<TIterator>(collection.begin(), collection.end()),
            IteratorWrapper<TIterator>(collection.end(), collection.end()));
    }

    /// @brief 
    /// @tparam TIterator 
    /// @tparam TCollection 
    /// @param collection 
    /// @return 
    template<Concepts::ForwardIterable TCollection, std::forward_iterator TIterator = typename std::decay_t<TCollection>::iterator>
    LinqView<IteratorWrapper<TIterator>> View(TCollection& collection) noexcept
    {
        return LinqView<IteratorWrapper<TIterator>>(
            IteratorWrapper<TIterator>(collection.begin(), collection.end()),
            IteratorWrapper<TIterator>(collection.end(), collection.end()));
    }

    /// @brief 
    /// @tparam TIterator 
    /// @param begin 
    /// @param end 
    /// @return 
    template<std::forward_iterator TIterator>
    LinqView<IteratorWrapper<TIterator>> View(TIterator&& begin, TIterator&& end) noexcept
    {
        return LinqView<IteratorWrapper<TIterator>>(
            IteratorWrapper<TIterator>(begin, end),
            IteratorWrapper<TIterator>(end, end));
    }

    template<template<typename...> typename>
    struct To;

    /// @brief
    template<>
    struct To<LinqContainer> final
    {
        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection&& linqCollection) const noexcept
        {
            return From(std::forward<TLinqCollection>(linqCollection));
        }
    };

    /// @brief
    template<>
    struct To<LinqGenerator> final
    {
        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection&& linqCollection) const noexcept
        {
            return Generator(std::forward<TLinqCollection>(linqCollection));
        }
    };

    /// @brief
    template<>
    struct To<LinqView> final
    {
        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection& linqCollection) const noexcept
        {
            return View(std::forward<TLinqCollection>(linqCollection));
        }
    };

    /// @brief
    template<>
    struct To<std::vector> final
    {
        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::vector<typename std::decay_t<TLinqCollection>::value_type>
        operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.ToVector()))
        {
            return linqCollection.ToVector();
        }
    };

    /// @brief
    template<>
    struct To<std::list> final
    {
        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::list<typename std::decay_t<TLinqCollection>::value_type>
        operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.ToList()))
        {
            return linqCollection.ToList();
        }
    };

    /// @brief
    template<>
    struct To<std::forward_list> final
    {
        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::forward_list<typename std::decay_t<TLinqCollection>::value_type>
        operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.ToForwardList()))
        {
            return linqCollection.ToForwardList();
        }
    };

    /// @brief
    template<>
    struct To<std::stack> final
    {
        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::stack<typename std::decay_t<TLinqCollection>::value_type>
        operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.ToStack()))
        {
            return linqCollection.ToStack();
        }
    };

    /// @brief
    template<>
    struct To<std::queue> final
    {
        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::queue<typename std::decay_t<TLinqCollection>::value_type>
        operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.ToQueue()))
        {
            return linqCollection.ToQueue();
        }
    };

    /// @brief
    template<>
    struct To<std::deque> final
    {
        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::deque<typename std::decay_t<TLinqCollection>::value_type>
        operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.ToDeque()))
        {
            return linqCollection.ToDeque();
        }
    };

    /// @brief
    template<>
    struct To<std::priority_queue> final
    {
        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::priority_queue<typename std::decay_t<TLinqCollection>::value_type>
        operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.ToPriorityQueue()))
        {
            return linqCollection.ToPriorityQueue();
        }
    };

    /// @brief
    template<>
    struct To<std::set> final
    {
        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::set<typename std::decay_t<TLinqCollection>::value_type>
        operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.ToSet()))
        {
            return linqCollection.ToSet();
        }
    };

    /// @brief
    template<>
    struct To<std::unordered_set> final
    {
        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::unordered_set<typename std::decay_t<TLinqCollection>::value_type>
        operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.ToUnorderedSet()))
        {
            return linqCollection.ToUnorderedSet();
        }
    };

    /// @brief
    template<>
    struct To<std::map> final
    {
        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection,
                 Concepts::IsPair TSource = typename std::decay_t<TLinqCollection>::value_type>
        std::map<typename PairTraits<TSource>::FirstType, typename PairTraits<TSource>::FirstType>
        operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.ToMap()))
        {
            return linqCollection.ToMap();
        }
    };

    /// @brief
    template<>
    struct To<std::unordered_map> final
    {
        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection,
                 Concepts::IsPair TSource = typename std::decay_t<TLinqCollection>::value_type>
        std::unordered_map<typename PairTraits<TSource>::FirstType, typename PairTraits<TSource>::FirstType>
        operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.ToUnorderedMap()))
        {
            return linqCollection.ToUnorderedMap();
        }
    };

    /// @brief
    /// @tparam N
    template<std::size_t N>
    struct ToArray final
    {
        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::array<typename std::decay_t<TLinqCollection>::value_type, N>
        operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.template ToArray<N>()))
        {
            return linqCollection.template ToArray<N>();
        }
    };

    /// @brief
    /// @tparam TSelector
    template<typename TSelector>
    struct Select final
    {
        /// @brief
        const std::decay_t<TSelector> _selector;

        /// @brief
        /// @param selector
        explicit Select(TSelector&& selector) noexcept :
            _selector(std::forward<TSelector>(selector)) {}

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.Select(_selector)))
        {
            return linqCollection.Select(_selector);
        }
    };

    /// @brief
    /// @tparam TPredicate
    template<typename TPredicate>
    struct Where final
    {
        /// @brief
        const std::decay_t<TPredicate> _predicate;

        /// @brief
        /// @param predicate
        explicit Where(TPredicate&& predicate) noexcept :
            _predicate(std::forward<TPredicate>(predicate)) {}

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.Where(_predicate)))
        {
            return linqCollection.Where(_predicate);
        }
    };

    /// @brief
    /// @tparam TPredicate
    template<typename TPredicate>
    struct RemoveWhere final
    {
        /// @brief
        const std::decay_t<TPredicate> _predicate;

        /// @brief
        /// @param predicate
        explicit RemoveWhere(TPredicate&& predicate) noexcept :
            _predicate(std::forward<TPredicate>(predicate)) {}

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.RemoveWhere(_predicate)))
        {
            return linqCollection.RemoveWhere(_predicate);
        }
    };

    /// @brief
    /// @tparam TMapFunction
    template<typename TMapFunction>
    struct Map final
    {
        /// @brief
        const std::decay_t<TMapFunction> _mapFunction;

        /// @brief
        /// @param mapFunction
        explicit Map(TMapFunction&& mapFunction) noexcept :
            _mapFunction(std::forward<TMapFunction>(mapFunction)) {}

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.Map(_mapFunction)))
        {
            return linqCollection.Map(_mapFunction);
        }
    };

    /// @brief
    /// @tparam TTransformFunction
    template<typename TTransformFunction>
    struct Transform final
    {
        /// @brief
        const std::decay_t<TTransformFunction> _transformFunction;

        /// @brief
        /// @param transformFunction
        explicit Transform(TTransformFunction&& transformFunction) noexcept :
            _transformFunction(std::forward<TTransformFunction>(transformFunction)) {}

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.Transform(_transformFunction)))
        {
            return linqCollection.Transform(_transformFunction);
        }
    };

    /// @brief
    /// @tparam TKeySelector
    template<typename TKeySelector>
    struct GroupBy final
    {
        /// @brief
        const std::decay_t<TKeySelector> _keySelector;

        /// @brief
        /// @param keySelector
        explicit GroupBy(TKeySelector&& keySelector) noexcept :
            _keySelector(std::forward<TKeySelector>(keySelector)) {}

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.GroupBy(_keySelector)))
        {
            return linqCollection.GroupBy(_keySelector);
        }
    };

    /// @brief
    /// @tparam TOtherCollection
    /// @tparam TInnerKeySelector
    /// @tparam TOtherKeySelector
    /// @tparam TResultSelector
    template<Concepts::ForwardIterable TOtherCollection,
             typename TInnerKeySelector, typename TOtherKeySelector, typename TResultSelector>
    struct Join final
    {
        /// @brief
        const TOtherCollection& _otherCollection;

        /// @brief
        const std::decay_t<TInnerKeySelector> _innerKeySelector;

        /// @brief
        const std::decay_t<TOtherKeySelector> _otherKeySelector;

        /// @brief
        const std::decay_t<TResultSelector> _resultSelector;

        /// @brief
        /// @param otherCollection
        /// @param innerKeySelector
        /// @param otherKeySelector
        /// @param resultSelector
        Join(const TOtherCollection& otherCollection, TInnerKeySelector&& innerKeySelector,
             TOtherKeySelector&& otherKeySelector, TResultSelector&& resultSelector) noexcept :
            _otherCollection(otherCollection),
            _innerKeySelector(std::forward<TInnerKeySelector>(innerKeySelector)),
            _otherKeySelector(std::forward<TOtherKeySelector>(otherKeySelector)),
            _resultSelector(std::forward<TResultSelector>(resultSelector)) {}

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.Join(_otherCollection, _innerKeySelector, _otherKeySelector, _resultSelector)))
        {
            return linqCollection.Join(_otherCollection, _innerKeySelector, _otherKeySelector, _resultSelector);
        }
    };

    /// @brief
    /// @tparam TOtherCollection
    template<Concepts::ForwardIterable TOtherCollection>
    struct Zip final
    {
        /// @brief
        const TOtherCollection& _otherCollection;

        /// @brief
        /// @param otherCollection
        explicit Zip(const TOtherCollection& otherCollection) noexcept :
            _otherCollection(otherCollection) {}

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.Zip(_otherCollection)))
        {
            return linqCollection.Zip(_otherCollection);
        }
    };

    /// @brief
    struct Skip final
    {
        /// @brief
        const std::size_t _count;

        /// @brief
        /// @param count
        explicit Skip(const std::size_t count) noexcept : _count(count) {}

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.Skip(std::declval<std::size_t>())))
        {
            return linqCollection.Skip(_count);
        }
    };

    /// @brief
    /// @tparam TPredicate
    template<typename TPredicate>
    struct SkipWhile final
    {
        /// @brief
        const std::decay_t<TPredicate> _predicate;

        /// @brief
        /// @param predicate
        explicit SkipWhile(TPredicate&& predicate) noexcept :
            _predicate(std::forward<TPredicate>(predicate)) {}

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.SkipWhile(_predicate)))
        {
            return linqCollection.SkipWhile(_predicate);
        }
    };

    /// @brief
    struct Take final
    {
        /// @brief
        const std::size_t _count;

        /// @brief
        /// @param count
        explicit Take(const std::size_t count) noexcept : _count(count) {}

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.Take(std::declval<std::size_t>())))
        {
            return linqCollection.Take(_count);
        }
    };

    /// @brief
    /// @tparam TPredicate
    template<typename TPredicate>
    struct TakeWhile final
    {
        /// @brief
        const std::decay_t<TPredicate> _predicate;

        /// @brief
        /// @param predicate
        explicit TakeWhile(TPredicate&& predicate) noexcept :
            _predicate(std::forward<TPredicate>(predicate)) {}

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(linqCollection.TakeWhile(_predicate)))
        {
            return linqCollection.TakeWhile(_predicate);
        }
    };

    /// @brief
    /// @tparam TLinqCollection
    /// @tparam TAdaptor
    /// @param linqCollection
    /// @param adaptor
    /// @return
    template<typename TLinqCollection, typename TAdaptor>
    auto operator|(TLinqCollection&& linqCollection, TAdaptor&& adaptor)
    noexcept(std::is_nothrow_invocable_v<TAdaptor, TLinqCollection>)
    {
        return adaptor(std::forward<TLinqCollection>(linqCollection));
    }

    /// @brief
    /// @tparam TRange
    /// @tparam TAdaptor
    /// @tparam TRangeIterator
    /// @tparam TLinqCollection
    /// @param range
    /// @param adaptor
    /// @return
    template<Concepts::Range TRange, typename TAdaptor, std::forward_iterator TRangeIterator = decltype(std::declval<TRange>().cbegin()),
             Concepts::LinqCollection TLinqCollection = decltype(View(std::declval<TRangeIterator>(), std::declval<TRangeIterator>()))>
    requires (!Concepts::LinqCollection<TRange> && Concepts::LinqAdaptor<TAdaptor, TLinqCollection>)
    auto operator|(const TRange& range, TAdaptor&& adaptor)
    noexcept(std::is_nothrow_invocable_v<TAdaptor, TLinqCollection>)
    {
        return adaptor(View(range.cbegin(), range.cend()));
    }

    /// @brief
    /// @tparam TRange
    /// @tparam TAdaptor
    /// @tparam TRangeIterator
    /// @tparam TLinqCollection
    /// @param range
    /// @param adaptor
    /// @return
    template<Concepts::Range TRange, typename TAdaptor, std::forward_iterator TRangeIterator = decltype(std::declval<TRange>().begin()),
             Concepts::LinqCollection TLinqCollection = decltype(View(std::declval<TRangeIterator>(), std::declval<TRangeIterator>()))>
    requires (!Concepts::LinqCollection<TRange> && Concepts::LinqAdaptor<TAdaptor, TLinqCollection>)
    auto operator|(TRange& range, TAdaptor&& adaptor)
    noexcept(std::is_nothrow_invocable_v<TAdaptor, TLinqCollection>)
    {
        return adaptor(View(range.begin(), range.end()));
    }

    /// @brief
    /// @tparam TRange
    /// @tparam TAdaptor
    /// @tparam TRangeIterator
    /// @tparam TLinqCollection
    /// @param range
    /// @param adaptor
    /// @return
    template<Concepts::Range TRange, typename TAdaptor, std::input_iterator TRangeIterator = decltype(std::declval<TRange>().begin()),
             Concepts::LinqCollection TLinqCollection = decltype(From(std::declval<TRangeIterator>(), std::declval<TRangeIterator>()))>
    requires (!Concepts::LinqCollection<TRange> && Concepts::LinqAdaptor<TAdaptor, TLinqCollection>)
    auto operator|(TRange&& range, TAdaptor&& adaptor)
    noexcept(std::is_nothrow_invocable_v<TAdaptor, TLinqCollection>)
    {
        return adaptor(From(range.begin(), range.end()));
    }
}

#endif