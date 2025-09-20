cmake_minimum_required(VERSION 3.28)

project(LINQ-benchmarks LANGUAGES CXX)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
if(is_top_level)
    find_package(ExtendedCpp REQUIRED)
endif()

find_package(benchmark REQUIRED)

set(LINQ_BENCHMARKS_SOURCE
        main.cpp
        SortDoubleBenchmarks.cpp
        SortIntBenchmarks.cpp
        SortStringBenchmarks.cpp)

add_executable(LINQ-benchmarks ${LINQ_BENCHMARKS_SOURCE})
target_link_libraries(LINQ-benchmarks PRIVATE ExtendedCpp::LINQ ExtendedCpp::Common benchmark::benchmark)