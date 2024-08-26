cmake_minimum_required(VERSION 3.5)

project(InfoLog-tests)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    enable_testing()
    find_package(ExtendedCpp REQUIRED)
endif()

find_package(GTest REQUIRED)

set(InfoLog_TESTS_HEADERS
        TestSender.h)

set(InfoLog_TESTS_SOURCES
        main.cpp
        ParserTests.cpp
        SenderTests.cpp
        TestSender.cpp)

add_executable(InfoLog-tests ${InfoLog_TESTS_HEADERS} ${InfoLog_TESTS_SOURCES})
target_link_libraries(InfoLog-tests PRIVATE ExtendedCpp::InfoLog ExtendedCpp::Xml ExtendedCpp::Reflection GTest::gtest GTest::gtest_main)