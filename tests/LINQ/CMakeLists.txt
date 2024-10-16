cmake_minimum_required(VERSION 3.28)

project(LINQ-tests LANGUAGES CXX)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
if(is_top_level)
    enable_testing()
    find_package(ExtendedCpp REQUIRED)
endif()

find_package(GTest REQUIRED)

set(LINQ_TESTS_INCLUDES
        LINQ_Tests.h)

set(LINQ_TESTS_SOURCE
        main.cpp
        AlgorithmTests.cpp
        SortTests.cpp
        LINQ_Tests.cpp
        LINQ_Generator_Tests.cpp
        LINQ_View_Tests.cpp)

add_executable(LINQ-tests ${LINQ_TESTS_INCLUDES} ${LINQ_TESTS_SOURCE})
target_link_libraries(LINQ-tests PRIVATE ExtendedCpp::LINQ GTest::gtest GTest::gtest_main)