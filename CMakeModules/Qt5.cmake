
FIND_PACKAGE(Qt5Core)

# Variable to hold the bin directory of Qt, it is dependent on Qt5Core_DIR
get_filename_component(Qt5_BIN_DIR ${Qt5Core_DIR}/../../../bin ABSOLUTE)

# Widgets finds its own dependencies (QtGui and QtCore).
FIND_PACKAGE(Qt5Widgets)

FIND_PACKAGE(Qt5OpenGL)

#------------------------------------------------------------------------------
# remove some anonying warning created by QT
#------------------------------------------------------------------------------
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd4127")

#--------------------------------
# Fonctions
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Instruct CMake to run moc automatically when needed.
#------------------------------------------------------------------------------
MACRO(QT_AUTOMOC)
    set(CMAKE_AUTOMOC ON)
ENDMACRO(QT_AUTOMOC)

#------------------------------------------------------------------------------
# Instruct CMake to run rcc automatically when needed.
#------------------------------------------------------------------------------
MACRO(QT_AUTORCC)
    set(CMAKE_AUTORCC ON)
ENDMACRO(QT_AUTORCC)

#------------------------------------------------------------------------------
# Instruct CMake to run uic automatically when needed.
#------------------------------------------------------------------------------
MACRO(QT_AUTOUIC)
    set(CMAKE_AUTOUIC ON)
ENDMACRO(QT_AUTOUIC)

# Copy all required Qt files to iInstallPath
MACRO(QT5_INSTALL iInstallPath)

    IF(WIN32)
        #MESSAGE("path to QtCore: " ${Qt5Core_DIR} )
        #MESSAGE("path to Qt Bin: " ${Qt5_BIN_DIR} )
        #MESSAGE("Qt5Widgets_LIBRARIES in use: " ${Qt5Widgets_LIBRARIES} )
        #MESSAGE("Qt5OpenGL_LIBRARIES in use: " ${Qt5OpenGL_LIBRARIES} )

        #--- dll related to Qt5Widgets
        SET(QT5WIDGETS_FILES_DEBUG "")
        SET(QT5WIDGETS_FILES_RELEASE "")
        IF (NOT "${Qt5Widgets_LIBRARIES}" STREQUAL "")
            SET(QT5WIDGETS_FILES_DEBUG ${Qt5_BIN_DIR}/Qt5Cored.dll
                ${Qt5_BIN_DIR}/Qt5Guid.dll
                ${Qt5_BIN_DIR}/Qt5Widgetsd.dll)

            SET(QT5WIDGETS_FILES_RELEASE ${Qt5_BIN_DIR}/Qt5Core.dll
                ${Qt5_BIN_DIR}/Qt5Gui.dll
                ${Qt5_BIN_DIR}/Qt5Widgets.dll )
        ENDIF()

        #--- dll related to Qt5OpenGL
        SET(QT5OPENGL_FILES_DEBUG "")
        SET(QT5OPENGL_FILES_RELEASE "")
        IF (NOT "${Qt5OpenGL_LIBRARIES}" STREQUAL "")
            SET(QT5OPENGL_FILES_DEBUG ${Qt5_BIN_DIR}/Qt5OpenGLd.dll )

            SET(QT5OPENGL_FILES_RELEASE ${Qt5_BIN_DIR}/Qt5OpenGL.dll )
        ENDIF()

        # --- and now install...
        # --- in DEBUG
        INSTALL(FILES ${QT5WIDGETS_FILES_DEBUG} CONFIGURATIONS Debug DESTINATION ${iInstallPath}/Debug)
        INSTALL(FILES ${QT5OPENGL_FILES_DEBUG} CONFIGURATIONS Debug DESTINATION ${iInstallPath}/Debug)

        # ----- RELEASE
        INSTALL(FILES ${QT5WIDGETS_FILES_RELEASE} CONFIGURATIONS Release DESTINATION ${iInstallPath}/Release)
        INSTALL(FILES ${QT5OPENGL_FILES_RELEASE} CONFIGURATIONS Release DESTINATION ${iInstallPath}/Release)

        # ----- RELWITHDEBINFO
        INSTALL(FILES ${QT5WIDGETS_FILES_RELEASE} CONFIGURATIONS RelWithDebInfo DESTINATION ${iInstallPath}/RelWithDebInfo)
        INSTALL(FILES ${QT5OPENGL_FILES_RELEASE} CONFIGURATIONS RelWithDebInfo DESTINATION ${iInstallPath}/RelWithDebInfo)
    ENDIF(WIN32)

ENDMACRO(QT5_INSTALL)