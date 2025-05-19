#ifndef Reflection_CollectionFieldInfo_H
#define Reflection_CollectionFieldInfo_H

#include <string>
#include <typeindex>
#include <any>

#include <ExtendedCpp/Reflection/FieldInfo.h>
#include <ExtendedCpp/Reflection/Concepts.h>

/// @brief 
namespace ExtendedCpp::Reflection
{
	/// @brief 
	class CollectionFieldInfo final : public FieldInfo
	{
	private:	
		std::type_index _valueTypeIndex;

		std::any (*_elementValueGetter)(const std::any& helper, std::any&& object, const std::size_t position);
		std::any (*_elementGetter)(const std::any& helper, std::any&& object, const std::size_t position);
		std::any (*_elementReader)(const std::any& helper, std::any&& object, const std::size_t position);
		void* (*_elementGetterPtr)(const std::any& helper, std::any&& object, const std::size_t position);
		const void* (*_elementReaderPtr)(const std::any& helper, std::any&& object, const std::size_t position);

		void (*_inserter)(const std::any& helper, std::any&& object, std::any&& element, const std::size_t position);
		void (*_frontInserter)(const std::any& helper, std::any&& object, std::any&& element);
		void (*_backInserter)(const std::any& helper, std::any&& object, std::any&& element);

		std::size_t (*_sizeGetter)(const std::any& helper, std::any&& object);

	public:
		/// @brief 
		/// @tparam TObject 
		/// @tparam TField 
		template<typename TObject, Concepts::Insertable TField>
		struct Helper final
		{
		private:
			TField TObject::*_fieldPtr;

		public:
			/// @brief 
			using value_type = TField::value_type;

			/// @brief 
			/// @param fieldPtr 
			explicit Helper(TField TObject::* fieldPtr) noexcept :
				_fieldPtr(fieldPtr) {}

			/// @brief 
			/// @param object 
			/// @return 
			[[nodiscard]]
			TField GetValue(std::any&& object) const
			{
				return std::any_cast<const TObject*>(std::move(object))->*_fieldPtr;
			}

			/// @brief 
			/// @param object 
			/// @return 
			[[nodiscard]]
			TField* GetField(std::any&& object) const
			{
				return &(std::any_cast<TObject*>(std::move(object))->*_fieldPtr);
			}

			/// @brief 
			/// @param object 
			/// @return 
			[[nodiscard]]
			const TField* ReadField(std::any&& object) const
			{
				return &(std::any_cast<const TObject*>(std::move(object))->*_fieldPtr);
			}

			/// @brief 
			/// @param object 
			/// @param position 
			/// @return 
			[[nodiscard]]
			value_type Value(std::any&& object, const std::size_t position) const
			{
				const TField* field = &(std::any_cast<const TObject*>(std::move(object))->*_fieldPtr);
				return *(field->begin() + position);
			}

			/// @brief 
			/// @param object 
			/// @param position 
			/// @return 
			[[nodiscard]]
			value_type* Get(std::any&& object, const std::size_t position) const
			{
				TField* field = &(std::any_cast<TObject*>(std::move(object))->*_fieldPtr);
				return &*(field->begin() + position);
			}

			/// @brief 
			/// @param object 
			/// @param position 
			/// @return 
			[[nodiscard]]
			const value_type* Read(std::any&& object, const std::size_t position) const
			{
				const TField* field = &(std::any_cast<const TObject*>(std::move(object))->*_fieldPtr);
				return &*(field->begin() + position);
			}

			/// @brief 
			/// @param object 
			/// @param element 
			/// @param position 
			void Insert(std::any&& object, std::any&& element, const std::size_t position) const
			{
				TField* field = &(std::any_cast<TObject*>(std::move(object))->*_fieldPtr);
				field->insert(field->begin() + position, std::any_cast<value_type>(std::move(element)));
			}

			/// @brief 
			/// @param object 
			/// @param element 
			void InsertFront(std::any&& object, std::any&& element) const
			{
				TField* field = &(std::any_cast<TObject*>(std::move(object))->*_fieldPtr);
				field->insert(field->begin(), std::any_cast<value_type>(std::move(element)));
			}

			/// @brief 
			/// @param object 
			/// @param element 
			void InsertBack(std::any&& object, std::any&& element) const
			{
				TField* field = &(std::any_cast<TObject*>(std::move(object))->*_fieldPtr);
				field->insert(field->end(), std::any_cast<value_type>(std::move(element)));
			}

			/// @brief 
			/// @param object 
			/// @return 
			[[nodiscard]]
			std::size_t Size(std::any&& object) const
			{
				return (std::any_cast<const TObject*>(std::move(object))->*_fieldPtr).size();
			}
		};

		/// @brief 
		/// @tparam THelper 
		/// @param fieldName 
		/// @param typeIndex 
		/// @param fieldHelper 
		template<typename THelper>
		CollectionFieldInfo(std::string&& fieldName, std::type_index typeIndex, THelper&& fieldHelper) noexcept :
			FieldInfo(std::move(fieldName), std::move(typeIndex), std::forward<THelper>(fieldHelper)),
			_valueTypeIndex(typeid(typename THelper::value_type)),
			_elementValueGetter([](const std::any& helper, std::any&& object, const std::size_t position)
				{ return std::any(std::any_cast<const THelper&>(helper).Value(std::move(object), position)); }),
			_elementGetter([](const std::any& helper, std::any&& object, const std::size_t position)
				{ return std::any(std::any_cast<const THelper&>(helper).Get(std::move(object), position)); }),
			_elementReader([](const std::any& helper, std::any&& object, const std::size_t position)
				{ return std::any(std::any_cast<const THelper&>(helper).Read(std::move(object), position)); }),
			_elementGetterPtr([](const std::any& helper, std::any&& object, const std::size_t position)
				{ return static_cast<void*>(std::any_cast<const THelper&>(helper).Get(std::move(object), position)); }),
			_elementReaderPtr([](const std::any& helper, std::any&& object, const std::size_t position)
				{ return static_cast<const void*>(std::any_cast<const THelper&>(helper).Read(std::move(object), position)); }),
			_inserter([](const std::any& helper, std::any&& object, std::any&& element, const std::size_t position)
				{ std::any_cast<const THelper&>(helper).Insert(std::move(object), std::move(element), position); }),
			_frontInserter([](const std::any& helper, std::any&& object, std::any&& element)
				{ std::any_cast<const THelper&>(helper).InsertFront(std::move(object), std::move(element)); }),
			_backInserter([](const std::any& helper, std::any&& object, std::any&& element)
				{ std::any_cast<const THelper&>(helper).InsertBack(std::move(object), std::move(element)); }),
			_sizeGetter([](const std::any& helper, std::any&& object)
				{ return std::any_cast<const THelper&>(helper).Size(std::move(object)); }) {}

		/// @brief 
		~CollectionFieldInfo() override = default;

		/// @brief 
		/// @tparam TValueType 
		/// @param object 
		/// @param position 
		/// @return 
		template<typename TValueType>
		TValueType GetElementValue(std::any object, const std::size_t position) const
		{
			return std::any_cast<TValueType>(_elementValueGetter(_fieldHelper, std::move(object), position));
		}

		/// @brief 
		/// @param object 
		/// @param position 
		/// @return 
		std::any GetElementValue(std::any object, const std::size_t position) const;

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
		TValueType* GetElement(std::any object, const std::size_t position) const
		{
			return std::any_cast<TValueType*>(_elementGetter(_fieldHelper, std::move(object), position));
		}

		/// @brief 
		/// @param object 
		/// @param position 
		/// @return 
		std::any GetElement(std::any object, const std::size_t position) const;

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
		/// @param object 
		/// @param position 
		/// @return 
		void* GetElementPtr(std::any object, const std::size_t position) const;

		/// @brief 
		/// @tparam TObject 
		/// @param object 
		/// @param position 
		/// @return 
		template<typename TObject>
		void* GetElementPtr(TObject* object, const std::size_t position) const
		{
			if (!object)
				throw std::invalid_argument("Object is null");
			return _elementGetterPtr(_fieldHelper, object, position);
		}

		/// @brief 
		/// @tparam TValueType 
		/// @param object 
		/// @param position 
		/// @return 
		template<typename TValueType>
		const TValueType* ReadElement(std::any object, const std::size_t position) const
		{
			return std::any_cast<const TValueType*>(_elementReader(_fieldHelper, std::move(object), position));
		}

		/// @brief 
		/// @param object 
		/// @param position 
		/// @return 
		std::any ReadElement(std::any object, const std::size_t position) const;

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
		/// @param object 
		/// @param position 
		/// @return 
		const void* ReadElementPtr(std::any object, const std::size_t position) const;

		/// @brief 
		/// @tparam TObject 
		/// @param object 
		/// @param position 
		/// @return 
		template<typename TObject>
		const void* ReadElementPtr(const TObject* object, const std::size_t position) const
		{
			if (!object)
				throw std::invalid_argument("Object is null");
			return _elementReaderPtr(_fieldHelper, object, position);
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

		/// @brief 
		/// @param object 
		/// @return 
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
			_sizeGetter(_fieldHelper, object);
		}

		/// @brief 
		/// @return 
		[[nodiscard]]
		std::type_index ValueTypeIndex() const noexcept;

		/// @brief 
		/// @return 
		[[nodiscard]]
		bool IsCollection() const noexcept override;
	};

	/// @brief 
	/// @tparam TObject 
	/// @tparam TField 
	/// @param name 
	/// @param fieldPtr 
	/// @return 
	template<typename TObject, typename TField>
	std::shared_ptr<MemberInfo> CreateCollectionFieldInfo(std::string&& name, TField TObject::*fieldPtr) noexcept
	{
		return std::make_shared<CollectionFieldInfo>(std::move(name), typeid(TField), CollectionFieldInfo::Helper(fieldPtr));
	}
}

#define FIELD(name) \
[]() \
{ \
	if constexpr (ExtendedCpp::Reflection::Concepts::Insertable<decltype(name)>) \
		return ExtendedCpp::Reflection::CreateCollectionFieldInfo(#name, &ThisClassType::name); \
	else \
		return ExtendedCpp::Reflection::CreateFieldInfo(#name, &ThisClassType::name); \
}()

#endif