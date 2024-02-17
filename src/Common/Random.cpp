#include <algorithm>
#include <random>

#include <Common/Random.h>

std::size_t Common::RandomInt(const std::size_t min, const std::size_t max) noexcept
{
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution distribution(min, max);
    return distribution(generator);
}

std::string Common::RandomString(const std::size_t size) noexcept
{
    auto randChar = []() -> char
    {
        constexpr char charset[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
        constexpr std::size_t maxIndex = sizeof(charset) - 1;
        return charset[RandomInt(0, maxIndex)];
    };

    std::string randomString(size, 0);
    std::generate_n(randomString.begin(), size, randChar);
    if (size != 0) randomString[randomString.size() - 1] = randChar();
    return randomString;
}