cmake_minimum_required(VERSION 3.5)

project(ExtendedCpp
        VERSION 1.0.0
        DESCRIPTION "Library which extends C++"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

include(GNUInstallDirs)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

macro(set_if_undefined variable)
    if(NOT DEFINED "${variable}")
        set("${variable}" ${ARGN})
    endif()
endmacro()

option(ExtendedCpp_SHARED_LIBS "Build libs as shared" OFF)
option(BUILD_ALL_TESTS "Build all tests for ExtendedCpp" OFF)
option(LINQ_BUILD_TESTS "Build LINQ library tests" OFF)
option(Events_BUILD_TESTS "Build Events library tests" OFF)
option(Json_BUILD_TESTS "Build Json library tests" OFF)
option(InfoLog_BUILD_TESTS "Build InfoLog library tests" OFF)
option(LINQ_BUILD_BENCHMARKS "Build LINQ benchmarks" OFF)

option(ExtendedCpp_INSTALL "Generate target for installing ExtendedCpp" ${is_top_level})
set_if_undefined(ExtendedCpp_INSTALL_CMAKEDIR
        "${CMAKE_INSTALL_LIBDIR}/cmake/ExtendedCpp" CACHE STRING
        "Install path for ExtendedCpp package-related CMake files")

if(DEFINED ExtendedCpp_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ${ExtendedCpp_SHARED_LIBS})
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

add_library(LINQ INTERFACE)
add_library(ExtendedCpp::LINQ ALIAS LINQ)

add_library(Events INTERFACE)
add_library(ExtendedCpp::Events ALIAS Events)

add_library(Json INTERFACE)
add_library(ExtendedCpp::Json ALIAS Json)

add_library(Xml)
add_library(ExtendedCpp::Xml ALIAS Xml)

add_library(InfoLog)
add_library(ExtendedCpp::InfoLog ALIAS InfoLog)

set(LINQ_INCLUDES
        include/LINQ/LINQ.h
        include/LINQ/OrderType.h
        include/LINQ/Algorithm.h
        include/LINQ/LinqContainer.h
        include/LINQ/TypeTraits.h
        include/LINQ/Sort.h
        include/LINQ/Aggregate.h)

set(Events_INCLUDES
        include/Events/Event.h
        include/Events/FunctionHandler.h
        include/Events/IEventHandler.h
        include/Events/MethodHandler.h
        include/Events/StaticMethodHandler.h)

set(Json_INCLUDES
        include/Json/json.hpp
        include/Json/json_fwd.hpp
        include/Json/JsonMacros.h
        include/Json/Json.h)

set(Xml_INCLUDES
    include/Xml/Xml.h
    include/Xml/pugixml.hpp
    include/Xml/pugiconfig.hpp)

set(Xml_SOURCE
        src/Xml/pugixml.cpp)

set(InfoLog_INCLUDES
        include/InfoLog/Configuration.h
        include/InfoLog/FileSender.h
        include/InfoLog/ISender.h
        include/InfoLog/Logger.h
        include/InfoLog/LoggerFactory.h
        include/InfoLog/LogLevel.h
        include/InfoLog/Parser.h
        include/InfoLog/Converter.h
        include/InfoLog/ConsoleSender.h
        include/InfoLog/CInfoLog.h)

set(InfoLog_SOURCE
        src/InfoLog/Configuration.cpp
        src/InfoLog/FileSender.cpp
        src/InfoLog/ISender.cpp
        src/InfoLog/Logger.cpp
        src/InfoLog/LoggerFactory.cpp
        src/InfoLog/Parser.cpp
        src/InfoLog/ConsoleSender.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
        ${LINQ_INCLUDES}
        ${Events_INCLUDES}
        ${Json_INCLUDES}
        ${Xml_INCLUDES}
        ${Xml_SOURCE}
        ${InfoLog_INCLUDES}
        ${InfoLog_SOURCE})

include(CMakePackageConfigHelpers)

target_sources(LINQ PRIVATE ${LINQ_INCLUDES})
target_sources(Events PRIVATE ${Events_INCLUDES})
target_sources(Json PRIVATE ${Json_INCLUDES})
target_sources(Xml PRIVATE ${Xml_INCLUDES} ${Xml_SOURCE})
target_sources(InfoLog PRIVATE ${InfoLog_INCLUDES} ${InfoLog_SOURCE})

target_include_directories(LINQ INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

target_include_directories(Events INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

target_include_directories(Json INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

target_include_directories(Xml PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/Xml")

target_include_directories(InfoLog PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/InfoLog")

set_target_properties(LINQ PROPERTIES
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION})

set_target_properties(Events PROPERTIES
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION})

set_target_properties(Json PROPERTIES
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION})

set_target_properties(Xml PROPERTIES
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION})

set_target_properties(InfoLog PROPERTIES
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION})

target_link_libraries(InfoLog PUBLIC Xml)

if(ExtendedCpp_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    configure_package_config_file(cmake/ExtendedCpp-config.cmake.in ExtendedCpp-config.cmake
            INSTALL_DESTINATION "${ExtendedCpp_INSTALL_CMAKEDIR}")

    write_basic_package_version_file(ExtendedCpp-config-version.cmake
            COMPATIBILITY SameMajorVersion)

    install(TARGETS LINQ EXPORT LINQ_export
            RUNTIME COMPONENT LINQ
            LIBRARY COMPONENT LINQ NAMELINK_COMPONENT LINQ-dev
            ARCHIVE COMPONENT LINQ-dev
            INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

    install(TARGETS Events EXPORT Events_export
            RUNTIME COMPONENT Events
            LIBRARY COMPONENT Events NAMELINK_COMPONENT Events-dev
            ARCHIVE COMPONENT Events-dev
            INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

    install(TARGETS Json EXPORT Json_export
            RUNTIME COMPONENT Json
            LIBRARY COMPONENT Json NAMELINK_COMPONENT Json-dev
            ARCHIVE COMPONENT Json-dev
            INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

    install(TARGETS Xml EXPORT Xml_export
            RUNTIME COMPONENT Xml
            LIBRARY COMPONENT Xml NAMELINK_COMPONENT Xml-dev
            ARCHIVE COMPONENT Xml-dev
            INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

    install(TARGETS InfoLog EXPORT InfoLog_export
            RUNTIME COMPONENT InfoLog
            LIBRARY COMPONENT InfoLog NAMELINK_COMPONENT InfoLog-dev
            ARCHIVE COMPONENT InfoLog-dev
            INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

    install(DIRECTORY include/
            TYPE INCLUDE
            COMPONENT ExtendedCpp-dev)

    set(targets_file "ExtendedCpp-shared-targets.cmake")
    if(NOT BUILD_SHARED_LIBS)
        set(targets_file "ExtendedCpp-static-targets.cmake")
    endif()

    install(EXPORT LINQ_export
            COMPONENT LINQ-dev
            FILE "${targets_file}"
            DESTINATION "${ExtendedCpp_INSTALL_CMAKEDIR}"
            NAMESPACE ExtendedCpp::)

    install(EXPORT Events_export
            COMPONENT Events-dev
            FILE "${targets_file}"
            DESTINATION "${ExtendedCpp_INSTALL_CMAKEDIR}"
            NAMESPACE ExtendedCpp::)

    install(EXPORT Json_export
            COMPONENT Json-dev
            FILE "${targets_file}"
            DESTINATION "${ExtendedCpp_INSTALL_CMAKEDIR}"
            NAMESPACE ExtendedCpp::)

    install(EXPORT Xml_export
            COMPONENT Xml-dev
            FILE "${targets_file}"
            DESTINATION "${ExtendedCpp_INSTALL_CMAKEDIR}"
            NAMESPACE ExtendedCpp::)

    install(EXPORT InfoLog_export
            COMPONENT InfoLog-dev
            FILE "${targets_file}"
            DESTINATION "${ExtendedCpp_INSTALL_CMAKEDIR}"
            NAMESPACE ExtendedCpp::)

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/ExtendedCpp-config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/ExtendedCpp-config-version.cmake"
            COMPONENT ExtendedCpp-dev
            DESTINATION "${ExtendedCpp_INSTALL_CMAKEDIR}")
endif()

if(LINQ_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/LINQ)
endif()

if(LINQ_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks/LINQ)
endif()

if(Events_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/Events)
endif()

if(Json_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/Json)
endif()

if(InfoLog_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/InfoLog)

    add_custom_command(
            TARGET InfoLog POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/LogConfig.xml
            ${CMAKE_CURRENT_BINARY_DIR}/tests/InfoLog/LogConfig.xml)

    if(BUILD_SHARED_LIBS)
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
    endif()
endif()