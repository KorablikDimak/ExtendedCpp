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
	auto innerInnerTask = []()->ExtendedCpp::Task<std::vector<char>>
	{
		ExtendedCpp::Asio::Aifstream aifstream("AifstramTest.txt");
		co_return co_await aifstream.ReadAsync(14);
	}();

	auto innerTask = [&]()->ExtendedCpp::Task<std::vector<char>>
	{
		co_return co_await innerInnerTask;
	}();

	auto task = [&]()->ExtendedCpp::Task<std::vector<char>>
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
	auto task = []()->ExtendedCpp::Task<std::vector<char>>
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
	auto task = []()->ExtendedCpp::Task<std::size_t>
	{
		ExtendedCpp::Asio::Aofstream aofstream("AofstramTest.txt");
		co_return co_await aofstream.WriteAsync({'C', 'o', 'r', 'r', 'e', 'c', 't', ' ', 't', 'e', 's', 't', '.', '\0'});
	}();

	// Assert
	ASSERT_EQ(task.Result(), 14);
	std::ifstream file("AofstramTest.txt");
	std::string line;
	std::getline(file, line);
	ASSERT_EQ(line.size(), 14);
}

TEST(AsioTests, AfstramTest)
{
	// Average
	// Act
	std::size_t writeResult = 0;

	auto task = [](std::size_t& writeResult)->ExtendedCpp::Task<std::vector<char>>
	{
		ExtendedCpp::Asio::Afstream afstream("AfstramTest.txt");
		writeResult = co_await afstream.WriteAsync({'C', 'o', 'r', 'r', 'e', 'c', 't', ' ', 't', 'e', 's', 't', '.', '\0'});
		afstream.ResetOffest();
		co_return co_await afstream.ReadAsync(14);
	}(writeResult);

	// Assert
	ASSERT_EQ(task.Result().size(), 14);
	ASSERT_EQ(writeResult, 14);
}

TEST(AsioTests, StreamOpeatorTest)
{
	double data = 6.89;
	double result = 0;

	auto task = [](double& data, double& result)->ExtendedCpp::Task<void>
	{
		ExtendedCpp::Asio::Afstream afstream("StreamOpeatorTest.txt");
		co_await(dynamic_cast<ExtendedCpp::Asio::Aostream&>(afstream) << data);
		afstream.ResetOffest();
		co_await(dynamic_cast<ExtendedCpp::Asio::Aistream&>(afstream) >> result);
	}(data, result);

	task.Wait();
	ASSERT_EQ(data, result);
}