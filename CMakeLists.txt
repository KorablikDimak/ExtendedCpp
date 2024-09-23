cmake_minimum_required(VERSION 3.5)

project(ExtendedCpp
        VERSION 2.0.1
        DESCRIPTION "Library which extends C++"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(cmake/utils.cmake)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

option(ExtendedCpp_INSTALL "Generate target for installing ExtendedCpp" ${is_top_level})
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

set_if_undefined(ExtendedCpp_INSTALL_CMAKEDIR
        "${CMAKE_INSTALL_LIBDIR}/extendedcpp" CACHE STRING
        "Install path for ExtendedCpp package-related CMake files")

set(LINQ_INCLUDES
        include/ExtendedCpp/LINQ.h
        include/ExtendedCpp/LINQ/OrderType.h
        include/ExtendedCpp/LINQ/Algorithm.h
        include/ExtendedCpp/LINQ/LinqContainer.h
        include/ExtendedCpp/LINQ/TypeTraits.h
        include/ExtendedCpp/LINQ/Sort.h
        include/ExtendedCpp/LINQ/Aggregate.h
        include/ExtendedCpp/LINQ/LinqGenerator.h
        include/ExtendedCpp/LINQ/Future.h
        include/ExtendedCpp/LINQ/YieldForeach.h
        include/ExtendedCpp/LINQ/Concepts.h
        include/ExtendedCpp/LINQ/LinqView.h
        include/ExtendedCpp/LINQ/Iterators.h)

set(Events_INCLUDES
        include/ExtendedCpp/Events.h
        include/ExtendedCpp/Events/Event.h
        include/ExtendedCpp/Events/FunctionHandler.h
        include/ExtendedCpp/Events/IEventHandler.h
        include/ExtendedCpp/Events/MethodHandler.h
        include/ExtendedCpp/Events/StaticMethodHandler.h
        include/ExtendedCpp/Events/ConstMethodHandler.h)

set(Json_INCLUDES
        include/ExtendedCpp/Json.h
        include/ExtendedCpp/Json/json.hpp
        include/ExtendedCpp/Json/json_fwd.hpp
        include/ExtendedCpp/Json/JsonMacros.h)

set(Xml_INCLUDES
        include/ExtendedCpp/Xml.h
        include/ExtendedCpp/Xml/pugixml.hpp
        include/ExtendedCpp/Xml/pugiconfig.hpp)

set(Xml_SOURCE
        src/Xml/pugixml.cpp)

set(InfoLog_INCLUDES
        include/ExtendedCpp/InfoLog.h
        include/ExtendedCpp/InfoLog/Configuration.h
        include/ExtendedCpp/InfoLog/FileSender.h
        include/ExtendedCpp/InfoLog/ISender.h
        include/ExtendedCpp/InfoLog/Logger.h
        include/ExtendedCpp/InfoLog/LoggerFactory.h
        include/ExtendedCpp/InfoLog/LogLevel.h
        include/ExtendedCpp/InfoLog/Parser.h
        include/ExtendedCpp/InfoLog/Converter.h
        include/ExtendedCpp/InfoLog/ConsoleSender.h
        include/ExtendedCpp/InfoLog/Concepts.h)

set(InfoLog_SOURCE
        src/InfoLog/Configuration.cpp
        src/InfoLog/FileSender.cpp
        src/InfoLog/ISender.cpp
        src/InfoLog/Logger.cpp
        src/InfoLog/LoggerFactory.cpp
        src/InfoLog/Parser.cpp
        src/InfoLog/ConsoleSender.cpp)

set(Common_INCLUDES
        include/ExtendedCpp/Cancellation/CancellationToken.h
        include/ExtendedCpp/Cancellation/CancellationTokenSource.h
        include/ExtendedCpp/Random.h
        include/ExtendedCpp/Matrix.h
        include/ExtendedCpp/Cancellation.h
        include/ExtendedCpp/Concepts.h)

set(Common_SOURCE
        src/Cancellation/CancellationToken.cpp
        src/Cancellation/CancellationTokenSource.cpp)

set(DI_INCLUDES
        include/ExtendedCpp/DI/ServiceProvider.h
        include/ExtendedCpp/DI/Register.h
        include/ExtendedCpp/DI/LifeTime.h
        include/ExtendedCpp/DI.h
        include/ExtendedCpp/DI/Injector.h
        include/ExtendedCpp/DI/Concepts.h)

set(DI_SOURCE
        src/DI/ServiceProvider.cpp)

set(Reflection_INCLUDES
        include/ExtendedCpp/Reflection/MemberInfo.h
        include/ExtendedCpp/Reflection/FieldInfo.h
        include/ExtendedCpp/Reflection/TypeInfo.h
        include/ExtendedCpp/Reflection.h
        include/ExtendedCpp/Reflection/TypeTraits.h
        include/ExtendedCpp/Reflection/MemberType.h
        include/ExtendedCpp/Reflection/ConstructorInfo.h
        include/ExtendedCpp/Reflection/Assembly.h
        include/ExtendedCpp/Reflection/StaticMethodInfo.h
        include/ExtendedCpp/Reflection/StaticFieldInfo.h
        include/ExtendedCpp/Reflection/MethodInfo.h
        include/ExtendedCpp/Reflection/Concepts.h)

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

add_library(DI ${DI_SOURCE} ${DI_INCLUDES})
add_library(ExtendedCpp::DI ALIAS DI)

add_library(Reflection ${Reflection_SOURCE} ${Reflection_INCLUDES})
add_library(ExtendedCpp::Reflection ALIAS Reflection)

set_interface_include_directories(LINQ Events Json)
set_public_include_directories(Xml InfoLog Common Reflection DI)
set_targets_version(LINQ Events Json Xml InfoLog Common DI Reflection)
target_link_libraries(InfoLog PRIVATE Xml Reflection)
target_link_libraries(DI PRIVATE Reflection)

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

    if(BUILD_SHARED_LIBS)
        add_custom_command(
                TARGET DI POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Reflection${CMAKE_SHARED_LIBRARY_SUFFIX}
                ${CMAKE_CURRENT_BINARY_DIR}/tests/DI/${CMAKE_SHARED_LIBRARY_PREFIX}Reflection${CMAKE_SHARED_LIBRARY_SUFFIX})

        add_custom_command(
                TARGET DI POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}DI${CMAKE_SHARED_LIBRARY_SUFFIX}
                ${CMAKE_CURRENT_BINARY_DIR}/tests/DI/${CMAKE_SHARED_LIBRARY_PREFIX}DI${CMAKE_SHARED_LIBRARY_SUFFIX})
    endif()
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