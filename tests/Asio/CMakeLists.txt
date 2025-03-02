cmake_minimum_required(VERSION 3.25)

project(Asio-tests LANGUAGES CXX)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
if(is_top_level)
    enable_testing()
    find_package(ExtendedCpp REQUIRED)
endif()

find_package(GTest REQUIRED)

set(Asio_TESTS_SOURCE
        main.cpp
        AsioTests.cpp)

add_executable(Asio-tests ${Asio_TESTS_SOURCE})
target_link_libraries(Asio-tests PRIVATE ExtendedCpp::Asio GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(Asio-tests)