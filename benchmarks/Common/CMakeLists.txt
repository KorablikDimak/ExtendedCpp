cmake_minimum_required(VERSION 3.5)

project(Common-benchmarks)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    find_package(ExtendedCpp REQUIRED)
endif()

find_package(benchmark REQUIRED)

set(SOURCES
        main.cpp
        MatrixBenchmark.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_executable(Common-benchmarks)
target_sources(Common-benchmarks PRIVATE ${SOURCES})

target_link_libraries(Common-benchmarks PRIVATE ExtendedCpp::Common benchmark::benchmark)

if(CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(Common-benchmarks PRIVATE -O3)
endif()