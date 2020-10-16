
SET( ARGS_DIR  ${CMAKE_SOURCE_DIR}/ThirdParties/args )
SET( ARGS_INCLUDE_DIR  ${ARGS_DIR} )

###############################################################################
# add the include path...
###############################################################################
include_directories( ${CMAKE_SOURCE_DIR}/ThirdParties )
include_directories( ${ARGS_INCLUDE_DIR} )

################################################
# add the source files to the thirdparty group
#################################################
set (iPATH_INCLUDE_FILES
    ${ARGS_INCLUDE_DIR}/args.hxx)

source_group (ThirdParties\\args FILES ${iPATH_INCLUDE_FILES})

# add sources and include to global variable so it can be compiled.
#set( SOURCE_FILES ${SOURCE_FILES} ${iPATH_SOURCE_FILES} )
set( INCLUDE_FILES ${INCLUDE_FILES} ${iPATH_INCLUDE_FILES} )