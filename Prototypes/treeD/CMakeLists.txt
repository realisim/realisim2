
PROJECT(TreeD)

#output apps in bin/"BuildConfiguration"/${PROJECT_NAME}
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

INCLUDE( "../../CMakeModules/Qt5.cmake" )
INCLUDE( "../../CMakeModules/OpenGL.cmake" )

set( SOURCE_FILES ${SOURCE_FILES} CMakeLists.txt )
add_component( "./" "application")
add_component( "../../Common/Math" "Math")
add_component( "../../Common/3d" "3d")
add_component( "../../Common/Utilities" "Utilities")


qt_automoc()

# create executable
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} )
target_link_libraries( ${PROJECT_NAME} Qt5::Widgets Qt5::OpenGL ${OPENGL_LIBRARIES} )

# install directive
qt5_install( ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} )
