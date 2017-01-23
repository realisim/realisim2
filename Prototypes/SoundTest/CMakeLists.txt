
PROJECT( SoundTest )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

SET( SOURCES ${SOURCES} "./CMakeLists.txt" )

ADD_COMPONENT( "." "Application" )
ADD_COMPONENT("../../Common/Math" "Math")
ADD_COMPONENT("../../Common/Interface/Audio" "Audio")
ADD_COMPONENT("../../Common/Utilities" "Utilities")

QT_AUTOMOC()

# create executable
ADD_EXECUTABLE( ${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} Qt5::Widgets OpenAL32)

# install directive
QT5_INSTALL( ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} )
OPENAL_INSTALL( ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} )

# #copy assets to bin/
INSTALL(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/assets/ DESTINATION ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}/assets )
