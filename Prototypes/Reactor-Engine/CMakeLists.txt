
PROJECT(Reactor-EngineProto)

#output apps in bin/"BuildConfiguration"/${PROJECT_NAME}
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

INCLUDE( "../../CMakeModules/Qt5.cmake" )
INCLUDE( "../../CMakeModules/OpenGL.cmake" )
INCLUDE( "../../CMakeModules/Lodepng.cmake" )
INCLUDE( "../../CMakeModules/Tga.cmake" )
INCLUDE( "../../CMakeModules/Half.cmake" )

set( SOURCE_FILES ${SOURCE_FILES} CMakeLists.txt )
add_component( "./" "application")

#------------------------------------------------------------------------------
# additional include directories
#
# AssetProcessor needs the Reactor Engine, most of the code does #include "Reactor/blabla/blabla"
# so we give access to Prjects Folder.
#------------------------------------------------------------------------------
INCLUDE_DIRECTORIES("../../Projects/")
INCLUDE_DIRECTORIES("../../Projects/Reactor")

qt_automoc()

# create executable
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} )
target_link_libraries( ${PROJECT_NAME} 
    Reactor
    CoreLib
    InterfaceLib
    MathLib
    ThreeDLib
    Qt5::Widgets Qt5::OpenGL )

# install directive
qt5_install( ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} )
opengl_install( ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} )
install(DIRECTORY ${CMAKE_SOURCE_DIR}/Projects/Reactor/Assets DESTINATION ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} )