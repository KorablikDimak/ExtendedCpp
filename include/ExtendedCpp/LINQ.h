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
    template<Concepts::ForwardIterable TCollection, typename TSource = typename std::decay_t<TCollection>::value_type>
    LinqContainer<TSource> From(TCollection&& collection) noexcept
    {
        return LinqContainer(std::vector<TSource>(collection.begin(), collection.end()));
    }

    /// @brief 
    /// @tparam TSource 
    /// @tparam TIterator 
    /// @param begin 
    /// @param end 
    /// @return 
    template<std::forward_iterator TIterator, typename TSource = typename std::iterator_traits<TIterator>::value_type>
    LinqContainer<TSource> From(TIterator&& begin, TIterator&& end) noexcept
    {
        return LinqContainer(std::vector<TSource>(std::forward<TIterator>(begin), std::forward<TIterator>(end)));
    }

    /// @brief
    /// @tparam TSource
    /// @param collection
    /// @return
    template<typename TSource>
    LinqContainer<TSource> From(std::vector<TSource>&& collection) noexcept
    {
        return LinqContainer(std::move(collection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqContainer<TSource> From(const std::stack<TSource>& collection) 
    noexcept(std::is_nothrow_move_assignable_v<TSource> && std::is_nothrow_copy_assignable_v<std::stack<TSource>>)
    {
        std::stack<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.top());
            copy.pop();
        }

        return LinqContainer(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqContainer<TSource> From(std::stack<TSource>&& collection) 
    noexcept(std::is_nothrow_move_assignable_v<TSource>)
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(collection.top());
            collection.pop();
        }

        return LinqContainer(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqContainer<TSource> From(const std::queue<TSource>& collection) 
    noexcept(std::is_nothrow_move_assignable_v<TSource> && std::is_nothrow_copy_assignable_v<std::queue<TSource>>)
    {
        std::queue<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.front());
            copy.pop();
        }

        return LinqContainer(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqContainer<TSource> From(std::queue<TSource>&& collection) 
    noexcept(std::is_nothrow_move_assignable_v<TSource>)
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(collection.front());
            collection.pop();
        }

        return LinqContainer(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqContainer<TSource> From(const std::priority_queue<TSource>& collection) 
    noexcept(std::is_nothrow_move_assignable_v<TSource> && std::is_nothrow_copy_assignable_v<std::priority_queue<TSource>>)
    {
        std::priority_queue<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.top());
            copy.pop();
        }

        return LinqContainer(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqContainer<TSource> From(std::priority_queue<TSource>&& collection) 
    noexcept(std::is_nothrow_move_assignable_v<TSource>)
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(collection.top());
            collection.pop();
        }

        return LinqContainer(std::move(vectorCollection));
    }

    /// @brief
    /// @tparam TKey
    /// @tparam TValue
    /// @param collection
    /// @return
    template<typename TKey, typename TValue>
    LinqContainer<std::pair<TKey, TValue>> From(const std::map<TKey, TValue>& collection) noexcept
    {
        return LinqContainer(std::vector<std::pair<TKey, TValue>>(collection.cbegin(), collection.cend()));
    }

    /// @brief
    /// @tparam TKey
    /// @tparam TValue
    /// @param collection
    /// @return
    template<typename TKey, typename TValue>
    LinqContainer<std::pair<TKey, TValue>> From(std::map<TKey, TValue>&& collection) noexcept
    {
        return LinqContainer(std::vector<std::pair<TKey, TValue>>(collection.begin(), collection.end()));
    }

    /// @brief
    /// @tparam TKey
    /// @tparam TValue
    /// @param collection
    /// @return
    template<typename TKey, typename TValue>
    LinqContainer<std::pair<TKey, TValue>> From(const std::unordered_map<TKey, TValue>& collection) noexcept
    {
        return LinqContainer(std::vector<std::pair<TKey, TValue>>(collection.cbegin(), collection.cend()));
    }

    /// @brief
    /// @tparam TKey
    /// @tparam TValue
    /// @param collection
    /// @return
    template<typename TKey, typename TValue>
    LinqContainer<std::pair<TKey, TValue>> From(std::unordered_map<TKey, TValue>&& collection) noexcept
    {
        return LinqContainer(std::vector<std::pair<TKey, TValue>>(collection.begin(), collection.end()));
    }

    /// @brief 
    /// @tparam TSource 
    /// @tparam TCollection 
    /// @param collection 
    /// @return 
    template<Concepts::InputIterable TCollection, typename TSource = typename std::decay_t<TCollection>::value_type>
    requires (!Concepts::IsPair<TSource>)
    LinqGenerator<TSource> Generator(TCollection&& collection) noexcept
    {
        return LinqGenerator<TSource>(std::forward<TCollection>(collection));
    }

    template<Concepts::InputIterable TCollection, Concepts::IsPair TSource = typename std::decay_t<TCollection>::value_type,
             typename TKey = std::remove_const_t<typename PairTraits<TSource>::FirstType>, typename TValue = typename PairTraits<TSource>::SecondType>
    LinqGenerator<std::pair<TKey, TValue>> Generator(TCollection&& collection) noexcept
    {
        return LinqGenerator<std::pair<TKey, TValue>>(std::forward<TCollection>(collection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @tparam TIterator 
    /// @param begin 
    /// @param end 
    /// @return 
    template<std::input_iterator TIterator, typename TSource = typename std::iterator_traits<TIterator>::value_type>
    LinqGenerator<TSource> Generator(TIterator&& begin, TIterator&& end) noexcept
    {
        return LinqGenerator<TSource>(std::forward<TIterator>(begin), std::forward<TIterator>(end));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqGenerator<TSource> Generator(const std::stack<TSource>& collection) 
    noexcept(std::is_nothrow_move_assignable_v<TSource> && std::is_nothrow_copy_assignable_v<std::stack<TSource>>)
    {
        std::stack<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.top());
            copy.pop();
        }

        return LinqGenerator<TSource>(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqGenerator<TSource> Generator(std::stack<TSource>&& collection) 
    noexcept(std::is_nothrow_move_assignable_v<TSource>)
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(collection.top());
            collection.pop();
        }

        return LinqGenerator<TSource>(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqGenerator<TSource> Generator(const std::queue<TSource>& collection) 
    noexcept(std::is_nothrow_move_assignable_v<TSource> && std::is_nothrow_copy_assignable_v<std::queue<TSource>>)
    {
        std::queue<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.front());
            copy.pop();
        }

        return LinqGenerator<TSource>(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqGenerator<TSource> Generator(std::queue<TSource>&& collection) 
    noexcept(std::is_nothrow_move_assignable_v<TSource>)
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = collection.front();
            collection.pop();
        }

        return LinqGenerator<TSource>(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqGenerator<TSource> Generator(const std::priority_queue<TSource>& collection) 
    noexcept(std::is_nothrow_move_assignable_v<TSource> && std::is_nothrow_copy_assignable_v<std::priority_queue<TSource>>)
    {
        std::priority_queue<TSource> copy = collection;

        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(copy.top());
            copy.pop();
        }

        return LinqGenerator<TSource>(std::move(vectorCollection));
    }

    /// @brief 
    /// @tparam TSource 
    /// @param collection 
    /// @return 
    template<typename TSource>
    LinqGenerator<TSource> Generator(std::priority_queue<TSource>&& collection) 
    noexcept(std::is_nothrow_move_assignable_v<TSource>)
    {
        std::vector<TSource> vectorCollection(collection.size());
        for (std::size_t i = 0; i < collection.size(); ++i)
        {
            vectorCollection[i] = std::move(collection.top());
            collection.pop();
        }

        return LinqGenerator<TSource>(std::move(vectorCollection));
    }

    /// @brief
    /// @tparam TIterator
    /// @tparam TCollection
    /// @param collection
    /// @return
    template<Concepts::ForwardIterable TCollection, typename TIterator = typename std::decay_t<TCollection>::const_iterator>
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
    template<Concepts::ForwardIterable TCollection, typename TIterator = typename std::decay_t<TCollection>::iterator>
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

    /// @brief 
    /// @tparam TSource 
    /// @tparam TIterator 
    /// @param collection 
    /// @return 
    template<typename TSource, typename TIterator = typename std::vector<TSource>::iterator>
    LinqView<IteratorWrapper<TIterator>> View(std::stack<TSource> collection) = delete;

    /// @brief 
    /// @tparam TSource 
    /// @tparam TIterator 
    /// @param collection 
    /// @return 
    template<typename TSource, typename TIterator = typename std::vector<TSource>::iterator>
    LinqView<IteratorWrapper<TIterator>> View(std::queue<TSource> collection) = delete;

    /// @brief 
    /// @tparam TSource 
    /// @tparam TIterator 
    /// @param collection 
    /// @return 
    template<typename TSource, typename TIterator = typename std::vector<TSource>::iterator>
    LinqView<IteratorWrapper<TIterator>> View(std::priority_queue<TSource> collection) = delete;

    /// @brief
    /// @tparam T
    template<typename T>
    struct To;

    /// @brief
    /// @tparam TSource
    template<typename TSource>
    struct To<LinqContainer<TSource>> final
    {
        /// @brief
        static constexpr bool IsLinqAdaptor = true;

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        LinqContainer<TSource> operator()(TLinqCollection&& linqCollection) const
        {
            return From(std::forward<TLinqCollection>(linqCollection));
        }
    };

    /// @brief
    /// @tparam TSource
    template<typename TSource>
    struct To<LinqGenerator<TSource>> final
    {
        /// @brief
        static constexpr bool IsLinqAdaptor = true;

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        LinqGenerator<TSource> operator()(TLinqCollection&& linqCollection) const
        {
            return Generator(std::forward<TLinqCollection>(linqCollection));
        }
    };

    /// @brief
    /// @tparam TSource
    template<typename TSource>
    struct To<LinqView<TSource>> final
    {
        /// @brief
        static constexpr bool IsLinqAdaptor = true;

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        auto operator()(TLinqCollection& linqCollection) const
        {
            return View(std::forward<TLinqCollection>(linqCollection));
        }
    };

    /// @brief
    /// @tparam TSource
    template<typename TSource>
    struct To<std::vector<TSource>> final
    {
        /// @brief
        static constexpr bool IsLinqAdaptor = true;

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::vector<TSource> operator()(TLinqCollection&& linqCollection) const
        noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TLinqCollection>().ToVector())>)
        {
            return linqCollection.ToVector();
        }
    };

    /// @brief
    /// @tparam TSource
    template<typename TSource>
    struct To<std::list<TSource>> final
    {
        /// @brief
        static constexpr bool IsLinqAdaptor = true;

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::list<TSource> operator()(TLinqCollection&& linqCollection) const
        noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TLinqCollection>().ToList())>)
        {
            return linqCollection.ToList();
        }
    };

    /// @brief
    /// @tparam TSource
    template<typename TSource>
    struct To<std::forward_list<TSource>> final
    {
        /// @brief
        static constexpr bool IsLinqAdaptor = true;

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::forward_list<TSource> operator()(TLinqCollection&& linqCollection) const
        noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TLinqCollection>().ToForwardList())>)
        {
            return linqCollection.ToForwardList();
        }
    };

    /// @brief
    /// @tparam TSource
    template<typename TSource>
    struct To<std::stack<TSource>> final
    {
        /// @brief
        static constexpr bool IsLinqAdaptor = true;

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::stack<TSource> operator()(TLinqCollection&& linqCollection) const
        noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TLinqCollection>().ToStack())>)
        {
            return linqCollection.ToStack();
        }
    };

    /// @brief
    /// @tparam TSource
    template<typename TSource>
    struct To<std::queue<TSource>> final
    {
        /// @brief
        static constexpr bool IsLinqAdaptor = true;

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::queue<TSource> operator()(TLinqCollection&& linqCollection) const
        noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TLinqCollection>().ToQueue())>)
        {
            return linqCollection.ToQueue();
        }
    };

    /// @brief
    /// @tparam TSource
    template<typename TSource>
    struct To<std::deque<TSource>> final
    {
        /// @brief
        static constexpr bool IsLinqAdaptor = true;

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::deque<TSource> operator()(TLinqCollection&& linqCollection) const
        noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TLinqCollection>().ToDeque())>)
        {
            return linqCollection.ToDeque();
        }
    };

    /// @brief
    /// @tparam TSource
    template<typename TSource>
    struct To<std::priority_queue<TSource>> final
    {
        /// @brief
        static constexpr bool IsLinqAdaptor = true;

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::priority_queue<TSource> operator()(TLinqCollection&& linqCollection) const
        noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TLinqCollection>().ToPriorityQueue())>)
        {
            return linqCollection.ToPriorityQueue();
        }
    };

    /// @brief
    /// @tparam TSource
    template<typename TSource>
    struct To<std::set<TSource>> final
    {
        /// @brief
        static constexpr bool IsLinqAdaptor = true;

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::set<TSource> operator()(TLinqCollection&& linqCollection) const
        noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TLinqCollection>().ToSet())>)
        {
            return linqCollection.ToSet();
        }
    };

    /// @brief
    /// @tparam TSource
    template<typename TSource>
    struct To<std::unordered_set<TSource>> final
    {
        /// @brief
        static constexpr bool IsLinqAdaptor = true;

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::unordered_set<TSource> operator()(TLinqCollection&& linqCollection) const
        noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TLinqCollection>().ToUnorderedSet())>)
        {
            return linqCollection.ToUnorderedSet();
        }
    };

    /// @brief
    /// @tparam TKey
    /// @tparam TValue
    template<typename TKey, typename TValue>
    struct To<std::map<TKey, TValue>> final
    {
        /// @brief
        static constexpr bool IsLinqAdaptor = true;

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::map<TKey, TValue> operator()(TLinqCollection&& linqCollection) const
        noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TLinqCollection>().ToMap())>)
        {
            return linqCollection.ToMap();
        }
    };

    /// @brief
    /// @tparam TKey
    /// @tparam TValue
    template<typename TKey, typename TValue>
    struct To<std::unordered_map<TKey, TValue>> final
    {
        /// @brief
        static constexpr bool IsLinqAdaptor = true;

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::unordered_map<TKey, TValue> operator()(TLinqCollection&& linqCollection) const
        noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TLinqCollection>().ToUnorderedMap())>)
        {
            return linqCollection.ToUnorderedMap();
        }
    };

    /// @brief
    /// @tparam TSource
    /// @tparam N
    template<typename TSource, std::size_t N>
    struct To<std::array<TSource, N>> final
    {
        /// @brief
        static constexpr bool IsLinqAdaptor = true;

        /// @brief
        /// @tparam TLinqCollection
        /// @param linqCollection
        /// @return
        template<Concepts::LinqCollection TLinqCollection>
        std::array<TSource, N> operator()(TLinqCollection&& linqCollection) const
        noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TLinqCollection>().template ToArray<N>())>)
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
        static constexpr bool IsLinqAdaptor = true;

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
        noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TLinqCollection>().Select(std::declval<TSelector>())), TSelector>)
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
        static constexpr bool IsLinqAdaptor = true;

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
        noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TLinqCollection>().Where(std::declval<TPredicate>())), TPredicate>)
        {
            return linqCollection.Where(_predicate);
        }
    };

    // TODO RemoveWhere

    /// @brief
    /// @tparam TMapFunction
    template<typename TMapFunction>
    struct Map final
    {
        /// @brief
        static constexpr bool IsLinqAdaptor = true;

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
        noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TLinqCollection>().Map(std::declval<TMapFunction>())),
                                             TMapFunction>)
        {
            return linqCollection.Map(_mapFunction);
        }
    };

    template<typename TTransformFunction>
    struct Transform final
    {
        /// @brief
        static constexpr bool IsLinqAdaptor = true;

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
        noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TLinqCollection>().Map(std::declval<TTransformFunction>())), TTransformFunction>)
        {
            return linqCollection.Transform(_transformFunction);
        }
    };

    // TODO GroupBy
    // TODO Join
    // TODO Zip
    // TODO Skip
    // TODO SkipWhile
    // TODO Take
    // TODO TakeWhile

    /// @brief
    /// @tparam TLinqCollection
    /// @tparam TAdaptor
    /// @param linqCollection
    /// @param adaptor
    /// @return
    template<Concepts::LinqCollection TLinqCollection, Concepts::LinqAdaptor TAdaptor>
    auto operator|(TLinqCollection&& linqCollection, TAdaptor&& adaptor)
    noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TAdaptor>()(std::declval<TLinqCollection>())), TLinqCollection>)
    {
        return adaptor(std::forward<TLinqCollection>(linqCollection));
    }

    /// @brief
    /// @tparam TCollection
    /// @tparam TAdaptor
    /// @param collection
    /// @param adaptor
    /// @return
    template<Concepts::ForwardIterable TCollection, Concepts::LinqAdaptor TAdaptor>
    requires (!Concepts::LinqCollection<TCollection>)
    auto operator|(const TCollection& collection, TAdaptor&& adaptor)
    noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TAdaptor>()
        (std::declval<LinqView<IteratorWrapper<typename TCollection::iterator>>>())), TCollection>)
    {
        return adaptor(View(collection));
    }

    /// @brief
    /// @tparam TCollection
    /// @tparam TAdaptor
    /// @param collection
    /// @param adaptor
    /// @return
    template<Concepts::ForwardIterable TCollection, Concepts::LinqAdaptor TAdaptor>
    requires (!Concepts::LinqCollection<TCollection>)
    auto operator|(TCollection&& collection, TAdaptor&& adaptor)
    noexcept(std::is_nothrow_invocable_v<decltype(std::declval<TAdaptor>()
        (std::declval<LinqContainer<typename TCollection::value_type>>())), TCollection>)
    {
        return adaptor(From(std::forward<TCollection>(collection)));
    }
}

#endif