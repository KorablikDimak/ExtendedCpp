cmake_minimum_required(VERSION 3.25)

project(ExtendedCpp
        VERSION 2.2.1
        DESCRIPTION "Library which extends C++"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(cmake/utils.cmake)

option(ExtendedCpp_INSTALL "Generate target for installing ExtendedCpp" ON)
option(BUILD_SHARED_LIBS "Build libs as shared" ON)

if(BUILD_SHARED_LIBS)
    set(LIBRARY_TYPE SHARED)
else()
    set(LIBRARY_TYPE STATIC)
endif()

option(BUILD_ALL_TESTS "Build all tests for ExtendedCpp" OFF)
option(LINQ_BUILD_TESTS "Build LINQ library tests" OFF)
option(Events_BUILD_TESTS "Build Events library tests" OFF)
option(Json_BUILD_TESTS "Build Json library tests" OFF)
option(Reflection_BUILD_TESTS "Build Reflection library tests" OFF)
option(InfoLog_BUILD_TESTS "Build InfoLog library tests" OFF)
option(Common_BUILD_TESTS "Build Common library tests" OFF)
option(DI_BUILD_TESTS "Build DI library tests" OFF)
option(Asio_BUILD_TESTS "Build Asio library tests" OFF)

option(LINQ_BUILD_BENCHMARKS "Build LINQ benchmarks" OFF)
option(Common_BUILD_BENCHMARKS "Build Common benchmarks" OFF)

set(LINQ_SOURCE
        src/LINQ/LINQ.cpp)

set(Events_SOURCE
        src/Events/Events.cpp)

set(Json_SOURCE
        src/Json/Json.cpp)

set(Xml_SOURCE
        src/Xml/pugixml.cpp)

set(Reflection_SOURCE
        src/Reflection/MemberInfo.cpp
        src/Reflection/FieldInfo.cpp
        src/Reflection/TypeInfo.cpp
        src/Reflection/StaticFieldInfo.cpp
        src/Reflection/Assembly.cpp
        src/Reflection/ConstructorInfo.cpp
        src/Reflection/MethodInfo.cpp
        src/Reflection/Reflection.cpp
        src/Reflection/StaticMethodInfo.cpp)

set(InfoLog_SOURCE
        src/InfoLog/Configuration.cpp
        src/InfoLog/FileSender.cpp
        src/InfoLog/ISender.cpp
        src/InfoLog/Logger.cpp
        src/InfoLog/LoggerFactory.cpp
        src/InfoLog/Parser.cpp
        src/InfoLog/ConsoleSender.cpp)

set(Common_SOURCE
        src/Cancellation/CancellationToken.cpp
        src/Cancellation/CancellationTokenSource.cpp)

set(DI_SOURCE
        src/DI/ServiceProvider.cpp)

set(Asio_SOURCE
        src/Asio/Asio.cpp)

add_library(LINQ ${LIBRARY_TYPE} ${LINQ_SOURCE})
add_library(ExtendedCpp::LINQ ALIAS LINQ)

add_library(Events ${LIBRARY_TYPE} ${Events_SOURCE})
add_library(ExtendedCpp::Events ALIAS Events)

add_library(Json ${LIBRARY_TYPE} ${Json_SOURCE})
add_library(ExtendedCpp::Json ALIAS Json)

add_library(Xml ${LIBRARY_TYPE} ${Xml_SOURCE})
add_library(ExtendedCpp::Xml ALIAS Xml)

add_library(Reflection ${LIBRARY_TYPE} ${Reflection_SOURCE})
add_library(ExtendedCpp::Reflection ALIAS Reflection)
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(Reflection PUBLIC /Zc:preprocessor)
endif()

add_library(InfoLog ${LIBRARY_TYPE} ${InfoLog_SOURCE})
add_library(ExtendedCpp::InfoLog ALIAS InfoLog)
target_link_libraries(InfoLog PUBLIC Xml Reflection)

add_library(Common ${LIBRARY_TYPE} ${Common_SOURCE})
add_library(ExtendedCpp::Common ALIAS Common)

add_library(DI ${LIBRARY_TYPE} ${DI_SOURCE})
add_library(ExtendedCpp::DI ALIAS DI)
target_link_libraries(DI PUBLIC Reflection)

add_library(Asio ${LIBRARY_TYPE} ${Asio_SOURCE})
add_library(ExtendedCpp::Asio ALIAS Asio)

set_include_directories(LINQ Events Json Xml InfoLog Common Reflection DI Asio)

if(ExtendedCpp_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
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

    install_projects(LINQ Events Json Xml InfoLog Common DI Reflection Asio)
endif()

if(LINQ_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/LINQ)
endif()

if(Events_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/Events)
endif()

if(Json_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/Json)
endif()

if(Reflection_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/Reflection)
endif()

if(InfoLog_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/InfoLog)
endif()

if(Common_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/Common)
endif()

if(DI_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/DI)
endif()

if(Asio_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/Asio)
endif()

if(LINQ_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks/LINQ)
endif()

if(Common_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks/Common)
endif()