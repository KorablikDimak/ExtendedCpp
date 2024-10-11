cmake_minimum_required(VERSION 3.28)

project(DI-tests LANGUAGES CXX)

find_package(ExtendedCpp REQUIRED)
find_package(GTest REQUIRED)

set(DI_TESTS_INCLUDES
        IService.h
        Service.h
        Target.h)

set(DI_TESTS_SOURCE
        main.cpp
        DI_Tests.cpp)

add_executable(DI-tests ${DI_TESTS_INCLUDES} ${DI_TESTS_SOURCE})
target_link_libraries(DI-tests PRIVATE ExtendedCpp::DI ExtendedCpp::Reflection GTest::gtest GTest::gtest_main)