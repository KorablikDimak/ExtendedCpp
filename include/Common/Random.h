#ifndef Common_Random_H
#define Common_Random_H

#include <string>

namespace Common
{
    std::size_t RandomInt(std::size_t min, std::size_t max) noexcept;
    std::string RandomString(std::size_t size) noexcept;
}

#endif