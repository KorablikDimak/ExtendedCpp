cmake_minimum_required(VERSION 3.28)

project(Json-tests LANGUAGES CXX)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
if(is_top_level)
    enable_testing()
    find_package(ExtendedCpp REQUIRED)
endif()

find_package(GTest REQUIRED)

set(Json_TESTS_INCLUDES
        ValueObject.h
        ValuesObject.h
        ObjectObject.h
        ObjectsObject.h)

set(Json_TESTS_SOURCE
        main.cpp
        Json_Tests.cpp)

add_executable(Json-tests ${Json_TESTS_INCLUDES} ${Json_TESTS_SOURCE})
target_link_libraries(Json-tests PRIVATE ExtendedCpp::Json GTest::gtest GTest::gtest_main)