
PROJECT( SoundTest )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

INCLUDE( "../../CMakeModules/Qt5.cmake" )
INCLUDE( "../../CMakeModules/OpenAL.cmake" )

#------------------------
# Add sources
#------------------------
set( SOURCE_FILES ${SOURCE_FILES} "./CMakeLists.txt" )
add_component( "." "Application" )
add_component("../../Common/Math" "Math")
add_component("../../Common/Interface/Audio" "Audio")
add_component("../../Common/Utilities" "Utilities")

qt_automoc()

#------------------------
# create executable
#------------------------
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} )

#------------------------
# libraries to link to
#------------------------
target_link_libraries( ${PROJECT_NAME} Qt5::Widgets ${OPENAL_LIBRARY} )

#------------------------
# install directive
#------------------------
qt5_install( ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} )
openal_install( ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} )

# #copy assets to bin/
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/assets/ DESTINATION ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}/assets )
