#ifndef Common_Reflection_H
#define Common_Reflection_H

#include <string>
#include <utility>
#include <array>
#include <tuple>
#include <typeinfo>

namespace Common
{
    template <size_t Index = 0, typename TTuple, size_t Size = std::tuple_size_v<std::remove_reference_t<TTuple>>,
            typename TFunction, typename... TArgs>
    void Foreach(TTuple&& tuple, TFunction&& function, TArgs&&... args)
    {
        if constexpr (Index < Size)
        {
            std::invoke(function, args..., std::get<Index>(tuple));

            if constexpr (Index + 1 < Size)
                Foreach<Index + 1>(std::forward<TTuple>(tuple),
                        std::forward<TFunction>(function),
                        std::forward<TArgs>(args)...);
        }
    }

    #define FIELD(name) \
    FieldMetaInfo(name, #name)

    #define META(...) \
    using MetaType = decltype(MetaInfo(__VA_ARGS__)); \
    MetaType Meta = MetaInfo(__VA_ARGS__); \

    template<typename TField>
    class FieldMetaInfo
    {
    private:
        TField* _field;
        std::string _name;

    public:
        using Type = TField;

        FieldMetaInfo() noexcept = default;

        FieldMetaInfo(TField& field, const std::string& name) noexcept
        {
            _field = &field;
            _name = name;
        }

        TField& Field() noexcept
        {
            return *_field;
        }

        const TField& Field() const noexcept
        {
            return *_field;
        }

        [[nodiscard]]
        const std::string& Name() const noexcept
        {
            return _name;
        }
    };

    template<typename... TFieldsMeta>
    class MetaInfo
    {
    private:
        std::tuple<TFieldsMeta...> _fieldsMeta{};
        std::vector<std::string> _fieldsName{};
        std::size_t _fieldsCount = std::tuple_size_v<std::tuple<TFieldsMeta...>>;

    public:
        explicit MetaInfo(TFieldsMeta&&... fieldsMeta) noexcept
        {
            _fieldsMeta = std::make_tuple(std::forward<TFieldsMeta>(fieldsMeta)...);
            Foreach(_fieldsMeta, [this](const auto& fieldMeta)
                { _fieldsName.push_back(fieldMeta.Name()); });
        }

        [[nodiscard]]
        std::size_t FieldsCount() const noexcept
        {
            return _fieldsCount;
        }

        template<std::size_t Index>
        decltype(auto) GetFieldMeta() const noexcept
        {
            return std::get<Index>(_fieldsMeta);
        }

        template <size_t Index = 0, size_t Size = std::tuple_size_v<std::tuple<TFieldsMeta...>>,
                typename TFunction, typename... TArgs>
        void ForeachFields(TFunction&& function, TArgs&&... args)
        {
            if constexpr (Index < Size)
            {
                std::invoke(function, args..., std::get<Index>(_fieldsMeta));

                if constexpr (Index + 1 < Size)
                    ForeachFields<Index + 1>(std::forward<TFunction>(function), std::forward<TArgs>(args)...);
            }
        }
    };
}

#endif