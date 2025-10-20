#include <gtest/gtest.h>

#include <ExtendedCpp/LINQ.h>

TEST(AdaptorTests, SelectAdaptorTest)
{
	// Average
	const std::vector data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

	// Act
	const auto result1 =
		ExtendedCpp::LINQ::From(data) |
		ExtendedCpp::LINQ::Select([](const int value){ return value * 2; }) |
		ExtendedCpp::LINQ::To<std::vector<int>>();

	// Assert
	for (std::size_t i = 0; i < data.size(); ++i)
		ASSERT_EQ(result1[i], data[i] * 2);

	// Act
	const auto result2 =
		ExtendedCpp::LINQ::Generator(data) |
		ExtendedCpp::LINQ::Select([](const int value){ return value * 2; }) |
		ExtendedCpp::LINQ::To<std::vector<int>>();

	// Assert
	for (std::size_t i = 0; i < data.size(); ++i)
		ASSERT_EQ(result2[i], data[i] * 2);

	// Act
	const auto result3 =
		ExtendedCpp::LINQ::View(data) |
		ExtendedCpp::LINQ::Select([](const int value){ return value * 2; }) |
		ExtendedCpp::LINQ::To<std::vector<int>>();

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
		ExtendedCpp::LINQ::To<ExtendedCpp::LINQ::LinqContainer<int>>();

	auto result2 =
		result1 |
		ExtendedCpp::LINQ::To<ExtendedCpp::LINQ::LinqGenerator<int>>();

	const auto result3 =
		result1 |
		ExtendedCpp::LINQ::To<ExtendedCpp::LINQ::LinqView<int>>();

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
	const auto result1_1 = linq | ExtendedCpp::LINQ::To<std::vector<int>>();
	const auto result1_2 = linq | ExtendedCpp::LINQ::To<std::list<int>>();
	const auto result1_3 = linq | ExtendedCpp::LINQ::To<std::forward_list<int>>();
	const auto result1_4 = linq | ExtendedCpp::LINQ::To<std::set<int>>();
	const auto result1_5 = linq | ExtendedCpp::LINQ::To<std::unordered_set<int>>();
	const auto result1_6 = linq | ExtendedCpp::LINQ::To<std::queue<int>>();
	const auto result1_7 = linq | ExtendedCpp::LINQ::To<std::deque<int>>();
	const auto result1_8 = linq | ExtendedCpp::LINQ::To<std::priority_queue<int>>();
	const auto result1_9 = linq | ExtendedCpp::LINQ::To<std::stack<int>>();
	const auto result1_10 = linq | ExtendedCpp::LINQ::To<std::array<int, 16>>();

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
		ASSERT_TRUE(linq.Contains(value));

	for (int value : result1_5)
		ASSERT_TRUE(linq.Contains(value));
}

TEST(AdaptorTests, ConvertLinqGeneratorToStdContainerTest)
{
	// Average
	const std::vector data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

	// Act
	const auto result1_1 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::vector<int>>();
	const auto result1_2 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::list<int>>();
	const auto result1_3 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::forward_list<int>>();
	const auto result1_4 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::set<int>>();
	const auto result1_5 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::unordered_set<int>>();
	const auto result1_6 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::queue<int>>();
	const auto result1_7 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::deque<int>>();
	const auto result1_8 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::priority_queue<int>>();
	const auto result1_9 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::stack<int>>();
	const auto result1_10 = ExtendedCpp::LINQ::Generator(data) | ExtendedCpp::LINQ::To<std::array<int, 16>>();

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
}

TEST(AdaptorTests, ConvertLinqViewToStdContainerTest)
{
	// Average
	const std::vector data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	const auto linq = ExtendedCpp::LINQ::View(data);

	// Act
	const auto result1_1 = linq | ExtendedCpp::LINQ::To<std::vector<int>>();
	const auto result1_2 = linq | ExtendedCpp::LINQ::To<std::list<int>>();
	const auto result1_3 = linq | ExtendedCpp::LINQ::To<std::forward_list<int>>();
	const auto result1_4 = linq | ExtendedCpp::LINQ::To<std::set<int>>();
	const auto result1_5 = linq | ExtendedCpp::LINQ::To<std::unordered_set<int>>();
	const auto result1_6 = linq | ExtendedCpp::LINQ::To<std::queue<int>>();
	const auto result1_7 = linq | ExtendedCpp::LINQ::To<std::deque<int>>();
	const auto result1_8 = linq | ExtendedCpp::LINQ::To<std::priority_queue<int>>();
	const auto result1_9 = linq | ExtendedCpp::LINQ::To<std::stack<int>>();
	const auto result1_10 = linq | ExtendedCpp::LINQ::To<std::array<int, 16>>();

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
}