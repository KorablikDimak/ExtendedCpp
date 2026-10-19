#ifndef REFLECTION_FIELD_INFO_H
#define REFLECTION_FIELD_INFO_H

#include <any>
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
        std::type_index _typeIndex;
        std::any _fieldHelper;

        std::any (*_fieldValueGetter)(const std::any& helper, std::any&& object);
        std::any (*_fieldGetter)(const std::any& helper, std::any&& object);
        std::any (*_fieldReader)(const std::any& helper, std::any&& object);
        void (*_fieldValueSetter)(const std::any& helper, std::any&& object, std::any&& value);

        std::any (*_fieldValueGetterToPtr)(const std::any& helper, const std::shared_ptr<void>& object);
        std::any (*_fieldGetterToPtr)(const std::any& helper, std::shared_ptr<void>& object);
        std::any (*_fieldReaderToPtr)(const std::any& helper, const std::shared_ptr<void>& object);
        void (*_fieldValueSetterToPtr)(const std::any& helper, std::shared_ptr<void>& object, std::any&& value);

    public:
        template<typename TObject, typename TField>
        struct Helper final
        {
        private:
            TField TObject::*const _fieldPtr;

        public:
            explicit Helper(TField TObject::*const fieldPtr) noexcept :
                _fieldPtr(fieldPtr) {}

            [[nodiscard]]
            TField GetValue(std::any&& object) const
            {
                return std::any_cast<const TObject*>(std::move(object))->*_fieldPtr;
            }

            [[nodiscard]]
            TField GetValue(const std::shared_ptr<void>& object) const noexcept
            {
                return std::static_pointer_cast<TObject>(object).get()->*_fieldPtr;
            }

            [[nodiscard]]
            TField* GetField(std::any&& object) const
            {
                return &(std::any_cast<TObject*>(std::move(object))->*_fieldPtr);
            }

            // ReSharper disable once CppParameterMayBeConstPtrOrRef
            [[nodiscard]]
            TField* GetField(std::shared_ptr<void>& object) const noexcept
            {
                return &(std::static_pointer_cast<TObject>(object).get()->*_fieldPtr);
            }

            [[nodiscard]]
            const TField* ReadField(std::any&& object) const
            {
                return &(std::any_cast<const TObject*>(std::move(object))->*_fieldPtr);
            }

            [[nodiscard]]
            const TField* ReadField(const std::shared_ptr<void>& object) const noexcept
            {
                return &(std::static_pointer_cast<TObject>(object).get()->*_fieldPtr);
            }

            void SetValue(std::any&& object, std::any&& value) const
            {
                std::any_cast<TObject*>(std::move(object))->*_fieldPtr =
                    std::move(std::any_cast<TField>(std::move(value)));
            }

            // ReSharper disable once CppParameterMayBeConstPtrOrRef
            void SetValue(std::shared_ptr<void>& object, std::any&& value) const
            {
                std::static_pointer_cast<TObject>(object).get()->*_fieldPtr =
                    std::move(std::any_cast<TField>(std::move(value)));
            }
        };

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

            _fieldValueGetter([](const std::any& helper, std::any&& object)
                { return std::any(std::any_cast<const THelper&>(helper).GetValue(std::move(object))); }),
            _fieldGetter([](const std::any& helper, std::any&& object)
                { return std::any(std::any_cast<const THelper&>(helper).GetField(std::move(object))); }),
            _fieldReader([](const std::any& helper, std::any&& object)
                { return std::any(std::any_cast<const THelper&>(helper).ReadField(std::move(object))); }),
            _fieldValueSetter([](const std::any& helper, std::any&& object, std::any&& value)
                { std::any_cast<const THelper&>(helper).SetValue(std::move(object), std::move(value)); }),

            _fieldValueGetterToPtr([](const std::any& helper, const std::shared_ptr<void>& object)
                { return std::any(std::any_cast<const THelper&>(helper).GetValue(object)); }),
            _fieldGetterToPtr([](const std::any& helper, std::shared_ptr<void>& object)
                { return std::any(std::any_cast<const THelper&>(helper).GetField(object)); }),
            _fieldReaderToPtr([](const std::any& helper, const std::shared_ptr<void>& object)
                { return std::any(std::any_cast<const THelper&>(helper).ReadField(object)); }),
            _fieldValueSetterToPtr([](const std::any& helper, std::shared_ptr<void>& object, std::any&& value)
                { std::any_cast<const THelper&>(helper).SetValue(object, std::move(value)); }) {}

        /// @brief
        ~FieldInfo() override = default;

        /// @brief
        /// @tparam TField
        /// @param object
        /// @return
        template<typename TField>
        TField GetValue(std::any object) const
        {
            return std::any_cast<TField>(_fieldValueGetter(_fieldHelper, std::move(object)));
        }

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::any GetValue(std::any object) const;

        /// @brief
        /// @tparam TField
        /// @tparam TObject
        /// @param object
        /// @return
        template<typename TField, typename TObject>
        TField GetValue(const TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<TField>(_fieldValueGetter(_fieldHelper, object));
        }

        /// @brief
        /// @tparam TObject
        /// @param object
        /// @return
        template<typename TObject>
        std::any GetValue(const TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _fieldValueGetter(_fieldHelper, object);
        }

        /// @brief
        /// @tparam TField
        /// @param object
        /// @return
        template<typename TField>
        TField GetValue(const std::shared_ptr<void>& object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<TField>(_fieldValueGetterToPtr(_fieldHelper, object));
        }

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::any GetValue(const std::shared_ptr<void>& object) const;

        /// @brief
        /// @tparam TField
        /// @tparam TObject
        /// @param object
        /// @return
        template<typename TField, typename TObject>
        TField GetValue(const std::shared_ptr<TObject>& object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<TField>(_fieldValueGetterToPtr(_fieldHelper, object));
        }

        /// @brief
        /// @tparam TObject
        /// @param object
        /// @return
        template<typename TObject>
        std::any GetValue(const std::shared_ptr<TObject>& object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _fieldValueGetterToPtr(_fieldHelper, object);
        }

        /// @brief
        /// @tparam TField
        /// @param object
        /// @return
        template<typename TField>
        TField* GetField(std::any object) const
        {
            return std::any_cast<TField*>(_fieldGetter(_fieldHelper, std::move(object)));
        }

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::any GetField(std::any object) const;

        /// @brief
        /// @tparam TField
        /// @tparam TObject
        /// @param object
        /// @return
        template<typename TField, typename TObject>
        TField* GetField(TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<TField*>(_fieldGetter(_fieldHelper, object));
        }

        /// @brief
        /// @tparam TObject
        /// @param object
        /// @return
        template<typename TObject>
        std::any GetField(TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _fieldGetter(_fieldHelper, object);
        }

        /// @brief
        /// @tparam TField
        /// @param object
        /// @return
        template<typename TField>
        TField* GetField(std::shared_ptr<void>& object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<TField*>(_fieldGetterToPtr(_fieldHelper, object));
        }

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::any GetField(std::shared_ptr<void>& object) const;

        /// @brief
        /// @tparam TField
        /// @tparam TObject
        /// @param object
        /// @return
        template<typename TField, typename TObject>
        TField* GetField(std::shared_ptr<TObject>& object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<TField*>(_fieldGetterToPtr(_fieldHelper, object));
        }

        /// @brief
        /// @tparam TObject
        /// @param object
        /// @return
        template<typename TObject>
        std::any GetField(std::shared_ptr<TObject>& object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _fieldGetterToPtr(_fieldHelper, object);
        }

        /// @brief
        /// @tparam TField
        /// @param object
        /// @return
        template<typename TField>
        const TField* ReadField(std::any object) const
        {
            return std::any_cast<const TField*>(_fieldReader(_fieldHelper, std::move(object)));
        }

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::any ReadField(std::any object) const;

        /// @brief
        /// @tparam TField
        /// @tparam TObject
        /// @param object
        /// @return
        template<typename TField, typename TObject>
        const TField* ReadField(const TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<const TField*>(_fieldReader(_fieldHelper, object));
        }

        /// @brief
        /// @tparam TObject
        /// @param object
        /// @return
        template<typename TObject>
        std::any ReadField(const TObject* object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _fieldReader(_fieldHelper, object);
        }

        /// @brief
        /// @tparam TField
        /// @param object
        /// @return
        template<typename TField>
        const TField* ReadField(const std::shared_ptr<void>& object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<const TField*>(_fieldReaderToPtr(_fieldHelper, object));
        }

        /// @brief
        /// @param object
        /// @return
        [[nodiscard]]
        std::any ReadField(const std::shared_ptr<void>& object) const;

        /// @brief
        /// @tparam TField
        /// @tparam TObject
        /// @param object
        /// @return
        template<typename TField, typename TObject>
        const TField* ReadField(const std::shared_ptr<TObject>& object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return std::any_cast<const TField*>(_fieldReaderToPtr(_fieldHelper, object));
        }

        /// @brief
        /// @tparam TObject
        /// @param object
        /// @return
        template<typename TObject>
        std::any ReadField(const std::shared_ptr<TObject>& object) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            return _fieldReaderToPtr(_fieldHelper, object);
        }

        template<typename TObject, typename TField>
        void SetValue(TObject* object, TField&& value) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            _fieldValueSetter(_fieldHelper, object, std::forward<TField>(value));
        }

        template<typename TField>
        void SetValue(std::any object, TField&& value) const
        {
            _fieldValueSetter(_fieldHelper, std::move(object), std::forward<TField>(value));
        }

        template<typename TObject, typename TField>
        void SetValue(std::shared_ptr<TObject>& object, TField&& value) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            _fieldValueSetterToPtr(_fieldHelper, object, std::forward<TField>(value));
        }

        template<typename TField>
        void SetValue(std::shared_ptr<void>& object, TField&& value) const
        {
            if (!object)
                throw std::invalid_argument("Object is null");
            _fieldValueSetterToPtr(_fieldHelper, object, std::forward<TField>(value));
        }

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
        virtual bool IsCollection() const noexcept
        {
            return false;
        }
    };

    /// @brief
    /// @tparam TObject
    /// @tparam TField
    /// @param name
    /// @param fieldPtr
    /// @return
    template<typename TObject, typename TField>
    std::shared_ptr<MemberInfo> CreateFieldInfo(std::string&& name, TField TObject::*fieldPtr) noexcept
    {
        return std::make_shared<FieldInfo>(std::move(name), typeid(TField), FieldInfo::Helper(fieldPtr));
    }
}

#endif