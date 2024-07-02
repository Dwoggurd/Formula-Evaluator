#-------------------------------------------------------------------------------
# Formula Evaluator
#-------------------------------------------------------------------------------
# Examples for Formula Evaluator
#-------------------------------------------------------------------------------

# Copy scripts to /out dir
file(GLOB PTREE_SCRIPTS "${CMAKE_CURRENT_SOURCE_DIR}/Scripts/*.in")
foreach(SRC ${PTREE_SCRIPTS})
    get_filename_component(DST ${SRC} NAME_WLE)
    configure_file("${SRC}" "${CMAKE_SOURCE_DIR}/out/${DST}"
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                         GROUP_READ GROUP_WRITE GROUP_EXECUTE
                         WORLD_READ WORLD_EXECUTE)
endforeach()

# Copy configs to /out dir
file(GLOB_RECURSE PTREE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Config/*")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${PTREE_FILES}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Config" DESTINATION "${CMAKE_SOURCE_DIR}/out/")
