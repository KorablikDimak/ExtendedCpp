#ifndef Reflection_Assembly_H
#define Reflection_Assembly_H

#include <map>
#include <mutex>

#include <ExtendedCpp/Reflection/TypeInfo.h>

/// @brief 
namespace ExtendedCpp::Reflection
{
    /// @brief 
    class Assembly final
    {
    private:
        static std::map<std::type_index, TypeInfo>&  TypeList() noexcept;
        static std::mutex& ListMutex() noexcept;

    public:
        /// @brief 
        Assembly() = delete;

        /// @brief 
        /// @param typeInfo 
        static void AddType(const TypeInfo& typeInfo) noexcept;

        /// @brief 
        /// @return 
        static std::vector<TypeInfo> GetTypes() noexcept;
    };
}

#endif