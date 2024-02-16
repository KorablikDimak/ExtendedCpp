function(install_projects)
    install(TARGETS ${ARGN}
            EXPORT ExtendedCpp_export
            COMPONENT "${PROJECT_NAME}"
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        foreach(t ${ARGN})
            get_target_property(t_pdb_name ${t} COMPILE_PDB_NAME)
            get_target_property(t_pdb_name_debug ${t} COMPILE_PDB_NAME_DEBUG)
            get_target_property(t_pdb_output_directory ${t} PDB_OUTPUT_DIRECTORY)
            install(FILES
                    "${t_pdb_output_directory}/\${CMAKE_INSTALL_CONFIG_NAME}/$<$<CONFIG:Debug>:${t_pdb_name_debug}>$<$<NOT:$<CONFIG:Debug>>:${t_pdb_name}>.pdb"
                    COMPONENT "${PROJECT_NAME}"
                    DESTINATION ${CMAKE_INSTALL_LIBDIR}
                    OPTIONAL)
        endforeach()
    endif()
endfunction()