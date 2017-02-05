# ROOT CMakeList
PROJECT( Realisim2 )

cmake_minimum_required (VERSION 2.8.11)

#------------------------------
# Compiler flags
#------------------------------
if(APPLE)
    # this is necessary for some c++11 features such as atomic.
    # MSVC does not require any special flags for c++11
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif(APPLE)

#------------------------------
# Global variables to hold source/include files
#------------------------------
set( SOURCE_FILES "" )
set( INCLUDE_FILES "" )

INCLUDE( "CMakeModules/AddComponent.cmake" )

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#------------------------------
# add variables to the GUI
#------------------------------
option(BUILD_UNIT_TESTS "Builds the unit tests" OFF)
option(BUILD_PROTOTYPE "Builds the protype specified by Prototype_to_build" OFF)
set(Prototype_to_build "" CACHE PATH "Path to prototype")

#-----------------------------
# add subdirectories
#-----------------------------
if( BUILD_PROTOTYPE )
    ADD_SUBDIRECTORY(Prototypes)
endif()

# unit tests if desired
if( BUILD_UNIT_TESTS )
    add_subdirectory(Common/Math/UnitTest)
    add_subdirectory(Common/Utilities/UnitTests)
endif()
