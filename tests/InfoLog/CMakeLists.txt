cmake_minimum_required(VERSION 3.25)

project(InfoLog-tests LANGUAGES CXX)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
if(is_top_level)
    enable_testing()
    find_package(ExtendedCpp REQUIRED)
endif()

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

add_custom_command(
        TARGET InfoLog-tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/LogConfig.xml
        ${CMAKE_CURRENT_BINARY_DIR}/LogConfig.xml)

enable_testing()
include(GoogleTest)
gtest_discover_tests(InfoLog-tests)