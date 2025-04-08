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

		std::any (*_elementGetter)(const std::any& helper, std::any&& object, const std::size_t position);
		std::any (*_elementReader)(const std::any& helper, std::any&& object, const std::size_t position);
		void* (*_elementGetterPtr)(const std::any& helper, std::any&& object, const std::size_t position);
		const void* (*_elementReaderPtr)(const std::any& helper, std::any&& object, const std::size_t position);

		void (*_inserter)(const std::any& helper, std::any&& object, const std::size_t position);
		void (*_fontInserter)(const std::any& helper, std::any&& object);
		void (*_backInserter)(const std::any& helper, std::any&& object);

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
			value_type* Get(std::any&& object, const std::size_t position) const
			{
				TField& field = std::any_cast<TObject*>(std::move(object))->*_fieldPtr;
				return &*(field.begin() + position);
			}

			/// @brief 
			/// @param object 
			/// @param position 
			/// @return 
			[[nodiscard]]
			const value_type* Read(std::any&& object, const std::size_t position) const
			{
				const TField& field = std::any_cast<const TObject*>(std::move(object))->*_fieldPtr;
				return &*(field.begin() + position);
			}

			/// @brief 
			/// @param object 
			/// @param position 
			void Insert(std::any&& object, const std::size_t position) const
			{
				TField& field = std::any_cast<TObject*>(std::move(object))->*_fieldPtr;
				std::copy(field.begin(), field.end(), std::inserter(field, field.begin() + position));
			}

			/// @brief 
			/// @param object 
			void InsertFront(std::any&& object) const
			{
				TField& field = std::any_cast<TObject*>(std::move(object))->*_fieldPtr;
				std::copy(field.begin(), field.end(), std::front_inserter(field));
			}

			/// @brief 
			/// @param object 
			void InsertBack(std::any&& object) const
			{
				TField& field = std::any_cast<TObject*>(std::move(object))->*_fieldPtr;
				std::copy(field.begin(), field.end(), std::back_inserter(field));
			}

			/// @brief 
			/// @param object 
			/// @return 
			[[nodiscard]]
			const std::size_t Size(std::any&& object) const
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
			_valueTypeIndex(typeid(THelper::value_type)),
			_elementGetter([](const std::any& helper, std::any&& object, const std::size_t position)
				{ return std::any(std::any_cast<const THelper&>(helper).Get(std::move(object), position)); }),
			_elementReader([](const std::any& helper, std::any&& object, const std::size_t position)
				{ return std::any(std::any_cast<const THelper&>(helper).Read(std::move(object), position)); }),
			_elementGetterPtr([](const std::any& helper, std::any&& object, const std::size_t position)
				{ return static_cast<void*>(std::any_cast<const THelper&>(helper).Get(std::move(object), position)); }),
			_elementReaderPtr([](const std::any& helper, std::any&& object, const std::size_t position)
				{ return static_cast<const void*>(std::any_cast<const THelper&>(helper).Read(std::move(object), position)); }),
			_inserter([](const std::any& helper, std::any&& object, const std::size_t position)
				{ std::any_cast<const THelper&>(helper).Insert(std::move(object), position); }),
			_fontInserter([](const std::any& helper, std::any&& object)
				{ std::any_cast<const THelper&>(helper).InsertFront(std::move(object)); }),
			_backInserter([](const std::any& helper, std::any&& object)
				{ std::any_cast<const THelper&>(helper).InsertBack(std::move(object)); }),
			_sizeGetter([](const std::any& helper, std::any&& object)
				{ return std::any_cast<const THelper&>(helper).Size(std::move(object)); }) {}

		/// @brief 
		~CollectionFieldInfo() override = default;


	};
}

#endif