cmake_minimum_required(VERSION 3.5)

project(Json-tests)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    enable_testing()
    find_package(ExtendedCpp REQUIRED)
endif()

find_package(GTest REQUIRED)

set(Json_TESTS_HEADERS
        ValueObject.h
        ValuesObject.h
        ObjectObject.h
        ObjectsObject.h)

set(Json_TESTS_SOURCES
        main.cpp
        Json_Tests.cpp)

add_executable(Json-tests ${Json_TESTS_HEADERS} ${Json_TESTS_SOURCES})
target_link_libraries(Json-tests PRIVATE ExtendedCpp::Json GTest::gtest GTest::gtest_main)