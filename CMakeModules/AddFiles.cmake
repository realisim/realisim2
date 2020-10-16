#------------------------------------------------------------------------------
# this maro will add files to SOURCE_FILES and INCLUDE_FILES which are used
# to create an executable.
# Furthermore it will regroup the files under a source_group for visual studio
# The source_group will appeal as iSourceGroup.
#------------------------------------------------------------------------------
macro( add_files iFileList iSourceGroup)

# example:
#   set (WIDGET_FILES 
#       "../../Common/Widgets/StatisticsGraph.h"
#       "../../Common/Widgets/StatisticsGraph.cpp"
#       "../../Common/Widgets/StatisticsAndEventsGraph.h"
#       "../../Common/Widgets/StatisticsAndEventsGraph.cpp" )
#   add_files( "${WIDGET_FILES}" "Widgets" )
#   

#------------------------------------------------------------------------------
# Glob all .h and .cpp in two separate list, these will be added to groups
# in the solution.
#------------------------------------------------------------------------------
set (NEW_SOURCE_FILES)
set (NEW_INCLUDE_FILES)
set (NEW_UI_FILES)
set (NEW_QT_QRC_FILES)
set (NEW_VERTEX_SHADER_FILES)
set (NEW_FRAGMENT_SHADER_FILES)
set (NEW_GEOMETRY_SHADER_FILES)
set (NEW_TESS_CONTROL_SHADER_FILES)
set (NEW_TESS_EVALUATION_SHADER_FILES)
set (NEW_COMPUTE_SHADER_FILES)

#message( "ADD_FILES  ${iFileList} in group "  ${iSourceGroup})

SET(_FILE_LIST ${iFileList})

foreach(F IN LISTS _FILE_LIST)
    #message("in list ${F}" )

    string(FIND ${F} ".cpp" pos)
    if(NOT ${pos} EQUAL "-1")
        set (NEW_SOURCE_FILES ${NEW_SOURCE_FILES} ${F})
    endif()

    string(FIND ${F} ".h" pos)
    if(NOT ${pos} EQUAL "-1")
        set (NEW_INCLUDE_FILES ${NEW_INCLUDE_FILES} ${F})
    endif()

    string(FIND ${F} ".hxx" pos)
    if(NOT ${pos} EQUAL "-1")
        set (NEW_INCLUDE_FILES ${NEW_INCLUDE_FILES} ${F})
    endif()

    string(FIND ${F} ".ui" pos)
    if(NOT ${pos} EQUAL "-1")
        set (NEW_UI_FILES ${NEW_UI_FILES} ${F})
    endif()

    string(FIND ${F} ".qrc" pos)
    if(NOT ${pos} EQUAL "-1")
        set (NEW_QT_QRC_FILES ${NEW_QT_QRC_FILES} ${F})
    endif()

    string(FIND ${F} ".vert" pos)
    if(NOT ${pos} EQUAL "-1")
        set (NEW_VERTEX_SHADER_FILES ${NEW_VERTEX_SHADER_FILES} ${F})
    endif()

    string(FIND ${F} ".frag" pos)
    if(NOT ${pos} EQUAL "-1")
        set (NEW_FRAGMENT_SHADER_FILES ${NEW_FRAGMENT_SHADER_FILES} ${F})
    endif()

    string(FIND ${F} ".geo" pos)
    if(NOT ${pos} EQUAL "-1")
        set (NEW_GEOMETRY_SHADER_FILES ${NEW_GEOMETRY_SHADER_FILES} ${F})
    endif()

    string(FIND ${F} ".tcs" pos)
    if(NOT ${pos} EQUAL "-1")
        set (NEW_TESS_CONTROL_SHADER_FILES ${NEW_TESS_CONTROL_SHADER_FILES} ${F})
    endif()

    string(FIND ${F} ".tes" pos)
    if(NOT ${pos} EQUAL "-1")
        set (NEW_TESS_EVALUATION_SHADER_FILES ${NEW_TESS_EVALUATION_SHADER_FILES} ${F})
    endif()

    string(FIND ${F} ".comp" pos)
    if(NOT ${pos} EQUAL "-1")
        set (NEW_COMPUTE_SHADER_FILES ${NEW_COMPUTE_SHADER_FILES} ${F})
    endif()
    
endforeach()


# Replace / by \\ present in iDasplayedName so we can create a folder hierachy in the solution.
# For example: 
#   ${iSourceGroup} = 'Simulator/Palettes'
#
string(REPLACE "/" "\\" source_group_msvc ${iSourceGroup})
source_group ("${source_group_msvc}" FILES ${NEW_SOURCE_FILES} ${NEW_INCLUDE_FILES})

# Qt ui place under /ui
source_group ("${source_group_msvc}\\ui" FILES ${NEW_UI_FILES} )

# Qt ui place under /qrc
source_group ("${source_group_msvc}\\qrc" FILES ${NEW_QT_QRC_FILES} )

# Create source groups for shaders
#
source_group (shaders\\vertex FILES ${NEW_VERTEX_SHADER_FILES})
source_group (shaders\\fragment FILES ${NEW_FRAGMENT_SHADER_FILES})
source_group (shaders\\geometry FILES ${NEW_GEOMETRY_SHADER_FILES})
source_group (shaders\\tesselation_control FILES ${NEW_TESS_CONTROL_SHADER_FILES})
source_group (shaders\\tesselation_evaluation FILES ${NEW_TESS_EVALUATION_SHADER_FILES})
source_group (shaders\\compute FILES ${NEW_COMPUTE_SHADER_FILES})

# add sources and include to global variable
set( SOURCE_FILES ${SOURCE_FILES} ${NEW_SOURCE_FILES} )
set( SOURCE_FILES ${SOURCE_FILES} ${NEW_UI_FILES} )
set( SOURCE_FILES ${SOURCE_FILES} ${NEW_QT_QRC_FILES} )
set( INCLUDE_FILES ${INCLUDE_FILES} ${NEW_INCLUDE_FILES} )

set( SOURCE_FILES ${SOURCE_FILES} ${NEW_VERTEX_SHADER_FILES} )
set( SOURCE_FILES ${SOURCE_FILES} ${NEW_FRAGMENT_SHADER_FILES} )
set( SOURCE_FILES ${SOURCE_FILES} ${NEW_GEOMETRY_SHADER_FILES} )
set( SOURCE_FILES ${SOURCE_FILES} ${NEW_TESS_CONTROL_SHADER_FILES} )
set( SOURCE_FILES ${SOURCE_FILES} ${NEW_TESS_EVALUATION_SHADER_FILES} )
set( SOURCE_FILES ${SOURCE_FILES} ${NEW_COMPUTE_SHADER_FILES}  )

endmacro(add_files)