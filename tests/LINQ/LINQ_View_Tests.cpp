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