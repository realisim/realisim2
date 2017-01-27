project(MathUnitTest)

include("../../../CMakeModules/Gtest.cmake")

# Add test cpp file
add_executable( ${PROJECT_NAME} UnitTest.cpp )

# Link test executable against gtest & gtest_main
target_link_libraries(${PROJECT_NAME} gtest gtest_main)
add_test( AllMathUnitTests ${PROJECT_NAME} )

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/bin/UnitTests)