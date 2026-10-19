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

        public:
            using value_type = typename TField::value_type;

            explicit Helper(TField TObject::*const fieldPtr) noexcept :
                _fieldPtr(fieldPtr) {}

            [[nodiscard]]
            value_type Value(std::any&& object, const std::size_t position) const
            {
                if (object.type() == typeid(const TObject*))
                {
                    const TObject* ptr = std::any_cast<const TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");

                    const TField& field = ptr->*_fieldPtr;
                    if (position >= field.size())
                        throw std::out_of_range("Position out of range");
                    return *(field.begin() + position);
                }

                if (object.type() == typeid(std::reference_wrapper<const TObject>))
                {
                    const TField& field = std::any_cast<std::reference_wrapper<const TObject>>(std::move(object)).get().*_fieldPtr;
                    if (position >= field.size())
                        throw std::out_of_range("Position out of range");
                    return *(field.begin() + position);
                }

                throw std::invalid_argument("Invalid object type");
            }

            [[nodiscard]]
            value_type Value(const std::shared_ptr<void>& object, const std::size_t position) const
            {
                const TField& field = std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
                if (position >= field.size())
                    throw std::out_of_range("Position out of range");
                return *(field.begin() + position);
            }

            [[nodiscard]]
            value_type& Ref(std::any&& object, const std::size_t position) const
            {
                if (object.type() == typeid(TObject*))
                {
                    TObject* ptr = std::any_cast<TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");

                    TField& field = ptr->*_fieldPtr;
                    if (position >= field.size())
                        throw std::out_of_range("Position out of range");
                    return *(field.begin() + position);
                }

                if (object.type() == typeid(std::reference_wrapper<TObject>))
                {
                    TField& field = std::any_cast<std::reference_wrapper<TObject>>(std::move(object)).get().*_fieldPtr;
                    if (position >= field.size())
                        throw std::out_of_range("Position out of range");
                    return *(field.begin() + position);
                }

                throw std::invalid_argument("Invalid object type");
            }

            // ReSharper disable once CppParameterMayBeConstPtrOrRef
            [[nodiscard]]
            value_type& Ref(std::shared_ptr<void>& object, const std::size_t position) const
            {
                TField& field = std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
                if (position >= field.size())
                    throw std::out_of_range("Position out of range");
                return *(field.begin() + position);
            }

            [[nodiscard]]
            const value_type& ConstRef(std::any&& object, const std::size_t position) const
            {
                if (object.type() == typeid(const TObject*))
                {
                    const TObject* ptr = std::any_cast<const TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");

                    const TField& field = ptr->*_fieldPtr;
                    if (position >= field.size())
                        throw std::out_of_range("Position out of range");
                    return *(field.begin() + position);
                }

                if (object.type() == typeid(std::reference_wrapper<const TObject>))
                {
                    const TField& field = std::any_cast<std::reference_wrapper<const TObject>>(std::move(object)).get().*_fieldPtr;
                    if (position >= field.size())
                        throw std::out_of_range("Position out of range");
                    return *(field.begin() + position);
                }

                throw std::invalid_argument("Invalid object type");
            }

            [[nodiscard]]
            const value_type& ConstRef(const std::shared_ptr<void>& object, const std::size_t position) const
            {
                const TField& field = std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
                if (position >= field.size())
                    throw std::out_of_range("Position out of range");
                return *(field.begin() + position);
            }

            [[nodiscard]]
            value_type* Ptr(std::any&& object, const std::size_t position) const
            {
                if (object.type() == typeid(TObject*))
                {
                    TObject* ptr = std::any_cast<TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");

                    TField& field = ptr->*_fieldPtr;
                    if (position >= field.size())
                        throw std::out_of_range("Position out of range");
                    return &(*(field.begin() + position));
                }

                if (object.type() == typeid(std::reference_wrapper<TObject>))
                {
                    TField& field = std::any_cast<std::reference_wrapper<TObject>>(std::move(object)).get().*_fieldPtr;
                    if (position >= field.size())
                        throw std::out_of_range("Position out of range");
                    return &(*(field.begin() + position));
                }

                throw std::invalid_argument("Invalid object type");
            }

            // ReSharper disable once CppParameterMayBeConstPtrOrRef
            [[nodiscard]]
            value_type* Ptr(std::shared_ptr<void>& object, const std::size_t position) const
            {
                TField& field = std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
                if (position >= field.size())
                    throw std::out_of_range("Position out of range");
                return &(*(field.begin() + position));
            }

            [[nodiscard]]
            const value_type* ConstPtr(std::any&& object, const std::size_t position) const
            {
                if (object.type() == typeid(const TObject*))
                {
                    const TObject* ptr = std::any_cast<const TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");

                    const TField& field = ptr->*_fieldPtr;
                    if (position >= field.size())
                        throw std::out_of_range("Position out of range");
                    return &(*(field.begin() + position));
                }

                if (object.type() == typeid(std::reference_wrapper<const TObject>))
                {
                    const TField& field = std::any_cast<std::reference_wrapper<const TObject>>(std::move(object)).get().*_fieldPtr;
                    if (position >= field.size())
                        throw std::out_of_range("Position out of range");
                    return &(*(field.begin() + position));
                }

                throw std::invalid_argument("Invalid object type");
            }

            [[nodiscard]]
            const value_type& ConstPtr(const std::shared_ptr<void>& object, const std::size_t position) const
            {
                const TField& field = std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
                if (position >= field.size())
                    throw std::out_of_range("Position out of range");
                return &(*(field.begin() + position));
            }

            void Insert(std::any&& object, std::any&& element, const std::size_t position) const
            {
                if (object.type() == typeid(TObject*))
                {
                    TObject* ptr = std::any_cast<TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");

                    TField& field = ptr->*_fieldPtr;
                    field.insert(field->begin() + position, std::any_cast<value_type>(std::move(element)));
                }

                if (object.type() == typeid(std::reference_wrapper<TObject>))
                {
                    TField& field = std::any_cast<std::reference_wrapper<TObject>>(std::move(object)).get().*_fieldPtr;
                    field.insert(field->begin() + position, std::any_cast<value_type>(std::move(element)));
                }

                throw std::invalid_argument("Invalid object type");
            }

            void InsertFront(std::any&& object, std::any&& element) const
            {
                if (object.type() == typeid(TObject*))
                {
                    TObject* ptr = std::any_cast<TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");

                    TField& field = ptr->*_fieldPtr;
                    field->insert(field->begin(), std::any_cast<value_type>(std::move(element)));
                }

                if (object.type() == typeid(std::reference_wrapper<TObject>))
                {
                    TField& field = std::any_cast<std::reference_wrapper<TObject>>(std::move(object)).get().*_fieldPtr;
                    field->insert(field->begin(), std::any_cast<value_type>(std::move(element)));
                }

                throw std::invalid_argument("Invalid object type");
            }

            void InsertBack(std::any&& object, std::any&& element) const
            {
                if (object.type() == typeid(TObject*))
                {
                    TObject* ptr = std::any_cast<TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");

                    TField& field = ptr->*_fieldPtr;
                    field->insert(field->end(), std::any_cast<value_type>(std::move(element)));
                }

                if (object.type() == typeid(std::reference_wrapper<TObject>))
                {
                    TField& field = std::any_cast<std::reference_wrapper<TObject>>(std::move(object)).get().*_fieldPtr;
                    field->insert(field->end(), std::any_cast<value_type>(std::move(element)));
                }

                throw std::invalid_argument("Invalid object type");
            }

            // ReSharper disable once CppParameterMayBeConstPtrOrRef
            void Insert(std::shared_ptr<void>& object, std::any&& element, const std::size_t position) const noexcept
            {
                TField& field = std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
                field.insert(field->begin() + position, std::any_cast<value_type>(std::move(element)));
            }

            // ReSharper disable once CppParameterMayBeConstPtrOrRef
            void InsertFront(std::shared_ptr<void>& object, std::any&& element) const noexcept
            {
                TField& field = std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
                field->insert(field->begin(), std::any_cast<value_type>(std::move(element)));
            }

            // ReSharper disable once CppParameterMayBeConstPtrOrRef
            void InsertBack(std::shared_ptr<void>& object, std::any&& element) const noexcept
            {
                TField& field = std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
                field->insert(field->end(), std::any_cast<value_type>(std::move(element)));
            }

            [[nodiscard]]
            std::size_t Size(std::any&& object) const
            {
                if (object.type() == typeid(const TObject*))
                {
                    const TObject* ptr = std::any_cast<const TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");

                    return (ptr->*_fieldPtr).size();
                }

                if (object.type() == typeid(std::reference_wrapper<const TObject>))
                    return (std::any_cast<std::reference_wrapper<const TObject>>(std::move(object)).get().*_fieldPtr).size();

                throw std::invalid_argument("Invalid object type");
            }

            void Clear(std::any&& object) const
            {
                (std::any_cast<TObject*>(std::move(object))->*_fieldPtr).clear();
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
        template<typename THelper>
        CollectionFieldInfo(std::string&& fieldName, std::type_index typeIndex, THelper&& fieldHelper) noexcept :
            FieldInfo(std::move(fieldName), std::move(typeIndex), std::forward<THelper>(fieldHelper)),
            _valueTypeIndex(typeid(typename THelper::value_type)),

            _elementValue([](const std::any& helper, std::any&& object, const std::size_t position)
                { return std::any(std::any_cast<const THelper&>(helper).Value(std::move(object), position)); }),
            _elementRef([](const std::any& helper, std::any&& object, const std::size_t position)
                { return std::any(std::any_cast<const THelper&>(helper).Get(std::move(object), position)); }),
            _elementConstRef([](const std::any& helper, std::any&& object, const std::size_t position)
                { return std::any(std::any_cast<const THelper&>(helper).Read(std::move(object), position)); }),

            _elementValueGetterFromPtr([](const std::any& helper, const std::shared_ptr<void>& object, const std::size_t position)
                { return std::any(std::any_cast<const THelper&>(helper).Value(object, position)); }),
            _elementGetterFromPtr([](const std::any& helper, std::shared_ptr<void>& object, const std::size_t position)
                { return std::any(std::any_cast<const THelper&>(helper).Get(object, position)); }),
            _elementReaderFromPtr([](const std::any& helper, const std::shared_ptr<void>& object, const std::size_t position)
                { return std::any(std::any_cast<const THelper&>(helper).Read(object, position)); }),

            _inserter([](const std::any& helper, std::any&& object, std::any&& element, const std::size_t position)
                { std::any_cast<const THelper&>(helper).Insert(std::move(object), std::move(element), position); }),
            _frontInserter([](const std::any& helper, std::any&& object, std::any&& element)
                { std::any_cast<const THelper&>(helper).InsertFront(std::move(object), std::move(element)); }),
            _backInserter([](const std::any& helper, std::any&& object, std::any&& element)
                { std::any_cast<const THelper&>(helper).InsertBack(std::move(object), std::move(element)); }),

            //_inserterFromPtr([](const std::any& helper, std::shared_ptr<void>& object, std::any&& element, const std::size_t position)
            //    { std::any_cast<const THelper&>(helper).Insert(object, std::move(element), position); }),
            //_frontInserterFromPtr([](const std::any& helper, std::shared_ptr<void>& object, std::any&& element)
            //    { std::any_cast<const THelper&>(helper).InsertFront(object, std::move(element)); }),
            //_backInserterFromPtr([](const std::any& helper, std::shared_ptr<void>& object, std::any&& element)
            //    { std::any_cast<const THelper&>(helper).InsertBack(object, std::move(element)); }),

            _sizeGetter([](const std::any& helper, std::any&& object)
                { return std::any_cast<const THelper&>(helper).Size(std::move(object)); }),
            _clear([](const std::any& helper, std::any&& object)
                { return std::any_cast<const THelper&>(helper).Clear(std::move(object)); }) {}

            //_sizeGetterFromPtr([](const std::any& helper, const std::shared_ptr<void>& object)
            //    { return std::any_cast<const THelper&>(helper).Size(object); }),
            //_clearFromPtr([](const std::any& helper, std::shared_ptr<void>& object)
            //    { return std::any_cast<const THelper&>(helper).Clear(object); }) {}

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
            return std::any_cast<TValueType>(_elementValue(_fieldHelper, std::move(object), position));
        }

        /// @brief
        /// @param object
        /// @param position
        /// @return
        [[nodiscard]]
        std::any GetElementValue(std::any object, std::size_t position) const;

        /// @brief
        /// @tparam TObject
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType, typename TObject>
        TValueType GetElementValue(const TObject* object, const std::size_t position) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<TValueType>(_elementValueGetter(_fieldHelper, object, position));
        }

        /// @brief
        /// @tparam TObject
        /// @param object
        /// @param position
        /// @return
        template<typename TObject>
        std::any GetElementValue(const TObject* object, const std::size_t position) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _elementValueGetter(_fieldHelper, object, position);
        }

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
            return std::any_cast<TValueType>(_elementValueGetterFromPtr(_fieldHelper, object, position));
        }

        /// @brief
        /// @param object
        /// @param position
        /// @return
        [[nodiscard]]
        std::any GetElementValue(const std::shared_ptr<void>& object, std::size_t position) const;

        /// @brief
        /// @tparam TObject
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType, typename TObject>
        TValueType GetElementValue(const std::shared_ptr<TObject>& object, const std::size_t position) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<TValueType>(_elementValueGetterFromPtr(_fieldHelper, object, position));
        }

        /// @brief
        /// @tparam TObject
        /// @param object
        /// @param position
        /// @return
        template<typename TObject>
        std::any GetElementValue(const std::shared_ptr<TObject>& object, const std::size_t position) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _elementValueGetterFromPtr(_fieldHelper, object, position);
        }

        /// @brief
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType>
        TValueType* GetElement(std::any object, const std::size_t position) const
        {
            return std::any_cast<TValueType*>(_elementRef(_fieldHelper, std::move(object), position));
        }

        /// @brief
        /// @param object
        /// @param position
        /// @return
        [[nodiscard]]
        std::any GetElement(std::any object, std::size_t position) const;

        /// @brief
        /// @tparam TObject
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType, typename TObject>
        TValueType* GetElement(TObject* object, const std::size_t position) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<TValueType*>(_elementGetter(_fieldHelper, object, position));
        }

        /// @brief
        /// @tparam TObject
        /// @param object
        /// @param position
        /// @return
        template<typename TObject>
        std::any GetElement(TObject* object, const std::size_t position) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _elementGetter(_fieldHelper, object, position);
        }

        /// @brief
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType>
        TValueType* GetElement(std::shared_ptr<void> object, const std::size_t position) const
        {
            return std::any_cast<TValueType*>(_elementGetterFromPtr(_fieldHelper, object, position));
        }

        /// @brief
        /// @param object
        /// @param position
        /// @return
        [[nodiscard]]
        std::any GetElement(std::shared_ptr<void> object, std::size_t position) const;

        /// @brief
        /// @tparam TObject
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType, typename TObject>
        TValueType* GetElement(std::shared_ptr<TObject>& object, const std::size_t position) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<TValueType*>(_elementGetterFromPtr(_fieldHelper, object, position));
        }

        /// @brief
        /// @tparam TObject
        /// @param object
        /// @param position
        /// @return
        template<typename TObject>
        std::any GetElement(std::shared_ptr<TObject>& object, const std::size_t position) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _elementGetterFromPtr(_fieldHelper, object, position);
        }

        /// @brief
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType>
        const TValueType* ReadElement(std::any object, const std::size_t position) const
        {
            return std::any_cast<const TValueType*>(_elementConstRef(_fieldHelper, std::move(object), position));
        }

        /// @brief
        /// @param object
        /// @param position
        /// @return
        [[nodiscard]]
        std::any ReadElement(std::any object, std::size_t position) const;

        /// @brief
        /// @tparam TObject
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType, typename TObject>
        const TValueType* ReadElement(const TObject* object, const std::size_t position) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<const TValueType*>(_elementReader(_fieldHelper, object, position));
        }

        /// @brief
        /// @tparam TObject
        /// @param object
        /// @param position
        /// @return
        template<typename TObject>
        std::any ReadElement(const TObject* object, const std::size_t position) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _elementReader(_fieldHelper, object, position);
        }

        /// @brief
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType>
        const TValueType* ReadElement(const std::shared_ptr<void>& object, const std::size_t position) const
        {
            return std::any_cast<const TValueType*>(_elementReaderFromPtr(_fieldHelper, object, position));
        }

        /// @brief
        /// @param object
        /// @param position
        /// @return
        [[nodiscard]]
        std::any ReadElement(const std::shared_ptr<void>& object, std::size_t position) const;

        /// @brief
        /// @tparam TObject
        /// @tparam TValueType
        /// @param object
        /// @param position
        /// @return
        template<typename TValueType, typename TObject>
        const TValueType* ReadElement(const std::shared_ptr<TObject>& object, const std::size_t position) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<const TValueType*>(_elementReaderFromPtr(_fieldHelper, object, position));
        }

        /// @brief
        /// @tparam TObject
        /// @param object
        /// @param position
        /// @return
        template<typename TObject>
        std::any ReadElement(const std::shared_ptr<TObject>& object, const std::size_t position) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _elementReaderFromPtr(_fieldHelper, object, position);
        }

        /// @brief
        /// @tparam TElement
        /// @param object
        /// @param element
        /// @param position
        template<typename TElement>
        void Insert(std::any object, TElement&& element, const std::size_t position) const
        {
            _inserter(_fieldHelper, std::move(object), std::forward<TElement>(element), position);
        }

        /// @brief
        /// @tparam TObject
        /// @tparam TElement
        /// @param object
        /// @param element
        /// @param position
        template<typename TObject, typename TElement>
        void Insert(TObject* object, TElement&& element, const std::size_t position) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            _inserter(_fieldHelper, object, std::forward<TElement>(element), position);
        }

        /// @brief
        /// @tparam TElement
        /// @param object
        /// @param element
        template<typename TElement>
        void InsertFront(std::any object, TElement&& element) const
        {
            _frontInserter(_fieldHelper, std::move(object), std::forward<TElement>(element));
        }

        /// @brief
        /// @tparam TObject
        /// @tparam TElement
        /// @param object
        /// @param element
        template<typename TObject, typename TElement>
        void InsertFront(TObject* object, TElement&& element) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            _frontInserter(_fieldHelper, object, std::forward<TElement>(element));
        }

        /// @brief
        /// @tparam TElement
        /// @param object
        /// @param element
        template<typename TElement>
        void InsertBack(std::any object, TElement&& element) const
        {
            _backInserter(_fieldHelper, std::move(object), std::forward<TElement>(element));
        }

        /// @brief
        /// @tparam TObject
        /// @tparam TElement
        /// @param object
        /// @param element
        template<typename TObject, typename TElement>
        void InsertBack(TObject* object, TElement&& element) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            _backInserter(_fieldHelper, object, std::forward<TElement>(element));
        }
/*
        /// @brief
        /// @tparam TElement
        /// @param object
        /// @param element
        /// @param position
        template<typename TElement>
        void Insert(std::any object, TElement&& element, const std::size_t position) const
        {
            _inserter(_fieldHelper, std::move(object), std::forward<TElement>(element), position);
        }

        /// @brief
        /// @tparam TObject
        /// @tparam TElement
        /// @param object
        /// @param element
        /// @param position
        template<typename TObject, typename TElement>
        void Insert(TObject* object, TElement&& element, const std::size_t position) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            _inserter(_fieldHelper, object, std::forward<TElement>(element), position);
        }

        /// @brief
        /// @tparam TElement
        /// @param object
        /// @param element
        template<typename TElement>
        void InsertFront(std::any object, TElement&& element) const
        {
            _frontInserter(_fieldHelper, std::move(object), std::forward<TElement>(element));
        }

        /// @brief
        /// @tparam TObject
        /// @tparam TElement
        /// @param object
        /// @param element
        template<typename TObject, typename TElement>
        void InsertFront(TObject* object, TElement&& element) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            _frontInserter(_fieldHelper, object, std::forward<TElement>(element));
        }

        /// @brief
        /// @tparam TElement
        /// @param object
        /// @param element
        template<typename TElement>
        void InsertBack(std::any object, TElement&& element) const
        {
            _backInserter(_fieldHelper, std::move(object), std::forward<TElement>(element));
        }

        /// @brief
        /// @tparam TObject
        /// @tparam TElement
        /// @param object
        /// @param element
        template<typename TObject, typename TElement>
        void InsertBack(TObject* object, TElement&& element) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            _backInserter(_fieldHelper, object, std::forward<TElement>(element));
        }
*/
        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::size_t Size(std::any object) const;

        /// @brief
        /// @tparam TObject
        /// @param object
        /// @return
        template<typename TObject>
        std::size_t Size(const TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _sizeGetter(_fieldHelper, object);
        }

        [[nodiscard]]
        std::size_t Size(const std::shared_ptr<void>& object) const;

        void Clear(std::any object) const;

        template<typename TObject>
        void Clear(TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            _clear(_fieldHelper, object);
        }

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
        static std::shared_ptr<MemberInfo> Create(std::string&& name, TField TObject::*fieldPtr) noexcept
        {
            return std::make_shared<CollectionFieldInfo>(std::move(name), typeid(TField), CollectionFieldInfo::Helper(fieldPtr));
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