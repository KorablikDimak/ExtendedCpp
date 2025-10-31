#include <fstream>
#include <gtest/gtest.h>

#include <ExtendedCpp/Asio.h>

TEST(AsioTests, AifstramTest)
{
	// Average
	std::ofstream file("AifstramTest.txt");
	file.write("Correct test.", 14);
	file.close();

	// Act
	const auto innerInnerTask = []()->ExtendedCpp::Task<std::vector<char>>
	{
		ExtendedCpp::Asio::Aifstream aifstream("AifstramTest.txt");
		co_return co_await aifstream.ReadAsync(14);
	}();

	const auto innerTask = [&]()->ExtendedCpp::Task<std::vector<char>>
	{
		co_return co_await innerInnerTask;
	}();

	const auto task = [&]()->ExtendedCpp::Task<std::vector<char>>
	{
		co_return co_await innerTask;
	}();

	// Assert
	ASSERT_EQ(task.Result().size(), 14);
}

TEST(AsioTests, ReadAllAsyncTest)
{
	// Average
	std::ofstream file("ReadAllAsyncTest.txt");
	file.write("Correct test.", 14);
	file.close();

	// Act
	const auto task = []()->ExtendedCpp::Task<std::vector<char>>
	{
		ExtendedCpp::Asio::Aifstream aifstream("ReadAllAsyncTest.txt");
		co_return co_await aifstream.ReadAllAsync();
	}();

	// Assert
	ASSERT_EQ(task.Result().size(), 14);
}

TEST(AsioTests, AofstramTest)
{
	// Average
	// Act
	const auto task = []()->ExtendedCpp::Task<std::size_t>
	{
		ExtendedCpp::Asio::Aofstream aofstream("AofstreamTest.txt");
		co_return co_await aofstream.WriteAsync({'C', 'o', 'r', 'r', 'e', 'c', 't', ' ', 't', 'e', 's', 't', '.', '\0'});
	}();

	// Assert
	ASSERT_EQ(task.Result(), 14);
	ASSERT_EQ(std::filesystem::file_size("AofstreamTest.txt"), 14);
}

TEST(AsioTests, AfstramTest)
{
	// Average
	// Act
	std::size_t writeResult = 0;

	const auto task = [](std::size_t& writeResult_)->ExtendedCpp::Task<std::vector<char>>
	{
		ExtendedCpp::Asio::Afstream afstream("AfstreamTest.txt");
		writeResult_ = co_await afstream.WriteAsync({'C', 'o', 'r', 'r', 'e', 'c', 't', ' ', 't', 'e', 's', 't', '.', '\0'});
		afstream.ResetOffest();
		co_return co_await afstream.ReadAsync(14);
	}(writeResult);

	// Assert
	ASSERT_EQ(task.Result().size(), 14);
	ASSERT_EQ(writeResult, 14);
}

TEST(AsioTests, StreamOpeatorTest1)
{
	constexpr double data = 6.89;
	double result = 0;

	const auto task = [](const double& data_, double& result_)->ExtendedCpp::Task<void>
	{
		ExtendedCpp::Asio::Afstream afstream("StreamOperatorTest1.txt");
		co_await (afstream << data_);
		afstream.ResetOffest();
		co_await (afstream >> result_);
	}(data, result);

	task.Wait();
	ASSERT_EQ(data, result);
}

TEST(AsioTests, StreamOpeatorTest2)
{
	constexpr double data = 0.8134;
	double result = 0;

	const auto task = [](const double& data_, double& result_)->ExtendedCpp::Task<void>
	{
		ExtendedCpp::Asio::Afstream afstreamOut("StreamOperatorTest2.out.txt");
		co_await (afstreamOut << data_);
		afstreamOut.ResetOffest();

		ExtendedCpp::Asio::Afstream afstreamIn("StreamOperatorTest2.in.txt");
		co_await (afstreamIn << afstreamOut);
		afstreamIn.ResetOffest();

		co_await (afstreamIn >> result_);
	}(data, result);

	task.Wait();
	ASSERT_EQ(data, result);
}