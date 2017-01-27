
set( OPENAL_DIR  ${CMAKE_SOURCE_DIR}/thirdParties/OpenAL/v1.1 )
#message( "OPENAL_DIR " ${OPENAL_DIR} )

if(WIN32)
    # add the include path...
    include_directories( ${OPENAL_DIR}/include/ )

    #Specify directories in which the linker will look for libraries.
    link_directories( ${OPENAL_DIR}/libs/Win64/ )
elseif(APPLE)
    find_package(OpenAL)
    if(OPENAL_FOUND)
        include_directories( ${OPENAL_INCLUDE_DIR} )
        # ${OPENAL_LIBRARY} is now defined and points to the OpenALFramework
        # to be linked with the executable
    endif()
endif()

# Copy all required Qt files to iInstallPath
macro(OPENAL_INSTALL iInstallPath)

    if(WIN32)
        #install openAl32.dll
        SET( dlls 
            ${OPENAL_DIR}/libs/Win64/OpenAL32.dll
            ${OPENAL_DIR}/libs/Win64/wrap_oal.dll )

        INSTALL(FILES ${dlls} CONFIGURATIONS debug DESTINATION ${iInstallPath}/Debug )
        INSTALL(FILES ${dlls}  CONFIGURATIONS Release  DESTINATION ${iInstallPath}/Release )
    endif()

endmacro(OPENAL_INSTALL)