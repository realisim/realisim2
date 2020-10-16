project(GeometryUnitTests)

#------------------------------------------------------------------------------
# set output apps directory to bin/"BuildConfiguration"/${PROJECT_NAME}
#------------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/UnitTests)

#------------------------
# Include necessary CMakeModules
#------------------------
include("../../../CMakeModules/Gtest.cmake")
include("../../../CMakeModules/TinyObjLoader.cmake")

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
add_component("../../Geometry/Loader" "Geometry/Loader")

set(CORE_FILE
    ../../Core/FileInfo.h
    ../../Core/Windows/FileInfo.cpp

    ../../Core/Path.h
    ../../Core/Windows/Path.cpp

    ../../Core/StringUtilities.h
    ../../Core/StringUtilities.cpp

    ../../Core/Timer.h
    ../../Core/Timer.cpp)
add_files("${CORE_FILE}" "Core")

#------------------------
# Add and link executable
#------------------------
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} )

# Link test executable against gtest & gtest_main
target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})
add_test( AllGeometryUnitTests ${PROJECT_NAME} )

#------------------------
# Install to bin/UnitTests
#------------------------
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/assets/ DESTINATION ${CMAKE_SOURCE_DIR}/bin/UnitTests/GeometryAssets )