project(GeometryUnitTests)

#------------------------------------------------------------------------------
# set output apps directory to bin/"BuildConfiguration"/${PROJECT_NAME}
#------------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/UnitTests)

#------------------------
# Include necessary CMakeModules
#------------------------
include("../../../CMakeModules/Gtest.cmake")

#------------------------
# Add define
#------------------------
add_definitions(-D_USE_MATH_DEFINES)

#------------------------
# Add sources
#------------------------
add_component("./" "TestFiles")
add_component("../../Math" "Math")
add_component("../../Geometry" "Geometry")

#------------------------
# Add and link executable
#------------------------
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} )

# Link test executable against gtest & gtest_main
target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})
add_test( AllGeometryUnitTests ${PROJECT_NAME} )