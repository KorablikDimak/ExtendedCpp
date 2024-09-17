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