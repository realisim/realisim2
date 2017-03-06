
#------------------------------------------------------------------------------
# OpenGL & GLEW
#
# Opengl on mac does not require GLEW. Windows does.
# Librairies (GL and glew) will be added to ${OPENGL_LIBRARIES}
# Include directories will be added.
#
#------------------------------------------------------------------------------
find_package(OpenGL REQUIRED)

IF(WIN32)
  #on ajoute GLEW sur windows, il est dans les thirdParties
  SET( OPENGL_INCLUDE_DIR ${OPENGL_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/ThirdParties/glew-1.13.0/include )
  SET( OPENGL_LIBRARIES ${OPENGL_LIBRARIES} ${CMAKE_SOURCE_DIR}/ThirdParties/glew-1.13.0/lib/Release/x64/glew32.lib)
ENDIF(WIN32)

# add the include directory
include_directories( ${OPENGL_INCLUDE_DIR} )

# the library to link against is ${OPENGL_LIBRARIES}

#------------------------------------------------------------------------------
# Copy all required OpenGL to iInstallPath
#------------------------------------------------------------------------------
macro(OPENGL_INSTALL iInstallPath)

#    if(WIN32)
#        #install openAl32.dll
#        SET( dlls 
#            ${OPENAL_DIR}/libs/Win64/OpenAL32.dll
#            ${OPENAL_DIR}/libs/Win64/wrap_oal.dll )
#
#        INSTALL(FILES ${dlls} CONFIGURATIONS debug DESTINATION ${iInstallPath}/Debug )
#        INSTALL(FILES ${dlls}  CONFIGURATIONS Release  DESTINATION ${iInstallPath}/Release )
#    endif()

endmacro(OPENGL_INSTALL)