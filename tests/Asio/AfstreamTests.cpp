#include <array>
#include <filesystem>

#include <gtest/gtest.h>

#include <ExtendedCpp/Asio.h>

namespace AfstramTestsUtility
{
    static void CreateFile(const char* filename, const std::string_view content)
    {
        std::ofstream file(filename, std::ios::binary | std::ios::trunc);
        if (!file)
            throw std::runtime_error("Cannot create test file");

        file.write(content.data(), static_cast<std::streamsize>(content.size()));

        if (!file)
            throw std::runtime_error("Cannot write test file");
    }

    static std::string ReadFile(const char* filename)
    {
        std::ifstream file(filename, std::ios::binary);
        return { std::istreambuf_iterator(file), std::istreambuf_iterator<char>() };
    }

    static std::string ToString(const std::span<const std::byte> bytes)
    {
        if (bytes.empty())
            return {};

        return { reinterpret_cast<const char*>(bytes.data()), bytes.size() }; // NOLINT(*-pro-type-reinterpret-cast)
    }
}

// ============================================================
// ReadAsync(std::size_t count)
// ============================================================

TEST(AfstreamTests, ReadAsyncCountTest1)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadAsyncCount1.txt", "Correct test.");

    // Act
    const auto task = []()-> ExtendedCpp::Task<std::vector<std::byte>>
    {
        ExtendedCpp::Asio::Afstream file("ReadAsyncCount1.txt");
        co_return co_await file.ReadAsync(7); // NOLINT(*-magic-numbers)
    }();

    const auto result = task.Result();

    // Assert
    EXPECT_EQ(AfstramTestsUtility::ToString(result), "Correct");
    EXPECT_EQ(result.size(), 7);
}

TEST(AfstreamTests, ReadAsyncCountTest2)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadAsyncCount2.txt", "Hello");

    // Act
    const auto task = []()-> ExtendedCpp::Task<std::vector<std::byte>>
    {
        ExtendedCpp::Asio::Afstream file("ReadAsyncCount2.txt");
        co_return co_await file.ReadAsync(10); // NOLINT(*-magic-numbers)
    }();

    const auto result = task.Result();

    // Assert
    EXPECT_EQ(AfstramTestsUtility::ToString(result), "Hello");
    EXPECT_EQ(result.size(), 5);
}

TEST(AfstreamTests, ReadAsyncCountTest3)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadAsyncCount3.txt", "Hello");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::vector<std::byte>>
    {
        ExtendedCpp::Asio::Afstream file("ReadAsyncCount3.txt", std::ios::out);
        co_return co_await file.ReadAsync(5); // NOLINT(*-magic-numbers)
    }();

    // Assert
    EXPECT_ANY_THROW(task.Result());
}

// ============================================================
// ReadAsync(std::span<std::byte> buffer)
// ============================================================

TEST(AfstreamTests, ReadAsyncSpanTest1)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadAsyncSpan1.txt", "Hello");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("ReadAsyncSpan1.txt");
        std::array<std::byte, 5> buffer{}; // NOLINT(*-magic-numbers)
        const auto read = co_await file.ReadAsync(std::span<std::byte>(buffer));
        EXPECT_EQ(AfstramTestsUtility::ToString(std::vector(buffer.begin(), buffer.end())), "Hello");
        co_return read;
    }();

    // Assert
    EXPECT_EQ(task.Result(), 5);
}

TEST(AfstreamTests, ReadAsyncSpanTest2)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadAsyncSpan2.txt", "Hi");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("ReadAsyncSpan2.txt");
        std::array<std::byte, 10> buffer{}; // NOLINT(*-magic-numbers)
        co_return co_await file.ReadAsync(std::span<std::byte>(buffer));
    }();

    // Assert
    EXPECT_EQ(task.Result(), 2);
}

TEST(AfstreamTests, ReadAsyncSpanTest3)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadAsyncSpan3.txt", "Hello");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("ReadAsyncSpan3.txt", std::ios::out);
        std::array<std::byte, 5> buffer{}; // NOLINT(*-magic-numbers)
        co_return co_await file.ReadAsync(std::span<std::byte>(buffer));
    }();

    // Assert
    EXPECT_ANY_THROW(task.Result());
}

// ============================================================
// ReadAsync(std::span<std::byte> buffer, std::size_t count)
// ============================================================

TEST(AfstreamTests, ReadAsyncSpanCountTest1)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadAsyncSpanCount1.txt", "Hello world");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("ReadAsyncSpanCount1.txt");
        std::array<std::byte, 10> buffer{}; // NOLINT(*-magic-numbers)
        co_return co_await file.ReadAsync(std::span<std::byte>(buffer), 5); // NOLINT(*-magic-numbers)
    }();

    // Assert
    EXPECT_EQ(task.Result(), 5);
}

TEST(AfstreamTests, ReadAsyncSpanCountTest2)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadAsyncSpanCount2.txt", "Hello");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("ReadAsyncSpanCount2.txt");
        std::array<std::byte, 5> buffer{}; // NOLINT(*-magic-numbers)
        co_return co_await file.ReadAsync(std::span<std::byte>(buffer), 100); // NOLINT(*-magic-numbers)
    }();

    // Assert
    EXPECT_EQ(task.Result(), 5);
}

TEST(AfstreamTests, ReadAsyncSpanCountTest3)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadAsyncSpanCount3.txt", "Hello");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("ReadAsyncSpanCount3.txt", std::ios::out);
        std::array<std::byte, 5> buffer{}; // NOLINT(*-magic-numbers)
        co_return co_await file.ReadAsync(std::span<std::byte>(buffer), 5); // NOLINT(*-magic-numbers)
    }();

    // Assert
    EXPECT_ANY_THROW(task.Result());
}

// ============================================================
// WriteAsync(std::span<const std::byte> buffer)
// ============================================================

TEST(AfstreamTests, WriteAsyncSpanTest1)
{
    // Average
    const std::string text = "Correct test.";

    // Act
    const auto task = [&]() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("WriteAsyncSpan1.txt", std::ios::out | std::ios::trunc);
        const auto buffer = std::as_bytes(std::span(text.data(), text.size()));
        co_return co_await file.WriteAsync(buffer);
    }();

    const auto written = task.Result();

    // Assert
    EXPECT_EQ(written, 13);
    EXPECT_EQ(AfstramTestsUtility::ReadFile("WriteAsyncSpan1.txt"), text);
}

TEST(AfstreamTests, WriteAsyncSpanTest2)
{
    // Average
    const std::string text = "Hello";

    // Act
    const auto task = [&]() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("WriteAsyncSpan2.txt", std::ios::out | std::ios::trunc);
        const auto buffer = std::as_bytes(std::span(text.data(), text.size()));
        co_return co_await file.WriteAsync(buffer);
    }();

    // Assert
    EXPECT_EQ(task.Result(), 5);
    EXPECT_EQ(AfstramTestsUtility::ReadFile("WriteAsyncSpan2.txt"), "Hello");
}

TEST(AfstreamTests, WriteAsyncSpanTest3)
{
    // Average
    AfstramTestsUtility::CreateFile("WriteAsyncSpan3.txt", "Original");
    const std::string text = "Changed";

    // Act
    const auto task = [&]() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("WriteAsyncSpan3.txt", std::ios::in);
        const auto buffer = std::as_bytes(std::span(text.data(), text.size()));
        co_return co_await file.WriteAsync(buffer);
    }();

    // Assert
    EXPECT_ANY_THROW(task.Result());
}

// ============================================================
// WriteAsync(std::span<const std::byte> buffer, std::size_t count)
// ============================================================

TEST(AfstreamTests, WriteAsyncSpanCountTest1)
{
    // Average
    const std::string text = "Hello world";

    // Act
    const auto task = [&]() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("WriteAsyncSpanCount1.txt", std::ios::out | std::ios::trunc);
        const auto buffer = std::as_bytes(std::span(text.data(), text.size()));
        co_return co_await file.WriteAsync(buffer, 5); // NOLINT(*-magic-numbers)
    }();

    // Assert
    EXPECT_EQ(task.Result(), 5);
    EXPECT_EQ(AfstramTestsUtility::ReadFile("WriteAsyncSpanCount1.txt"), "Hello");
}

TEST(AfstreamTests, WriteAsyncSpanCountTest2)
{
    // Average
    const std::string text = "Hello";

    // Act
    const auto task = [&]() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("WriteAsyncSpanCount2.txt", std::ios::out | std::ios::trunc);
        const auto buffer = std::as_bytes(std::span(text.data(), text.size()));
        co_return co_await file.WriteAsync(buffer, 100); // NOLINT(*-magic-numbers)
    }();

    // Assert
    EXPECT_EQ(task.Result(), 5);
    EXPECT_EQ(AfstramTestsUtility::ReadFile("WriteAsyncSpanCount2.txt"), "Hello");
}

TEST(AfstreamTests, WriteAsyncSpanCountTest3)
{
    // Average
    AfstramTestsUtility::CreateFile("WriteAsyncSpanCount3.txt", "Original");
    const std::string text = "Changed";

    // Act
    const auto task = [&]() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("WriteAsyncSpanCount3.txt", std::ios::in);
        const auto buffer = std::as_bytes(std::span(text.data(), text.size()));
        co_return co_await file.WriteAsync(buffer, text.size());
    }();

    // Assert
    EXPECT_ANY_THROW(task.Result());
}

// ============================================================
// WriteAsync(std::string_view buffer)
// ============================================================

TEST(AfstreamTests, WriteAsyncStringTest1)
{
    // Average
    constexpr std::string_view text = "Correct test.";

    // Act
    const auto task = [&]() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("WriteAsyncString1.txt", std::ios::out | std::ios::trunc);
        co_return co_await file.WriteAsync(text);
    }();

    // Assert
    EXPECT_EQ(task.Result(), 13);
    EXPECT_EQ(AfstramTestsUtility::ReadFile("WriteAsyncString1.txt"), text);
}

TEST(AfstreamTests, WriteAsyncStringTest2)
{
    // Average
    constexpr std::string_view text;

    // Act
    const auto task = [&]() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("WriteAsyncString2.txt", std::ios::out | std::ios::trunc);
        co_return co_await file.WriteAsync(text);
    }();

    // Assert
    EXPECT_EQ(task.Result(), 0);
    EXPECT_TRUE(AfstramTestsUtility::ReadFile("WriteAsyncString2.txt").empty());
}

TEST(AfstreamTests, WriteAsyncStringTest3)
{
    // Average
    AfstramTestsUtility::CreateFile("WriteAsyncString3.txt", "Original");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("WriteAsyncString3.txt", std::ios::in);
        co_return co_await file.WriteAsync(std::string_view("Changed"));
    }();

    // Assert
    EXPECT_ANY_THROW(task.Result());
}

// ============================================================
// WriteAsync(std::string_view buffer, std::size_t count)
// ============================================================

TEST(AfstreamTests, WriteAsyncStringCountTest1)
{
    // Average
    constexpr std::string_view text = "Hello world";

    // Act
    const auto task = [&]() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("WriteAsyncStringCount1.txt", std::ios::out | std::ios::trunc);
        co_return co_await file.WriteAsync(text, 5); // NOLINT(*-magic-numbers)
    }();

    // Assert
    EXPECT_EQ(task.Result(), 5);
    EXPECT_EQ(AfstramTestsUtility::ReadFile("WriteAsyncStringCount1.txt"), "Hello");
}

TEST(AfstreamTests, WriteAsyncStringCountTest2)
{
    // Average
    constexpr std::string_view text = "Hello";

    // Act
    const auto task = [&]() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("WriteAsyncStringCount2.txt", std::ios::out | std::ios::trunc);
        co_return co_await file.WriteAsync(text, 100); // NOLINT(*-magic-numbers)
    }();

    // Assert
    EXPECT_EQ(task.Result(), 5);
    EXPECT_EQ(AfstramTestsUtility::ReadFile("WriteAsyncStringCount2.txt"), "Hello");
}

TEST(AfstreamTests, WriteAsyncStringCountTest3)
{
    // Average
    AfstramTestsUtility::CreateFile("WriteAsyncStringCount3.txt", "Original");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::streamsize>
    {
        ExtendedCpp::Asio::Afstream file("WriteAsyncStringCount3.txt", std::ios::in);
        co_return co_await file.WriteAsync(std::string_view("Changed"), 7); // NOLINT(*-magic-numbers)
    }();

    // Assert
    EXPECT_ANY_THROW(task.Result());
}

// ============================================================
// ReadAllAsync()
// ============================================================

TEST(AfstreamTests, ReadAllAsyncTest1)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadAllAsync1.txt", "First line\nSecond line\n");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::vector<std::byte>>
    {
        ExtendedCpp::Asio::Afstream file("ReadAllAsync1.txt");
        co_return co_await file.ReadAllAsync();
    }();

    const auto result = task.Result();

    // Assert
    EXPECT_EQ(AfstramTestsUtility::ToString(result), "First line\nSecond line\n");
}

TEST(AfstreamTests, ReadAllAsyncTest2)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadAllAsync2.txt", "Hello world");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::vector<std::byte>>
    {
        ExtendedCpp::Asio::Afstream file("ReadAllAsync2.txt");
        co_await file.ReadAsync(6); // NOLINT(*-magic-numbers)
        co_return co_await file.ReadAllAsync();
    }();

    const auto result = task.Result();

    // Assert
    EXPECT_EQ(AfstramTestsUtility::ToString(result), "world");
}

TEST(AfstreamTests, ReadAllAsyncTest3)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadAllAsync3.txt", "Hello");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::vector<std::byte>>
    {
        ExtendedCpp::Asio::Afstream file("ReadAllAsync3.txt", std::ios::out);
        co_return co_await file.ReadAllAsync();
    }();

    // Assert
    EXPECT_ANY_THROW(task.Result());
}

// ============================================================
// ReadLineAsync()
// ============================================================

TEST(AfstreamTests, ReadLineAsyncTest1)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadLineAsync1.txt", "First line\nSecond line\n");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::string>
    {
        ExtendedCpp::Asio::Afstream file("ReadLineAsync1.txt");
        co_return co_await file.ReadLineAsync();
    }();

    // Assert
    EXPECT_EQ(task.Result(), "First line");
}

TEST(AfstreamTests, ReadLineAsyncTest2)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadLineAsync2.txt", "Last line");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::string>
    {
        ExtendedCpp::Asio::Afstream file("ReadLineAsync2.txt");
        co_return co_await file.ReadLineAsync();
    }();

    // Assert
    EXPECT_EQ(task.Result(), "Last line");
}

TEST(AfstreamTests, ReadLineAsyncTest3)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadLineAsync3.txt", "Hello\n");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::string>
    {
        ExtendedCpp::Asio::Afstream file("ReadLineAsync3.txt", std::ios::out);
        co_return co_await file.ReadLineAsync();
    }();

    // Assert
    EXPECT_ANY_THROW(task.Result());
}

TEST(AfstreamTests, ReadLineAsyncTest4)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadLineAsync3.txt", "First line\nSecond line\nLast line");
    ExtendedCpp::Asio::Afstream file("ReadLineAsync3.txt");

    // Act
    const auto task1 = [&]() -> ExtendedCpp::Task<std::string> { co_return co_await file.ReadLineAsync(); }();

    task1.Wait();

    const auto task2 = [&]() -> ExtendedCpp::Task<std::string> { co_return co_await file.ReadLineAsync(); }();

    task2.Wait();

    const auto task3 = [&]() -> ExtendedCpp::Task<std::string> { co_return co_await file.ReadLineAsync(); }();

    task3.Wait();

    // Assert
    EXPECT_EQ(task1.Result(), "First line");
    EXPECT_EQ(task2.Result(), "Second line");
    EXPECT_EQ(task3.Result(), "Last line");
}

// ============================================================
// ReadTextAsync()
// ============================================================

TEST(AfstreamTests, ReadTextAsyncTest1)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadTextAsync1.txt", "First line\nSecond line\n");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::string>
    {
        ExtendedCpp::Asio::Afstream file("ReadTextAsync1.txt");
        co_return co_await file.ReadTextAsync();
    }();

    // Assert
    EXPECT_EQ(task.Result(), "First line\nSecond line\n");
}

TEST(AfstreamTests, ReadTextAsyncTest2)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadTextAsync2.txt", "Hello world");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::string>
    {
        ExtendedCpp::Asio::Afstream file("ReadTextAsync2.txt");
        co_await file.ReadAsync(6); // NOLINT(*-magic-numbers)
        co_return co_await file.ReadTextAsync();
    }();

    // Assert
    EXPECT_EQ(task.Result(), "world");
}

TEST(AfstreamTests, ReadTextAsyncTest3)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadTextAsync3.txt", "Hello");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::string>
    {
        ExtendedCpp::Asio::Afstream file("ReadTextAsync3.txt", std::ios::out);
        co_return co_await file.ReadTextAsync();
    }();

    // Assert
    EXPECT_ANY_THROW(task.Result());
}

// ============================================================
// SynchronizesTests
// ============================================================

TEST(AfstreamTests, ReadAsyncSynchronizesReadPosition)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadAsyncSynchronizesReadPosition.txt", "0123456789");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::vector<std::byte>>
    {
        ExtendedCpp::Asio::Afstream file("ReadAsyncSynchronizesReadPosition.txt");
        co_return co_await file.ReadAsync(5); // NOLINT(*-magic-numbers)
    }();

    const auto result = task.Result();

    // Assert
    EXPECT_EQ(AfstramTestsUtility::ToString(result), "01234");
}

TEST(AfstreamTests, ReadAsyncUpdatesTellg)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadAsyncUpdatesTellg.txt", "0123456789");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::streamoff>
    {
        ExtendedCpp::Asio::Afstream file("ReadAsyncUpdatesTellg.txt");
        co_await file.ReadAsync(5); // NOLINT(*-magic-numbers)
        co_return file.tellg();
    }();

    const auto result = task.Result();

    // Assert
    EXPECT_EQ(result, 5);
}

TEST(AfstreamTests, WriteAsyncUpdatesTellp)
{
    // Average
    AfstramTestsUtility::CreateFile("WriteAsyncUpdatesTellp.txt", "0123456789");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::streamoff>
    {
        ExtendedCpp::Asio::Afstream file("WriteAsyncUpdatesTellp.txt");
        co_await file.WriteAsync("ABCDE");
        co_return file.tellp();
    }();

    const auto result = task.Result();

    // Assert
    EXPECT_EQ(result, 5);
}

TEST(AfstreamTests, ReadAsyncSynchronizesPositionAfterSeekg)
{
    // Average
    AfstramTestsUtility::CreateFile("ReadAsyncSynchronizesPositionAfterSeekg.txt", "0123456789");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::vector<std::byte>>
    {
        ExtendedCpp::Asio::Afstream file("ReadAsyncSynchronizesPositionAfterSeekg.txt");
        file.seekg(4, std::ios::beg);
        co_return co_await file.ReadAsync(3);
    }();

    const auto result = task.Result();

    // Assert
    EXPECT_EQ(AfstramTestsUtility::ToString(result), "456");
}

TEST(AfstreamTests, WriteAsyncSynchronizesPositionAfterSeekp)
{
    // Average
    AfstramTestsUtility::CreateFile("WriteAsyncSynchronizesPositionAfterSeekp.txt", "0123456789");

    // Act
    const auto task = []() -> ExtendedCpp::Task<std::streamoff>
    {
        ExtendedCpp::Asio::Afstream file("WriteAsyncSynchronizesPositionAfterSeekp.txt");
        file.seekp(4, std::ios::beg);
        co_await file.WriteAsync("ABC");
        co_return file.tellp();
    }();

    const auto result = task.Result();

    // Assert
    EXPECT_EQ(result, 7);
}