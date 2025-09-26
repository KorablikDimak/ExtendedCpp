#include <thread>
#include <gtest/gtest.h>

#include <ExtendedCpp/Channel.h>

TEST(ChannelTests, ChannelCounterTest)
{
    // Average
    auto [sender, receiver] = ExtendedCpp::Channel<int>::Create();

    {
        auto sender2(sender);
        auto receiver2(receiver);
    }

    // Act
    [](ExtendedCpp::Channel<int, ExtendedCpp::ChannelType::Sender>){}(std::move(sender));

    // Assert
    ASSERT_TRUE(receiver.Closed());
}

TEST(ChannelTests, SendReciveTest)
{
    // Average
    std::vector<int> result;
    auto [sender, receiver] = ExtendedCpp::Channel<int>::Create();

    // Act
    std::thread senderThread([](auto sender)
    {
        for (int i = 0; i < 1000; ++i)
            sender.Send(i);
    }, std::move(sender));

    senderThread.join();

    std::thread receiverThread([&result](auto receiver)
    {
        for (int i = 0; i < 1000; ++i)
            result.push_back(receiver.Receive());
    }, std::move(receiver));

    receiverThread.join();

    // Assert
    for (int i = 0; i < 1000; ++i)
        ASSERT_EQ(result[i], i);
}

TEST(ChannelTests, SendReciveOperatorTest)
{
    // Average
    std::vector<int> result;
    auto [sender, receiver] = ExtendedCpp::Channel<int>::Create();

    // Act
    std::thread senderThread([](auto sender)
    {
        for (int i = 0; i < 1000; ++i)
            sender << i;
    }, std::move(sender));

    senderThread.join();

    std::thread receiverThread([&result](auto receiver)
    {
        for (int i = 0; i < 1000; ++i)
        {
            int receivedValue;
            receiver >> receivedValue;
            result.push_back(receivedValue);
        }
    }, std::move(receiver));

    receiverThread.join();

    // Assert
    for (int i = 0; i < 1000; ++i)
        ASSERT_EQ(result[i], i);
}

TEST(ChannelTests, WhiheChannelTest)
{
    // Average
    std::vector<int> result;
    auto [sender, receiver] = ExtendedCpp::Channel<int>::Create();

    std::thread senderThread([](auto sender)
    {
        for (int i = 0; i < 1000; ++i)
        {
            sender.Send(i);
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    }, std::move(sender));

    // Act
    std::thread receiverThread([&result](auto receiver)
    {
        while (receiver)
        {
            try
            {
                result.push_back(receiver.Receive());
            }
            catch(const std::domain_error&)
            {
                break;
            }
        }
    }, std::move(receiver));

    senderThread.join();
    receiverThread.join();

    // Assert
    for (int i = 0; i < 1000; ++i)
        ASSERT_EQ(result[i], i);
}

TEST(ChannelTests, TryReciveTest)
{
    // Average
    std::vector<int> result;
    auto [sender, receiver] = ExtendedCpp::Channel<int>::Create();

    std::thread senderThread([](auto sender)
    {
        for (int i = 0; i < 1000; ++i)
        {
            sender.Send(i);
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    }, std::move(sender));

    // Act
    std::thread receiverThread([&result](auto receiver)
    {
        while (receiver)
        {
            std::optional<int> received = receiver.TryReceive();
            if (received.has_value())
                result.push_back(received.value());
        }
    }, std::move(receiver));

    senderThread.join();
    receiverThread.join();

    // Assert
    for (int i = 0; i < 1000; ++i)
        ASSERT_EQ(result[i], i);
}

TEST(ChannelTests, ReceiveAsyncTest)
{
    // Average
    auto [sender, receiver] = ExtendedCpp::Channel<int>::Create();

    std::thread senderThread([](auto sender)
    {
        for (int i = 0; i < 1000; ++i)
        {
            sender.Send(i);
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    },std::move(sender));

    // Act
    const auto task = [](auto receiver)->ExtendedCpp::Task<std::vector<int>>
    {
        std::vector<int> result;
        for (int i = 0; i < 1000; ++i)
            result.push_back(co_await receiver.ReceiveAsync());
        co_return std::move(result);
    }(std::move(receiver));

    senderThread.join();
    const std::vector<int> result = task.Result();

    // Assert
    for (int i = 0; i < 1000; ++i)
        ASSERT_EQ(result[i], i);
}