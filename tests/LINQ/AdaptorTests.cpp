#include <gtest/gtest.h>

#include <ExtendedCpp/LINQ.h>

#include "LINQ_Tests.h"

TEST(AdaptorTests, SelectAdaptorTest)
{
	// Average
	const std::vector data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

	// Act
	const auto result1 =
		ExtendedCpp::LINQ::From(data) |
		ExtendedCpp::LINQ::Select([](const int value){ return value * 2; }) |
		ExtendedCpp::LINQ::To<std::vector>();

	// Assert
	for (std::size_t i = 0; i < data.size(); ++i)
		ASSERT_EQ(result1[i], data[i] * 2);

	// Act
	const auto result2 =
		ExtendedCpp::LINQ::Generator(data) |
		ExtendedCpp::LINQ::Select([](const int value){ return value * 2; }) |
		ExtendedCpp::LINQ::To<std::vector>();

	// Assert
	for (std::size_t i = 0; i < data.size(); ++i)
		ASSERT_EQ(result2[i], data[i] * 2);

	// Act
	const auto result3 =
		ExtendedCpp::LINQ::View(data) |
		ExtendedCpp::LINQ::Select([](const int value){ return value * 2; }) |
		ExtendedCpp::LINQ::To<std::vector>();

	// Assert
	for (std::size_t i = 0; i < data.size(); ++i)
		ASSERT_EQ(result3[i], data[i] * 2);
}

TEST(AdaptorTests, ConvertToTest)
{
	// Average
	const std::vector data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

	// Act
	const auto result1 =
		data |
		ExtendedCpp::LINQ::To<ExtendedCpp::LINQ::LinqContainer>();

	auto result2 =
		result1 |
		ExtendedCpp::LINQ::To<ExtendedCpp::LINQ::LinqGenerator>();

	const auto result3 =
		result1 |
		ExtendedCpp::LINQ::To<ExtendedCpp::LINQ::LinqView>();

	const auto vector1 = result1.ToVector();
	const auto vector2 = result2.ToVector();
	const auto vector3 = result3.ToVector();

	// Assert
	for (std::size_t i = 0; i < data.size(); ++i)
	{
		ASSERT_EQ(vector1[i], data[i]);
		ASSERT_EQ(vector2[i], data[i]);
		ASSERT_EQ(vector3[i], data[i]);
	}
}

TEST(AdaptorTests, ConvertLinqContainerToStdContainerTest)
{
	// Average
	const std::vector data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	const auto linq = ExtendedCpp::LINQ::From(data);

	// Act
	const auto result1_1 = linq | ExtendedCpp::LINQ::To<std::vector>();
	const auto result1_2 = linq | ExtendedCpp::LINQ::To<std::list>();
	const auto result1_3 = linq | ExtendedCpp::LINQ::To<std::forward_list>();
	const auto result1_4 = linq | ExtendedCpp::LINQ::To<std::set>();
	const auto result1_5 = linq | ExtendedCpp::LINQ::To<std::unordered_set>();
	auto result1_6 = linq | ExtendedCpp::LINQ::To<std::queue>();
	const auto result1_7 = linq | ExtendedCpp::LINQ::To<std::deque>();
	auto result1_8 = linq | ExtendedCpp::LINQ::To<std::priority_queue>();
	auto result1_9 = linq | ExtendedCpp::LINQ::To<std::stack>();
	const auto result1_10 = linq | ExtendedCpp::LINQ::ToArray<16>();

	// Assert
	ASSERT_EQ(result1_1.size(), data.size());
	ASSERT_EQ(result1_2.size(), data.size());
	ASSERT_EQ(result1_4.size(), data.size());
	ASSERT_EQ(result1_5.size(), data.size());
	ASSERT_EQ(result1_6.size(), data.size());
	ASSERT_EQ(result1_7.size(), data.size());
	ASSERT_EQ(result1_8.size(), data.size());
	ASSERT_EQ(result1_9.size(), data.size());
	ASSERT_EQ(result1_10.size(), data.size());

	for (std::size_t i = 0; i < data.size(); ++i)
	{
		ASSERT_EQ(data[i], result1_1[i]);
		ASSERT_EQ(data[i], result1_7[i]);
		ASSERT_EQ(data[i], result1_10[i]);
	}

	std::size_t i = 0;
	for (int value : result1_2)
	{
		ASSERT_EQ(data[i], value);
		++i;
	}

	i = 0;
	for (int value : result1_3)
	{
		ASSERT_EQ(data[i], value);
		++i;
	}

	for (int value : result1_4)
		ASSERT_TRUE(linq.Contains(value));

	for (int value : result1_5)
		ASSERT_TRUE(linq.Contains(value));

	for (int value : data)
	{
		ASSERT_EQ(value, result1_6.front());
		result1_6.pop();
	}

	for (int value : ExtendedCpp::LINQ::From(data).Reverse())
	{
		ASSERT_EQ(value, result1_8.top());
		result1_8.pop();
	}

	for (int value : ExtendedCpp::LINQ::From(data).Reverse())
	{
		ASSERT_EQ(value, result1_9.top());
		result1_9.pop();
	}
}

TEST(AdaptorTests, ConvertLinqGeneratorToStdContainerTest)
{
	// Average
	const std::vector data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

	// Act
	const auto result1_1 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::vector>();
	const auto result1_2 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::list>();
	const auto result1_3 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::forward_list>();
	const auto result1_4 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::set>();
	const auto result1_5 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::unordered_set>();
	auto result1_6 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::queue>();
	const auto result1_7 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::deque>();
	auto result1_8 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::priority_queue>();
	auto result1_9 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::stack>();
	const auto result1_10 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::ToArray<16>();

	// Assert
	ASSERT_EQ(result1_1.size(), data.size());
	ASSERT_EQ(result1_2.size(), data.size());
	ASSERT_EQ(result1_4.size(), data.size());
	ASSERT_EQ(result1_5.size(), data.size());
	ASSERT_EQ(result1_6.size(), data.size());
	ASSERT_EQ(result1_7.size(), data.size());
	ASSERT_EQ(result1_8.size(), data.size());
	ASSERT_EQ(result1_9.size(), data.size());
	ASSERT_EQ(result1_10.size(), data.size());

	for (std::size_t i = 0; i < data.size(); ++i)
	{
		ASSERT_EQ(data[i],result1_1[i]);
		ASSERT_EQ(data[i],result1_7[i]);
		ASSERT_EQ(data[i],result1_10[i]);
	}

	std::size_t i = 0;
	for (int value : result1_2)
	{
		ASSERT_EQ(data[i], value);
		++i;
	}

	i = 0;
	for (int value : result1_3)
	{
		ASSERT_EQ(data[i], value);
		++i;
	}

	for (int value : result1_4)
		ASSERT_TRUE(ExtendedCpp::LINQ::From(data).Contains(value));

	for (int value : result1_5)
		ASSERT_TRUE(ExtendedCpp::LINQ::From(data).Contains(value));

	for (int value : data)
	{
		ASSERT_EQ(value, result1_6.front());
		result1_6.pop();
	}

	for (int value : ExtendedCpp::LINQ::From(data).Reverse())
	{
		ASSERT_EQ(value, result1_8.top());
		result1_8.pop();
	}

	for (int value : ExtendedCpp::LINQ::From(data).Reverse())
	{
		ASSERT_EQ(value, result1_9.top());
		result1_9.pop();
	}
}

TEST(AdaptorTests, ConvertLinqViewToStdContainerTest)
{
	// Average
	const std::vector data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	const auto linq = ExtendedCpp::LINQ::View(data);

	// Act
	const auto result1_1 = linq | ExtendedCpp::LINQ::To<std::vector>();
	const auto result1_2 = linq | ExtendedCpp::LINQ::To<std::list>();
	const auto result1_3 = linq | ExtendedCpp::LINQ::To<std::forward_list>();
	const auto result1_4 = linq | ExtendedCpp::LINQ::To<std::set>();
	const auto result1_5 = linq | ExtendedCpp::LINQ::To<std::unordered_set>();
	auto result1_6 = linq | ExtendedCpp::LINQ::To<std::queue>();
	const auto result1_7 = linq | ExtendedCpp::LINQ::To<std::deque>();
	auto result1_8 = linq | ExtendedCpp::LINQ::To<std::priority_queue>();
	auto result1_9 = linq | ExtendedCpp::LINQ::To<std::stack>();
	const auto result1_10 = linq | ExtendedCpp::LINQ::ToArray<16>();

	// Assert
	ASSERT_EQ(result1_1.size(), data.size());
	ASSERT_EQ(result1_2.size(), data.size());
	ASSERT_EQ(result1_4.size(), data.size());
	ASSERT_EQ(result1_5.size(), data.size());
	ASSERT_EQ(result1_6.size(), data.size());
	ASSERT_EQ(result1_7.size(), data.size());
	ASSERT_EQ(result1_8.size(), data.size());
	ASSERT_EQ(result1_9.size(), data.size());
	ASSERT_EQ(result1_10.size(), data.size());

	for (std::size_t i = 0; i < data.size(); ++i)
	{
		ASSERT_EQ(data[i],result1_1[i]);
		ASSERT_EQ(data[i],result1_7[i]);
		ASSERT_EQ(data[i],result1_10[i]);
	}

	std::size_t i = 0;
	for (int value : result1_2)
	{
		ASSERT_EQ(data[i], value);
		++i;
	}

	i = 0;
	for (int value : result1_3)
	{
		ASSERT_EQ(data[i], value);
		++i;
	}

	for (int value : result1_4)
		ASSERT_TRUE(ExtendedCpp::LINQ::From(data).Contains(value));

	for (int value : result1_5)
		ASSERT_TRUE(ExtendedCpp::LINQ::From(data).Contains(value));

	for (int value : data)
	{
		ASSERT_EQ(value, result1_6.front());
		result1_6.pop();
	}

	for (int value : ExtendedCpp::LINQ::From(data).Reverse())
	{
		ASSERT_EQ(value, result1_8.top());
		result1_8.pop();
	}

	for (int value : ExtendedCpp::LINQ::From(data).Reverse())
	{
		ASSERT_EQ(value, result1_9.top());
		result1_9.pop();
	}
}

TEST(AdaptorTests, WhereTest)
{
	// Average
	const std::vector<std::string> names { "Tom", "Alice", "Bob", "Sam", "Tim", "Tomas", "Bill" };

	// Act
	const auto selectedNames = names |
		ExtendedCpp::LINQ::Where([](const std::string& name){ return name.size() == 3; }) |
		ExtendedCpp::LINQ::To<std::vector>();

	// Assert
	ASSERT_EQ(4, selectedNames.size());
}

TEST(AdaptorTests, RemoveWhereTest)
{
	// Average
	const std::vector<std::string> names { "Tom", "Alice", "Bob", "Sam", "Tim", "Tomas", "Bill" };

	// Act
	const auto selectedNames = names |
		ExtendedCpp::LINQ::RemoveWhere([](const std::string& name){ return name.size() == 3; }) |
		ExtendedCpp::LINQ::To<std::vector>();

	// Assert
	ASSERT_EQ(3, selectedNames.size());
}

TEST(AdaptorTests, JoinTest)
{
	// Average
	const Employer person1("Tom", "Microsoft");
	const Employer person2("Bob", "Google");
	const Employer person3("Sam", "Microsoft");
	const Employer person4("Alice", "Google");
	const Employer person5("Jon", "Google");

	const std::vector people { person1, person2, person3, person4, person5 };

	const ItCompany company1("Microsoft", "C#");
	const ItCompany company2("Google", "C++");

	const std::vector companies { company1, company2 };

	// Act
	const auto employees = people |
		ExtendedCpp::LINQ::Join(companies,
			[](const Employer& employer){ return employer.CompanyName; },
			[](const ItCompany& company){ return company.Name; },
			[](const Employer& employer, const ItCompany& company){ return Employer(employer.Name, company.Name, company.Language); }) |
		ExtendedCpp::LINQ::To<std::vector>();

	// Asset
	ASSERT_EQ("C#", employees[0].Language);
	ASSERT_EQ("C++", employees[1].Language);
	ASSERT_EQ("C#", employees[2].Language);
	ASSERT_EQ("C++", employees[3].Language);
	ASSERT_EQ("C++", employees[4].Language);
}

TEST(AdaptorTests, ZipTest)
{
	// Average
	const std::vector numbers1 { 4, 3, 6, 1, 5 };
	const std::vector numbers2 { 3, 2, 1, 4, 7 };

	// Act
	const auto result = numbers1 |
		ExtendedCpp::LINQ::Zip(numbers2) |
		ExtendedCpp::LINQ::To<std::vector>();

	// Asserta
	ASSERT_EQ(4, result[0].first);
	ASSERT_EQ(3, result[0].second);

	ASSERT_EQ(3, result[1].first);
	ASSERT_EQ(2, result[1].second);
}

TEST(AdaptorTests, SkipTest)
{
	// Average
	const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

	// Act
	const auto result = numbers |
		ExtendedCpp::LINQ::Skip(3) |
		ExtendedCpp::LINQ::To<std::vector>();

	// Assert
	ASSERT_EQ(3, result.size());
	ASSERT_EQ(4, result[0]);
	ASSERT_EQ(5, result[1]);
	ASSERT_EQ(6, result[2]);
}

TEST(AdaptorTests, SkipWhileTest)
{
	// Average
	const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

	// Act
	const auto result = numbers |
		ExtendedCpp::LINQ::SkipWhile([](const int n) { return n < 4; }) |
		ExtendedCpp::LINQ::To<std::vector>();

	// Assert
	ASSERT_EQ(3, result.size());
	ASSERT_EQ(4, result[0]);
	ASSERT_EQ(5, result[1]);
	ASSERT_EQ(6, result[2]);
}

TEST(AdaptorTests, TakeTest)
{
	// Average
	const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

	// Act
	const auto result = numbers |
		ExtendedCpp::LINQ::Take(3) |
		ExtendedCpp::LINQ::To<std::vector>();

	// Assert
	ASSERT_EQ(3, result.size());
	ASSERT_EQ(1, result[0]);
	ASSERT_EQ(2, result[1]);
	ASSERT_EQ(3, result[2]);
}

TEST(AdaptorTests, TakeWhileTest)
{
	// Average
	const std::vector numbers{ 1, 2, 3, 4, 5, 6 };

	// Act
	const auto result = numbers |
		ExtendedCpp::LINQ::TakeWhile([](const int n) { return n < 4; }) |
		ExtendedCpp::LINQ::To<std::vector>();

	// Assert
	ASSERT_EQ(3, result.size());
	ASSERT_EQ(1, result[0]);
	ASSERT_EQ(2, result[1]);
	ASSERT_EQ(3, result[2]);
}

TEST(AdaptorTests, EmptyCollectionTest)
{
	// Average
	constexpr std::vector<char> emptyVector;

	// Act
	const auto emptyResult = emptyVector |
		ExtendedCpp::LINQ::Where([](char){ return true; }) |
		ExtendedCpp::LINQ::Select([](const char& element){ return element; }) |
		ExtendedCpp::LINQ::Take(10) |
		ExtendedCpp::LINQ::Skip(3) |
		ExtendedCpp::LINQ::To<std::vector>();

	// Assert
	ASSERT_EQ(emptyResult.size(), 0);
}

TEST(AdaptorTests, TransformTest)
{
	// Average
	const std::vector numbers = { 1, 2, 3, 4 };
	const std::vector assertVector = { 2, 4, 6, 8 };

	// Act
	const auto transformed = numbers |
		ExtendedCpp::LINQ::Transform([](int& number){ number *= 2; }) |
		ExtendedCpp::LINQ::To<std::vector>();

	// Assert
	for (std::size_t i = 0; i < assertVector.size(); ++i)
		ASSERT_EQ(transformed[i], assertVector[i]);
}

TEST(AdaptorTests, MapTest)
{
	// Average
	const std::vector numbers = { 1, 2, 3, 4 };
	const std::vector assertVector = { 2, 4, 6, 8 };

	// Act
	const auto mapped = numbers |
		ExtendedCpp::LINQ::Map([](const int number){ return number * 2; }) |
		ExtendedCpp::LINQ::To<std::vector>();

	// Assert
	for (std::size_t i = 0; i < assertVector.size(); ++i)
		ASSERT_EQ(mapped[i], assertVector[i]);
}

TEST(AdaptorTests, GroupByTest)
{
	// Average
	const Employer person1("Tom", "Microsoft");
	const Employer person2("Bob", "Google");
	const Employer person3("Sam", "Microsoft");
	const Employer person4("Alice", "Google");
	const Employer person5("Jon", "Google");

	const std::vector people { person1, person2, person3, person4, person5 };

	// Act
	auto companies = people |
		ExtendedCpp::LINQ::GroupBy([](const Employer& employer){ return employer.CompanyName; });

	// Assert
	ASSERT_EQ(2, companies.size());
	ASSERT_EQ(2, companies["Microsoft"].size());
	ASSERT_EQ(3, companies["Google"].size());
}

TEST(AdaptorTests, CatchExceptionContainerTest)
{
	// Average
	const std::vector numbers = { 1, 2, 3, 4 };

	// Act
	// Assert
	try
	{
		const auto _ =
			ExtendedCpp::LINQ::From(numbers) |
			ExtendedCpp::LINQ::Select([](const int n)
			{
				if (n == 2)
					throw std::invalid_argument("CatchExceptionContainerTest");
				return n;
			}) |
			ExtendedCpp::LINQ::To<std::vector>();

		ASSERT_TRUE(false);
	}
	catch (const std::invalid_argument&)
	{
		ASSERT_TRUE(true);
	}
}

TEST(AdaptorTests, CatchExceptionGeneratorTest)
{
	// Average
	const std::vector numbers = { 1, 2, 3, 4 };

	// Act
	// Assert
	try
	{
		auto _ =
			ExtendedCpp::LINQ::Generator(numbers) |
			ExtendedCpp::LINQ::Select([](const int n)
			{
				if (n == 2)
					throw std::invalid_argument("CatchExceptionGeneratorTest");
				return n;
			}) |
			ExtendedCpp::LINQ::To<std::vector>();
		ASSERT_TRUE(false);
	}
	catch (const std::invalid_argument&)
	{
		ASSERT_TRUE(true);
	}
}

TEST(AdaptorTests, CatchExceptionViewTest)
{
	// Average
	const std::vector numbers = { 1, 2, 3, 4 };

	// Act
	const auto view = ExtendedCpp::LINQ::View(numbers) |
		ExtendedCpp::LINQ::Select([](const int n)
		{
			if (n == 2)
				throw std::invalid_argument("CatchExceptionViewTest");
			return n;
		});

	// Assert
	try
	{
		const auto _ = view |
			ExtendedCpp::LINQ::To<std::vector>();
		ASSERT_TRUE(false);
	}
	catch (const std::invalid_argument&)
	{
		ASSERT_TRUE(true);
	}
}