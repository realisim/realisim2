
SET( OPENAL_DIR  ${CMAKE_SOURCE_DIR}/thirdParties/OpenAL/v1.1 )
#MESSAGE( "OPENAL_DIR " ${OPENAL_DIR} )

# add the include path...
INCLUDE_DIRECTORIES( ${OPENAL_DIR}/include/ )

#Specify directories in which the linker will look for libraries.
LINK_DIRECTORIES( ${OPENAL_DIR}/libs/Win64/ )

# Copy all required Qt files to iInstallPath
MACRO(OPENAL_INSTALL iInstallPath)

    #install openAl32.dll
    INSTALL(FILES ${OPENAL_DIR}/libs/Win64/OpenAL32.dll CONFIGURATIONS debug DESTINATION ${iInstallPath}/Debug )
    INSTALL(FILES ${OPENAL_DIR}/libs/Win64/OpenAL32.dll  CONFIGURATIONS Release  DESTINATION ${iInstallPath}/Release )

ENDMACRO(OPENAL_INSTALL)