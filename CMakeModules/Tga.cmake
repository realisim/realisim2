
set( TGA_DIR  ${CMAKE_SOURCE_DIR}/thirdParties/tga/v4d8df65 )
#message( "TGA_DIR " ${TGA_DIR} )


# add the include path...
include_directories(${CMAKE_SOURCE_DIR}/thirdParties)

################################################
# add the source files to the thirdparty group
#################################################
file( GLOB iPATH_INCLUDE_FILES ${TGA_DIR}/*.hpp )

source_group (ThirdParties\\tga FILES ${iPATH_INCLUDE_FILES})

# add sources and include to global variable so it can be compiled.
set( INCLUDE_FILES ${INCLUDE_FILES} ${iPATH_INCLUDE_FILES} )


###############################################
# compilation defines
###############################################
add_definitions(-DREALISIM_THIRDPARTIES_TGA)
