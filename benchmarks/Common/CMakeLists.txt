cmake_minimum_required(VERSION 3.28)

project(Common-benchmarks LANGUAGES CXX)

find_package(ExtendedCpp REQUIRED)
find_package(benchmark REQUIRED)

set(Common_BENCHMARKS_SOURCE
        main.cpp
        MatrixBenchmark.cpp)

add_executable(Common-benchmarks ${Common_BENCHMARKS_SOURCE})
target_link_libraries(Common-benchmarks PRIVATE ExtendedCpp::Common benchmark::benchmark)