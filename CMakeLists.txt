# ROOT CMakeList
PROJECT( Realisim2 )

cmake_minimum_required (VERSION 2.8.11)

#Global variables to hold source/include files
set( SOURCE_FILES "" )
set( INCLUDE_FILES "" )

INCLUDE( "CMakeModules/AddComponent.cmake" )

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# add variables to the GUI
set(Prototype_to_build "" CACHE PATH "Path to prototype")
option(BUILD_UNIT_TESTS "Builds the unit tests" OFF)

# add our code and projects
ADD_SUBDIRECTORY(Prototypes)

# unit tests if desired
if( BUILD_UNIT_TESTS )
    add_subdirectory(Common/Math/UnitTest)
endif()
