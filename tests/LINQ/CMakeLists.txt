cmake_minimum_required(VERSION 3.5)

project(LINQ-tests)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    enable_testing()
    find_package(ExtendedCpp REQUIRED)
endif()

find_package(GTest REQUIRED)

set(LINQ_TESTS_HEADERS
        LINQ_Tests.h)

set(LINQ_TESTS_SOURCES
        main.cpp
        AlgorithmTests.cpp
        SortTests.cpp
        LINQ_Tests.cpp
        LINQ_Generator_Tests.cpp)

add_executable(LINQ-tests ${LINQ_TESTS_HEADERS} ${LINQ_TESTS_SOURCES})
target_link_libraries(LINQ-tests PRIVATE ExtendedCpp::LINQ GTest::gtest GTest::gtest_main)