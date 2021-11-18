project(GeometryLib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${PROJECT_NAME})

add_component("./" "Geometry")

#------------------------
# Add and link library
#------------------------
add_library( ${PROJECT_NAME} STATIC ${SOURCE_FILES} ${INCLUDE_FILES} )
target_link_libraries( ${PROJECT_NAME} 
    MathLib )

#------------------------
# install target
# copy all include files to the appropriate folder
#
#------------------------
INSTALL(FILES ${INCLUDE_FILES} DESTINATION ${CMAKE_SOURCE_DIR}/lib/${PROJECT_NAME}/include)
