#ifndef LINQ_LINQ_H
#define LINQ_LINQ_H

#include <ExtendedCpp/LINQ/LinqContainer.h>
#include <ExtendedCpp/LINQ/LinqGenerator.h>
#include <ExtendedCpp/LINQ/LinqView.h>
#include <ExtendedCpp/LINQ/YieldForeach.h>

/// @brief
// ReSharper disable once CppInconsistentNaming
namespace ExtendedCpp::LINQ
{
    /// @brief
    /// @tparam TCollection
    /// @tparam TSource
    /// @param collection 
    /// @return
    template<Concepts::InputIterable TCollection, typename TSource = typename TCollection::value_type>
    LinqContainer<TSource> From(const TCollection& collection) noexcept
    {
        return LinqContainer<TSource>
            (std::vector<TSource>(collection.begin(), collection.end()));
    }

    /// @brief
    /// @tparam TIterator
    /// @tparam TSource
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
    /// @tparam TCollection
    template<Concepts::InputIterable TCollection>
    requires std::copy_constructible<TCollection>
    class CollectionFactory final
    {
    public:
        /// @brief
        using TSource = typename std::decay_t<TCollection>::value_type;

        /// @brief
        /// @param collection
        explicit CollectionFactory(TCollection collection) noexcept :
            _collection(std::move(collection)) {}

        /// @brief
        /// @return
        auto operator()() const
        {
            return YieldForeach(_collection);
        }

    private:
        TCollection _collection;
    };

    /// @brief
    /// @tparam TIterator
    template<std::input_iterator TIterator>
    class IteratorFactory final
    {
    public:
        /// @brief
        using TSource = typename std::iterator_traits<TIterator>::value_type;

        /// @brief
        /// @param begin
        /// @param end
        explicit IteratorFactory(TIterator begin, TIterator end) noexcept :
            _begin(std::move(begin)),
            _end(std::move(end)) {}

        /// @brief
        /// @return
        auto operator()() const
        {
            return YieldForeach(_begin, _end);
        }

    private:
        TIterator _begin;
        TIterator _end;
    };

    /// @brief
    /// @tparam TCollection
    /// @tparam TSource
    /// @param collection 
    /// @return
    template<Concepts::InputIterable TCollection, typename TSource = typename std::decay_t<TCollection>::value_type>
    requires (!Concepts::IsPair<TSource>)
    auto Generator(TCollection&& collection) noexcept
    {
        return LinqGenerator<TSource, CollectionFactory<std::decay_t<TCollection>>, std::integral_constant<bool, Concepts::IsNothrowIterable<TCollection>>>
            (CollectionFactory<std::decay_t<TCollection>>(std::forward<TCollection>(collection)));
    }

    /// @brief
    /// @tparam TCollection
    /// @tparam TSource
    /// @tparam TKey
    /// @tparam TValue
    /// @param collection
    /// @return
    template<Concepts::InputIterable TCollection,
             Concepts::IsPair TSource = typename std::decay_t<TCollection>::value_type,
             typename TKey = std::remove_const_t<typename PairTraits<TSource>::FirstType>,
             typename TValue = typename PairTraits<TSource>::SecondType>
    auto Generator(TCollection&& collection) noexcept
    {
        return LinqGenerator<std::pair<TKey, TValue>, CollectionFactory<std::decay_t<TCollection>>, std::integral_constant<bool, Concepts::IsNothrowIterable<TCollection>>>
            (CollectionFactory<std::decay_t<TCollection>>(std::forward<TCollection>(collection)));
    }

    /// @brief
    /// @tparam TIterator
    /// @tparam TSource
    /// @tparam NothrowIterable
    /// @param begin 
    /// @param end 
    /// @return
    template<std::input_iterator TIterator,
             typename TSource = typename std::iterator_traits<TIterator>::value_type,
             bool NothrowIterable = ++std::declval<TIterator>()>
    auto Generator(TIterator&& begin, TIterator&& end) noexcept
    {
        return LinqGenerator<TSource, IteratorFactory<std::decay_t<TIterator>>, std::integral_constant<bool, NothrowIterable>>
            (IteratorFactory<std::decay_t<TIterator>>(std::forward<TIterator>(begin), std::forward<TIterator>(end)));
    }
/*
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
*/
    /// @brief
    /// @tparam TCollection
    /// @tparam TIterator
    /// @param collection
    /// @return
    template<Concepts::ForwardIterable TCollection, std::forward_iterator TIterator = typename std::decay_t<TCollection>::const_iterator>
    LinqView<IteratorWrapper<TIterator>> View(const TCollection& collection) noexcept
    {
        return LinqView<IteratorWrapper<TIterator>>(
            IteratorWrapper<TIterator>(collection.cbegin(), collection.cend()),
            IteratorWrapper<TIterator>(collection.cend(), collection.cend()));
    }

    /// @brief
    /// @tparam TCollection
    /// @tparam TIterator
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
    LinqView<IteratorWrapper<TIterator>> View(const TIterator& begin, const TIterator& end) noexcept
    {
        return LinqView<IteratorWrapper<TIterator>>(
            IteratorWrapper<TIterator>(begin, end),
            IteratorWrapper<TIterator>(end, end));
    }

    /// @brief
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).ToVector()))
        {
            return std::forward<TLinqCollection>(linqCollection).ToVector();
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).ToList()))
        {
            return std::forward<TLinqCollection>(linqCollection).ToList();
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).ToForwardList()))
        {
            return std::forward<TLinqCollection>(linqCollection).ToForwardList();
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).ToStack()))
        {
            return std::forward<TLinqCollection>(linqCollection).ToStack();
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).ToQueue()))
        {
            return std::forward<TLinqCollection>(linqCollection).ToQueue();
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).ToDeque()))
        {
            return std::forward<TLinqCollection>(linqCollection).ToDeque();
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).ToPriorityQueue()))
        {
            return std::forward<TLinqCollection>(linqCollection).ToPriorityQueue();
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).ToSet()))
        {
            return std::forward<TLinqCollection>(linqCollection).ToSet();
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).ToUnorderedSet()))
        {
            return std::forward<TLinqCollection>(linqCollection).ToUnorderedSet();
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).ToMap()))
        {
            return std::forward<TLinqCollection>(linqCollection).ToMap();
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).ToUnorderedMap()))
        {
            return std::forward<TLinqCollection>(linqCollection).ToUnorderedMap();
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).template ToArray<N>()))
        {
            return std::forward<TLinqCollection>(linqCollection).template ToArray<N>();
        }
    };

    /// @brief
    /// @tparam TSelector
    template<typename TSelector>
    struct Select final
    {
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).Select(_selector)))
        {
            return std::forward<TLinqCollection>(std::forward<TLinqCollection>(linqCollection)).Select(_selector);
        }

    private:
        const std::decay_t<TSelector> _selector;
    };

    /// @brief
    /// @tparam TPredicate
    template<typename TPredicate>
    struct Where final
    {
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).Where(_predicate)))
        {
            return std::forward<TLinqCollection>(linqCollection).Where(_predicate);
        }

    private:
        const std::decay_t<TPredicate> _predicate;
    };

    /// @brief
    /// @tparam TPredicate
    template<typename TPredicate>
    struct RemoveWhere final
    {
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).RemoveWhere(_predicate)))
        {
            return std::forward<TLinqCollection>(linqCollection).RemoveWhere(_predicate);
        }

    private:
        const std::decay_t<TPredicate> _predicate;
    };

    /// @brief
    /// @tparam TMapFunction
    template<typename TMapFunction>
    struct Map final
    {
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).Map(_mapFunction)))
        {
            return std::forward<TLinqCollection>(linqCollection).Map(_mapFunction);
        }

    private:
        const std::decay_t<TMapFunction> _mapFunction;
    };

    /// @brief
    /// @tparam TTransformFunction
    template<typename TTransformFunction>
    struct Transform final
    {
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).Transform(_transformFunction)))
        {
            return std::forward<TLinqCollection>(linqCollection).Transform(_transformFunction);
        }

    private:
        const std::decay_t<TTransformFunction> _transformFunction;
    };

    /// @brief
    /// @tparam TKeySelector
    template<typename TKeySelector>
    struct GroupBy final
    {
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).GroupBy(_keySelector)))
        {
            return std::forward<TLinqCollection>(linqCollection).GroupBy(_keySelector);
        }

    private:
        const std::decay_t<TKeySelector> _keySelector;
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).Join(_otherCollection, _innerKeySelector, _otherKeySelector, _resultSelector)))
        {
            return std::forward<TLinqCollection>(linqCollection).Join(_otherCollection, _innerKeySelector, _otherKeySelector, _resultSelector);
        }

    private:
        const TOtherCollection& _otherCollection;
        const std::decay_t<TInnerKeySelector> _innerKeySelector;
        const std::decay_t<TOtherKeySelector> _otherKeySelector;
        const std::decay_t<TResultSelector> _resultSelector;
    };

    /// @brief
    /// @tparam TOtherCollection
    template<Concepts::ForwardIterable TOtherCollection>
    struct Zip final
    {
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).Zip(_otherCollection)))
        {
            return std::forward<TLinqCollection>(linqCollection).Zip(_otherCollection);
        }

    private:
        const TOtherCollection& _otherCollection;
    };

    /// @brief
    struct Skip final
    {
        /// @brief
        /// @param count
        explicit Skip(const std::size_t count) noexcept : _count(count) {}

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).Skip(std::declval<std::size_t>())))
        {
            return std::forward<TLinqCollection>(linqCollection).Skip(_count);
        }

    private:
        const std::size_t _count;
    };

    /// @brief
    /// @tparam TPredicate
    template<typename TPredicate>
    struct SkipWhile final
    {
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).SkipWhile(_predicate)))
        {
            return std::forward<TLinqCollection>(linqCollection).SkipWhile(_predicate);
        }

    private:
        const std::decay_t<TPredicate> _predicate;
    };

    /// @brief
    struct Take final
    {
        /// @brief
        /// @param count
        explicit Take(const std::size_t count) noexcept : _count(count) {}

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection&& linqCollection) const
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).Take(std::declval<std::size_t>())))
        {
            return std::forward<TLinqCollection>(linqCollection).Take(_count);
        }

    private:
        const std::size_t _count;
    };

    /// @brief
    /// @tparam TPredicate
    template<typename TPredicate>
    struct TakeWhile final
    {
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
        noexcept(noexcept(std::forward<TLinqCollection>(linqCollection).TakeWhile(_predicate)))
        {
            return std::forward<TLinqCollection>(linqCollection).TakeWhile(_predicate);
        }

    private:
        const std::decay_t<TPredicate> _predicate;
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
        return std::forward<TAdaptor>(adaptor)(std::forward<TLinqCollection>(linqCollection));
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
        return std::forward<TAdaptor>(adaptor)(View(range.cbegin(), range.cend()));
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
        return std::forward<TAdaptor>(adaptor)(View(range.begin(), range.end()));
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
        return std::forward<TAdaptor>(adaptor)(From(
            std::forward<TRange>(range).begin(),
            std::forward<TRange>(range).end()));
    }
}

#endif