cmake_minimum_required(VERSION 3.28)

project(Common-tests LANGUAGES CXX)

find_package(ExtendedCpp REQUIRED)
find_package(GTest REQUIRED)

set(Common_TESTS_SOURCE
        main.cpp
        MatrixTests.cpp
        RandomTests.cpp)

add_executable(Common-tests ${Common_TESTS_SOURCE})
target_link_libraries(Common-tests PRIVATE ExtendedCpp::Common GTest::gtest GTest::gtest_main)