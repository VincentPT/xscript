cmake_minimum_required(VERSION 3.27)
project(MultiThreadCpp C CXX)

SET (PROJECT_SOURCE_FILES
	MultiThreadCpp.cpp
	../TutorialCommon.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ffscriptLibrary)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/MultiThreadCpp.c955" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")