#include <fstream>
#include <future>
#include <gtest/gtest.h>

#include <ExtendedCpp/Asio.h>

TEST(AsioTests, AifstramTest)
{
    // Average
    std::ofstream file("AifstramTest.txt");
    file.write("Correct test.", 14);
    file.close();

    // Act
    auto task = []()->ExtendedCpp::Task<std::vector<char>>
    {
        ExtendedCpp::Asio::Aifstream aifstream("AifstramTest.txt");
        co_return co_await aifstream.ReadAsync(14);
    }();

    // Assert
    ASSERT_EQ(task.Result().size(), 14);
}

TEST(AsioTests, AofstramTest)
{
    // Average
    // Act
    auto task = []()->ExtendedCpp::Task<ssize_t>
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
    ExtendedCpp::Asio::Afstream afstream("AfstramTest.txt");

    auto taskWrite = [&afstream]()->ExtendedCpp::Task<ssize_t>
    {
        co_return co_await afstream.WriteAsync({'C', 'o', 'r', 'r', 'e', 'c', 't', ' ', 't', 'e', 's', 't', '.', '\0'});
    }();
    taskWrite.Wait();

    afstream.ResetOffest();

    auto taskRead = [&afstream]()->ExtendedCpp::Task<std::vector<char>>
    {
        co_return co_await afstream.ReadAsync(14);
    }();
    taskRead.Wait();

    // Assert
    ASSERT_EQ(taskWrite.Result(), 14);
    ASSERT_EQ(taskRead.Result().size(), 14);
}