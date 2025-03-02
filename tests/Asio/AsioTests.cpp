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
    ExtendedCpp::Asio::Aifstream aifstream("AifstramTest.txt");
    ExtendedCpp::Future<std::vector<char>> future = aifstream.ReadAsync(14);
    std::vector<char> result = future.Result();

    // Assert
    ASSERT_EQ(result.size(), 14);
}

TEST(AsioTests, AofstramTest)
{
    // Average
    // Act
    ExtendedCpp::Asio::Aofstream aofstream("AofstramTest.txt");
    ExtendedCpp::Future<ssize_t> future = 
        aofstream.WriteAsync({'C', 'o', 'r', 'r', 'e', 'c', 't', ' ', 't', 'e', 's', 't', '.', '\0'});
    ssize_t result = future.Result();

    // Assert
    ASSERT_EQ(result, 14);
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

    ExtendedCpp::Future<ssize_t> writeFuture = 
        afstream.WriteAsync({'C', 'o', 'r', 'r', 'e', 'c', 't', ' ', 't', 'e', 's', 't', '.', '\0'});
    ssize_t writeResult = writeFuture.Result();

    afstream.ResetOffest();

    ExtendedCpp::Future<std::vector<char>> readFuture = afstream.ReadAsync(14);
    std::vector<char> readResult = readFuture.Result();

    // Assert
    ASSERT_EQ(writeResult, 14);
    ASSERT_EQ(readResult.size(), 14);
}