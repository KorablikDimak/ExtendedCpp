cmake_minimum_required(VERSION 3.5)

project(Reflection-tests)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    enable_testing()
    find_package(ExtendedCpp REQUIRED)
endif()

find_package(GTest REQUIRED)

set(Reflection_TESTS_HEADERS
        TestStruct.h)

set(Reflection_TESTS_SOURCES
        main.cpp
        ReflectionTests.cpp
        TestStruct.cpp)

add_executable(Reflection-tests ${Reflection_TESTS_HEADERS} ${Reflection_TESTS_SOURCES})
target_link_libraries(Reflection-tests PRIVATE ExtendedCpp::Reflection GTest::gtest GTest::gtest_main)