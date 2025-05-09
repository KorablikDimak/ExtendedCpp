function(set_include_directories)
    foreach(target ${ARGN})
        target_include_directories(${target} PUBLIC
                $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
                $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
    endforeach()
endfunction()

function(install_projects)
    install(TARGETS ${ARGN}
            EXPORT ${PROJECT_NAME}-targets
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
        foreach(target ${ARGN})
            get_target_property(target_pdb_name ${target} COMPILE_PDB_NAME)
            get_target_property(target_pdb_name_debug ${target} COMPILE_PDB_NAME_DEBUG)
            get_target_property(target_pdb_output_directory ${target} PDB_OUTPUT_DIRECTORY)
            install(FILES
                    ${target_pdb_output_directory}/\${CMAKE_INSTALL_CONFIG_NAME}/$<$<CONFIG:Debug>:${target_pdb_name_debug}>$<$<NOT:$<CONFIG:Debug>>:${target_pdb_name}>.pdb
                    DESTINATION ${CMAKE_INSTALL_LIBDIR}
                    OPTIONAL)
        endforeach()
    endif()
endfunction()