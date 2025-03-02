#include <gtest/gtest.h>

#include <ExtendedCpp/LINQ.h>

#include "LINQ_Tests.h"

TEST(LINQ_View_Tests, SelectTest)
{
    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);

    const std::vector people { person1, person2, person3, person4 };

    // Act
    const std::vector names = ExtendedCpp::LINQ::View(people)
            .Select([](const Person& person){ return person.Name; })
            .ToVector();

    // Assert
    ASSERT_EQ("Tom", names[0]);
    ASSERT_EQ("Bob", names[1]);
    ASSERT_EQ("Sam", names[2]);
    ASSERT_EQ("Alice", names[3]);
}

TEST(LINQ_View_Tests, WhereTest)
{
    // Average
    const std::vector names { "Tom", "Alice", "Bob", "Sam", "Tim", "Tomas", "Bill" };

    // Act
    const std::vector selectedNames = ExtendedCpp::LINQ::View(names)
            .Where([](const std::string& name){ return name.size() == 3; })
            .ToVector();

    // Assert
    ASSERT_EQ(4, selectedNames.size());
}

TEST(LINQ_View_Tests, RemoveWhereTest)
{
    // Average
    const std::vector names { "Tom", "Alice", "Bob", "Sam", "Tim", "Tomas", "Bill" };

    // Act
    const std::vector selectedNames = ExtendedCpp::LINQ::View(names)
            .RemoveWhere([](const std::string& name){ return name.size() == 3; })
            .ToVector();

    // Assert
    ASSERT_EQ(3, selectedNames.size());
}

TEST(LINQ_View_Tests, JoinTest)
{
    // Average
    Employer person1("Tom", "Microsoft");
    Employer person2("Bob", "Google");
    Employer person3("Sam", "Microsoft");
    Employer person4("Alice", "Google");
    Employer person5("Jon", "Google");

    std::vector people { person1, person2, person3, person4, person5 };

    ItCompany company1("Microsoft", "C#");
    ItCompany company2("Google", "C++");

    std::vector companies { company1, company2 };

    // Act
    const std::vector employees = ExtendedCpp::LINQ::View(people)
            .Join(companies,
                  [](const Employer& employer){ return employer.CompanyName; },
                  [](const ItCompany& company){ return company.Name; },
                  [](const Employer& employer, const ItCompany& company){ return Employer(employer.Name, company.Name, company.Language); })
            .ToVector();

    // Asset
    ASSERT_EQ("C#", employees[0].Language);
    ASSERT_EQ("C++", employees[1].Language);
    ASSERT_EQ("C#", employees[2].Language);
    ASSERT_EQ("C++", employees[3].Language);
    ASSERT_EQ("C++", employees[4].Language);
}

TEST(LINQ_View_Tests, ZipTest)
{
    // Average
    const std::vector numbers1 { 4, 3, 6, 1, 5 };
    const std::vector numbers2 { 3, 2, 1, 4, 7 };

    // Act
    const std::vector result = ExtendedCpp::LINQ::View(numbers1)
            .Zip(numbers2)
            .ToVector();

    // Assert
    ASSERT_EQ(4, result[0].first);
    ASSERT_EQ(3, result[0].second);

    ASSERT_EQ(3, result[1].first);
    ASSERT_EQ(2, result[1].second);
}

TEST(LINQ_View_Tests, SkipTest)
{
    // Average
    const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

    // Act
    const std::vector result = ExtendedCpp::LINQ::View(numbers)
        .Skip(3)
        .ToVector();

    // Assert
    ASSERT_EQ(3, result.size());
    ASSERT_EQ(4, result[0]);
    ASSERT_EQ(5, result[1]);
    ASSERT_EQ(6, result[2]);
}

TEST(LINQ_View_Tests, SkipWhileTest)
{
    // Average
    const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

    // Act
    const std::vector result = ExtendedCpp::LINQ::View(numbers)
        .SkipWhile([](const int n) { return n < 4; })
        .ToVector();

    // Assert
    ASSERT_EQ(3, result.size());
    ASSERT_EQ(4, result[0]);
    ASSERT_EQ(5, result[1]);
    ASSERT_EQ(6, result[2]);
}

TEST(LINQ_View_Tests, TakeTest)
{
    // Average
    const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

    // Act
    const std::vector result = ExtendedCpp::LINQ::View(numbers)
        .Take(3)
        .ToVector();

    // Assert
    ASSERT_EQ(3, result.size());
    ASSERT_EQ(1, result[0]);
    ASSERT_EQ(2, result[1]);
    ASSERT_EQ(3, result[2]);
}

TEST(LINQ_View_Tests, TakeWhileTest)
{
    // Average
    const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

    // Act
    const std::vector result = ExtendedCpp::LINQ::View(numbers)
        .TakeWhile([](const int n) { return n < 4; })
        .ToVector();

    // Assert
    ASSERT_EQ(3, result.size());
    ASSERT_EQ(1, result[0]);
    ASSERT_EQ(2, result[1]);
    ASSERT_EQ(3, result[2]);
}

TEST(LINQ_View_Tests, EmptyCollectionTest)
{
    // Average
    const std::vector<char> emptyVector;

    // Act
    const auto linq = ExtendedCpp::LINQ::View(emptyVector);
    const auto emptyResultVector = linq
        .Where([](char){ return true; })
        .Select([](const char& element){ return element; })
        .ToVector();

    // Assert
    ASSERT_EQ(emptyResultVector.size(), 0);
}

TEST(LINQ_View_Tests, TransformTest)
{
    // Average
    const std::vector<int> numbers = { 1, 2, 3, 4 };
    const std::vector<int> assertVector = { 2, 4, 6, 8 };

    // Act
    const auto transformed = ExtendedCpp::LINQ::View(numbers)
        .Transform([](int& number){ number *= 2; })
        .ToVector();

    // Assert
    for (std::size_t i = 0; i < assertVector.size(); ++i)
        ASSERT_EQ(transformed[i], assertVector[i]);
}

TEST(LINQ_View_Tests, MapTest)
{
    // Average
    const std::vector<int> numbers = { 1, 2, 3, 4 };
    const std::vector<int> assertVector = { 2, 4, 6, 8 };

    // Act
    const auto mapped = ExtendedCpp::LINQ::View(numbers)
        .Map([](int number){ return number * 2; })
        .ToVector();

    // Assert
    for (std::size_t i = 0; i < assertVector.size(); ++i)
        ASSERT_EQ(mapped[i], assertVector[i]);
}