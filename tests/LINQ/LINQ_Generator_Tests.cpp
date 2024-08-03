#include <gtest/gtest.h>

#include <LINQ/LINQ.h>

#include "LINQ_Tests.h"

TEST(LINQ_Generator_Tests, YieldForeachTest)
{
    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);

    const std::vector people { person1, person2, person3, person4 };

    // Act
    auto future = LINQ::YieldForeach(people);

    // Assert
    while (future)
        ASSERT_FALSE(future.Next().Name.empty());
}

TEST(LINQ_Generator_Tests, SelectTest)
{
    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);

    const std::vector people { person1, person2, person3, person4 };

    // Act
    const std::vector names = LINQ::Generator(people)
            .Select([](const Person& person){ return person.Name; })
            .ToVector();

    // Assert
    ASSERT_EQ("Tom", names[0]);
    ASSERT_EQ("Bob", names[1]);
    ASSERT_EQ("Sam", names[2]);
    ASSERT_EQ("Alice", names[3]);
}

TEST(LINQ_Generator_Tests, SelectMany1Test)
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
    std::vector employees = LINQ::Generator(companies)
            .SelectMany([](const Company& company){ return company.People; })
            .ToVector();

    // Assert
    ASSERT_EQ(4, employees.size());

    ASSERT_EQ("Tom", employees[0].Name);
    ASSERT_EQ("Bob", employees[1].Name);
    ASSERT_EQ("Sam", employees[2].Name);
    ASSERT_EQ("Alice", employees[3].Name);
}

TEST(LINQ_Generator_Tests, SelectMany2Test)
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
    std::vector employees = LINQ::Generator(companies)
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

TEST(LINQ_Generator_Tests, WhereTest)
{
    // Average
    const std::vector names { "Tom", "Alice", "Bob", "Sam", "Tim", "Tomas", "Bill" };

    // Act
    const std::vector selectedNames = LINQ::Generator(names)
            .Where([](const std::string& name){ return name.size() == 3; })
            .ToVector();

    // Assert
    ASSERT_EQ(4, selectedNames.size());
}

TEST(LINQ_Generator_Tests, OrderByTest)
{
    // Average
    const std::vector numbers { 1, 5, 9, 2, 0, 145, 2 };

    // Act
    const std::vector sortedNumbers = LINQ::Generator(numbers)
            .Order(OrderType::DESC)
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

TEST(LINQ_Generator_Tests, OrderByWithSelectorTest)
{
    // Average
    Person person1("Tom", 23);
    Person person2("Bob", 27);
    Person person3("Sam", 29);
    Person person4("Alice", 24);

    std::vector people { person1, person2, person3, person4 };

    // Act
    std::vector sortedPeople = LINQ::Generator(people)
            .OrderBy([](const Person& person){ return person.Age; }, OrderType::ASC)
            .ToVector();

    // Assert
    ASSERT_EQ(23, sortedPeople[0].Age);
    ASSERT_EQ(24, sortedPeople[1].Age);
    ASSERT_EQ(27, sortedPeople[2].Age);
    ASSERT_EQ(29, sortedPeople[3].Age);

    // Act
    std::vector sortedAges = LINQ::Generator(people)
            .OrderBy([](const Person& person){ return person.Age; }, OrderType::ASC)
            .Select([](const Person& person){ return person.Age; })
            .ToVector();

    // Assert
    ASSERT_EQ(23, sortedAges[0]);
    ASSERT_EQ(24, sortedAges[1]);
    ASSERT_EQ(27, sortedAges[2]);
    ASSERT_EQ(29, sortedAges[3]);
}

TEST(LINQ_Generator_Tests, ExceptTest1)
{
    // Average
    const std::vector numbers1 { 4, 3, 6, 1, 5 };
    const std::vector numbers2 { 3, 2, 1, 4, 7 };

    // Act
    const std::vector result = LINQ::Generator(numbers1)
            .Except(numbers2)
            .ToVector();

    // Assert
    ASSERT_EQ(2, result.size());
}

TEST(LINQ_Generator_Tests, ExceptTest2)
{
    // Average
    const std::vector numbers1 { 4, 3, 6, 1, 5 };
    std::vector numbers2 { 3, 2, 1, 4, 7 };

    // Act
    const std::vector result = LINQ::Generator(numbers1)
            .Except(std::move(numbers2))
            .ToVector();

    // Assert
    ASSERT_EQ(2, result.size());
}

TEST(LINQ_Generator_Tests, IntersectTest1)
{
    // Average
    const std::vector numbers1 { 4, 3, 6, 1, 5 };
    const std::vector numbers2 { 3, 2, 1, 4, 7 };

    // Act
    const std::vector result = LINQ::Generator(numbers1)
            .Intersect(numbers2)
            .ToVector();

    // Assert
    ASSERT_EQ(3, result.size());
}

TEST(LINQ_Generator_Tests, IntersectTest2)
{
    // Average
    const std::vector numbers1 { 4, 3, 6, 1, 5 };
    std::vector numbers2 { 3, 2, 1, 4, 7 };

    // Act
    const std::vector result = LINQ::Generator(numbers1)
            .Intersect(std::move(numbers2))
            .ToVector();

    // Assert
    ASSERT_EQ(3, result.size());
}

TEST(LINQ_Generator_Tests, DistinctTest)
{
    // Average
    const std::vector numbers {1, 3, 6, 1, 6, 4};

    // Act
    const std::vector result = LINQ::Generator(numbers)
            .Distinct()
            .ToVector();

    // Assert
    ASSERT_EQ(4, result.size());
}

TEST(LINQ_Generator_Tests, UnionTest1)
{
    // Average
    const std::vector numbers1 { 4, 3, 6, 1, 5 };
    const std::vector numbers2 { 3, 2, 1, 4, 7 };

    // Act
    const std::vector result = LINQ::Generator(numbers1)
            .Union(numbers2)
            .ToVector();

    // Assert
    ASSERT_EQ(7, result.size());
}

TEST(LINQ_Generator_Tests, UnionTest2)
{
    // Average
    const std::vector numbers1 { 4, 3, 6, 1, 5 };
    std::vector numbers2 { 3, 2, 1, 4, 7 };

    // Act
    const std::vector result = LINQ::Generator(numbers1)
            .Union(std::move(numbers2))
            .ToVector();

    // Assert
    ASSERT_EQ(7, result.size());
}

TEST(LINQ_Generator_Tests, AggregateTest)
{
    // Average
    const std::vector numbers { 1, 2, 3, 4, 5 };

    // Act
    const int result = LINQ::Generator(numbers)
            .Aggregate<int>([](const int x, const int y){ return x * y; });

    // Assert
    ASSERT_EQ(120, result);
}

TEST(LINQ_Generator_Tests, CountPredicateTest)
{
    // Average
    const std::vector numbers { 1, 1, 2, 2, 3 };

    // Act
    const std::size_t result = LINQ::Generator(numbers)
            .Count([](const int n){ return n == 2; });

    // Assert
    ASSERT_EQ(2, result);
}

TEST(LINQ_Generator_Tests, SumTest)
{
    // Average
    const std::vector numbers { 1, 1, 2, 2, 3 };

    // Act
    const int result = LINQ::Generator(numbers)
            .Sum();

    // Assert
    ASSERT_EQ(9, result);
}

TEST(LINQ_Generator_Tests, SumWithSelectorTest)
{
    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);

    const std::vector people { person1, person2, person3, person4 };

    // Act
    const int result = LINQ::Generator(people)
            .Sum([](const Person& person){ return person.Age; });

    // Assert
    ASSERT_EQ(103, result);
}

TEST(LINQ_Generator_Tests, MinTest)
{
    // Average
    const std::vector numbers { 1, 1, 2, 2, 3 };

    // Act
    const int result = LINQ::Generator(numbers)
            .Min();

    // Assert
    ASSERT_EQ(1, result);
}

TEST(LINQ_Generator_Tests, MinWithSelectorTest)
{
    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);

    const std::vector people { person1, person2, person3, person4 };

    // Act
    const int result = LINQ::Generator(people)
            .Min([](const Person& person){ return person.Age; });

    // Assert
    ASSERT_EQ(23, result);
}

TEST(LINQ_Generator_Tests, MaxTest)
{
    // Average
    const std::vector numbers { 1, 1, 2, 2, 3 };

    // Act
    const int result = LINQ::Generator(numbers)
            .Max();

    // Assert
    ASSERT_EQ(3, result);
}

TEST(LINQ_Generator_Tests, MaxWithSelectorTest)
{
    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 29);
    const Person person4("Alice", 24);

    const std::vector people { person1, person2, person3, person4 };

    // Act
    const int result = LINQ::Generator(people)
            .Max([](const Person& person){ return person.Age; });

    // Assert
    ASSERT_EQ(29, result);
}

TEST(LINQ_Generator_Tests, AverageTest)
{
    // Average
    const std::vector numbers { 1, 1, 3, 3 };

    // Act
    const int result = LINQ::Generator(numbers)
            .Average();

    // Assert
    ASSERT_EQ(2, result);
}

TEST(LINQ_Generator_Tests, AverageWithSelectorTest)
{
    // Average
    const Person person1("Tom", 23);
    const Person person2("Bob", 27);
    const Person person3("Sam", 26);
    const Person person4("Alice", 24);

    const std::vector people { person1, person2, person3, person4 };

    // Act
    const int result = LINQ::Generator(people)
            .Average([](const Person& person){ return person.Age; });

    // Assert
    ASSERT_EQ(25, result);
}

TEST(LINQ_Generator_Tests, SkipTest)
{
    // Average
    const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

    // Act
    const std::vector result = LINQ::Generator(numbers)
            .Skip(3)
            .ToVector();

    // Assert
    ASSERT_EQ(3, result.size());
    ASSERT_EQ(4, result[0]);
    ASSERT_EQ(5, result[1]);
    ASSERT_EQ(6, result[2]);
}

TEST(LINQ_Generator_Tests, SkipWhileTest)
{
    // Average
    const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

    // Act
    const std::vector result = LINQ::Generator(numbers)
            .SkipWhile([](const int n){ return n < 4; })
            .ToVector();

    // Assert
    ASSERT_EQ(3, result.size());
    ASSERT_EQ(4, result[0]);
    ASSERT_EQ(5, result[1]);
    ASSERT_EQ(6, result[2]);
}

TEST(LINQ_Generator_Tests, TakeTest)
{
    // Average
    const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

    // Act
    const std::vector result = LINQ::Generator(numbers)
            .Take(3)
            .ToVector();

    // Assert
    ASSERT_EQ(3, result.size());
    ASSERT_EQ(1, result[0]);
    ASSERT_EQ(2, result[1]);
    ASSERT_EQ(3, result[2]);
}

TEST(LINQ_Generator_Tests, TakeWhileTest)
{
    // Average
    const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

    // Act
    const std::vector result = LINQ::Generator(numbers)
            .TakeWhile([](const int n){ return n < 4; })
            .ToVector();

    // Assert
    ASSERT_EQ(3, result.size());
    ASSERT_EQ(1, result[0]);
    ASSERT_EQ(2, result[1]);
    ASSERT_EQ(3, result[2]);
}

TEST(LINQ_Generator_Tests, GroupByTest)
{
    // Average
    Employer person1("Tom", "Microsoft");
    Employer person2("Bob", "Google");
    Employer person3("Sam", "Microsoft");
    Employer person4("Alice", "Google");
    Employer person5("Jon", "Google");

    std::vector people { person1, person2, person3, person4, person5 };

    // Act
    std::map companies = LINQ::Generator(people)
            .GroupBy([](const Employer& employer){ return employer.CompanyName; })
            .ToMap();

    // Assert
    ASSERT_EQ(2, companies.size());
    ASSERT_EQ(2, companies["Microsoft"].size());
    ASSERT_EQ(3, companies["Google"].size());
}

TEST(LINQ_Generator_Tests, JoinTest1)
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
    const std::vector employees = LINQ::Generator(people)
            .Join(companies,
                [](const Employer& employer){ return employer.CompanyName; },
                [](const ItCompany& company){ return company.Name; },
                [](const Employer& employer, const ItCompany& company){ return Employer(employer.Name, company.Name, company.Language); })
            .ToVector();

    // Asset
    ASSERT_EQ(employees.size(), 5);
    ASSERT_EQ("C#", employees[0].Language);
    ASSERT_EQ("C++", employees[1].Language);
    ASSERT_EQ("C#", employees[2].Language);
    ASSERT_EQ("C++", employees[3].Language);
    ASSERT_EQ("C++", employees[4].Language);
}

TEST(LINQ_Generator_Tests, JoinTest2)
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
    const std::vector employees = LINQ::Generator(people)
            .Join(std::move(companies),
                  [](const Employer& employer){ return employer.CompanyName; },
                  [](const ItCompany& company){ return company.Name; },
                  [](const Employer& employer, const ItCompany& company){ return Employer(employer.Name, company.Name, company.Language); })
            .ToVector();

    // Asset
    ASSERT_EQ(employees.size(), 5);
    ASSERT_EQ("C#", employees[0].Language);
    ASSERT_EQ("C++", employees[1].Language);
    ASSERT_EQ("C#", employees[2].Language);
    ASSERT_EQ("C++", employees[3].Language);
    ASSERT_EQ("C++", employees[4].Language);
}

TEST(LINQ_Generator_Tests, GroupJoinTest1)
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
    const std::vector result = LINQ::Generator(people)
            .GroupJoin(companies,
                [](const Employer& employer) { return employer.CompanyName; },
                [](const ItCompany& company) { return company.Name; },
                [](const std::vector<Employer>& emps, const ItCompany &company){ return ItCompany(company.Name, emps); })
            .ToVector();

    ASSERT_EQ(2, result.size());
    ASSERT_EQ(3, result[0].People.size());
    ASSERT_EQ(2, result[1].People.size());
}

TEST(LINQ_Generator_Tests, GroupJoinTest2)
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
    const std::vector result = LINQ::Generator(people)
            .GroupJoin(std::move(companies),
                       [](const Employer& employer) { return employer.CompanyName; },
                       [](const ItCompany& company) { return company.Name; },
                       [](const std::vector<Employer>& emps, const ItCompany &company){ return ItCompany(company.Name, emps); })
            .ToVector();

    ASSERT_EQ(2, result.size());
    ASSERT_EQ(3, result[0].People.size());
    ASSERT_EQ(2, result[1].People.size());
}

TEST(LINQ_Generator_Tests, ZipTest1)
{
    // Average
    const std::vector numbers1 { 4, 3, 6, 1, 5 };
    const std::vector numbers2 { 3, 2, 1, 4, 7 };

    // Act
    const std::vector result = LINQ::Generator(numbers1)
        .Zip(numbers2)
        .ToVector();

    // Assert
    ASSERT_EQ(4, result[0].first);
    ASSERT_EQ(3, result[0].second);

    ASSERT_EQ(3, result[1].first);
    ASSERT_EQ(2, result[1].second);
}

TEST(LINQ_Generator_Tests, ZipTest2)
{
    // Average
    const std::vector numbers1 { 4, 3, 6, 1, 5 };
    std::vector numbers2 { 3, 2, 1, 4, 7 };

    // Act
    const std::vector result = LINQ::Generator(numbers1)
            .Zip(std::move(numbers2))
            .ToVector();

    // Assert
    ASSERT_EQ(4, result[0].first);
    ASSERT_EQ(3, result[0].second);

    ASSERT_EQ(3, result[1].first);
    ASSERT_EQ(2, result[1].second);
}

TEST(LINQ_Generator_Tests, AllTest)
{
    // Average
    const std::vector numbers { 4, 3, 6, 1, 5 };

    // Act
    bool result = LINQ::Generator(numbers)
            .All([](const int i){ return i < 7; });

    // Assert
    ASSERT_TRUE(result);

    // Act
    result = LINQ::Generator(numbers)
            .All([](const int i){ return i < 3; });

    // Assert
    ASSERT_FALSE(result);
}

TEST(LINQ_Generator_Tests, AnyTest)
{
    // Average
    const std::vector numbers { 4, 3, 6, 1, 5 };

    // Act
    bool result = LINQ::Generator(numbers)
            .Any([](const int i){ return i < 2; });

    // Assert
    ASSERT_TRUE(result);

    // Act
    result = LINQ::Generator(numbers)
            .Any([](const int i){ return i > 7; });

    // Assert
    ASSERT_FALSE(result);
}

TEST(LINQ_Generator_Tests, FromContainerTest)
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
    auto linq1 = LINQ::Generator(numbersVector);
    auto linq2 = LINQ::Generator(numbersArray);
    auto linq3 = LINQ::Generator(numbersList);
    auto linq4 = LINQ::Generator(numbersForwardList);
    auto linq5 = LINQ::Generator(numbersStack);
    auto linq6 = LINQ::Generator(numbersQueue);
    auto linq7 = LINQ::Generator(numbersDequeue);
    auto linq8 = LINQ::Generator(numbersPriorityQueue);
    auto linq9 = LINQ::Generator(numbersSet);
    auto linq10 = LINQ::Generator(numbersUnorderedSet);
    auto linq11 = LINQ::Generator(numbersMap);
    auto linq12 = LINQ::Generator(numbersUnorderedMap);

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

    // Assert
    for (int i = 0; i < 7; ++i)
    {
        ASSERT_EQ(numbersVector[i], result1[i]);
        ASSERT_EQ(numbersVector[i], result2[i]);
        ASSERT_EQ(numbersVector[i], result3[i]);
        ASSERT_EQ(numbersVector[i], result4[i]);
        ASSERT_EQ(numbersVector[i], result7[i]);
    }
}

TEST(LINQ_Generator_Tests, ToContainerTest)
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

    auto linq1 = LINQ::Generator(numbers);
    auto linq2 = LINQ::Generator(numbers);
    auto linq3 = LINQ::Generator(numbers);
    auto linq4 = LINQ::Generator(numbers);
    auto linq5 = LINQ::Generator(numbers);
    auto linq6 = LINQ::Generator(numbers);
    auto linq7 = LINQ::Generator(numbers);
    auto linq8 = LINQ::Generator(numbers);
    auto linq9 = LINQ::Generator(numbers);
    auto linq10 = LINQ::Generator(numbers);
    auto linqPairs1 = LINQ::Generator(numberPairs);
    auto linqPairs2 = LINQ::Generator(numberPairs);

    // Act
    std::vector vectorResult = linq1.ToVector();
    std::array arrayResult = linq2.ToArray<7>();
    std::list listResult = linq3.ToList();
    std::forward_list forwardListResult = linq4.ToForwardList();
    std::stack stackResult = linq5.ToStack();
    std::queue queueResult = linq6.ToQueue();
    std::deque dequeResult = linq7.ToDeque();
    std::priority_queue priorityQueueResult = linq8.ToPriorityQueue();
    std::set setResult = linq9.ToSet();
    std::unordered_set unorderedSetResult = linq10.ToUnorderedSet();
    std::map mapResult = linqPairs1.ToMap();
    std::unordered_map unorderedMapResult = linqPairs2.ToUnorderedMap();

    // Assert
    ASSERT_TRUE(vectorResult.size() == 7);
    ASSERT_TRUE(arrayResult.size() == 7);
    ASSERT_TRUE(listResult.size() == 7);
    ASSERT_TRUE(stackResult.size() == 7);
    ASSERT_TRUE(queueResult.size() == 7);
    ASSERT_TRUE(dequeResult.size() == 7);
    ASSERT_TRUE(priorityQueueResult.size() == 7);
    ASSERT_TRUE(setResult.size() == 7);
    ASSERT_TRUE(unorderedSetResult.size() == 7);
    ASSERT_TRUE(mapResult.size() == 7);
    ASSERT_TRUE(unorderedMapResult.size() == 7);

    for (int i = 0; i < 7; ++i)
    {
        ASSERT_EQ(numbers[i], vectorResult[i]);
        ASSERT_EQ(numbers[i], arrayResult[i]);
        ASSERT_EQ(numbers[i], dequeResult[i]);
    }
}

TEST(LINQ_Generator_Tests, ReverseTest)
{
    // Average
    const std::vector numbers { 8, 7, 1, 9, 50, 0, 3 };

    // Act
    const std::vector reverseNumbers = LINQ::Generator(numbers).Reverse().ToVector();

    // Assert
    for (std::size_t i = 0, j = reverseNumbers.size() - 1; i < numbers.size(); ++i, --j)
        ASSERT_EQ(numbers[i], reverseNumbers[j]);
}