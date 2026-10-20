#ifndef REFLECTION_COLLECTION_FIELD_INFO_H
#define REFLECTION_COLLECTION_FIELD_INFO_H

#include <any>
#include <ranges>
#include <string>
#include <typeindex>

#include <ExtendedCpp/Reflection/Concepts.h>
#include <ExtendedCpp/Reflection/FieldInfo.h>

/// @brief
namespace ExtendedCpp::Reflection
{
    /// @brief
    class CollectionFieldInfo final : public FieldInfo
    {
    private:
        const std::type_index _valueTypeIndex;
        const std::any _collectionFieldHelper;

        std::any (*const _elementValue)(const std::any& helper, std::any&& object, std::size_t position);
        std::any (*const _elementRef)(const std::any& helper, std::any&& object, std::size_t position);
        std::any (*const _elementConstRef)(const std::any& helper, std::any&& object, std::size_t position);
        std::any (*const _elementPtr)(const std::any& helper, std::any&& object, std::size_t position);
        std::any (*const _elementConstPtr)(const std::any& helper, std::any&& object, std::size_t position);

        std::any (*const _elementValueFromPtr)(const std::any& helper, const std::shared_ptr<void>& object, std::size_t position);
        std::any (*const _elementRefFromPtr)(const std::any& helper, std::shared_ptr<void>& object, std::size_t position);
        std::any (*const _elementConstRefFromPtr)(const std::any& helper, const std::shared_ptr<void>& object, std::size_t position);
        std::any (*const _elementPtrFromPtr)(const std::any& helper, std::shared_ptr<void>& object, std::size_t position);
        std::any (*const _elementConstPtrFromPtr)(const std::any& helper, const std::shared_ptr<void>& object, std::size_t position);

        void (*const _inserter)(const std::any& helper, std::any&& object, std::any&& element, std::size_t position);
        void (*const _frontInserter)(const std::any& helper, std::any&& object, std::any&& element);
        void (*const _backInserter)(const std::any& helper, std::any&& object, std::any&& element);

        void (*const _inserterFromPtr)(const std::any& helper, std::shared_ptr<void>& object, std::any&& element, std::size_t position);
        void (*const _frontInserterFromPtr)(const std::any& helper, std::shared_ptr<void>& object, std::any&& element);
        void (*const _backInserterFromPtr)(const std::any& helper, std::shared_ptr<void>& object, std::any&& element);

        std::size_t (*const _size)(const std::any& helper, std::any&& object);
        void (*const _clear)(const std::any& helper, std::any&& object);

        std::size_t (*const _sizeFromPtr)(const std::any& helper, const std::shared_ptr<void>& object);
        void (*const _clearFromPtr)(const std::any& helper, std::shared_ptr<void>& object);

        template<typename TObject, typename TField>
        requires std::ranges::random_access_range<TField> && std::ranges::sized_range<TField>
        struct Helper final
        {
        private:
            TField TObject::*const _fieldPtr;

            TField& FieldRef(std::any&& object) const
            {
                if (object.type() == typeid(TObject*))
                {
                    TObject* ptr = std::any_cast<TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");

                    return ptr->*_fieldPtr;
                }

                if (object.type() == typeid(std::reference_wrapper<TObject>))
                    return std::any_cast<std::reference_wrapper<TObject>>(std::move(object)).get().*_fieldPtr;

                throw std::invalid_argument(std::format("Invalid object type which is {}, but expected {} or {}",
                    object.type().name(), typeid(TObject*).name(), typeid(std::reference_wrapper<TObject>).name()));
            }

            const TField& FieldConstRef(std::any&& object) const
            {
                if (object.type() == typeid(TObject*))
                {
                    const TObject* ptr = std::any_cast<TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");

                    return ptr->*_fieldPtr;
                }

                if (object.type() == typeid(const TObject*))
                {
                    const TObject* ptr = std::any_cast<const TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");

                    return ptr->*_fieldPtr;
                }

                if (object.type() == typeid(std::reference_wrapper<TObject>))
                    return std::any_cast<std::reference_wrapper<TObject>>(std::move(object)).get().*_fieldPtr;

                if (object.type() == typeid(std::reference_wrapper<const TObject>))
                    return std::any_cast<std::reference_wrapper<const TObject>>(std::move(object)).get().*_fieldPtr;

                throw std::invalid_argument(std::format("Invalid object type which is {}, but expected {}, {}, {} or {}",
                    object.type().name(), typeid(TObject*).name(), typeid(const TObject*).name(),
                    typeid(std::reference_wrapper<TObject>).name(), typeid(std::reference_wrapper<const TObject>).name()));
            }

        public:
            using value_type = typename TField::value_type;

            explicit Helper(TField TObject::*const fieldPtr) noexcept :
                _fieldPtr(fieldPtr) {}

            [[nodiscard]]
            value_type Value(std::any&& object, const std::size_t position) const
            {
                const TField& field = FieldConstRef(std::move(object));
                if (position >= field.size())
                    throw std::out_of_range(std::format("Position which is {} must be less then size which is {}", position, field.size()));
                return *(field.begin() + position);
            }

            [[nodiscard]]
            value_type Value(const std::shared_ptr<void>& object, const std::size_t position) const
            {
                const TField& field = std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
                if (position >= field.size())
                    throw std::out_of_range(std::format("Position which is {} must be less then size which is {}", position, field.size()));
                return *(field.begin() + position);
            }

            [[nodiscard]]
            std::reference_wrapper<value_type> Ref(std::any&& object, const std::size_t position) const
            {
                TField& field = FieldRef(std::move(object));
                if (position >= field.size())
                    throw std::out_of_range(std::format("Position which is {} must be less then size which is {}", position, field.size()));
                return std::ref(*(field.begin() + position));
            }

            // ReSharper disable once CppParameterMayBeConstPtrOrRef
            [[nodiscard]]
            std::reference_wrapper<value_type> Ref(std::shared_ptr<void>& object, const std::size_t position) const
            {
                TField& field = std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
                if (position >= field.size())
                    throw std::out_of_range(std::format("Position which is {} must be less then size which is {}", position, field.size()));
                return std::ref(*(field.begin() + position));
            }

            [[nodiscard]]
            std::reference_wrapper<const value_type> ConstRef(std::any&& object, const std::size_t position) const
            {
                const TField& field = FieldConstRef(std::move(object));
                if (position >= field.size())
                    throw std::out_of_range(std::format("Position which is {} must be less then size which is {}", position, field.size()));
                return std::cref(*(field.begin() + position));
            }

            [[nodiscard]]
            std::reference_wrapper<const value_type> ConstRef(const std::shared_ptr<void>& object, const std::size_t position) const
            {
                const TField& field = std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
                if (position >= field.size())
                    throw std::out_of_range(std::format("Position which is {} must be less then size which is {}", position, field.size()));
                return std::cref(*(field.begin() + position));
            }

            [[nodiscard]]
            value_type* Ptr(std::any&& object, const std::size_t position) const
            {
                TField& field = FieldRef(std::move(object));
                if (position >= field.size())
                    throw std::out_of_range(std::format("Position which is {} must be less then size which is {}", position, field.size()));
                return &(*(field.begin() + position));
            }

            // ReSharper disable once CppParameterMayBeConstPtrOrRef
            [[nodiscard]]
            value_type* Ptr(std::shared_ptr<void>& object, const std::size_t position) const
            {
                TField& field = std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
                if (position >= field.size())
                    throw std::out_of_range(std::format("Position which is {} must be less then size which is {}", position, field.size()));
                return &(*(field.begin() + position));
            }

            [[nodiscard]]
            const value_type* ConstPtr(std::any&& object, const std::size_t position) const
            {
                const TField& field = FieldConstRef(std::move(object));
                if (position >= field.size())
                    throw std::out_of_range(std::format("Position which is {} must be less then size which is {}", position, field.size()));
                return &(*(field.begin() + position));
            }

            [[nodiscard]]
            const value_type* ConstPtr(const std::shared_ptr<void>& object, const std::size_t position) const
            {
                const TField& field = std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
                if (position >= field.size())
                    throw std::out_of_range(std::format("Position which is {} must be less then size which is {}", position, field.size()));
                return &(*(field.begin() + position));
            }

            void Insert(std::any&& object, std::any&& element, const std::size_t position) const
            {
                TField& field = FieldRef(std::move(object));
                field.insert(field.begin() + position, std::any_cast<value_type>(std::move(element)));
            }

            void InsertFront(std::any&& object, std::any&& element) const
            {
                TField& field = FieldRef(std::move(object));
                field.insert(field.begin(), std::any_cast<value_type>(std::move(element)));
            }

            void InsertBack(std::any&& object, std::any&& element) const
            {
                TField& field = FieldRef(std::move(object));
                field.insert(field.end(), std::any_cast<value_type>(std::move(element)));
            }

            // ReSharper disable once CppParameterMayBeConstPtrOrRef
            void Insert(std::shared_ptr<void>& object, std::any&& element, const std::size_t position) const noexcept
            {
                TField& field = std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
                field.insert(field.begin() + position, std::any_cast<value_type>(std::move(element)));
            }

            // ReSharper disable once CppParameterMayBeConstPtrOrRef
            void InsertFront(std::shared_ptr<void>& object, std::any&& element) const noexcept
            {
                TField& field = std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
                field.insert(field.begin(), std::any_cast<value_type>(std::move(element)));
            }

            // ReSharper disable once CppParameterMayBeConstPtrOrRef
            void InsertBack(std::shared_ptr<void>& object, std::any&& element) const noexcept
            {
                TField& field = std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
                field.insert(field.end(), std::any_cast<value_type>(std::move(element)));
            }

            [[nodiscard]]
            std::size_t Size(std::any&& object) const
            {
                return FieldConstRef(std::move(object)).size();
            }

            void Clear(std::any&& object) const
            {
                FieldRef(std::move(object)).clear();
            }

            [[nodiscard]]
            std::size_t Size(const std::shared_ptr<void>& object) const noexcept
            {
                return (std::static_pointer_cast<TObject>(object).get()->*_fieldPtr).size();
            }

            // ReSharper disable once CppParameterMayBeConstPtrOrRef
            void Clear(std::shared_ptr<void>& object) const noexcept
            {
                (std::static_pointer_cast<TObject>(object).get()->*_fieldPtr).clear();
            }
        };

    public:
        /// @brief
        /// @tparam THelper
        /// @param fieldName
        /// @param typeIndex
        /// @param fieldHelper
        /// @param collectionFieldHelper
        template<typename THelper, typename TCollectionHelper>
        CollectionFieldInfo(std::string&& fieldName, std::type_index typeIndex, THelper&& fieldHelper, TCollectionHelper&& collectionFieldHelper) noexcept :
            FieldInfo(std::move(fieldName), std::move(typeIndex), std::forward<THelper>(fieldHelper)),
            _valueTypeIndex(typeid(typename std::decay_t<TCollectionHelper>::value_type)),
            _collectionFieldHelper(std::forward<TCollectionHelper>(collectionFieldHelper)),

            _elementValue([](const std::any& helper, std::any&& object, const std::size_t position)
                { return std::any(std::any_cast<TCollectionHelper>(helper).Value(std::move(object), position)); }),
            _elementRef([](const std::any& helper, std::any&& object, const std::size_t position)
                { return std::any(std::any_cast<TCollectionHelper>(helper).Ref(std::move(object), position)); }),
            _elementConstRef([](const std::any& helper, std::any&& object, const std::size_t position)
                { return std::any(std::any_cast<TCollectionHelper>(helper).ConstRef(std::move(object), position)); }),
            _elementPtr([](const std::any& helper, std::any&& object, const std::size_t position)
                { return std::any(std::any_cast<TCollectionHelper>(helper).Ptr(std::move(object), position)); }),
            _elementConstPtr([](const std::any& helper, std::any&& object, const std::size_t position)
                { return std::any(std::any_cast<TCollectionHelper>(helper).ConstPtr(std::move(object), position)); }),

            _elementValueFromPtr([](const std::any& helper, const std::shared_ptr<void>& object, const std::size_t position)
                { return std::any(std::any_cast<TCollectionHelper>(helper).Value(object, position)); }),
            _elementRefFromPtr([](const std::any& helper, std::shared_ptr<void>& object, const std::size_t position)
                { return std::any(std::any_cast<TCollectionHelper>(helper).Ref(object, position)); }),
            _elementConstRefFromPtr([](const std::any& helper, const std::shared_ptr<void>& object, const std::size_t position)
                { return std::any(std::any_cast<TCollectionHelper>(helper).ConstRef(object, position)); }),
            _elementPtrFromPtr([](const std::any& helper, std::shared_ptr<void>& object, const std::size_t position)
                { return std::any(std::any_cast<TCollectionHelper>(helper).Ptr(object, position)); }),
            _elementConstPtrFromPtr([](const std::any& helper, const std::shared_ptr<void>& object, const std::size_t position)
                { return std::any(std::any_cast<TCollectionHelper>(helper).ConstPtr(object, position)); }),

            _inserter([](const std::any& helper, std::any&& object, std::any&& element, const std::size_t position)
                { std::any_cast<TCollectionHelper>(helper).Insert(std::move(object), std::move(element), position); }),
            _frontInserter([](const std::any& helper, std::any&& object, std::any&& element)
                { std::any_cast<TCollectionHelper>(helper).InsertFront(std::move(object), std::move(element)); }),
            _backInserter([](const std::any& helper, std::any&& object, std::any&& element)
                { std::any_cast<TCollectionHelper>(helper).InsertBack(std::move(object), std::move(element)); }),

            _inserterFromPtr([](const std::any& helper, std::shared_ptr<void>& object, std::any&& element, const std::size_t position)
                { std::any_cast<TCollectionHelper>(helper).Insert(object, std::move(element), position); }),
            _frontInserterFromPtr([](const std::any& helper, std::shared_ptr<void>& object, std::any&& element)
                { std::any_cast<TCollectionHelper>(helper).InsertFront(object, std::move(element)); }),
            _backInserterFromPtr([](const std::any& helper, std::shared_ptr<void>& object, std::any&& element)
                { std::any_cast<TCollectionHelper>(helper).InsertBack(object, std::move(element)); }),

            _size([](const std::any& helper, std::any&& object)
                { return std::any_cast<TCollectionHelper>(helper).Size(std::move(object)); }),
            _clear([](const std::any& helper, std::any&& object)
                { return std::any_cast<TCollectionHelper>(helper).Clear(std::move(object)); }),

            _sizeFromPtr([](const std::any& helper, const std::shared_ptr<void>& object)
                { return std::any_cast<TCollectionHelper>(helper).Size(object); }),
            _clearFromPtr([](const std::any& helper, std::shared_ptr<void>& object)
                { return std::any_cast<TCollectionHelper>(helper).Clear(object); }) {}

        /// @brief
        ~CollectionFieldInfo() noexcept override = default;

        /// @brief
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType>
        TValueType GetElementValue(std::any object, const std::size_t position) const
        {
            return std::any_cast<TValueType>(_elementValue(_collectionFieldHelper, std::move(object), position));
        }

        /// @brief
        /// @param object
        /// @param position
        /// @return
        [[nodiscard]]
        std::any GetElementValue(std::any object, std::size_t position) const;

        /// @brief
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType>
        TValueType GetElementValue(const std::shared_ptr<void>& object, const std::size_t position) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<TValueType>(_elementValueFromPtr(_collectionFieldHelper, object, position));
        }

        /// @brief
        /// @param object
        /// @param position
        /// @return
        [[nodiscard]]
        std::any GetElementValue(const std::shared_ptr<void>& object, std::size_t position) const;

        /// @brief
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType>
        TValueType& GetElementRef(std::any object, const std::size_t position) const
        {
            return std::any_cast<std::reference_wrapper<TValueType>>(_elementRef(_collectionFieldHelper, std::move(object), position)).get();
        }

        /// @brief
        /// @param object
        /// @param position
        /// @return
        [[nodiscard]]
        std::any GetElementRef(std::any object, std::size_t position) const;

        /// @brief
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType>
        TValueType& GetElementRef(std::shared_ptr<void>& object, const std::size_t position) const
        {
            return std::any_cast<std::reference_wrapper<TValueType>>(_elementRefFromPtr(_collectionFieldHelper, object, position)).get();
        }

        /// @brief
        /// @param object
        /// @param position
        /// @return
        [[nodiscard]]
        std::any GetElementRef(std::shared_ptr<void>& object, std::size_t position) const;

        /// @brief
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType>
        const TValueType& GetElementConstRef(std::any object, const std::size_t position) const
        {
            return std::any_cast<std::reference_wrapper<const TValueType>>(_elementRef(_collectionFieldHelper, std::move(object), position)).get();
        }

        /// @brief
        /// @param object
        /// @param position
        /// @return
        [[nodiscard]]
        std::any GetElementConstRef(std::any object, std::size_t position) const;

        /// @brief
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType>
        const TValueType& GetElementConstRef(const std::shared_ptr<void>& object, const std::size_t position) const
        {
            return std::any_cast<std::reference_wrapper<const TValueType>>(_elementConstRefFromPtr(_collectionFieldHelper, object, position)).get();
        }

        /// @brief
        /// @param object
        /// @param position
        /// @return
        [[nodiscard]]
        std::any GetElementConstRef(const std::shared_ptr<void>& object, std::size_t position) const;

        /// @brief
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType>
        TValueType* GetElementPtr(std::any object, const std::size_t position) const
        {
            return std::any_cast<TValueType*>(_elementPtr(_collectionFieldHelper, std::move(object), position));
        }

        /// @brief
        /// @param object
        /// @param position
        /// @return
        [[nodiscard]]
        std::any GetElementPtr(std::any object, std::size_t position) const;

        /// @brief
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType>
        TValueType* GetElementPtr(std::shared_ptr<void>& object, const std::size_t position) const
        {
            return std::any_cast<TValueType*>(_elementPtrFromPtr(_collectionFieldHelper, object, position));
        }

        /// @brief
        /// @param object
        /// @param position
        /// @return
        [[nodiscard]]
        std::any GetElementPtr(std::shared_ptr<void>& object, std::size_t position) const;

        /// @brief
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType>
        const TValueType* GetElementConstPtr(std::any object, const std::size_t position) const
        {
            return std::any_cast<const TValueType*>(_elementConstPtr(_collectionFieldHelper, std::move(object), position));
        }

        /// @brief
        /// @param object
        /// @param position
        /// @return
        [[nodiscard]]
        std::any GetElementConstPtr(std::any object, std::size_t position) const;

        /// @brief
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType>
        const TValueType* GetElementConstPtr(const std::shared_ptr<void>& object, const std::size_t position) const
        {
            return std::any_cast<const TValueType*>(_elementConstPtrFromPtr(_collectionFieldHelper, object, position));
        }

        /// @brief
        /// @param object
        /// @param position
        /// @return
        [[nodiscard]]
        std::any GetElementConstPtr(const std::shared_ptr<void>& object, std::size_t position) const;

        /// @brief
        /// @tparam TElement
        /// @param object
        /// @param element
        /// @param position
        template<typename TElement>
        void Insert(std::any object, TElement&& element, const std::size_t position) const
        {
            _inserter(_collectionFieldHelper, std::move(object), std::forward<TElement>(element), position);
        }

        /// @brief
        /// @tparam TElement
        /// @param object
        /// @param element
        template<typename TElement>
        void InsertFront(std::any object, TElement&& element) const
        {
            _frontInserter(_collectionFieldHelper, std::move(object), std::forward<TElement>(element));
        }

        /// @brief
        /// @tparam TElement
        /// @param object
        /// @param element
        template<typename TElement>
        void InsertBack(std::any object, TElement&& element) const
        {
            _backInserter(_collectionFieldHelper, std::move(object), std::forward<TElement>(element));
        }

        /// @brief
        /// @tparam TElement
        /// @param object
        /// @param element
        /// @param position
        template<typename TElement>
        void Insert(std::shared_ptr<void>& object, TElement&& element, const std::size_t position) const
        {
            _inserterFromPtr(_collectionFieldHelper, std::move(object), std::forward<TElement>(element), position);
        }

        /// @brief
        /// @tparam TElement
        /// @param object
        /// @param element
        template<typename TElement>
        void InsertFront(std::shared_ptr<void>& object, TElement&& element) const
        {
            _frontInserterFromPtr(_collectionFieldHelper, std::move(object), std::forward<TElement>(element));
        }

        /// @brief
        /// @tparam TElement
        /// @param object
        /// @param element
        template<typename TElement>
        void InsertBack(std::shared_ptr<void>& object, TElement&& element) const
        {
            _backInserterFromPtr(_collectionFieldHelper, std::move(object), std::forward<TElement>(element));
        }

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::size_t Size(std::any object) const;

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::size_t Size(const std::shared_ptr<void>& object) const;

        /// @brief
        /// @param object
        void Clear(std::any object) const;

        /// @brief
        /// @param object
        void Clear(std::shared_ptr<void>& object) const;

        /// @brief
        /// @return
        [[nodiscard]]
        std::type_index ValueTypeIndex() const noexcept;

        /// @brief
        /// @return
        [[nodiscard]]
        bool IsCollection() const noexcept override
        {
            return true;
        }

        /// @brief
        /// @tparam TObject
        /// @tparam TField
        /// @param name
        /// @param fieldPtr
        /// @return
        template<typename TObject, typename TField>
        static std::shared_ptr<MemberInfo> Create(std::string&& name, TField TObject::*const fieldPtr) noexcept
        {
            return std::make_shared<CollectionFieldInfo>(std::move(name), typeid(TField),
                FieldInfo::Helper(fieldPtr), CollectionFieldInfo::Helper(fieldPtr));
        }
    };
}

#define FIELD(name) \
[]() \
{ \
    if constexpr (std::ranges::random_access_range<decltype(name)> && std::ranges::sized_range<decltype(name)>) \
        return ExtendedCpp::Reflection::CollectionFieldInfo::Create(#name, &ThisClassType::name); \
    else \
        return ExtendedCpp::Reflection::FieldInfo::Create(#name, &ThisClassType::name); \
}()

#endif