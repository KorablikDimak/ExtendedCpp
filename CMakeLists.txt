cmake_minimum_required(VERSION 3.5)

project(ExtendedCpp
        VERSION 1.6.6
        DESCRIPTION "Library which extends C++"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(cmake/utils.cmake)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

option(BUILD_SHARED_LIBS "Build libs as shared" ON)
option(BUILD_ALL_TESTS "Build all tests for ExtendedCpp" OFF)
option(LINQ_BUILD_TESTS "Build LINQ library tests" OFF)
option(Events_BUILD_TESTS "Build Events library tests" OFF)
option(Json_BUILD_TESTS "Build Json library tests" OFF)
option(InfoLog_BUILD_TESTS "Build InfoLog library tests" OFF)
option(Common_BUILD_TESTS "Build Common library tests" OFF)
option(DI_BUILD_TESTS "Build DI library tests" OFF)
option(Reflection_BUILD_TESTS "Build Reflection library tests" OFF)
option(LINQ_BUILD_BENCHMARKS "Build LINQ benchmarks" OFF)
option(Common_BUILD_BENCHMARKS "Build Common benchmarks" OFF)
option(ExtendedCpp_INSTALL "Generate target for installing ExtendedCpp" ${is_top_level})
set_if_undefined(ExtendedCpp_INSTALL_CMAKEDIR
        "${CMAKE_INSTALL_LIBDIR}/extendedcpp" CACHE STRING
        "Install path for ExtendedCpp package-related CMake files")

set(LINQ_INCLUDES
        include/LINQ/LINQ.h
        include/LINQ/OrderType.h
        include/LINQ/Algorithm.h
        include/LINQ/LinqContainer.h
        include/LINQ/TypeTraits.h
        include/LINQ/Sort.h
        include/LINQ/Aggregate.h
        include/LINQ/LinqGenerator.h
        include/LINQ/Future.h
        include/LINQ/YieldForeach.h)

set(Events_INCLUDES
        include/Events/Events.h
        include/Events/Event.h
        include/Events/FunctionHandler.h
        include/Events/IEventHandler.h
        include/Events/MethodHandler.h
        include/Events/StaticMethodHandler.h)

set(Json_INCLUDES
        include/Json/Json.h
        include/Json/json.hpp
        include/Json/json_fwd.hpp
        include/Json/JsonMacros.h)

set(Xml_INCLUDES
        include/Xml/Xml.h
        include/Xml/pugixml.hpp
        include/Xml/pugiconfig.hpp)

set(Xml_SOURCE
        src/Xml/pugixml.cpp)

set(InfoLog_INCLUDES
        include/InfoLog/InfoLog.h
        include/InfoLog/Configuration.h
        include/InfoLog/FileSender.h
        include/InfoLog/ISender.h
        include/InfoLog/Logger.h
        include/InfoLog/LoggerFactory.h
        include/InfoLog/LogLevel.h
        include/InfoLog/Parser.h
        include/InfoLog/Converter.h
        include/InfoLog/ConsoleSender.h)

set(InfoLog_SOURCE
        src/InfoLog/Configuration.cpp
        src/InfoLog/FileSender.cpp
        src/InfoLog/ISender.cpp
        src/InfoLog/Logger.cpp
        src/InfoLog/LoggerFactory.cpp
        src/InfoLog/Parser.cpp
        src/InfoLog/ConsoleSender.cpp)

set(Common_INCLUDES
        include/Common/CancellationToken.h
        include/Common/CancellationTokenSource.h
        include/Common/Random.h
        include/Common/Matrix.h
        include/Common/Common.h)

set(Common_SOURCE
        src/Common/CancellationToken.cpp
        src/Common/CancellationTokenSource.cpp)

set(DI_INCLUDES
        include/DI/ServiceProvider.h
        include/DI/Register.h
        include/DI/LifeTime.h
        include/DI/DI.h)

set(Reflection_INCLUDES
        include/Reflection/MemberInfo.h
        include/Reflection/FieldInfo.h
        include/Reflection/TypeInfo.h
        include/Reflection/Reflection.h
        include/Reflection/TypeTraits.h
        include/Reflection/MemberType.h
        include/Reflection/ConstructorInfo.h
        include/Reflection/Assembly.h
        include/Reflection/StaticMethodInfo.h
        include/Reflection/StaticFieldInfo.h
        include/Reflection/MethodInfo.h)

set(Reflection_SOURCE
        src/Reflection/MemberInfo.cpp
        src/Reflection/FieldInfo.cpp
        src/Reflection/TypeInfo.cpp
        src/Reflection/StaticFieldInfo.cpp
        src/Reflection/Assembly.cpp)

add_library(LINQ INTERFACE ${LINQ_INCLUDES})
add_library(ExtendedCpp::LINQ ALIAS LINQ)

add_library(Events INTERFACE ${Events_INCLUDES})
add_library(ExtendedCpp::Events ALIAS Events)

add_library(Json INTERFACE ${Json_INCLUDES})
add_library(ExtendedCpp::Json ALIAS Json)

add_library(Xml ${Xml_INCLUDES} ${Xml_SOURCE})
add_library(ExtendedCpp::Xml ALIAS Xml)

add_library(InfoLog ${InfoLog_INCLUDES} ${InfoLog_SOURCE})
add_library(ExtendedCpp::InfoLog ALIAS InfoLog)

add_library(Common ${Common_INCLUDES} ${Common_SOURCE})
add_library(ExtendedCpp::Common ALIAS Common)

add_library(DI INTERFACE ${DI_INCLUDES})
add_library(ExtendedCpp::DI ALIAS DI)

add_library(Reflection ${Reflection_SOURCE} ${Reflection_INCLUDES})
add_library(ExtendedCpp::Reflection ALIAS Reflection)

set_interface_include_directories(LINQ Events Json DI)
set_public_include_directories(Xml InfoLog Common Reflection)
set_targets_version(LINQ Events Json Xml InfoLog Common DI Reflection)
target_link_libraries(InfoLog PRIVATE Xml Reflection)

if(ExtendedCpp_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    configure_package_config_file(
            cmake/extendedcpp-config.cmake.in extendedcpp-config.cmake
            INSTALL_DESTINATION "${ExtendedCpp_INSTALL_CMAKEDIR}")

    write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/extendedcpp-config-version.cmake"
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY SameMajorVersion)

    install(DIRECTORY include/
            TYPE INCLUDE)

    install(EXPORT ExtendedCpp_export
            NAMESPACE ExtendedCpp::
            FILE "extendedcpp-targets.cmake"
            DESTINATION ${ExtendedCpp_INSTALL_CMAKEDIR})

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/extendedcpp-config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/extendedcpp-config-version.cmake"
            DESTINATION ${ExtendedCpp_INSTALL_CMAKEDIR})

    install_projects(LINQ Events Json Xml InfoLog Common DI Reflection)
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
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/InfoLog/LogConfig.xml
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

        add_custom_command(
                TARGET InfoLog POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Reflection${CMAKE_SHARED_LIBRARY_SUFFIX}
                ${CMAKE_CURRENT_BINARY_DIR}/tests/InfoLog/${CMAKE_SHARED_LIBRARY_PREFIX}Reflection${CMAKE_SHARED_LIBRARY_SUFFIX})
    endif()
endif()

if(Common_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/Common)
endif()

if(Common_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks/Common)
endif()

if(DI_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/DI)
endif()

if(Reflection_BUILD_TESTS OR BUILD_ALL_TESTS)
    enable_testing()
    add_subdirectory(tests/Reflection)

    if(BUILD_SHARED_LIBS)
        add_custom_command(
                TARGET Reflection POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Reflection${CMAKE_SHARED_LIBRARY_SUFFIX}
                ${CMAKE_CURRENT_BINARY_DIR}/tests/Reflection/${CMAKE_SHARED_LIBRARY_PREFIX}Reflection${CMAKE_SHARED_LIBRARY_SUFFIX})
    endif()
endif()