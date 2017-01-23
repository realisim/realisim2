MACRO( ADD_COMPONENT iPath iDisplayedName)

# add current directory to includes paths
# add parent directory to includes paths for case like:
#   ADD_COMPONENT("../../Math" "Math")
#   ...
#   in code, we want to do: #include "Math/Matrix4.h"
#   
INCLUDE_DIRECTORIES( ${iPath} )
INCLUDE_DIRECTORIES( ${iPath}/.. )

#MESSAGE( "Globbing: " ${CMAKE_CURRENT_BINARY_DIR}/${iPath} )

FILE( GLOB iPATH_SOURCE_FILES
  ${iPath}/*.cpp )

FILE( GLOB iPATH_INCLUDE_FILES
  ${iPath}/*.h )

SOURCE_GROUP (${iDisplayedName} FILES ${iPATH_SOURCE_FILES} ${iPATH_INCLUDE_FILES})

# add sources and include to global variable
SET( SOURCE_FILES ${SOURCE_FILES} ${iPATH_SOURCE_FILES} )
SET( INCLUDE_FILES ${INCLUDE_FILES} ${iPATH_INCLUDE_FILES} )

ENDMACRO(ADD_COMPONENT)