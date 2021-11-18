
PROJECT(Shader)

#output apps in bin/"BuildConfiguration"/${PROJECT_NAME}
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

INCLUDE( "../../CMakeModules/Half.cmake" )
INCLUDE( "../../CMakeModules/Lodepng.cmake" )
INCLUDE( "../../CMakeModules/OpenGL.cmake" )
INCLUDE( "../../CMakeModules/Qt5.cmake" )
INCLUDE( "../../CMakeModules/Tga.cmake" )

include_directories("..")

set( SOURCE_FILES ${SOURCE_FILES} CMakeLists.txt )
add_component( "./" "application")
add_component( "../GlPrototyping" "GlPrototyping")
add_component( "../GlPrototyping/assets" "GlPrototyping/assets")
add_component( "../../Common/Math" "Math")
add_component( "../../Common/Rendering" "Rendering")
add_component( "../../Common/Rendering/Gpu" "Rendering/Gpu")
add_component( "../../Common/Core" "Core")
add_component( "../../Common/Core/${PLATFORM_SPECIFIC_FOLDER}" "Core/${PLATFORM_SPECIFIC_FOLDER}")
add_component( "../../Common/Core/ImageSupport" "Core/ImageSupport")
add_component( "../../Common/Geometry" "Geometry")

qt_automoc()

# create executable
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} )
target_link_libraries( ${PROJECT_NAME} Qt5::Widgets Qt5::OpenGL ${OPENGL_LIBRARIES} )

# install directive
qt5_install( ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} )
opengl_install( ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} )

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../GlPrototyping/assets/ DESTINATION ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}/assets )
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/assets/ DESTINATION ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}/assets )
