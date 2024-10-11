cmake_minimum_required(VERSION 3.28)

project(InfoLog-tests LANGUAGES CXX)

find_package(ExtendedCpp REQUIRED)
find_package(GTest REQUIRED)

set(InfoLog_TESTS_INCLUDES
        TestSender.h)

set(InfoLog_TESTS_SOURCE
        main.cpp
        ParserTests.cpp
        SenderTests.cpp
        TestSender.cpp)

add_executable(InfoLog-tests ${InfoLog_TESTS_INCLUDES} ${InfoLog_TESTS_SOURCE})
target_link_libraries(InfoLog-tests PRIVATE ExtendedCpp::InfoLog ExtendedCpp::Xml ExtendedCpp::Reflection GTest::gtest GTest::gtest_main)