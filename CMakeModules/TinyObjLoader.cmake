
set( TINYOBJLOADER_DIR  ${CMAKE_SOURCE_DIR}/thirdParties/tinyObjLoader/v2.0.0rc7 )
#message( "TINYOBJLOADER_DIR " ${TINYOBJLOADER_DIR} )


# add the include path...
include_directories(${CMAKE_SOURCE_DIR}/thirdParties)
include_directories( ${TINYOBJLOADER_DIR}/sources/ )

################################################
# add the source files to the thirdparty group
#################################################
file( GLOB iPATH_SOURCE_FILES ${TINYOBJLOADER_DIR}/src//*.cc )
file( GLOB iPATH_INCLUDE_FILES ${TINYOBJLOADER_DIR}/src//*.h )

source_group (ThirdParties\\tinyObjLoader FILES ${iPATH_INCLUDE_FILES})
source_group (ThirdParties\\tinyObjLoader FILES ${iPATH_SOURCE_FILES})

# add sources and include to global variable so it can be compiled.
set( SOURCE_FILES ${SOURCE_FILES} ${iPATH_SOURCE_FILES} )
set( INCLUDE_FILES ${INCLUDE_FILES} ${iPATH_INCLUDE_FILES} )

