cmake_minimum_required(VERSION 3.28)

project(Reflection-tests LANGUAGES CXX)

find_package(ExtendedCpp REQUIRED)
find_package(GTest REQUIRED)

set(Reflection_TESTS_INCLUDES
        TestStruct.h
        TestTemplate.h)

set(Reflection_TESTS_SOURCE
        main.cpp
        ReflectionTests.cpp
        TestStruct.cpp)

add_executable(Reflection-tests ${Reflection_TESTS_INCLUDES} ${Reflection_TESTS_SOURCE})
target_link_libraries(Reflection-tests PRIVATE ExtendedCpp::Reflection GTest::gtest GTest::gtest_main)