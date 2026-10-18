cmake_minimum_required(VERSION 3.28)

project(Common-benchmarks LANGUAGES CXX)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
if (is_top_level)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    find_package(ExtendedCpp REQUIRED)
endif ()

add_benchmark_for(ExtendedCpp::Common)