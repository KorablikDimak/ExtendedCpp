#include <gtest/gtest.h>

#include <LINQ/Sort.h>

#include "LINQ_Tests.h"

TEST(SortTests, SelectionSortTest)
{
    // Average
    short arr1[10] = { 7, 4, 2, 10, 2, 50, 5, 21, 40, 25 };
    const short arrCorrect1[10] = { 2, 2, 4, 5, 7, 10, 21, 25, 40, 50 };

    // Act
    LINQ::Sort::SelectionSort(arr1, 0, 9);

    // Assert
    for (short i = 0; i < 10; ++i)
        ASSERT_TRUE(arr1[i] == arrCorrect1[i]);

    // Average
    std::vector arr2 = { 6, 3, 18, 9, 11, 23, 10, 33 };
    const std::vector arrCorrect2 = { 3, 6, 9, 10, 11, 18, 23, 33 };

    // Act
    LINQ::Sort::SelectionSort(arr2.data(), 0, 7);

    // Assert
    for (unsigned char i = 0; i < 8; ++i)
        ASSERT_TRUE(arr2[i] == arrCorrect2[i]);

    // Average
    short* arr3[6] = { new short(9), new short(8), new short(5), new short(1), new short(3), new short(14) };
    short* arrCorrect3[6] = { new short(1), new short(3), new short(5), new short(8), new short(9), new short(14) };

    // Act
    LINQ::Sort::SelectionSort(arr3, 0, 5);

    // Assert
    for (unsigned char i = 0; i < 6; ++i)
    {
        ASSERT_TRUE(*arr3[i] == *arrCorrect3[i]);

        delete arr3[i];
        delete arrCorrect3[i];
    }

    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);
    Person persons[] = { person1, person2, person3, person4 };
    constexpr unsigned char sortedAges[] = { 23, 24, 27, 29 };

    // Act
    LINQ::Sort::SelectionSort(persons, 0, 3, [](const Person& person){ return person.Age; });

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
    LINQ::Sort::InsertionSort(arr1, 0, 9);

    // Assert
    for (short i = 0; i < 10; ++i)
        ASSERT_TRUE(arr1[i] == arrCorrect1[i]);

    // Average
    std::vector arr2 = { 6, 3, 18, 9, 11, 23, 10, 33 };
    const std::vector arrCorrect2 = { 3, 6, 9, 10, 11, 18, 23, 33 };

    // Act
    LINQ::Sort::InsertionSort(arr2.data(), 0, 7);

    // Assert
    for (unsigned char i = 0; i < 8; ++i)
        ASSERT_TRUE(arr2[i] == arrCorrect2[i]);

    // Average
    short* arr3[6] = { new short(9), new short(8), new short(5), new short(1), new short(3), new short(14) };
    short* arrCorrect3[6] = { new short(1), new short(3), new short(5), new short(8), new short(9), new short(14) };

    // Act
    LINQ::Sort::InsertionSort(arr3, 0, 5);

    // Assert
    for (unsigned char i = 0; i < 6; ++i)
    {
        ASSERT_TRUE(*arr3[i] == *arrCorrect3[i]);

        delete arr3[i];
        delete arrCorrect3[i];
    }

    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);
    Person persons[] = { person1, person2, person3, person4 };
    constexpr unsigned char sortedAges[] = { 29, 27, 24, 23 };

    // Act
    LINQ::Sort::InsertionSort(persons, 0, 3, [](const Person& person){ return person.Age; }, OrderType::DESC);

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
    LINQ::Sort::BucketSort(arr, 0, 19);

    // Assert
    for (short i = 0; i < 20; ++i)
        ASSERT_TRUE(arr[i] == arrCorrect[i]);

    // Act
    LINQ::Sort::BucketSort(arr, 0, 19, OrderType::DESC);

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
    LINQ::Sort::BucketSort(persons, 0, 3, [](const Person& person){ return person.Age; }, OrderType::DESC);

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
    LINQ::Sort::CombSort(arr, 0, 19);

    // Assert
    for (short i = 0; i < 20; ++i)
        ASSERT_TRUE(arr[i] == arrCorrect[i]);

    // Act
    LINQ::Sort::CombSort(arr, 0, 19, OrderType::DESC);

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
    LINQ::Sort::CombSort(persons, 0, 3, [](const Person& person){ return person.Age; }, OrderType::DESC);

    // Assert
    for (short i = 0; i < 4; ++i)
        ASSERT_TRUE(persons[i].Age == sortedAges[i]);

    // Average
    short* arr3[6] = { new short(9), new short(8), new short(5), new short(1), new short(3), new short(14) };
    short* arrCorrect3[6] = { new short(1), new short(3), new short(5), new short(8), new short(9), new short(14) };

    // Act
    LINQ::Sort::CombSort(arr3, 0, 5);

    // Assert
    for (unsigned char i = 0; i < 6; ++i)
    {
        ASSERT_TRUE(*arr3[i] == *arrCorrect3[i]);

        delete arr3[i];
        delete arrCorrect3[i];
    }
}

TEST(SortTests, MergeSortTest)
{
    // Average
    std::vector arr1 = { 6, 3, 18, 9, 12, 23, 10, 33 };
    const std::vector arrCorrect1 = { 3, 6, 9, 10, 12, 18, 23, 33 };

    // Act
    LINQ::Sort::MergeSort(arr1.data(), 0, 7);

    // Assert
    for (short i = 0; i < 8; ++i)
        ASSERT_TRUE(arr1[i] == arrCorrect1[i]);

    // Average
    short* arr2[6] = { new short(9), new short(8), new short(5), new short(1), new short(3), new short(14) };
    short* arrCorrect2[6] = { new short(1), new short(3), new short(5), new short(8), new short(9), new short(14) };

    // Act
    LINQ::Sort::MergeSort(arr2, 0, 5);

    // Assert
    for (unsigned char i = 0; i < 6; ++i)
    {
        ASSERT_TRUE(*arr2[i] == *arrCorrect2[i]);

        delete arr2[i];
        delete arrCorrect2[i];
    }

    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);
    Person persons[] = { person1, person2, person3, person4 };
    constexpr unsigned char sortedAges[] = { 29, 27, 24, 23 };

    // Act
    LINQ::Sort::MergeSort(persons, 0, 3, [](const Person& person){ return person.Age; }, OrderType::DESC);

    // Assert
    for (short i = 0; i < 4; ++i)
        ASSERT_TRUE(persons[i].Age == sortedAges[i]);
}

TEST(SortTests, QuickSortTest)
{
    // Average
    short arr1[10] = { 7, 3, 6, 10, 2, 50, 5, 21, 40, 25 };
    const short arrCorrect1[10] = { 2, 3, 5, 6, 7, 10, 21, 25, 40, 50 };

    // Act
    LINQ::Sort::QuickSort(arr1, 0, 9);

    // Assert
    for (short i = 0; i < 10; ++i)
        ASSERT_TRUE(arr1[i] == arrCorrect1[i]);

    // Average
    std::vector arr2 = { 6, 3, 18, 9, 12, 23, 10, 33 };
    const std::vector arrCorrect2 = { 3, 6, 9, 10, 12, 18, 23, 33 };

    // Act
    LINQ::Sort::QuickSort(arr2.data(), 0, 7);

    // Assert
    for (unsigned char i = 0; i < 8; ++i)
        ASSERT_TRUE(arr2[i] == arrCorrect2[i]);

    // Average
    short* arr3[6] = { new short(9), new short(8), new short(5), new short(1), new short(3), new short(14) };
    short* arrCorrect3[6] = { new short(1), new short(3), new short(5), new short(8), new short(9), new short(14) };

    // Act
    LINQ::Sort::QuickSort(arr3, 0, 5);

    // Assert
    for (unsigned char i = 0; i < 6; ++i)
    {
        ASSERT_TRUE(*arr3[i] == *arrCorrect3[i]);

        delete arr3[i];
        delete arrCorrect3[i];
    }

    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);
    Person persons[] = { person1, person2, person3, person4 };
    constexpr unsigned char sortedAges[] = { 29, 27, 24, 23 };

    // Act
    LINQ::Sort::QuickSort(persons, 0, 3, [](const Person& person){ return person.Age; }, OrderType::DESC);

    // Assert
    for (short i = 0; i < 4; ++i)
        ASSERT_TRUE(persons[i].Age == sortedAges[i]);
}

TEST(SortTests, TimSortTest)
{
    // Average
    short arr1[10] = { 7, 3, 6, 10, 2, 50, 5, 21, 40, 25 };
    const short arrCorrect1[10] = { 2, 3, 5, 6, 7, 10, 21, 25, 40, 50 };

    // Act
    LINQ::Sort::TimSort(arr1, 0, 9);

    // Assert
    for (short i = 0; i < 10; ++i)
        ASSERT_TRUE(arr1[i] == arrCorrect1[i]);

    // Average
    std::vector arr2 = { 6, 3, 18, 9, 12, 23, 10, 33 };
    const std::vector arrCorrect2 = { 3, 6, 9, 10, 12, 18, 23, 33 };

    // Act
    LINQ::Sort::TimSort(arr2.data(), 0, 7);

    // Assert
    for (unsigned char i = 0; i < 8; ++i)
        ASSERT_TRUE(arr2[i] == arrCorrect2[i]);

    // Average
    short* arr3[6] = { new short(9), new short(8), new short(5), new short(1), new short(3), new short(14) };
    short* arrCorrect3[6] = { new short(1), new short(3), new short(5), new short(8), new short(9), new short(14) };

    // Act
    LINQ::Sort::TimSort(arr3, 0, 5);

    // Assert
    for (unsigned char i = 0; i < 6; ++i)
    {
        ASSERT_TRUE(*arr3[i] == *arrCorrect3[i]);

        delete arr3[i];
        delete arrCorrect3[i];
    }

    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);
    Person persons[] = { person1, person2, person3, person4 };
    constexpr unsigned char sortedAges[] = { 29, 27, 24, 23 };

    // Act
    LINQ::Sort::TimSort(persons, 0, 3, [](const Person& person){ return person.Age; }, OrderType::DESC);

    // Assert
    for (short i = 0; i < 4; ++i)
        ASSERT_TRUE(persons[i].Age == sortedAges[i]);
}