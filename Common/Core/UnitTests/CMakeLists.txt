project(CoreUnitTests)

#------------------------
# Include necessary CMakeModules
#------------------------
include("../../../CMakeModules/Gtest.cmake")
INCLUDE( "../../../CMakeModules/Lodepng.cmake" )
INCLUDE( "../../../CMakeModules/Half.cmake" )
INCLUDE( "../../../CMakeModules/Tga.cmake" )

#------------------------
# Add sources
#------------------------

add_component("./" "TestFiles")
add_component("./${PLATFORM_SPECIFIC_FOLDER}" "TestFiles")

#------------------------
# Add and link executable
#------------------------
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} )

# Link test executable against gtest & gtest_main
target_link_libraries(${PROJECT_NAME} 
    ${GTEST_BOTH_LIBRARIES}
    CoreLib
    MathLib
    )
add_test( AllCoreUnitTests ${PROJECT_NAME} )

#------------------------
# install directive
#------------------------

#------------------------
# Install to bin/UnitTests
#------------------------
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/assets/ DESTINATION ${CMAKE_SOURCE_DIR}/bin/UnitTests/CoreAssets )