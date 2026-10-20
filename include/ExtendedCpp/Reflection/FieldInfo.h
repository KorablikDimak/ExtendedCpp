#ifndef REFLECTION_FIELD_INFO_H
#define REFLECTION_FIELD_INFO_H

#include <any>
#include <format>
#include <memory>
#include <stdexcept>
#include <typeindex>

#include <ExtendedCpp/Reflection/MemberInfo.h>

/// @brief
namespace ExtendedCpp::Reflection
{
    /// @brief
    class FieldInfo : public MemberInfo
    {
    protected:
        const std::type_index _typeIndex;
        const std::any _fieldHelper;
        const bool _isConst;

        std::any (*const _value)(const std::any& helper, std::any&& object);
        std::any (*const _ref)(const std::any& helper, std::any&& object);
        std::any (*const _constRef)(const std::any& helper, std::any&& object);
        std::any (*const _ptr)(const std::any& helper, std::any&& object);
        std::any (*const _constPtr)(const std::any& helper, std::any&& object);
        void (*const _valueSetter)(const std::any& helper, std::any&& object, std::any&& value);

        std::any (*const _valueFromPtr)(const std::any& helper, const std::shared_ptr<void>& object);
        std::any (*const _refFromPtr)(const std::any& helper, std::shared_ptr<void>& object);
        std::any (*const _constRefFromPtr)(const std::any& helper, const std::shared_ptr<void>& object) noexcept;
        std::any (*const _ptrFromPtr)(const std::any& helper, std::shared_ptr<void>& object);
        std::any (*const _constPtrFromPtr)(const std::any& helper, const std::shared_ptr<void>& object) noexcept;
        void (*const _valueSetterFromPtr)(const std::any& helper, std::shared_ptr<void>& object, std::any&& value);

        template<typename TObject, typename TField>
        struct Helper final
        {
        private:
            TField TObject::*const _fieldPtr;

        public:
            using ObjectType = TObject;
            using FieldType = TField;
            static constexpr bool IS_CONST_FIELD = std::is_const_v<TField>;

            explicit Helper(TField TObject::*const fieldPtr) noexcept :
                _fieldPtr(fieldPtr) {}

            [[nodiscard]]
            TField Value(std::any&& object) const
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

            [[nodiscard]]
            TField Value(const std::shared_ptr<void>& object) const noexcept
            {
                return std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
            }

            [[nodiscard]]
            std::reference_wrapper<TField> Ref(std::any&& object) const
            {
                if (object.type() == typeid(TObject*))
                {
                    TObject* ptr = std::any_cast<TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");
                    return std::ref(ptr->*_fieldPtr);
                }

                if (object.type() == typeid(std::reference_wrapper<TObject>))
                    return std::ref(std::any_cast<std::reference_wrapper<TObject>>(std::move(object)).get().*_fieldPtr);

                throw std::invalid_argument(std::format("Invalid object type which is {}, but expected {} or {}",
                    object.type().name(), typeid(TObject*).name(), typeid(std::reference_wrapper<TObject>).name()));
            }

            // ReSharper disable once CppParameterMayBeConstPtrOrRef
            [[nodiscard]]
            std::reference_wrapper<TField> Ref(std::shared_ptr<void>& object) const noexcept
            {
                return std::ref(std::static_pointer_cast<TObject>(object).get()->*_fieldPtr);
            }

            [[nodiscard]]
            std::reference_wrapper<const TField> ConstRef(std::any&& object) const
            {
                if (object.type() == typeid(TObject*))
                {
                    const TObject* ptr = std::any_cast<TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");
                    return std::cref(ptr->*_fieldPtr);
                }

                if (object.type() == typeid(const TObject*))
                {
                    const TObject* ptr = std::any_cast<const TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");
                    return std::cref(ptr->*_fieldPtr);
                }

                if (object.type() == typeid(std::reference_wrapper<TObject>))
                    return std::cref(std::any_cast<std::reference_wrapper<TObject>>(std::move(object)).get().*_fieldPtr);

                if (object.type() == typeid(std::reference_wrapper<const TObject>))
                    return std::cref(std::any_cast<std::reference_wrapper<const TObject>>(std::move(object)).get().*_fieldPtr);

                throw std::invalid_argument(std::format("Invalid object type which is {}, but expected {}, {}, {} or {}",
                    object.type().name(), typeid(TObject*).name(), typeid(const TObject*).name(),
                    typeid(std::reference_wrapper<TObject>).name(), typeid(std::reference_wrapper<const TObject>).name()));
            }

            [[nodiscard]]
            std::reference_wrapper<const TField> ConstRef(const std::shared_ptr<void>& object) const noexcept
            {
                return std::cref(std::static_pointer_cast<TObject>(object).get()->*_fieldPtr);
            }

            [[nodiscard]]
            TField* Ptr(std::any&& object) const
            {
                if (object.type() == typeid(TObject*))
                {
                    TObject* ptr = std::any_cast<TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");
                    return &(ptr->*_fieldPtr);
                }

                if (object.type() == typeid(std::reference_wrapper<TObject>))
                    return &(std::any_cast<std::reference_wrapper<TObject>>(std::move(object)).get().*_fieldPtr);

                throw std::invalid_argument(std::format("Invalid object type which is {}, but expected {} or {}",
                    object.type().name(), typeid(TObject*).name(), typeid(std::reference_wrapper<TObject>).name()));
            }

            // ReSharper disable once CppParameterMayBeConstPtrOrRef
            [[nodiscard]]
            TField* Ptr(std::shared_ptr<void>& object) const noexcept
            {
                return &(std::static_pointer_cast<TObject>(object).get()->*_fieldPtr);
            }

            [[nodiscard]]
            const TField* ConstPtr(std::any&& object) const
            {
                if (object.type() == typeid(TObject*))
                {
                    const TObject* ptr = std::any_cast<TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");
                    return &(ptr->*_fieldPtr);
                }

                if (object.type() == typeid(const TObject*))
                {
                    const TObject* ptr = std::any_cast<const TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");
                    return &(ptr->*_fieldPtr);
                }

                if (object.type() == typeid(std::reference_wrapper<TObject>))
                    return &(std::any_cast<std::reference_wrapper<TObject>>(std::move(object)).get().*_fieldPtr);

                if (object.type() == typeid(std::reference_wrapper<const TObject>))
                    return &(std::any_cast<std::reference_wrapper<const TObject>>(std::move(object)).get().*_fieldPtr);

                throw std::invalid_argument(std::format("Invalid object type which is {}, but expected {}, {}, {} or {}",
                    object.type().name(), typeid(TObject*).name(), typeid(const TObject*).name(),
                    typeid(std::reference_wrapper<TObject>).name(), typeid(std::reference_wrapper<const TObject>).name()));
            }

            [[nodiscard]]
            const TField* ConstPtr(const std::shared_ptr<void>& object) const noexcept
            {
                return &(std::static_pointer_cast<TObject>(object).get()->*_fieldPtr);
            }

            void SetValue(std::any&& object, std::any&& value) const
            {
                if (object.type() == typeid(TObject*))
                {
                    TObject* ptr = std::any_cast<TObject*>(std::move(object));
                    if (!ptr)
                        throw std::invalid_argument("Object is null");
                    ptr->*_fieldPtr = std::any_cast<TField>(std::move(value));
                    return;
                }

                if (object.type() == typeid(std::reference_wrapper<TObject>))
                {
                    std::any_cast<std::reference_wrapper<TObject>>(std::move(object)).get().*_fieldPtr =
                        std::any_cast<TField>(std::move(value));
                    return;
                }

                throw std::invalid_argument(std::format("Invalid object type which is {}, but expected {} or {}",
                    object.type().name(), typeid(TObject*).name(), typeid(std::reference_wrapper<TObject>).name()));
            }

            // ReSharper disable once CppParameterMayBeConstPtrOrRef
            void SetValue(std::shared_ptr<void>& object, std::any&& value) const
            {
                std::static_pointer_cast<TObject>(object).get()->*_fieldPtr = std::move(std::any_cast<TField>(std::move(value)));
            }
        };

    public:
        /// @brief
        /// @tparam THelper
        /// @param fieldName
        /// @param typeIndex
        /// @param fieldHelper
        template<typename THelper>
        FieldInfo(std::string&& fieldName, const std::type_index typeIndex, THelper&& fieldHelper) noexcept :
            MemberInfo(std::move(fieldName)),
            _typeIndex(typeIndex),
            _fieldHelper(std::forward<THelper>(fieldHelper)),
            _isConst(std::remove_cvref_t<THelper>::IS_CONST_FIELD),

            _value([](const std::any& helper, std::any&& object) -> std::any
                {
                    if constexpr (!std::copy_constructible<typename std::remove_cvref_t<THelper>::FieldType>)
                        throw std::domain_error(std::format("You try get value, but field {} of {} is not copy constructible.",
                            typeid(typename std::remove_cvref_t<THelper>::FieldType).name(), typeid(typename std::remove_cvref_t<THelper>::ObjectType).name()));
                    else
                        return std::any(std::any_cast<THelper>(helper).Value(std::move(object)));
                }),
            _ref([](const std::any& helper, std::any&& object) -> std::any
                {
                    if constexpr (std::remove_cvref_t<THelper>::IS_CONST_FIELD)
                        throw std::domain_error(std::format("You try get non const reference, but field {} of {} is const.",
                            typeid(typename std::remove_cvref_t<THelper>::FieldType).name(), typeid(typename std::remove_cvref_t<THelper>::ObjectType).name()));
                    else
                        return std::any(std::any_cast<THelper>(helper).Ref(std::move(object)));
                }),
            _constRef([](const std::any& helper, std::any&& object)
                { return std::any(std::any_cast<THelper>(helper).ConstRef(std::move(object))); }),
            _ptr([](const std::any& helper, std::any&& object) -> std::any
                {
                    if constexpr (std::remove_cvref_t<THelper>::IS_CONST_FIELD)
                        throw std::domain_error(std::format("You try get non const pointer, but field {} of {} is const.",
                            typeid(typename std::remove_cvref_t<THelper>::FieldType).name(), typeid(typename std::remove_cvref_t<THelper>::ObjectType).name()));
                    else
                        return std::any(std::any_cast<THelper>(helper).Ptr(std::move(object)));
                }),
            _constPtr([](const std::any& helper, std::any&& object)
                { return std::any(std::any_cast<THelper>(helper).ConstPtr(std::move(object))); }),
            _valueSetter([](const std::any& helper, std::any&& object, std::any&& value)
                {
                    if constexpr (std::remove_cvref_t<THelper>::IS_CONST_FIELD)
                        throw std::domain_error(std::format("You try set value, but field {} of {} is const.",
                            typeid(typename std::remove_cvref_t<THelper>::FieldType).name(), typeid(typename std::remove_cvref_t<THelper>::ObjectType).name()));
                    else
                        std::any_cast<THelper>(helper).SetValue(std::move(object), std::move(value));
                }),

            _valueFromPtr([](const std::any& helper, const std::shared_ptr<void>& object) -> std::any
                {
                    if constexpr (!std::copy_constructible<typename std::remove_cvref_t<THelper>::FieldType>)
                        throw std::domain_error(std::format("You try get value, but field {} of {} is not copy constructible.",
                            typeid(typename std::remove_cvref_t<THelper>::FieldType).name(), typeid(typename std::remove_cvref_t<THelper>::ObjectType).name()));
                    else
                        return std::any(std::any_cast<THelper>(helper).Value(object));
                }),
            _refFromPtr([](const std::any& helper, std::shared_ptr<void>& object) -> std::any
                {
                    if constexpr (std::remove_cvref_t<THelper>::IS_CONST_FIELD)
                        throw std::domain_error(std::format("You try get non const reference, but field {} of {} is const.",
                            typeid(typename std::remove_cvref_t<THelper>::FieldType).name(), typeid(typename std::remove_cvref_t<THelper>::ObjectType).name()));
                    else
                        return std::any(std::any_cast<THelper>(helper).Ref(object));
                }),
            _constRefFromPtr([](const std::any& helper, const std::shared_ptr<void>& object) noexcept
                { return std::any(std::any_cast<THelper>(helper).ConstRef(object)); }),
            _ptrFromPtr([](const std::any& helper, std::shared_ptr<void>& object) -> std::any
                {
                    if constexpr (std::remove_cvref_t<THelper>::IS_CONST_FIELD)
                        throw std::domain_error(std::format("You try get non const pointer, but field {} of {} is const.",
                            typeid(typename std::remove_cvref_t<THelper>::FieldType).name(), typeid(typename std::remove_cvref_t<THelper>::ObjectType).name()));
                    else
                        return std::any(std::any_cast<THelper>(helper).Ptr(object));
                }),
            _constPtrFromPtr([](const std::any& helper, const std::shared_ptr<void>& object) noexcept
                { return std::any(std::any_cast<THelper>(helper).ConstPtr(object)); }),
            _valueSetterFromPtr([](const std::any& helper, std::shared_ptr<void>& object, std::any&& value)
                {
                    if constexpr (std::remove_cvref_t<THelper>::IS_CONST_FIELD)
                        throw std::domain_error(std::format("You try set value, but field {} of {} is const.",
                            typeid(typename std::remove_cvref_t<THelper>::FieldType).name(), typeid(typename std::remove_cvref_t<THelper>::ObjectType).name()));
                    else
                        std::any_cast<THelper>(helper).SetValue(object, std::move(value));
                }) {}

        /// @brief
        ~FieldInfo() noexcept override = default;

        /// @brief
        /// @tparam TField
        /// @param object
        /// @return
        template<std::copy_constructible TField>
        [[nodiscard]]
        TField GetValue(std::any object) const
        {
            return std::any_cast<TField>(_value(_fieldHelper, std::move(object)));
        }

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::any GetValue(std::any object) const;

        /// @brief
        /// @tparam TField
        /// @param object
        /// @return
        template<std::copy_constructible TField>
        [[nodiscard]]
        TField GetValue(const std::shared_ptr<void>& object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<TField>(_valueFromPtr(_fieldHelper, object));
        }

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::any GetValue(const std::shared_ptr<void>& object) const;

        /// @brief
        /// @tparam TField
        /// @param object
        /// @return
        template<typename TField>
        [[nodiscard]]
        TField& GetRef(std::any object) const
        {
            return std::any_cast<std::reference_wrapper<TField>>(_ref(_fieldHelper, std::move(object))).get();
        }

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::any GetRef(std::any object) const;

        /// @brief
        /// @tparam TField
        /// @param object
        /// @return
        template<typename TField>
        [[nodiscard]]
        TField& GetRef(std::shared_ptr<void>& object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<std::reference_wrapper<TField>>(_refFromPtr(_fieldHelper, object)).get();
        }

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::any GetRef(std::shared_ptr<void>& object) const;

        /// @brief
        /// @tparam TField
        /// @param object
        /// @return
        template<typename TField>
        [[nodiscard]]
        const TField& GetConstRef(std::any object) const
        {
            return std::any_cast<std::reference_wrapper<const TField>>(_constRef(_fieldHelper, std::move(object))).get();
        }

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::any GetConstRef(std::any object) const;

        /// @brief
        /// @tparam TField
        /// @param object
        /// @return
        template<typename TField>
        [[nodiscard]]
        const TField& GetConstRef(const std::shared_ptr<void>& object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<std::reference_wrapper<const TField>>(_constRefFromPtr(_fieldHelper, object)).get();
        }

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::any GetConstRef(const std::shared_ptr<void>& object) const;

        /// @brief
        /// @tparam TField
        /// @param object
        /// @return
        template<typename TField>
        [[nodiscard]]
        TField* GetPtr(std::any object) const
        {
            return std::any_cast<TField*>(_ptr(_fieldHelper, std::move(object)));
        }

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::any GetPtr(std::any object) const;

        /// @brief
        /// @tparam TField
        /// @param object
        /// @return
        template<typename TField>
        [[nodiscard]]
        TField* GetPtr(std::shared_ptr<void>& object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<TField*>(_ptrFromPtr(_fieldHelper, object));
        }

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::any GetPtr(std::shared_ptr<void>& object) const;

        /// @brief
        /// @tparam TField
        /// @param object
        /// @return
        template<typename TField>
        [[nodiscard]]
        const TField* GetConstPtr(std::any object) const
        {
            return std::any_cast<const TField*>(_constPtr(_fieldHelper, std::move(object)));
        }

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::any GetConstPtr(std::any object) const;

        /// @brief
        /// @tparam TField
        /// @param object
        /// @return
        template<typename TField>
        [[nodiscard]]
        const TField* GetConstPtr(const std::shared_ptr<void>& object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<const TField*>(_constPtrFromPtr(_fieldHelper, object));
        }

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::any GetConstPtr(const std::shared_ptr<void>& object) const;

        /// @brief
        /// @param object
        /// @param value
        void SetValue(std::any object, std::any value) const;

        /// @brief
        /// @param object
        /// @param value
        void SetValue(std::shared_ptr<void>& object, std::any value) const;

        /// @brief
        /// @return
        [[nodiscard]]
        std::type_index TypeIndex() const noexcept;

        /// @brief
        /// @return
        [[nodiscard]]
        MemberType GetMemberType() const noexcept override;

        /// @brief
        /// @return
        [[nodiscard]]
        virtual bool IsCollection() const noexcept;

        /// @brief
        /// @return
        [[nodiscard]]
        bool IsConst() const noexcept;

        /// @brief
        /// @tparam TObject
        /// @tparam TField
        /// @param name
        /// @param fieldPtr
        /// @return
        template<typename TObject, typename TField>
        [[nodiscard]]
        static std::shared_ptr<MemberInfo> Create(std::string&& name, TField TObject::*const fieldPtr) noexcept
        {
            return std::make_shared<FieldInfo>(std::move(name), typeid(TField), Helper(fieldPtr));
        }
    };
}

#endif