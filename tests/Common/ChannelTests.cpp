#include <thread>
#include <gtest/gtest.h>

#include <ExtendedCpp/Channel.h>

TEST(ChannelTests, ChannelCounterTest)
{
    auto [sender, reciver] = ExtendedCpp::Channel<int>::Create();

    {
        auto sender2(sender);
        auto reciver2(reciver);
    }

    [](ExtendedCpp::Channel<int, ExtendedCpp::ChannelType::Sender>){}(std::move(sender));

    ASSERT_TRUE(reciver.Closed());
}

TEST(ChannelTests, SendReciveTest)
{
    std::vector<int> result;
    auto [sender, reciver] = ExtendedCpp::Channel<int>::Create();

    std::thread senderThread([&sender]()
    {
        for (int i = 0; i < 1000; ++i)
            sender.Send(i);
    });

    std::thread reciverThread([&result, &reciver]()
    {
        for (int i = 0; i < 1000; ++i)
            result.push_back(reciver.Recive());
    });

    senderThread.join();
    reciverThread.join();

    for (int i = 0; i < 1000; ++i)
        ASSERT_EQ(result[i], i);
}

TEST(ChannelTests, SendReciveOperatorTest)
{
    std::vector<int> result;
    auto [sender, reciver] = ExtendedCpp::Channel<int>::Create();

    std::thread senderThread([&sender]()
    {
        for (int i = 0; i < 1000; ++i)
            sender << i;
    });

    std::thread reciverThread([&result, &reciver]()
    {
        for (int i = 0; i < 1000; ++i)
        {
            int recivedValue;
            reciver >> recivedValue;
            result.push_back(recivedValue);
        }
    });

    senderThread.join();
    reciverThread.join();

    for (int i = 0; i < 1000; ++i)
        ASSERT_EQ(result[i], i);
}

TEST(ChannelTests, WhiheChannelTest)
{
    std::vector<int> result;
    auto [sender, reciver] = ExtendedCpp::Channel<int>::Create();

    std::thread senderThread([](ExtendedCpp::Channel<int, ExtendedCpp::ChannelType::Sender> sender)
    {
        for (int i = 0; i < 1000; ++i)
        {
            sender.Send(i);
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
            
    }, std::move(sender));

    std::thread reciverThread([&result, &reciver]()
    {
        while (reciver)
        {
            try
            {
                result.push_back(reciver.Recive());
            }
            catch(const std::domain_error&)
            {
                break;
            }
        }
    });

    senderThread.join();
    reciverThread.join();

    for (int i = 0; i < 1000; ++i)
        ASSERT_EQ(result[i], i);
}

TEST(ChannelTests, TryReciveTest)
{
    std::vector<int> result;
    auto [sender, reciver] = ExtendedCpp::Channel<int>::Create();

    std::thread senderThread([](ExtendedCpp::Channel<int, ExtendedCpp::ChannelType::Sender> sender)
    {
        for (int i = 0; i < 1000; ++i)
        {
            sender.Send(i);
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
            
    }, std::move(sender));

    std::thread reciverThread([&result, &reciver]()
    {
        while (reciver)
        {
            std::optional<int> recived = reciver.TryRecive();
            if (recived.has_value())
                result.push_back(recived.value());
        }
    });

    senderThread.join();
    reciverThread.join();

    for (int i = 0; i < 1000; ++i)
        ASSERT_EQ(result[i], i);
}