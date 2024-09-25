cmake_minimum_required(VERSION 3.5)

project(LINQ-benchmarks)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    find_package(ExtendedCpp REQUIRED)
endif()

find_package(benchmark REQUIRED)

set(SOURCES
        main.cpp
        SortDoubleBenchmarks.cpp
        SortIntBenchmarks.cpp
        SortStringBenchmarks.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_executable(LINQ-benchmarks)
target_sources(LINQ-benchmarks PRIVATE ${SOURCES})

target_link_libraries(LINQ-benchmarks PRIVATE ExtendedCpp::LINQ ExtendedCpp::Common benchmark::benchmark)