#include <thread>
#include <gtest/gtest.h>

#include <ExtendedCpp/Channel.h>

TEST(ChannelTests, ChannelCounterTest)
{
    auto [sender, receiver] = ExtendedCpp::Channel<int>::Create();

    {
        auto sender2(sender);
        auto receiver2(receiver);
    }

    [](ExtendedCpp::Channel<int, ExtendedCpp::ChannelType::Sender>){}(std::move(sender));

    ASSERT_TRUE(receiver.Closed());
}

TEST(ChannelTests, SendReciveTest)
{
    std::vector<int> result;
    auto [sender, receiver] = ExtendedCpp::Channel<int>::Create();

    std::thread senderThread([](auto sender)
    {
        for (int i = 0; i < 1000; ++i)
            sender.Send(i);
    }, std::move(sender));

    std::thread receiverThread([&result](auto receiver)
    {
        for (int i = 0; i < 1000; ++i)
            result.push_back(receiver.Receive());
    }, std::move(receiver));

    senderThread.join();
    receiverThread.join();

    for (int i = 0; i < 1000; ++i)
        ASSERT_EQ(result[i], i);
}

TEST(ChannelTests, SendReciveOperatorTest)
{
    std::vector<int> result;
    auto [sender, receiver] = ExtendedCpp::Channel<int>::Create();

    std::thread senderThread([](auto sender)
    {
        for (int i = 0; i < 1000; ++i)
            sender << i;
    }, std::move(sender));

    std::thread receiverThread([&result](auto receiver)
    {
        for (int i = 0; i < 1000; ++i)
        {
            int receivedValue;
            receiver >> receivedValue;
            result.push_back(receivedValue);
        }
    }, std::move(receiver));

    senderThread.join();
    receiverThread.join();

    for (int i = 0; i < 1000; ++i)
        ASSERT_EQ(result[i], i);
}

TEST(ChannelTests, WhiheChannelTest)
{
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

    for (int i = 0; i < 1000; ++i)
        ASSERT_EQ(result[i], i);
}

TEST(ChannelTests, TryReciveTest)
{
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

    for (int i = 0; i < 1000; ++i)
        ASSERT_EQ(result[i], i);
}