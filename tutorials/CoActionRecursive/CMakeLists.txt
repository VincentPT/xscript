cmake_minimum_required(VERSION 3.27)
project(CoActionRecursive C CXX)

SET (PROJECT_SOURCE_FILES
	CoActionRecursive.cpp
	../TutorialCommon.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ffscriptLibrary)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/CoActionRecursive.c955" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")