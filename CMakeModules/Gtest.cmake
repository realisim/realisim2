
set( GTEST_DIR  ${CMAKE_SOURCE_DIR}/thirdParties/Gtest/v1.8.0 )
message( "GTEST_DIR " ${GTEST_DIR} )

# add the include path...
include_directories( ${GTEST_DIR}/include/ )

#Specify directories in which the linker will look for libraries.
link_directories( ${GTEST_DIR}/libs/osx/ )

enable_testing()
