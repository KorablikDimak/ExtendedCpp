cmake_minimum_required(VERSION 3.25)

project(Common-tests LANGUAGES CXX)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
if(is_top_level)
    enable_testing()
    find_package(ExtendedCpp REQUIRED)
endif()

find_package(GTest REQUIRED)

set(Common_TESTS_SOURCE
        main.cpp
        MatrixTests.cpp
        RandomTests.cpp
        ChannelTests.cpp)

add_executable(Common-tests ${Common_TESTS_SOURCE})
target_link_libraries(Common-tests PRIVATE ExtendedCpp::Common GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(Common-tests)