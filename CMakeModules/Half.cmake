set( HALF_DIR  ${CMAKE_SOURCE_DIR}/thirdParties/Half/v1.12.0 )
set( HALF_INCLUDE_DIR  ${CMAKE_SOURCE_DIR}/thirdParties/Half/v1.12.0/include )


# add the include path...
include_directories( ${HALF_DIR}/include/ )

################################################
# add the source files to the thirdparty group
#################################################
file( GLOB iPATH_INCLUDE_FILES ${HALF_INCLUDE_DIR}/Half/half.hpp )

source_group (ThirdParties\\Half FILES ${iPATH_INCLUDE_FILES})

# add sources and include to global variable so it can be compiled.
set( INCLUDE_FILES ${INCLUDE_FILES} ${iPATH_INCLUDE_FILES} )

