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