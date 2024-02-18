macro(set_if_undefined variable)
    if(NOT DEFINED "${variable}")
        set("${variable}" ${ARGN})
    endif()
endmacro()

function(set_interface_include_directories)
    foreach(target ${ARGN})
        target_include_directories(${target} INTERFACE
                "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
                "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>"
                "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
    endforeach()
endfunction()

function(set_public_include_directories)
    foreach(target ${ARGN})
        target_include_directories(${target} PUBLIC
                "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
                "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>"
                "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
                PRIVATE "${PROJECT_SOURCE_DIR}/src")
    endforeach()
endfunction()

function(set_targets_version)
    foreach(target ${ARGN})
        set_target_properties(${target} PROPERTIES
                SOVERSION ${PROJECT_VERSION_MAJOR}
                VERSION ${PROJECT_VERSION})
    endforeach()
endfunction()

function(install_projects)
    install(TARGETS ${ARGN}
            EXPORT "${PROJECT_NAME}_export"
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        foreach(target ${ARGN})
            get_target_property(target_pdb_name ${target} COMPILE_PDB_NAME)
            get_target_property(target_pdb_name_debug ${target} COMPILE_PDB_NAME_DEBUG)
            get_target_property(target_pdb_output_directory ${target} PDB_OUTPUT_DIRECTORY)
            install(FILES
                    "${target_pdb_output_directory}/\${CMAKE_INSTALL_CONFIG_NAME}/$<$<CONFIG:Debug>:${target_pdb_name_debug}>$<$<NOT:$<CONFIG:Debug>>:${target_pdb_name}>.pdb"
                    DESTINATION ${CMAKE_INSTALL_LIBDIR}
                    OPTIONAL)
        endforeach()
    endif()
endfunction()