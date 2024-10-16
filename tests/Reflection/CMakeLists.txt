cmake_minimum_required(VERSION 3.28)

project(Reflection-tests LANGUAGES CXX)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
if(is_top_level)
    enable_testing()
    find_package(ExtendedCpp REQUIRED)
endif()

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