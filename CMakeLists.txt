cmake_minimum_required(VERSION 3.25)

project(ExtendedCpp
        VERSION 2.4.0
        DESCRIPTION "Library which extends C++"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif ()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(cmake/utils.cmake)

option(ExtendedCpp_INSTALL "Generate target for installing ExtendedCpp" ON)
option(BUILD_SHARED_LIBS "Build libs as shared" ON)

if (BUILD_SHARED_LIBS)
    set(LIBRARY_TYPE SHARED)
else ()
    set(LIBRARY_TYPE STATIC)
endif ()

option(BUILD_ALL_TESTS "Build all tests for ExtendedCpp" OFF)
option(Asio_BUILD_TESTS "Build Asio library tests" OFF)
option(Common_BUILD_TESTS "Build Common library tests" OFF)
option(DI_BUILD_TESTS "Build DI library tests" OFF)
option(Events_BUILD_TESTS "Build Events library tests" OFF)
option(InfoLog_BUILD_TESTS "Build InfoLog library tests" OFF)
option(Json_BUILD_TESTS "Build Json library tests" OFF)
option(LINQ_BUILD_TESTS "Build LINQ library tests" OFF)
option(Reflection_BUILD_TESTS "Build Reflection library tests" OFF)
option(Serialization_Json_BUILD_TESTS "Build Serialization_Json library tests" OFF)

option(Common_BUILD_BENCHMARKS "Build Common benchmarks" OFF)
option(LINQ_BUILD_BENCHMARKS "Build LINQ benchmarks" OFF)

add_library(Asio ${LIBRARY_TYPE})
add_library(ExtendedCpp::Asio ALIAS Asio)
target_link_libraries(Asio INTERFACE Common)
if (APPLE AND ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    target_compile_options(Asio PRIVATE -fblocks)
    target_compile_definitions(Asio PUBLIC DARWIN_IO)
elseif (UNIX OR APPLE AND NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    target_compile_definitions(Asio PUBLIC UNIX_IO)
elseif (WIN32)
    target_compile_definitions(Asio PUBLIC WINDOWS_IO)
endif ()

add_library(Common INTERFACE)
add_library(ExtendedCpp::Common ALIAS Common)
target_sources(Common INTERFACE FILE_SET HEADERS BASE_DIRS "${PROJECT_SOURCE_DIR}/include" FILES
        "include/ExtendedCpp/Cancellation/CancellationToken.h"
        "include/ExtendedCpp/Cancellation/CancellationTokenSource.h"
        "include/ExtendedCpp/Cancellation.h"
        "include/ExtendedCpp/Concepts.h"
        "include/ExtendedCpp/Matrix.h"
        "include/ExtendedCpp/Random.h"
        "include/ExtendedCpp/Task.h")

add_library(DI INTERFACE)
add_library(ExtendedCpp::DI ALIAS DI)
target_link_libraries(DI INTERFACE Reflection)

add_library(Events INTERFACE)
add_library(ExtendedCpp::Events ALIAS Events)

add_library(InfoLog ${LIBRARY_TYPE})
add_library(ExtendedCpp::InfoLog ALIAS InfoLog)
target_link_libraries(InfoLog PUBLIC Xml Reflection)

add_library(Json INTERFACE)
add_library(ExtendedCpp::Json ALIAS Json)

add_library(LINQ INTERFACE)
add_library(ExtendedCpp::LINQ ALIAS LINQ)

add_library(Reflection ${LIBRARY_TYPE})
add_library(ExtendedCpp::Reflection ALIAS Reflection)
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(Reflection PUBLIC /Zc:preprocessor)
endif ()

add_library(Serialization_Json ${LIBRARY_TYPE})
add_library(ExtendedCpp::Serialization::Json ALIAS Serialization_Json)
target_link_libraries(Serialization_Json PUBLIC Json Reflection)

add_library(Xml ${LIBRARY_TYPE})
add_library(ExtendedCpp::Xml ALIAS Xml)

set_interface_directories(Common DI Events Json LINQ)
set_include_directories(Asio InfoLog Reflection Serialization_Json Xml)

add_target_interface_sources(DI Events Json LINQ)
add_target_sources(Asio InfoLog Reflection Serialization_Json Xml)

if (ExtendedCpp_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    write_basic_package_version_file(
            ${CMAKE_CURRENT_BINARY_DIR}/extendedcpp-config-version.cmake
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY SameMajorVersion)

    install(FILES
            ${CMAKE_MODULE_PATH}/extendedcpp-config.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/extendedcpp-config-version.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/extendedcpp)

    install(DIRECTORY include/
            TYPE INCLUDE)

    install(EXPORT ExtendedCpp-targets
            NAMESPACE ExtendedCpp::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/extendedcpp)

    export(EXPORT ExtendedCpp-targets
            NAMESPACE ExtendedCPP::
            FILE ${CMAKE_CURRENT_BINARY_DIR}/extendedcpp-targets.cmake)

    configure_file(${CMAKE_MODULE_PATH}/extendedcpp-config.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/extendedcpp-config.cmake
            COPYONLY)

    install_projects(Asio Common DI Events InfoLog Json LINQ Reflection Serialization_Json Xml)
endif ()

if (Asio_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/Asio)

    if (BUILD_SHARED_LIBS AND WIN32)
        add_custom_command(
                TARGET Asio POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Asio${CMAKE_SHARED_LIBRARY_SUFFIX}
                ${CMAKE_CURRENT_BINARY_DIR}/tests/Asio/${CMAKE_SHARED_LIBRARY_PREFIX}Asio${CMAKE_SHARED_LIBRARY_SUFFIX})
    endif ()
endif ()

if (Common_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/Common)
endif ()

if (DI_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/DI)

    if (BUILD_SHARED_LIBS AND WIN32)
        add_custom_command(
                TARGET Reflection POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Reflection${CMAKE_SHARED_LIBRARY_SUFFIX}
                ${CMAKE_CURRENT_BINARY_DIR}/tests/DI/${CMAKE_SHARED_LIBRARY_PREFIX}Reflection${CMAKE_SHARED_LIBRARY_SUFFIX})
    endif ()
endif ()

if (Events_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/Events)
endif ()

if (InfoLog_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/InfoLog)

    if (BUILD_SHARED_LIBS AND WIN32)
        add_custom_command(
                TARGET InfoLog POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}InfoLog${CMAKE_SHARED_LIBRARY_SUFFIX}
                ${CMAKE_CURRENT_BINARY_DIR}/tests/InfoLog/${CMAKE_SHARED_LIBRARY_PREFIX}InfoLog${CMAKE_SHARED_LIBRARY_SUFFIX})

        add_custom_command(
                TARGET InfoLog POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Xml${CMAKE_SHARED_LIBRARY_SUFFIX}
                ${CMAKE_CURRENT_BINARY_DIR}/tests/InfoLog/${CMAKE_SHARED_LIBRARY_PREFIX}Xml${CMAKE_SHARED_LIBRARY_SUFFIX})

        add_custom_command(
                TARGET InfoLog POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Reflection${CMAKE_SHARED_LIBRARY_SUFFIX}
                ${CMAKE_CURRENT_BINARY_DIR}/tests/InfoLog/${CMAKE_SHARED_LIBRARY_PREFIX}Reflection${CMAKE_SHARED_LIBRARY_SUFFIX})
    endif ()
endif ()

if (Json_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/Json)
endif ()

if (LINQ_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/LINQ)
endif ()

if (Reflection_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/Reflection)

    if (BUILD_SHARED_LIBS AND WIN32)
        add_custom_command(
                TARGET Reflection POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Reflection${CMAKE_SHARED_LIBRARY_SUFFIX}
                ${CMAKE_CURRENT_BINARY_DIR}/tests/Reflection/${CMAKE_SHARED_LIBRARY_PREFIX}Reflection${CMAKE_SHARED_LIBRARY_SUFFIX})
    endif ()
endif ()

if (Serialization_Json_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/Serialization/Json)

    if (BUILD_SHARED_LIBS AND WIN32)
        add_custom_command(
                TARGET Serialization_Json POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Serialization_Json${CMAKE_SHARED_LIBRARY_SUFFIX}
                ${CMAKE_CURRENT_BINARY_DIR}/tests/Serialization/Json/${CMAKE_SHARED_LIBRARY_PREFIX}Serialization_Json${CMAKE_SHARED_LIBRARY_SUFFIX})

        add_custom_command(
                TARGET Serialization_Json POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Reflection${CMAKE_SHARED_LIBRARY_SUFFIX}
                ${CMAKE_CURRENT_BINARY_DIR}/tests/Serialization/Json/${CMAKE_SHARED_LIBRARY_PREFIX}Reflection${CMAKE_SHARED_LIBRARY_SUFFIX})
    endif ()
endif ()

if (Common_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks/Common)
endif ()

if (LINQ_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks/LINQ)
endif ()