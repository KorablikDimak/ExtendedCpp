#include <gtest/gtest.h>

#include <Common/Random.h>

TEST(RandomTests, RandomStringTest)
{
    // Average
    const std::vector charSet = {'a', 'b', 'c'};

    // Act
    const std::string randomString1 = Common::RandomString(5, charSet);
    const std::string randomString2 = Common::RandomString(5);

    // Assert
    ASSERT_TRUE(randomString1.size() == 5);
    ASSERT_TRUE(randomString2.size() == 5);
    ASSERT_TRUE(randomString1.find('d') == std::string::npos);
}

TEST(RandomTests, RandomIntTest)
{
    // Average
    const int min = -100;
    const int max = 100;

    // Act
    const int random1 = Common::RandomInt(min, max);
    const int random2 = Common::RandomInt(-20, 30);

    // Assert
    ASSERT_TRUE(random1 >= -100 && random1 <= 100);
    ASSERT_TRUE(random2 >= -20 && random2 <= 30);
}

TEST(RandomTests, RandomRealTest)
{
    // Average
    const double min = -0.5;
    const double max = 1.2;

    // Act
    const double random1 = Common::RandomReal(min, max);
    const double random2 = Common::RandomReal(-2.7, 3.9);

    // Assert
    ASSERT_TRUE(random1 >= -0.5 && random1 <= 1.2);
    ASSERT_TRUE(random2 >= -2.7 && random2 <= 3.9);
}

TEST(RandomTests, RandomNumberTest)
{
    // Average
    const unsigned int min = 110;
    const unsigned int max = 150;

    // Act
    const unsigned int random1 = Common::Random(min, max);
    const double random2 = Common::Random(0.8, 6.3);

    // Assert
    ASSERT_TRUE(random1 >= 110 && random1 <= 150);
    ASSERT_TRUE(random2 >= 0.8 && random2 <= 6.3);
}

TEST(RandomTests, RandomSequenceTest)
{
    // Average
    const unsigned int min = 40;
    const unsigned int max = 80;

    // Act
    const auto sequence = Common::RandomSequence(min, max, 10);

    // Assert
    for (const auto element : sequence)
        ASSERT_TRUE(element >= 40 && element <= 80);
}