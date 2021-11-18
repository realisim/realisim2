# ROOT CMakeList
PROJECT( Realisim2 )

cmake_minimum_required (VERSION 2.8.12)

#------------------------------
# Global variables to hold source/include files
#------------------------------
set( SOURCE_FILES "" )
set( INCLUDE_FILES "" )

# general include_directories
include_directories("Common")

#------------------------------------------------------------------------------
# GLobal variable to hold the platform specific folder.
# this represent the name of the folder containing the platform
# specific code, libs etc...
#------------------------------------------------------------------------------
SET(PLATFORM_SPECIFIC_FOLDER "")
IF(WIN32)
    SET(PLATFORM_SPECIFIC_FOLDER "windows")
ELSEIF(APPLE)
    SET(PLATFORM_SPECIFIC_FOLDER "mac")
ENDIF(WIN32)
message("PLATFORM_SPECIFIC_FOLDER " ${PLATFORM_SPECIFIC_FOLDER})


#------------------------------------------------------------------------------
# add supplementary functions to cmake
#------------------------------------------------------------------------------
INCLUDE( "CMakeModules/AddComponent.cmake" )
INCLUDE( "CMakeModules/AddFiles.cmake" )

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#------------------------------------------------------------------------------
# Options and variables in CMake GUI
#------------------------------------------------------------------------------
option(BUILD_REALISIM_AS_STATIC_LIBS "Builds all realisim libs" OFF)
option(BUILD_PROJECT_LIGHTBEAM "Builds LightBeam" OFF)
option(BUILD_PROJECT_REACTOR "Builds Reactor" OFF)
option(BUILD_UNIT_TESTS "Builds the unit tests" OFF)
option(BUILD_PROTOTYPE "Builds the protype specified by Prototype_to_build" OFF)
set(Prototype_to_build "" CACHE PATH "Path to prototype")

#------------------------------------------------------------------------------
# Compiler flags
#------------------------------------------------------------------------------

if(BUILD_UNIT_TESTS)
    # gtest is a bit outdate and uses TR1
    add_definitions(-D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
endif()

if(MSVC)  
    # Force to always compile with W4 (Max Error.)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
      string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()

    # Force treatWarning as error
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")

    # add extra secure development lifecycle.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX /sdl")

    # enable M_PI and other math constants
    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(-DNOMINMAX)

    # multi processor build
    add_definitions("/MP")

    #--------------------------
    # debug flags
    #--------------------------
    # enable edit and continue
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI") 

endif(MSVC)

#------------------------------------------------------------------------------
# add subdirectories
#------------------------------------------------------------------------------
if(BUILD_REALISIM_AS_STATIC_LIBS)
    ADD_SUBDIRECTORY(Common/Core)
    ADD_SUBDIRECTORY(Common/Math)
endif()

if( BUILD_PROJECT_LIGHTBEAM )
    ADD_SUBDIRECTORY(Projects/LightBeam)
endif()

if( BUILD_PROJECT_REACTOR )
    ADD_SUBDIRECTORY(Projects/Reactor)
endif()

if( BUILD_PROTOTYPE )
    ADD_SUBDIRECTORY(Prototypes)
endif()

# unit tests if desired
if( BUILD_UNIT_TESTS )
    add_subdirectory(Common/Core/UnitTests)
    add_subdirectory(Common/Geometry/UnitTests)
    add_subdirectory(Common/Math/UnitTests)
endif()
