function(set_include_directories)
    foreach (target ${ARGN})
        target_include_directories(${target} PUBLIC
                $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
                $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
    endforeach ()
endfunction()

function(set_interface_directories)
    foreach (target ${ARGN})
        target_include_directories(${target} INTERFACE
                $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
                $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
    endforeach ()
endfunction()

function(add_target_sources)
    foreach (target ${ARGN})
        string(REPLACE "_" "/" target_path "${target}")
        file(GLOB ${target}_HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/${target_path}/*.h")
        list(APPEND ${target}_HEADERS "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/${target_path}.h")
        file(GLOB ${target}_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/${target_path}/*.cpp")

        target_sources(${target}
                PRIVATE ${${target}_SOURCES}
                PUBLIC FILE_SET HEADERS
                BASE_DIRS "${PROJECT_SOURCE_DIR}/include"
                FILES ${${target}_HEADERS})
    endforeach ()
endfunction()

function(add_target_interface_sources)
    foreach (target ${ARGN})
        string(REPLACE "_" "/" target_path "${target}")
        file(GLOB ${target}_HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/${target_path}/*.h")
        list(APPEND ${target}_HEADERS "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/${target_path}.h")

        target_sources(${target}
                INTERFACE FILE_SET HEADERS
                BASE_DIRS "${PROJECT_SOURCE_DIR}/include"
                FILES ${${target}_HEADERS})
    endforeach ()
endfunction()

function(install_projects)
    install(TARGETS ${ARGN}
            EXPORT ${PROJECT_NAME}-targets
            FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    if (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
        foreach (target ${ARGN})
            get_target_property(target_pdb_name ${target} COMPILE_PDB_NAME)
            get_target_property(target_pdb_name_debug ${target} COMPILE_PDB_NAME_DEBUG)
            get_target_property(target_pdb_output_directory ${target} PDB_OUTPUT_DIRECTORY)
            install(FILES
                    ${target_pdb_output_directory}/\${CMAKE_INSTALL_CONFIG_NAME}/$<$<CONFIG:Debug>:${target_pdb_name_debug}>$<$<NOT:$<CONFIG:Debug>>:${target_pdb_name}>.pdb
                    DESTINATION ${CMAKE_INSTALL_LIBDIR}
                    OPTIONAL)
        endforeach ()
    endif ()
endfunction()

function(add_gtest_for target)
    find_package(GTest REQUIRED)

    string(REPLACE "::" "_" target_name "${target}")
    set(tests_name "${target_name}-tests")

    file(GLOB ${tests_name}_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/*.cpp")

    add_executable(${tests_name} ${${tests_name}_SOURCES})
    target_link_libraries(${tests_name} PRIVATE ${target} GTest::gtest GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(${tests_name})
endfunction()

function(add_benchmark_for target)
    find_package(benchmark REQUIRED)

    string(REPLACE "::" "_" target_name "${target}")
    set(benchmarks_name "${target_name}-benchmarks")

    file(GLOB ${benchmarks_name}_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/*.cpp")

    add_executable(${benchmarks_name} ${${benchmarks_name}_SOURCES})
    target_link_libraries(${benchmarks_name} PRIVATE ${target} benchmark::benchmark)

    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(${benchmarks_name} PRIVATE -Wno-c2y-extensions)
    endif ()
endfunction()