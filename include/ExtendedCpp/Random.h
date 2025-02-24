#ifndef Common_Random_H
#define Common_Random_H

#include <algorithm>
#include <random>
#include <string>
#include <chrono>

/// @brief Namespace for random number and string generation utilities
namespace ExtendedCpp::Random
{
    /// @brief Generates a random real number within the specified range
    /// @tparam TReal The type of the real number
    /// @param min The minimum value of the range
    /// @param max The maximum value of the range
    /// @return A random real number within the specified range
    template<typename TReal>
    TReal RandomReal(const TReal min, const TReal max) noexcept
    {
        std::random_device device;
        std::mt19937 generator(device());
        generator.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
        std::uniform_real_distribution distribution(min, max);
        return distribution(generator);
    }

    /// @brief Generates a random integer within the specified range
    /// @tparam TInteger The type of the integer
    /// @param min The minimum value of the range
    /// @param max The maximum value of the range
    /// @return A random integer within the specified range
    template<typename TInteger>
    TInteger RandomInt(const TInteger min, const TInteger max) noexcept
    {
        std::random_device device;
        std::mt19937 generator(device());
        generator.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
        std::uniform_int_distribution distribution(min, max);
        return distribution(generator);
    }

    /// @brief Generates a random string of the specified size using alphanumeric characters
    /// @param size The size of the random string
    /// @return A random string of the specified size
    inline std::string RandomString(const std::size_t size) noexcept
    {
        if (size == 0)
            return {};

        auto randChar = []() -> char
        {
            constexpr char charset[] =
                    "0123456789"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz";
            constexpr int maxIndex = sizeof(charset) - 1;
            return charset[RandomInt<int>(0, maxIndex)];
        };

        std::string randomString(size, 0);
        std::generate_n(randomString.begin(), size, randChar);
        return randomString;
    }

    /// @brief Generates a random string of the specified size using characters from the provided character set
    /// @tparam TChar The type of the characters
    /// @param size The size of the random string
    /// @param charSet The character set to use for generating the random string
    /// @return A random string of the specified size
    template<typename TChar>
    std::basic_string<TChar> RandomString(const std::size_t size, const std::vector<TChar>& charSet) noexcept
    {
        if (size == 0 || charSet.empty())
            return {};

        auto randChar = [&charSet]() -> char
        { return charSet[RandomInt<std::size_t>(0, charSet.size() - 1)]; };

        std::basic_string<TChar> randomString(size, 0);
        std::generate_n(randomString.begin(), size, randChar);
        return randomString;
    }

    /// @brief Generates a random number within the specified range
    /// @tparam TNumber The type of the number
    /// @param min The minimum value of the range
    /// @param max The maximum value of the range
    /// @return A random number within the specified range
    template<typename TNumber>
    TNumber Random(const TNumber min, const TNumber max) noexcept
    {
        if constexpr (std::is_same_v<TNumber, double> || std::is_same_v<TNumber, float>)
            return RandomReal<TNumber>(min, max);
        else
            return RandomInt<TNumber>(min, max);
    }

    /// @brief Generates a sequence of random numbers within the specified range
    /// @tparam TNumber The type of the numbers
    /// @param min The minimum value of the range
    /// @param max The maximum value of the range
    /// @param size The size of the sequence
    /// @return A vector containing the sequence of random numbers
    template<typename TNumber>
    std::vector<TNumber> RandomSequence(const TNumber min, const TNumber max, const std::size_t size) noexcept
    {
        std::vector<TNumber> sequence(size);
        for (std::size_t i = 0; i < size; ++i)
            sequence[i] = Random<TNumber>(min, max);
        return sequence;
    }
}

#endif