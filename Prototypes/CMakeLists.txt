
# check if a prototype has been selected
IF (NOT ${Prototype_to_build} STREQUAL "" )

    get_filename_component(FOLDERNAME ${Prototype_to_build} NAME)
    ADD_SUBDIRECTORY( ${FOLDERNAME} )

ENDIF()