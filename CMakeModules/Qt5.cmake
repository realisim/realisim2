

# Widgets finds its own dependencies (QtGui and QtCore).
FIND_PACKAGE(Qt5Widgets)

FIND_PACKAGE(Qt5OpenGL)

#--------------------------------
# Fonctions
#--------------------------------

MACRO(QT_AUTOMOC)
    # Instruct CMake to run moc automatically when needed.
    set(CMAKE_AUTOMOC ON)
ENDMACRO(QT_AUTOMOC)

# Copy all required Qt files to iInstallPath
MACRO(QT5_INSTALL iInstallPath iModuleList)

ENDMACRO(QT5_INSTALL)