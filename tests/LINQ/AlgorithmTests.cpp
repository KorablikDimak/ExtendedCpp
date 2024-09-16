#include <gtest/gtest.h>

#include <ExtendedCpp/LINQ/Algorithm.h>

TEST(AlgorithmTests, BinarySearchTest)
{
    // Average
    const int arr1[10] = { 1, 3, 5, 8, 9, 14, 56, 102, 304, 400 };

    // Act
    // Assert
    ASSERT_EQ(ExtendedCpp::LINQ::Algorithm::BinarySearch(3, arr1, 0, 9), 1);
    ASSERT_EQ(ExtendedCpp::LINQ::Algorithm::BinarySearch(400, arr1, 0, 9), 9);
    ASSERT_EQ(ExtendedCpp::LINQ::Algorithm::BinarySearch(102, arr1, 0, 9), 7);
    ASSERT_EQ(ExtendedCpp::LINQ::Algorithm::BinarySearch(1022, arr1, 0, 9), ExtendedCpp::LINQ::NPOS);

    // Act
    // Assert
    ASSERT_EQ(ExtendedCpp::LINQ::Algorithm::BinarySearch(5, arr1, 0, 9, [](const int number){ return number + 2; }), 1);
    ASSERT_EQ(ExtendedCpp::LINQ::Algorithm::BinarySearch(800, arr1, 0, 9, [](const int number){ return number * 2; }), 9);
    ASSERT_EQ(ExtendedCpp::LINQ::Algorithm::BinarySearch(800, arr1, 0, 9, [](const int number){ return number * 0; }), ExtendedCpp::LINQ::NPOS);

    int* arr2[10] = { new int(1), new int(3), new int(5), new int(8), new int(9), new int(14),
                        new int(56), new int(102), new int(304), new int(400) };

    // Act
    // Assert
    ASSERT_EQ(ExtendedCpp::LINQ::Algorithm::BinarySearch(3, arr2, 0, 9), 1);
    ASSERT_EQ(ExtendedCpp::LINQ::Algorithm::BinarySearch(400, arr2, 0, 9), 9);
    ASSERT_EQ(ExtendedCpp::LINQ::Algorithm::BinarySearch(102, arr2, 0, 9), 7);
    ASSERT_EQ(ExtendedCpp::LINQ::Algorithm::BinarySearch(1022, arr2, 0, 9), ExtendedCpp::LINQ::NPOS);

    // Act
    // Assert
    ASSERT_EQ(ExtendedCpp::LINQ::Algorithm::BinarySearch(5, arr2, 0, 9, [](const int* number){ return *number; }), 2);
    ASSERT_EQ(ExtendedCpp::LINQ::Algorithm::BinarySearch(400, arr2, 0, 9, [](const int* number){ return *number; }), 9);
    ASSERT_EQ(ExtendedCpp::LINQ::Algorithm::BinarySearch(401, arr2, 0, 9, [](const int* number){ return *number; }), ExtendedCpp::LINQ::NPOS);

    for (auto &element : arr2)
        delete element;
}

TEST(AlgorithmTests, CountEqualKeysTest)
{
    // Average
    const short arr[10] = { 7, 6, 6, 10, 2, 2, 5, 6, 40, 6 };

    // Act
    std::map<short, std::size_t> dict = ExtendedCpp::LINQ::Algorithm::CountEqualKeys(arr, 0, 9);

    // Assert
    if (dict[6] == 3 && dict[7] == 1 && dict[2] == 2)
    {
        ASSERT_TRUE(true);
        return;
    }

    ASSERT_TRUE(false);
}

TEST(AlgorithmTests, CountCommonSubsequenceTest)
{
    // Average
    const short arr1[10] = { 7, 6, 6, 10, 2, 2, 5, 6, 40, 6 };
    const short arr2[10] = { 7, 6, 7, 10, 2, 2, 5, 6, 41, 6 };

    // Act
    const std::size_t count = ExtendedCpp::LINQ::Algorithm::CountCommonSubsequence(arr1, 10, arr2, 10);

    // Assert
    if (count == 8)
    {
        ASSERT_TRUE(true);
        return;
    }

    ASSERT_TRUE(false);
}

TEST(AlgorithmTests, ContainsTest)
{
    // Average
    const std::vector<short> collection = { 7, 6, 6, 10, 2, 2, 5, 6, 40, 6 };
    const std::vector<short> subCollection1 = { 10, 2, 2, 5 };
    const std::vector<short> subCollection2 = { 7, 2, 2, 40 };

    // Act
    // Assert
    if (ExtendedCpp::LINQ::Algorithm::Contains(collection.data(), collection.size(), subCollection1.data(), subCollection1.size()) &&
        !ExtendedCpp::LINQ::Algorithm::Contains(collection.data(), collection.size(), subCollection2.data(), subCollection2.size()))
    {
        ASSERT_TRUE(true);
        return;
    }

    ASSERT_TRUE(false);
}

TEST(AlgorithmTests, IndexAtTest)
{
    // Average
    const short collection[10] = { 7, 6, 6, 10, 2, 2, 5, 6, 40, 6 };
    constexpr short subCollection1[4] = { 10, 2, 2, 5 };
    constexpr short subCollection2[4] = { 7, 2, 2, 40 };

    // Act
    // Assert
    ASSERT_EQ(ExtendedCpp::LINQ::Algorithm::IndexAt(collection, 10, subCollection1, 4), 3);
    ASSERT_EQ(ExtendedCpp::LINQ::Algorithm::IndexAt(collection, 10, subCollection2, 4), ExtendedCpp::LINQ::NPOS);
}