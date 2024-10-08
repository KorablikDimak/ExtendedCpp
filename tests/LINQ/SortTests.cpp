#include <gtest/gtest.h>

#include <ExtendedCpp/LINQ/Sort.h>

#include "LINQ_Tests.h"

TEST(SortTests, SelectionSortTest)
{
    // Average
    short arr1[10] = { 7, 4, 2, 10, 2, 50, 5, 21, 40, 25 };
    const short arrCorrect1[10] = { 2, 2, 4, 5, 7, 10, 21, 25, 40, 50 };

    // Act
    ExtendedCpp::LINQ::Sort::SelectionSort(arr1, 0, 9);

    // Assert
    for (short i = 0; i < 10; ++i)
        ASSERT_TRUE(arr1[i] == arrCorrect1[i]);

    // Average
    std::vector arr2 = { 6, 3, 18, 9, 11, 23, 10, 33 };
    const std::vector arrCorrect2 = { 3, 6, 9, 10, 11, 18, 23, 33 };

    // Act
    ExtendedCpp::LINQ::Sort::SelectionSort(arr2.data(), 0, 7);

    // Assert
    for (unsigned char i = 0; i < 8; ++i)
        ASSERT_TRUE(arr2[i] == arrCorrect2[i]);

    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);
    Person persons[] = { person1, person2, person3, person4 };
    constexpr unsigned char sortedAges[] = { 23, 24, 27, 29 };

    // Act
    ExtendedCpp::LINQ::Sort::SelectionSort(persons, 0, 3, [](const Person& person){ return person.Age; });

    // Assert
    for (short i = 0; i < 4; ++i)
        ASSERT_TRUE(persons[i].Age == sortedAges[i]);
}

TEST(SortTests, InsertionSortTest)
{
    // Average
    short arr1[10] = { 7, 4, 2, 10, 2, 50, 5, 21, 40, 25 };
    const short arrCorrect1[10] = { 2, 2, 4, 5, 7, 10, 21, 25, 40, 50 };

    // Act
    ExtendedCpp::LINQ::Sort::InsertionSort(arr1, 0, 9);

    // Assert
    for (short i = 0; i < 10; ++i)
        ASSERT_TRUE(arr1[i] == arrCorrect1[i]);

    // Average
    std::vector arr2 = { 6, 3, 18, 9, 11, 23, 10, 33 };
    const std::vector arrCorrect2 = { 3, 6, 9, 10, 11, 18, 23, 33 };

    // Act
    ExtendedCpp::LINQ::Sort::InsertionSort(arr2.data(), 0, 7);

    // Assert
    for (unsigned char i = 0; i < 8; ++i)
        ASSERT_TRUE(arr2[i] == arrCorrect2[i]);

    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);
    Person persons[] = { person1, person2, person3, person4 };
    constexpr unsigned char sortedAges[] = { 29, 27, 24, 23 };

    // Act
    ExtendedCpp::LINQ::Sort::InsertionSort(persons, 0, 3, [](const Person& person){ return person.Age; }, ExtendedCpp::LINQ::OrderType::DESC);

    // Assert
    for (short i = 0; i < 4; ++i)
        ASSERT_TRUE(persons[i].Age == sortedAges[i]);
}

TEST(SortTests, BucketSortTest)
{
    // Average
    short arr[20] = { 7, 4, 2, 10, 2, -7, 50, 5, 21, 40, 25, 6, 3, -9, 18, 9, 12, 23, 10, 33 };
    constexpr short arrCorrect[20] = { -9, -7, 2, 2, 3, 4, 5, 6, 7, 9, 10, 10, 12, 18, 21, 23, 25, 33, 40, 50 };

    // Act
    ExtendedCpp::LINQ::Sort::BucketSort(arr, 0, 19);

    // Assert
    for (short i = 0; i < 20; ++i)
        ASSERT_TRUE(arr[i] == arrCorrect[i]);

    // Act
    ExtendedCpp::LINQ::Sort::BucketSort(arr, 0, 19, ExtendedCpp::LINQ::OrderType::DESC);

    // Assert
    for (short i = 0, j = 19; i < 20; ++i, --j)
        ASSERT_TRUE(arr[i] == arrCorrect[j]);

    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);
    Person persons[] = { person1, person2, person3, person4 };
    constexpr unsigned char sortedAges[] = { 29, 27, 24, 23 };

    // Act
    ExtendedCpp::LINQ::Sort::BucketSort(persons, 0, 3, [](const Person& person){ return person.Age; }, ExtendedCpp::LINQ::OrderType::DESC);

    // Assert
    for (short i = 0; i < 4; ++i)
        ASSERT_TRUE(persons[i].Age == sortedAges[i]);
}

TEST(SortTests, CombSortTest)
{
    // Average
    short arr[20] = { 7, 4, 2, 10, 2, -7, 50, 5, 21, 40, 25, 6, 3, -9, 18, 9, 12, 23, 10, 33 };
    constexpr short arrCorrect[20] = { -9, -7, 2, 2, 3, 4, 5, 6, 7, 9, 10, 10, 12, 18, 21, 23, 25, 33, 40, 50 };

    // Act
    ExtendedCpp::LINQ::Sort::CombSort(arr, 0, 19);

    // Assert
    for (short i = 0; i < 20; ++i)
        ASSERT_TRUE(arr[i] == arrCorrect[i]);

    // Act
    ExtendedCpp::LINQ::Sort::CombSort(arr, 0, 19, ExtendedCpp::LINQ::OrderType::DESC);

    // Assert
    for (short i = 0, j = 19; i < 20; ++i, --j)
        ASSERT_TRUE(arr[i] == arrCorrect[j]);

    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);
    Person persons[] = { person1, person2, person3, person4 };
    constexpr unsigned char sortedAges[] = { 29, 27, 24, 23 };

    // Act
    ExtendedCpp::LINQ::Sort::CombSort(persons, 0, 3, [](const Person& person){ return person.Age; }, ExtendedCpp::LINQ::OrderType::DESC);

    // Assert
    for (short i = 0; i < 4; ++i)
        ASSERT_TRUE(persons[i].Age == sortedAges[i]);
}

TEST(SortTests, MergeSortTest)
{
    // Average
    std::vector arr1 = { 6, 3, 18, 9, 12, 23, 10, 33 };
    const std::vector arrCorrect1 = { 3, 6, 9, 10, 12, 18, 23, 33 };

    // Act
    ExtendedCpp::LINQ::Sort::MergeSort(arr1.data(), 0, 7);

    // Assert
    for (short i = 0; i < 8; ++i)
        ASSERT_TRUE(arr1[i] == arrCorrect1[i]);

    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);
    Person persons[] = { person1, person2, person3, person4 };
    constexpr unsigned char sortedAges[] = { 29, 27, 24, 23 };

    // Act
    ExtendedCpp::LINQ::Sort::MergeSort(persons, 0, 3, [](const Person& person){ return person.Age; }, ExtendedCpp::LINQ::OrderType::DESC);

    // Assert
    for (short i = 0; i < 4; ++i)
        ASSERT_TRUE(persons[i].Age == sortedAges[i]);
}

TEST(SortTests, QuickSortTest1)
{
    // Average
    short arr1[10] = { 7, 3, 6, 10, 2, 50, 5, 21, 40, 25 };
    const short arrCorrect1[10] = { 2, 3, 5, 6, 7, 10, 21, 25, 40, 50 };

    // Act
    ExtendedCpp::LINQ::Sort::QuickSort(arr1, 0, 9);

    // Assert
    for (short i = 0; i < 10; ++i)
        ASSERT_TRUE(arr1[i] == arrCorrect1[i]);

    // Average
    std::vector arr2 = { 6, 3, 18, 9, 12, 23, 10, 33 };
    const std::vector arrCorrect2 = { 3, 6, 9, 10, 12, 18, 23, 33 };

    // Act
    ExtendedCpp::LINQ::Sort::QuickSort(arr2.data(), 0, 7);

    // Assert
    for (unsigned char i = 0; i < 8; ++i)
        ASSERT_TRUE(arr2[i] == arrCorrect2[i]);

    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);
    Person persons[] = { person1, person2, person3, person4 };
    constexpr unsigned char sortedAges[] = { 29, 27, 24, 23 };

    // Act
    ExtendedCpp::LINQ::Sort::QuickSort(persons, 0, 3, [](const Person& person){ return person.Age; }, ExtendedCpp::LINQ::OrderType::DESC);

    // Assert
    for (short i = 0; i < 4; ++i)
        ASSERT_TRUE(persons[i].Age == sortedAges[i]);
}

TEST(SortTests, QuickSortTest2)
{
    // Average
    short arr[8] = { 0, 5, 2, 10, 9, 7, 4, 1 };
    const short arrCorrect[8] = { 0, 1, 2, 4, 5, 7, 9, 10 };

    // Act
    ExtendedCpp::LINQ::Sort::QuickSort(arr, 0, 7);

    // Assert
    for (short i = 0; i < 8; ++i)
        ASSERT_TRUE(arr[i] == arrCorrect[i]);
}

TEST(SortTests, TimSortTest)
{
    // Average
    short arr1[10] = { 7, 3, 6, 10, 2, 50, 5, 21, 40, 25 };
    const short arrCorrect1[10] = { 2, 3, 5, 6, 7, 10, 21, 25, 40, 50 };

    // Act
    ExtendedCpp::LINQ::Sort::TimSort(arr1, 0, 9);

    // Assert
    for (short i = 0; i < 10; ++i)
        ASSERT_TRUE(arr1[i] == arrCorrect1[i]);

    // Average
    std::vector arr2 = { 6, 3, 18, 9, 12, 23, 10, 33 };
    const std::vector arrCorrect2 = { 3, 6, 9, 10, 12, 18, 23, 33 };

    // Act
    ExtendedCpp::LINQ::Sort::TimSort(arr2.data(), 0, 7);

    // Assert
    for (unsigned char i = 0; i < 8; ++i)
        ASSERT_TRUE(arr2[i] == arrCorrect2[i]);

    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);
    Person persons[] = { person1, person2, person3, person4 };
    constexpr unsigned char sortedAges[] = { 29, 27, 24, 23 };

    // Act
    ExtendedCpp::LINQ::Sort::TimSort(persons, 0, 3, [](const Person& person){ return person.Age; }, ExtendedCpp::LINQ::OrderType::DESC);

    // Assert
    for (short i = 0; i < 4; ++i)
        ASSERT_TRUE(persons[i].Age == sortedAges[i]);
}