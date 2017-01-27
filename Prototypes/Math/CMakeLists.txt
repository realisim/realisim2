
PROJECT(Math)

#output apps in bin/"BuildConfiguration"/${PROJECT_NAME}
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

#------------------------
# Add sources
#------------------------
set( SOURCE_FILES ${SOURCE_FILES} CMakeLists.txt )
add_component( "." "application")
add_component( "../../Common/Math" "Math")

#------------------------
# Create executable
#------------------------
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} )
target_link_libraries( ${PROJECT_NAME} )

