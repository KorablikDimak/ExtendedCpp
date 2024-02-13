cmake_minimum_required(VERSION 3.5)

project(InfoLog-tests)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    enable_testing()
    find_package(ExtendedCpp REQUIRED)
endif()

find_package(GTest REQUIRED)

set(SOURCES
        main.cpp
        ParserTests.cpp
        SenderTests.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_executable(InfoLog-tests)
target_sources(InfoLog-tests PRIVATE ${HEADERS} ${SOURCES})

target_link_libraries(InfoLog-tests PRIVATE ExtendedCpp::InfoLog GTest::gtest GTest::gtest_main)