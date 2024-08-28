#ifndef Common_Random_H
#define Common_Random_H

#include <algorithm>
#include <random>
#include <string>

namespace Common
{
    template<typename TReal>
    TReal RandomReal(const TReal min, const TReal max) noexcept
    {
        std::random_device device;
        std::mt19937 generator(device());
        generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_real_distribution distribution(min, max);
        return distribution(generator);
    }

    template<typename TInteger>
    TInteger RandomInt(const TInteger min, const TInteger max) noexcept
    {
        std::random_device device;
        std::mt19937 generator(device());
        generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution distribution(min, max);
        return distribution(generator);
    }

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

    template<typename TChar = char>
    std::basic_string<TChar> RandomString(const std::size_t size, const std::vector<TChar>& charSet) noexcept
    {
        if (size == 0 || charSet.empty())
            return {};

        auto randChar = [&charSet]() -> char
        { return charSet[RandomInt<int>(0, charSet.size() - 1)]; };

        std::basic_string<TChar> randomString(size, 0);
        std::generate_n(randomString.begin(), size, randChar);
        return randomString;
    }

    template<typename TNumber>
    TNumber Random(const TNumber min, const TNumber max) noexcept
    {
        if constexpr (std::is_same_v<TNumber, double> || std::is_same_v<TNumber, float>)
            return RandomReal<TNumber>(min, max);
        else
            return RandomInt<TNumber>(min, max);
    }

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