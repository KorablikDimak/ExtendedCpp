#include <gtest/gtest.h>

#include <ExtendedCpp/LINQ.h>

#include "LINQ_Tests.h"

TEST(LINQ_Tests, SelectTest)
{
    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);

    const std::vector people { person1, person2, person3, person4 };

    // Act
    const std::vector names = ExtendedCpp::LINQ::From(people)
            .Select([](const Person& person){ return person.Name; })
            .ToVector();

    // Assert
    ASSERT_EQ("Tom", names[0]);
    ASSERT_EQ("Bob", names[1]);
    ASSERT_EQ("Sam", names[2]);
    ASSERT_EQ("Alice", names[3]);
}

TEST(LINQ_Tests, SelectMany1Test)
{
    // Average
    Person person1("Tom", 23);
    Person person2("Bob", 27);
    Person person3("Sam", 29);
    Person person4("Alice", 24);

    Company company1("Microsoft");
    company1.People.push_back(person1);
    company1.People.push_back(person2);

    Company company2("Google");
    company2.People.push_back(person3);
    company2.People.push_back(person4);

    std::vector companies { company1, company2 };

    // Act
    std::vector employees = ExtendedCpp::LINQ::From(companies)
            .SelectMany([](const Company& company){ return company.People; })
            .ToVector();

    // Assert
    ASSERT_EQ(4, employees.size());

    ASSERT_EQ("Tom", employees[0].Name);
    ASSERT_EQ("Bob", employees[1].Name);
    ASSERT_EQ("Sam", employees[2].Name);
    ASSERT_EQ("Alice", employees[3].Name);
}

TEST(LINQ_Tests, SelectMany2Test)
{
    // Average
    Person person1("Tom", 23);
    Person person2("Bob", 27);
    Person person3("Sam", 29);
    Person person4("Alice", 24);

    Company company1("Microsoft");
    company1.People.push_back(person1);
    company1.People.push_back(person2);

    Company company2("Google");
    company2.People.push_back(person3);
    company2.People.push_back(person4);

    std::vector companies { company1, company2 };

    // Act
    std::vector employees = ExtendedCpp::LINQ::From(companies)
            .SelectMany([](const Company& company){ return company.People; },
                [](const Company& company, const Person& person){ return Employer(person.Name, company.Name); })
            .ToVector();

    // Assert
    ASSERT_EQ(4, employees.size());

    ASSERT_EQ("Tom", employees[0].Name);
    ASSERT_EQ("Bob", employees[1].Name);
    ASSERT_EQ("Sam", employees[2].Name);
    ASSERT_EQ("Alice", employees[3].Name);

    ASSERT_EQ("Microsoft", employees[0].CompanyName);
    ASSERT_EQ("Microsoft", employees[1].CompanyName);
    ASSERT_EQ("Google", employees[2].CompanyName);
    ASSERT_EQ("Google", employees[3].CompanyName);
}

TEST(LINQ_Tests, WhereTest)
{
    // Average
    const std::vector names { "Tom", "Alice", "Bob", "Sam", "Tim", "Tomas", "Bill" };

    // Act
    const std::vector selectedNames = ExtendedCpp::LINQ::From(names)
            .Where([](const std::string& name){ return name.size() == 3; })
            .ToVector();

    // Assert
    ASSERT_EQ(4, selectedNames.size());
}

TEST(LINQ_Tests, RemoveWhereTest)
{
    // Average
    const std::vector names { "Tom", "Alice", "Bob", "Sam", "Tim", "Tomas", "Bill" };

    // Act
    const std::vector selectedNames = ExtendedCpp::LINQ::From(names)
            .RemoveWhere([](const std::string& name){ return name.size() == 3; })
            .ToVector();

    // Assert
    ASSERT_EQ(3, selectedNames.size());
}

TEST(LINQ_Tests, OrderByTest)
{
    // Average
    const std::vector numbers { 1, 5, 9, 2, 0, 145, 2 };

    // Act
    const std::vector sortedNumbers = ExtendedCpp::LINQ::From(numbers)
            .Order(ExtendedCpp::LINQ::OrderType::DESC)
            .ToVector();

    // Assert
    ASSERT_EQ(145, sortedNumbers[0]);
    ASSERT_EQ(9, sortedNumbers[1]);
    ASSERT_EQ(5, sortedNumbers[2]);
    ASSERT_EQ(2, sortedNumbers[3]);
    ASSERT_EQ(2, sortedNumbers[4]);
    ASSERT_EQ(1, sortedNumbers[5]);
    ASSERT_EQ(0, sortedNumbers[6]);
}

TEST(LINQ_Tests, OrderByWithSelectorTest)
{
    // Average
    Person person1("Tom", 23);
    Person person2("Bob", 27);
    Person person3("Sam", 29);
    Person person4("Alice", 24);

    std::vector people { person1, person2, person3, person4 };

    // Act
    std::vector sortedPeople = ExtendedCpp::LINQ::From(people)
            .OrderBy([](const Person& person){ return person.Age; }, ExtendedCpp::LINQ::OrderType::ASC)
            .ToVector();

    // Assert
    ASSERT_EQ(23, sortedPeople[0].Age);
    ASSERT_EQ(24, sortedPeople[1].Age);
    ASSERT_EQ(27, sortedPeople[2].Age);
    ASSERT_EQ(29, sortedPeople[3].Age);

    // Act
    std::vector sortedAges = ExtendedCpp::LINQ::From(people)
            .OrderBy([](const Person& person){ return person.Age; }, ExtendedCpp::LINQ::OrderType::ASC)
            .Select([](const Person& person){ return person.Age; })
            .ToVector();

    // Assert
    ASSERT_EQ(23, sortedAges[0]);
    ASSERT_EQ(24, sortedAges[1]);
    ASSERT_EQ(27, sortedAges[2]);
    ASSERT_EQ(29, sortedAges[3]);
}

TEST(LINQ_Tests, ExceptTest)
{
    // Average
    const std::vector numbers1 { 4, 3, 6, 1, 5 };
    const std::vector numbers2 { 3, 2, 1, 4, 7 };

    // Act
    const std::vector result = ExtendedCpp::LINQ::From(numbers1)
            .Except(numbers2)
            .ToVector();

    // Assert
    ASSERT_EQ(2, result.size());
}

TEST(LINQ_Tests, IntersectTest)
{
    // Average
    const std::vector numbers1 { 4, 3, 6, 1, 5 };
    const std::vector numbers2 { 3, 2, 1, 4, 7 };

    // Act
    const std::vector result = ExtendedCpp::LINQ::From(numbers1)
            .Intersect(numbers2)
            .ToVector();

    // Assert
    ASSERT_EQ(3, result.size());
}

TEST(LINQ_Tests, DistinctTest)
{
    // Average
    const std::vector numbers {1, 3, 6, 1, 6, 4};

    // Act
    const std::vector result = ExtendedCpp::LINQ::From(numbers)
            .Distinct()
            .ToVector();

    // Assert
    ASSERT_EQ(4, result.size());
}

TEST(LINQ_Tests, UnionTest)
{
    // Average
    const std::vector numbers1 { 4, 3, 6, 1, 5 };
    const std::vector numbers2 { 3, 2, 1, 4, 7 };

    // Act
    const std::vector result = ExtendedCpp::LINQ::From(numbers1)
            .Union(numbers2)
            .ToVector();

    // Assert
    ASSERT_EQ(7, result.size());
}

TEST(LINQ_Tests, AggregateTest)
{
    // Average
    const std::vector numbers { 1, 2, 3, 4, 5 };

    // Act
    const int result = ExtendedCpp::LINQ::From(numbers)
            .Aggregate<int>([](const int x, const int y){ return x * y; });

    // Assert
    ASSERT_EQ(120, result);
}

TEST(LINQ_Tests, CountTest)
{
    // Average
    const std::vector numbers { 1, 1, 2, 2, 3 };

    // Act
    const std::size_t result = ExtendedCpp::LINQ::From(numbers)
            .Count();

    // Assert
    ASSERT_EQ(5, result);
}

TEST(LINQ_Tests, CountPredicateTest)
{
    // Average
    const std::vector numbers { 1, 1, 2, 2, 3 };

    // Act
    const std::size_t result = ExtendedCpp::LINQ::From(numbers)
            .Count([](const int n){ return n == 2; });

    // Assert
    ASSERT_EQ(2, result);
}

TEST(LINQ_Tests, SumTest)
{
    // Average
    const std::vector numbers { 1, 1, 2, 2, 3 };

    // Act
    const int result = ExtendedCpp::LINQ::From(numbers)
            .Sum();

    // Assert
    ASSERT_EQ(9, result);
}

TEST(LINQ_Tests, SumWithSelectorTest)
{
    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);

    const std::vector people { person1, person2, person3, person4 };

    // Act
    const int result = ExtendedCpp::LINQ::From(people)
            .Sum([](const Person& person){ return person.Age; });

    // Assert
    ASSERT_EQ(103, result);
}

TEST(LINQ_Tests, MinTest)
{
    // Average
    const std::vector numbers { 1, 1, 2, 2, 3 };

    // Act
    const int result = ExtendedCpp::LINQ::From(numbers)
            .Min();

    // Assert
    ASSERT_EQ(1, result);
}

TEST(LINQ_Tests, MinWithSelectorTest)
{
    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);

    const std::vector people { person1, person2, person3, person4 };

    // Act
    const int result = ExtendedCpp::LINQ::From(people)
            .Min([](const Person& person){ return person.Age; });

    // Assert
    ASSERT_EQ(23, result);
}

TEST(LINQ_Tests, MaxTest)
{
    // Average
    const std::vector numbers { 1, 1, 2, 2, 3 };

    // Act
    const int result = ExtendedCpp::LINQ::From(numbers)
            .Max();

    // Assert
    ASSERT_EQ(3, result);
}

TEST(LINQ_Tests, MaxWithSelectorTest)
{
    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);

    const std::vector people { person1, person2, person3, person4 };

    // Act
    const int result = ExtendedCpp::LINQ::From(people)
            .Max([](const Person& person){ return person.Age; });

    // Assert
    ASSERT_EQ(29, result);
}

TEST(LINQ_Tests, AverageTest)
{
    // Average
    const std::vector numbers { 1, 1, 3, 3 };

    // Act
    const int result = ExtendedCpp::LINQ::From(numbers)
            .Average();

    // Assert
    ASSERT_EQ(2, result);
}

TEST(LINQ_Tests, AverageWithSelectorTest)
{
    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 26);
    const Person person4("Alice", 24);

    const std::vector people { person1, person2, person3, person4 };

    // Act
    const int result = ExtendedCpp::LINQ::From(people)
            .Average([](const Person& person){ return person.Age; });

    // Assert
    ASSERT_EQ(25, result);
}

TEST(LINQ_Tests, FirstTest)
{
    // Average
    const std::vector numbers{ 9, 2, 3, 4, 5, 6 };

    // Act
    const int result = ExtendedCpp::LINQ::From(numbers)
            .First();

    // Assert
    ASSERT_EQ(9, result);
}

TEST(LINQ_Tests, FirstPredicateTest)
{
    // Average
    const std::vector numbers{ 9, 8, 3, 4, 5, 6 };

    // Act
    const int result = ExtendedCpp::LINQ::From(numbers)
            .First([](const int n){ return n < 4; });

    // Assert
    ASSERT_EQ(3, result);
}

TEST(LINQ_Tests, FirstOrDefaultTest)
{
    // Average
    const std::vector<int> numbers;

    // Act
    const int result = ExtendedCpp::LINQ::From(numbers)
            .FirstOrDefault(4);

    // Assert
    ASSERT_EQ(4, result);
}

TEST(LINQ_Tests, FirstOrDefaultPredicateTest)
{
    // Average
    const std::vector numbers{ 9, 8, 3, 4, 5, 6 };

    // Act
    const int result = ExtendedCpp::LINQ::From(numbers)
            .FirstOrDefault([](const int n){ return n < 3; }, 100);

    // Assert
    ASSERT_EQ(100, result);
}

TEST(LINQ_Tests, LastTest)
{
    // Average
    const std::vector numbers{ 9, 2, 3, 4, 5, 6 };

    // Act
    const int result = ExtendedCpp::LINQ::From(numbers)
            .Last();

    // Assert
    ASSERT_EQ(6, result);
}

TEST(LINQ_Tests, LastPredicateTest)
{
    // Average
    const std::vector numbers{ 9, 8, 3, 4, 5, 6 };

    // Act
    const int result = ExtendedCpp::LINQ::From(numbers)
            .Last([](const int n){ return n < 6; });

    // Assert
    ASSERT_EQ(5, result);
}

TEST(LINQ_Tests, LastOrDefaultTest)
{
    // Average
    const std::vector<int> numbers;

    // Act
    const int result = ExtendedCpp::LINQ::From(numbers)
            .LastOrDefault(4);

    // Assert
    ASSERT_EQ(4, result);
}

TEST(LINQ_Tests, LastOrDefaultPredicateTest)
{
    // Average
    const std::vector numbers{ 9, 8, 3, 4, 5, 6 };

    // Act
    const int result = ExtendedCpp::LINQ::From(numbers)
            .LastOrDefault([](const int n){ return n < 3; }, 100);

    // Assert
    ASSERT_EQ(100, result);
}

TEST(LINQ_Tests, SkipTest)
{
    // Average
    const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

    // Act
    const std::vector result = ExtendedCpp::LINQ::From(numbers)
            .Skip(3)
            .ToVector();

    // Assert
    ASSERT_EQ(3, result.size());
    ASSERT_EQ(4, result[0]);
    ASSERT_EQ(5, result[1]);
    ASSERT_EQ(6, result[2]);
}

TEST(LINQ_Tests, SkipLastTest)
{
    // Average
    const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

    // Act
    const std::vector result = ExtendedCpp::LINQ::From(numbers)
            .SkipLast(3)
            .ToVector();

    // Assert
    ASSERT_EQ(3, result.size());
    ASSERT_EQ(1, result[0]);
    ASSERT_EQ(2, result[1]);
    ASSERT_EQ(3, result[2]);
}

TEST(LINQ_Tests, SkipWhileTest)
{
    // Average
    const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

    // Act
    const std::vector result = ExtendedCpp::LINQ::From(numbers)
            .SkipWhile([](const int n){ return n < 4; })
            .ToVector();

    // Assert
    ASSERT_EQ(3, result.size());
    ASSERT_EQ(4, result[0]);
    ASSERT_EQ(5, result[1]);
    ASSERT_EQ(6, result[2]);
}

TEST(LINQ_Tests, TakeTest)
{
    // Average
    const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

    // Act
    const std::vector result = ExtendedCpp::LINQ::From(numbers)
            .Take(3)
            .ToVector();

    // Assert
    ASSERT_EQ(3, result.size());
    ASSERT_EQ(1, result[0]);
    ASSERT_EQ(2, result[1]);
    ASSERT_EQ(3, result[2]);
}

TEST(LINQ_Tests, TakeLastTest)
{
    // Average
    const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

    // Act
    const std::vector result = ExtendedCpp::LINQ::From(numbers)
            .TakeLast(3)
            .ToVector();

    // Assert
    ASSERT_EQ(3, result.size());
    ASSERT_EQ(4, result[0]);
    ASSERT_EQ(5, result[1]);
    ASSERT_EQ(6, result[2]);
}

TEST(LINQ_Tests, TakeWhileTest)
{
    // Average
    const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

    // Act
    const std::vector result = ExtendedCpp::LINQ::From(numbers)
            .TakeWhile([](const int n){ return n < 4; })
            .ToVector();

    // Assert
    ASSERT_EQ(3, result.size());
    ASSERT_EQ(1, result[0]);
    ASSERT_EQ(2, result[1]);
    ASSERT_EQ(3, result[2]);
}

TEST(LINQ_Tests, GroupByTest)
{
    // Average
    Employer person1("Tom", "Microsoft");
    Employer person2("Bob", "Google");
    Employer person3("Sam", "Microsoft");
    Employer person4("Alice", "Google");
    Employer person5("Jon", "Google");

    std::vector people { person1, person2, person3, person4, person5 };

    // Act
    std::map companies = ExtendedCpp::LINQ::From(people)
            .GroupBy([](const Employer& employer){ return employer.CompanyName; });

    // Assert
    ASSERT_EQ(2, companies.size());
    ASSERT_EQ(2, companies["Microsoft"].size());
    ASSERT_EQ(3, companies["Google"].size());
}

TEST(LINQ_Tests, JoinTest)
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
    const std::vector employees = ExtendedCpp::LINQ::From(people)
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

TEST(LINQ_Tests, GroupJoinTest)
{
    // Average
    Employer person1("Tom", "Microsoft");
    Employer person2("Bob", "Google");
    Employer person3("Sam", "Microsoft");
    Employer person4("Alice", "Google");
    Employer person5("Jon", "Google");

    std::vector people{person1, person2, person3, person4, person5};

    ItCompany company1("Microsoft", "C#");
    ItCompany company2("Google", "C++");

    std::vector companies{company1, company2};

    // Act
    const std::vector result = ExtendedCpp::LINQ::From(people)
            .GroupJoin(companies,
                [](const Employer& employer) { return employer.CompanyName; },
                [](const ItCompany& company) { return company.Name; },
                [](const std::vector<Employer>& emps, const ItCompany &company){ return ItCompany(company.Name, emps); })
            .ToVector();

    ASSERT_EQ(2, result.size());
    ASSERT_EQ(3, result[0].People.size());
    ASSERT_EQ(2, result[1].People.size());
}

TEST(LINQ_Tests, ZipTest)
{
    // Average
    const std::vector numbers1 { 4, 3, 6, 1, 5 };
    const std::vector numbers2 { 3, 2, 1, 4, 7 };

    // Act
    const std::vector result = ExtendedCpp::LINQ::From(numbers1)
        .Zip(numbers2)
        .ToVector();

    // Assert
    ASSERT_EQ(4, result[0].first);
    ASSERT_EQ(3, result[0].second);

    ASSERT_EQ(3, result[1].first);
    ASSERT_EQ(2, result[1].second);
}

TEST(LINQ_Tests, AllTest)
{
    // Average
    const std::vector numbers { 4, 3, 6, 1, 5 };

    // Act
    bool result = ExtendedCpp::LINQ::From(numbers)
            .All([](const int i){ return i < 7; });

    // Assert
    ASSERT_TRUE(result);

    // Act
    result = ExtendedCpp::LINQ::From(numbers)
            .All([](const int i){ return i < 3; });

    // Assert
    ASSERT_FALSE(result);
}

TEST(LINQ_Tests, AnyTest)
{
    // Average
    const std::vector numbers { 4, 3, 6, 1, 5 };

    // Act
    bool result = ExtendedCpp::LINQ::From(numbers)
            .Any([](const int i){ return i < 2; });

    // Assert
    ASSERT_TRUE(result);

    // Act
    result = ExtendedCpp::LINQ::From(numbers)
            .Any([](const int i){ return i > 7; });

    // Assert
    ASSERT_FALSE(result);
}

TEST(LINQ_Tests, ContainsTest)
{
    // Average
    const std::vector numbers { 4, 3, 6, 1, 5 };

    // Act
    bool result = ExtendedCpp::LINQ::From(numbers)
            .Contains(4);

    // Assert
    ASSERT_TRUE(result);

    // Act
    result = ExtendedCpp::LINQ::From(numbers)
            .Contains(7);

    // Assert
    ASSERT_FALSE(result);
}

TEST(LINQ_Tests, ContainsSubCollectionTest)
{
    // Average
    const std::vector numbers { 4, 3, 6, 1, 5 };
    const std::vector subCollection1 { 3, 6, 1 };
    const std::vector subCollection2 { 3, 6, 5 };

    // Act
    bool result = ExtendedCpp::LINQ::From(numbers)
            .Contains(subCollection1);

    // Assert
    ASSERT_TRUE(result);

    // Act
    result = ExtendedCpp::LINQ::From(numbers)
            .Contains(subCollection2);

    // Assert
    ASSERT_FALSE(result);
}

TEST(LINQ_Tests, FromContainerTest)
{
    // Average
    const std::vector numbersVector { 8, 7, 1, 9, 50, 0, 3 };
    constexpr std::array numbersArray { 8, 7, 1, 9, 50, 0, 3 };
    const std::list numbersList { 8, 7, 1, 9, 50, 0, 3 };
    const std::forward_list numbersForwardList { 8, 7, 1, 9, 50, 0, 3 };

    std::stack<int> numbersStack;
    numbersStack.push(8);
    numbersStack.push(7);
    numbersStack.push(1);
    numbersStack.push(9);
    numbersStack.push(50);
    numbersStack.push(0);
    numbersStack.push(3);

    std::queue<int> numbersQueue;
    numbersQueue.push(8);
    numbersQueue.push(7);
    numbersQueue.push(1);
    numbersQueue.push(9);
    numbersQueue.push(50);
    numbersQueue.push(0);
    numbersQueue.push(3);

    std::deque numbersDequeue { 8, 7, 1, 9, 50, 0, 3 };

    std::priority_queue<int> numbersPriorityQueue;
    numbersPriorityQueue.push(8);
    numbersPriorityQueue.push(7);
    numbersPriorityQueue.push(1);
    numbersPriorityQueue.push(9);
    numbersPriorityQueue.push(50);
    numbersPriorityQueue.push(0);
    numbersPriorityQueue.push(3);

    std::set numbersSet { 8, 7, 1, 9, 50, 0, 3 };
    std::unordered_set numbersUnorderedSet { 8, 7, 1, 9, 50, 0, 3 };

    std::map<int, int> numbersMap;
    numbersMap.insert(std::make_pair(8, 8));
    numbersMap.insert(std::make_pair(7, 7));
    numbersMap.insert(std::make_pair(1, 1));
    numbersMap.insert(std::make_pair(9, 9));
    numbersMap.insert(std::make_pair(50, 50));
    numbersMap.insert(std::make_pair(0, 0));
    numbersMap.insert(std::make_pair(3, 3));

    std::unordered_map<int, int> numbersUnorderedMap;
    numbersUnorderedMap.insert(std::make_pair(8, 8));
    numbersUnorderedMap.insert(std::make_pair(7, 7));
    numbersUnorderedMap.insert(std::make_pair(1, 1));
    numbersUnorderedMap.insert(std::make_pair(9, 9));
    numbersUnorderedMap.insert(std::make_pair(50, 50));
    numbersUnorderedMap.insert(std::make_pair(0, 0));
    numbersUnorderedMap.insert(std::make_pair(3, 3));

    // Act
    auto linq1 = ExtendedCpp::LINQ::From(numbersVector);
    auto linq2 = ExtendedCpp::LINQ::From(numbersArray);
    auto linq3 = ExtendedCpp::LINQ::From(numbersList);
    auto linq4 = ExtendedCpp::LINQ::From(numbersForwardList);
    auto linq5 = ExtendedCpp::LINQ::From(numbersStack);
    auto linq6 = ExtendedCpp::LINQ::From(numbersQueue);
    auto linq7 = ExtendedCpp::LINQ::From(numbersDequeue);
    auto linq8 = ExtendedCpp::LINQ::From(numbersPriorityQueue);
    auto linq9 = ExtendedCpp::LINQ::From(numbersSet);
    auto linq10 = ExtendedCpp::LINQ::From(numbersUnorderedSet);
    auto linq11 = ExtendedCpp::LINQ::From(numbersMap);
    auto linq12 = ExtendedCpp::LINQ::From(numbersUnorderedMap);
    auto linq13 = ExtendedCpp::LINQ::From(linq1.cbegin(), linq1.cend());
    auto linq14 = ExtendedCpp::LINQ::From(linq1);

    std::vector result1 = linq1.ToVector();
    std::vector result2 = linq2.ToVector();
    std::vector result3 = linq3.ToVector();
    std::vector result4 = linq4.ToVector();
    std::vector result5 = linq5.ToVector();
    std::vector result6 = linq6.ToVector();
    std::vector result7 = linq7.ToVector();
    std::vector result8 = linq8.ToVector();
    std::vector result9 = linq9.ToVector();
    std::vector result10 = linq10.ToVector();
    std::vector result11 = linq11.ToVector();
    std::vector result12 = linq12.ToVector();
    std::vector result13 = linq13.ToVector();
    std::vector result14 = linq14.ToVector();

    // Assert
    for (int i = 0; i < 7; ++i)
    {
        ASSERT_EQ(numbersVector[i], result1[i]);
        ASSERT_EQ(numbersVector[i], result2[i]);
        ASSERT_EQ(numbersVector[i], result3[i]);
        ASSERT_EQ(numbersVector[i], result4[i]);
        ASSERT_EQ(numbersVector[i], result7[i]);
        ASSERT_EQ(numbersVector[i], result13[i]);
        ASSERT_EQ(numbersVector[i], result14[i]);
    }

    for (int number : result5)
        ASSERT_TRUE(linq1.Contains(number));

    for (int number : result6)
        ASSERT_TRUE(linq1.Contains(number));

    for (int number : result8)
        ASSERT_TRUE(linq1.Contains(number));

    for (int number : result9)
        ASSERT_TRUE(linq1.Contains(number));

    for (int number : result10)
        ASSERT_TRUE(linq1.Contains(number));

    for (std::pair<int, int> pair : result11)
        ASSERT_TRUE(linq1.Contains(pair.first));

    for (std::pair<int, int> pair : result12)
        ASSERT_TRUE(linq1.Contains(pair.first));
}

TEST(LINQ_Tests, ToContainerTest)
{
    // Average
    std::vector numbers { 8, 7, 1, 9, 50, 0, 3 };
    std::vector<std::pair<int, int>> numberPairs;
    numberPairs.emplace_back(8, 8);
    numberPairs.emplace_back(7, 7);
    numberPairs.emplace_back(1, 1);
    numberPairs.emplace_back(9, 9);
    numberPairs.emplace_back(50, 50);
    numberPairs.emplace_back(0, 0);
    numberPairs.emplace_back(3, 3);
    auto linq = ExtendedCpp::LINQ::From(numbers);
    auto linqPairs = ExtendedCpp::LINQ::From(numberPairs);

    // Act
    std::vector vectorResult = linq.ToVector();
    std::array arrayResult = linq.ToArray<7>();
    std::list listResult = linq.ToList();
    std::forward_list forwardListResult = linq.ToForwardList();
    std::stack stackResult = linq.ToStack();
    std::queue queueResult = linq.ToQueue();
    std::deque dequeResult = linq.ToDeque();
    std::priority_queue priorityQueueResult = linq.ToPriorityQueue();
    std::set setResult = linq.ToSet();
    std::unordered_set unorderedSetResult = linq.ToUnorderedSet();
    std::map mapResult = linqPairs.ToMap();
    std::unordered_map unorderedMapResult = linqPairs.ToUnorderedMap();

    // Assert
    for (int i = 0; i < 7; ++i)
    {
        ASSERT_EQ(numbers[i], vectorResult[i]);
        ASSERT_EQ(numbers[i], arrayResult[i]);

        ASSERT_TRUE(linq.Contains(stackResult.top()));
        stackResult.pop();

        ASSERT_TRUE(linq.Contains(queueResult.front()));
        queueResult.pop();

        ASSERT_TRUE(linq.Contains(priorityQueueResult.top()));
        priorityQueueResult.pop();
    }

    for (int number : listResult)
        ASSERT_TRUE(linq.Contains(number));

    for (int number : forwardListResult)
        ASSERT_TRUE(linq.Contains(number));

    for (int number : dequeResult)
        ASSERT_TRUE(linq.Contains(number));

    for (int number : setResult)
        ASSERT_TRUE(linq.Contains(number));

    for (int number : unorderedSetResult)
        ASSERT_TRUE(linq.Contains(number));

    for (auto pair : mapResult)
        ASSERT_TRUE(linq.Contains(pair.first));

    for (auto pair : unorderedMapResult)
        ASSERT_TRUE(linq.Contains(pair.first));
}

TEST(LINQ_Tests, ReverseTest)
{
    // Average
    const std::vector numbers { 8, 7, 1, 9, 50, 0, 3 };

    // Act
    const std::vector reverseNumbers = ExtendedCpp::LINQ::From(numbers).Reverse().ToVector();

    // Assert
    for (std::size_t i = 0, j = reverseNumbers.size() - 1; i < numbers.size(); ++i, --j)
        ASSERT_EQ(numbers[i], reverseNumbers[j]);
}

TEST(LINQ_Tests, AtTest)
{
    // Average
    const std::vector numbers { 8, 7, 1, 9, 50, 0, 3 };

    // Act
    const std::size_t index = ExtendedCpp::LINQ::From(numbers).At(4);

    // Assert
    ASSERT_EQ(50, index);
}

TEST(LINQ_Tests, IndexAtTest)
{
    // Average
    const std::vector numbers { 8, 7, 1, 9, 50, 0, 3 };

    // Act
    const std::size_t number = ExtendedCpp::LINQ::From(numbers).IndexAt(50);
    const std::size_t npos = ExtendedCpp::LINQ::From(numbers).IndexAt(500);

    // Assert
    ASSERT_EQ(4, number);
    ASSERT_EQ(ExtendedCpp::LINQ::NPOS, npos);
}

TEST(LINQ_Tests, PushBackTest)
{
    // Average
    const std::vector numbers { 8, 7, 1, 9, 50, 0, 3 };
    const std::vector addNumbers { 4, 2, 10 };

    const std::vector checkVector1 { 8, 7, 1, 9, 50, 0, 3, 11 };
    const std::vector checkVector2 { 8, 7, 1, 9, 50, 0, 3, 4, 2, 10 };

    // Act
    const std::vector result1 = ExtendedCpp::LINQ::From(numbers).PushBack(11).ToVector();
    const std::vector result2 = ExtendedCpp::LINQ::From(numbers).PushBack(addNumbers).ToVector();

    // Assert
    for (int i = 0; i < checkVector1.size(); ++i)
        ASSERT_EQ(checkVector1[i], result1[i]);

    for (int i = 0; i < checkVector2.size(); ++i)
        ASSERT_EQ(checkVector2[i], result2[i]);
}

TEST(LINQ_Tests, PushFrontTest)
{
    // Average
    const std::vector numbers { 8, 7, 1, 9, 50, 0, 3 };
    const std::vector addNumbers { 4, 2, 10 };

    const std::vector checkVector1 { 11, 8, 7, 1, 9, 50, 0, 3 };
    const std::vector checkVector2 { 4, 2, 10, 8, 7, 1, 9, 50, 0, 3 };

    // Act
    const std::vector result1 = ExtendedCpp::LINQ::From(numbers).PushFront(11).ToVector();
    const std::vector result2 = ExtendedCpp::LINQ::From(numbers).PushFront(addNumbers).ToVector();

    // Assert
    for (int i = 0; i < checkVector1.size(); ++i)
        ASSERT_EQ(checkVector1[i], result1[i]);

    for (int i = 0; i < checkVector2.size(); ++i)
        ASSERT_EQ(checkVector2[i], result2[i]);
}

TEST(LINQ_Tests, InsertTest)
{
    // Average
    const std::vector numbers { 8, 7, 1, 9, 50, 0, 3 };
    const std::vector addNumbers { 4, 2, 10 };

    const std::vector checkVector1 { 8, 7, 1, 11, 9, 50, 0, 3 };
    const std::vector checkVector2 { 8, 7, 1, 4, 2, 10, 9, 50, 0, 3 };

    // Act
    const std::vector result1 = ExtendedCpp::LINQ::From(numbers).Insert(11, 3).ToVector();
    const std::vector result2 = ExtendedCpp::LINQ::From(numbers).Insert(addNumbers, 3).ToVector();

    // Assert
    for (int i = 0; i < checkVector1.size(); ++i)
        ASSERT_EQ(checkVector1[i], result1[i]);

    for (int i = 0; i < checkVector2.size(); ++i)
        ASSERT_EQ(checkVector2[i], result2[i]);
}

TEST(LINQ_Tests, EraseTest)
{
    // Average
    const std::vector numbers { 8, 7, 1, 9, 50, 0, 3 };

    const std::vector checkVector1 { 8, 7, 1, 9, 0, 3 };
    const std::vector checkVector2 { 8, 7, 3 };

    // Act
    const std::vector result1 = ExtendedCpp::LINQ::From(numbers).Erase(4).ToVector();
    const std::vector result2 = ExtendedCpp::LINQ::From(numbers).Erase(2, 5).ToVector();

    // Assert
    for (int i = 0; i < checkVector1.size(); ++i)
        ASSERT_EQ(checkVector1[i], result1[i]);

    for (int i = 0; i < checkVector2.size(); ++i)
        ASSERT_EQ(checkVector2[i], result2[i]);
}

TEST(LINQ_Tests, ForeachTest)
{
    // Average
    const std::vector numbers { 8, 7, 1, 9, 50, 6, 3 };

    // Act
    const auto clinq = ExtendedCpp::LINQ::From(numbers);
    auto linq = ExtendedCpp::LINQ::From(numbers);

    // Assert
    for (const auto& element : clinq)
        ASSERT_TRUE(element > 0);

    for (auto& element : linq)
        ASSERT_TRUE(element > 0);
}