Project(Reactor)

#output apps in bin/"BuildConfiguration"/${PROJECT_NAME}
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

#------------------------
# Add define
#------------------------
add_definitions(-D_USE_MATH_DEFINES)
add_definitions(-DNOMINMAX)

INCLUDE( "../../CMakeModules/Half.cmake" )
INCLUDE( "../../CMakeModules/OpenGL.cmake" )

set( SOURCE_FILES ${SOURCE_FILES} CMakeLists.txt )
add_component( "./" "Reactor")
add_component( "./Systems" "Reactor/Systems")
add_component( "./Systems/Renderer" "Reactor/Systems/Renderer")
add_component( "./DataStructures" "Reactor/DataStructures")
add_component( "../../Common/Rendering" "Common/Rendering")
add_component( "../../Common/Rendering/Gpu" "Common/Rendering/Gpu")

# create library target
add_library( ${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} )
target_link_libraries( ${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    )