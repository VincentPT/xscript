cmake_minimum_required(VERSION 3.2)
project(SingleExpression C CXX)

SET (PROJECT_SOURCE_FILES
	SingleExpression.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ffscriptLibrary)