cmake_minimum_required(VERSION 3.28)

project(Events-tests LANGUAGES CXX)

find_package(ExtendedCpp REQUIRED)
find_package(GTest REQUIRED)

set(Events_TESTS_INCLUDES
        Events_Tests.h)

set(Events_TESTS_SOURCE
        main.cpp
        Events_Tests.cpp)

add_executable(Events-tests ${Events_TESTS_INCLUDES} ${Events_TESTS_SOURCE})
target_link_libraries(Events-tests PRIVATE ExtendedCpp::Events GTest::gtest GTest::gtest_main)