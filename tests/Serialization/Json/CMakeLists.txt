cmake_minimum_required(VERSION 3.25)

project(Serialization_Json-tests LANGUAGES CXX)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
if (is_top_level)
    enable_testing()
    find_package(ExtendedCpp REQUIRED)
endif ()

find_package(GTest REQUIRED)

set(Serialization_Json_TESTS_SOURCE
        main.cpp
        SerializeTests.cpp)

add_executable(Serialization_Json-tests ${Serialization_Json_TESTS_SOURCE})
target_link_libraries(Serialization_Json-tests PRIVATE ExtendedCpp::Serialization::Json GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(Serialization_Json-tests)