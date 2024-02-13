cmake_minimum_required(VERSION 3.5)

project(Events-tests)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    enable_testing()
    find_package(ExtendedCpp REQUIRED)
endif()

find_package(GTest REQUIRED)

set(HEADERS
        Events_Tests.h)

set(SOURCES
        main.cpp
        Events_Tests.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADERS} ${SOURCES})

add_executable(Events-tests)
target_sources(Events-tests PRIVATE ${HEADERS} ${SOURCES})

target_link_libraries(Events-tests PRIVATE ExtendedCpp::Events GTest::gtest GTest::gtest_main)