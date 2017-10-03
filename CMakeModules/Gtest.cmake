
set( GTEST_DIR  ${CMAKE_SOURCE_DIR}/thirdParties/Gtest/v1.8.0 )
#message( "GTEST_DIR " ${GTEST_DIR} )

set( GTEST_BOTH_LIBRARIES gtest gtest_main)

set( GTEST_LIBRARY gtest )
set( GTEST_LIBRARY_DEBUG gtest )

set( GTEST_MAIN_LIBRARY gtest_main)
set( GTEST_MAIN_LIBRARY_DEBUG gtest_main)

# add the include path...
include_directories( ${GTEST_DIR}/include/ )

#Specify directories in which the linker will look for libraries.
if(WIN32)
    link_directories( ${GTEST_DIR}/libs/Win64/ )
elseIF(APPLE)
    link_directories( ${GTEST_DIR}/libs/osx/ )
endif()

enable_testing()
