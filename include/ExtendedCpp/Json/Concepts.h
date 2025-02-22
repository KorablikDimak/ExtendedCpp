#ifndef Json_Concepts_H
#define Json_Concepts_H

#include <utility>
#include <memory>

/// @brief 
namespace ExtendedCpp::Concepts
{
    template<typename T>
    concept IsSharedPtr = std::same_as<std::decay_t<T>, std::shared_ptr<typename std::decay_t<T>::element_type>>;
}

#endif