cmake_minimum_required(VERSION 3.28)

project(LINQ-benchmarks LANGUAGES CXX)

find_package(ExtendedCpp REQUIRED)
find_package(benchmark REQUIRED)

set(LINQ_BENCHMARKS_SOURCE
        main.cpp
        SortDoubleBenchmarks.cpp
        SortIntBenchmarks.cpp
        SortStringBenchmarks.cpp)

add_executable(LINQ-benchmarks ${LINQ_BENCHMARKS_SOURCE})
target_link_libraries(LINQ-benchmarks PRIVATE ExtendedCpp::LINQ ExtendedCpp::Common benchmark::benchmark)