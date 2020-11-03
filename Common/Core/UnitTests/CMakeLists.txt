project(CoreUnitTests)

#------------------------------------------------------------------------------
# set output apps directory to bin/"BuildConfiguration"/${PROJECT_NAME}
#------------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/UnitTests)

#------------------------
# Include necessary CMakeModules
#------------------------
include("../../../CMakeModules/Gtest.cmake")
INCLUDE( "../../../CMakeModules/Lodepng.cmake" )
INCLUDE( "../../../CMakeModules/Half.cmake" )
INCLUDE( "../../../CMakeModules/Tga.cmake" )

#------------------------
# Add define
#------------------------
add_definitions(-D_USE_MATH_DEFINES)

#------------------------
# Add sources
#------------------------

add_component("./" "TestFiles")
add_component("./${PLATFORM_SPECIFIC_FOLDER}" "TestFiles")
add_component("../" "Core")
add_component("../${PLATFORM_SPECIFIC_FOLDER}" "Core")
add_component("../ImageSupport" "Core/ImageSupport")
add_component("../../Math" "Math")

#------------------------
# Add and link executable
#------------------------
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} )

# Link test executable against gtest & gtest_main
target_link_libraries(${PROJECT_NAME} 
    ${GTEST_BOTH_LIBRARIES} 
    )
add_test( AllCoreUnitTests ${PROJECT_NAME} )

#------------------------
# install directive
#------------------------

#------------------------
# Install to bin/UnitTests
#------------------------
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/assets/ DESTINATION ${CMAKE_SOURCE_DIR}/bin/UnitTests/CoreAssets )