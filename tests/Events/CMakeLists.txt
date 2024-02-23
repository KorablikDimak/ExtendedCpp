cmake_minimum_required(VERSION 3.5)

project(Events-tests)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    enable_testing()
    find_package(ExtendedCpp REQUIRED)
endif()

find_package(GTest REQUIRED)

set(Events_TESTS_HEADERS
        Events_Tests.h)

set(Events_TESTS_SOURCES
        main.cpp
        Events_Tests.cpp)

add_executable(Events-tests ${Events_TESTS_HEADERS} ${Events_TESTS_SOURCES})
target_link_libraries(Events-tests PRIVATE ExtendedCpp::Events GTest::gtest GTest::gtest_main)