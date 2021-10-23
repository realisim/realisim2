project(RealisimCoreLib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${PROJECT_NAME})

include(../../CMakeModules/Half.cmake)
include(../../CMakeModules/LodePng.cmake)
include(../../CMakeModules/Tga.cmake)

add_component("./" "Core")

#------------------------
# Add and link library
#------------------------
add_library( ${PROJECT_NAME} STATIC ${SOURCE_FILES} ${INCLUDE_FILES} )

#------------------------
# install target
# copy all include files to the appropriate folder
#
#------------------------
INSTALL(FILES ${INCLUDE_FILES} DESTINATION ${CMAKE_SOURCE_DIR}/lib/${PROJECT_NAME}/include)
