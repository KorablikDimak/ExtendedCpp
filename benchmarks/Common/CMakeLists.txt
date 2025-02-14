cmake_minimum_required(VERSION 3.28)

project(Common-benchmarks LANGUAGES CXX)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
if(is_top_level)
    enable_testing()
    find_package(ExtendedCpp REQUIRED)
endif()

find_package(benchmark REQUIRED)

set(Common_BENCHMARKS_SOURCE
        main.cpp
        MatrixBenchmark.cpp)

add_executable(Common-benchmarks ${Common_BENCHMARKS_SOURCE})
target_link_libraries(Common-benchmarks PRIVATE ExtendedCpp::Common benchmark::benchmark)