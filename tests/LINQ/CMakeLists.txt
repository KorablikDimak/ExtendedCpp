cmake_minimum_required(VERSION 3.25)

project(LINQ-tests LANGUAGES CXX)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
if (is_top_level)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    find_package(ExtendedCpp REQUIRED)
    enable_testing()
endif ()

add_gtest_for(ExtendedCpp::LINQ)