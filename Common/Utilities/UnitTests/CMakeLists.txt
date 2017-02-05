project(UtilitiesUnitTests)

#------------------------
# Include necessary CMakeModules
#------------------------
include("../../../CMakeModules/Gtest.cmake")

#------------------------
# Add sources
#------------------------
add_component("./" "TestFiles")
add_component("../" "Utilities")

#------------------------
# Add and link executable
#------------------------
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} )

# Link test executable against gtest & gtest_main
target_link_libraries(${PROJECT_NAME} gtest gtest_main)
add_test( AllUtilitiesUnitTests ${PROJECT_NAME} )

#------------------------
# Install to bin/UnitTests
#------------------------
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/bin/UnitTests)