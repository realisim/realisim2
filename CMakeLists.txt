# ROOT CMakeList
PROJECT( Realisim2 )

cmake_minimum_required (VERSION 2.8.11)

#Global variables to hold source/include files
SET( SOURCE_FILES "" )
SET( INCLUDE_FILES "" )

INCLUDE( "CMakeModules/AddComponent.cmake" )
INCLUDE( "CMakeModules/OpenAL.cmake" )
INCLUDE( "CMakeModules/Qt5.cmake" )

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# add variables to the GUI
SET(Prototype_to_build "" CACHE PATH "Path to prototype")

ADD_SUBDIRECTORY(Prototypes)
