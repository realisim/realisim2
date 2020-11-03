
set( LODEPNG_DIR  ${CMAKE_SOURCE_DIR}/thirdParties/lodepng/v20161127 )
#message( "LODEPNG_DIR " ${LODEPNG_DIR} )


# add the include path...
include_directories( ${LODEPNG_DIR}/sources/ )

################################################
# add the source files to the thirdparty group
#################################################
file( GLOB iPATH_SOURCE_FILES ${LODEPNG_DIR}/sources//*.cpp )
file( GLOB iPATH_INCLUDE_FILES ${LODEPNG_DIR}/sources//*.h )

source_group (ThirdParties\\lodepng FILES ${iPATH_INCLUDE_FILES})
source_group (ThirdParties\\lodepng FILES ${iPATH_SOURCE_FILES})

# add sources and include to global variable so it can be compiled.
set( SOURCE_FILES ${SOURCE_FILES} ${iPATH_SOURCE_FILES} )
set( INCLUDE_FILES ${INCLUDE_FILES} ${iPATH_INCLUDE_FILES} )

###############################################
# compilation defines
###############################################
add_definitions(-DREALISIM_THIRDPARTIES_LODEPNG)
