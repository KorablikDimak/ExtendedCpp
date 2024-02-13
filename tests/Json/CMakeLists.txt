cmake_minimum_required(VERSION 3.5)

project(Json-tests)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    enable_testing()
    find_package(ExtendedCpp REQUIRED)
endif()

find_package(GTest REQUIRED)

set(HEADERS
        ValueObject.h
        ValuesObject.h
        ObjectObject.h
        ObjectsObject.h)

set(SOURCES
        main.cpp
        Json_Tests.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADERS} ${SOURCES})

add_executable(Json-tests)
target_sources(Json-tests PRIVATE ${HEADERS} ${SOURCES})

target_link_libraries(Json-tests PRIVATE ExtendedCpp::Json GTest::gtest GTest::gtest_main)