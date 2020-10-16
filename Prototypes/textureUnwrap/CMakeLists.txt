
PROJECT(textureUnwrap)

#output apps in bin/"BuildConfiguration"/${PROJECT_NAME}
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

INCLUDE( "../../CMakeModules/Args.cmake" )
INCLUDE( "../../CMakeModules/Lodepng.cmake" )
INCLUDE( "../../CMakeModules/Half.cmake" )
INCLUDE( "../../CMakeModules/Lodepng.cmake" )
INCLUDE( "../../CMakeModules/TinyObjLoader.cmake" )


set( SOURCE_FILES ${SOURCE_FILES} CMakeLists.txt )
add_component( "./" "application")
add_component( "../../Common/Core" "Core")
add_component( "../../Common/Geometry" "Geometry")
add_component( "../../Common/Math" "Math")
add_component( "../../Common/Core/${PLATFORM_SPECIFIC_FOLDER}" "Core/${PLATFORM_SPECIFIC_FOLDER}")
add_component( "../../Common/Core/ImageSupport" "Core/ImageSupport")

set(GeometryLoaders
    "../../Common/Geometry/Loader/ObjLoader.h"
    "../../Common/Geometry/Loader/ObjLoader.cpp")
add_files("${GeometryLoaders}" "Geometry/Loader")

# create executable
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} )

target_link_libraries( ${PROJECT_NAME} 
    )

