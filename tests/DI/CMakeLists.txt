cmake_minimum_required(VERSION 3.5)

project(DI-tests)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    enable_testing()
    find_package(ExtendedCpp REQUIRED)
endif()

find_package(GTest REQUIRED)

set(DI_TESTS_HEADERS
        IService.h
        Service.h
        Target.h)

set(DI_TESTS_SOURCES
        main.cpp
        DI_Tests.cpp)

add_executable(DI-tests ${DI_TESTS_HEADERS} ${DI_TESTS_SOURCES})
target_link_libraries(DI-tests PRIVATE ExtendedCpp::DI ExtendedCpp::Reflection GTest::gtest GTest::gtest_main)