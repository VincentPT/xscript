cmake_minimum_required (VERSION 3.2)
project(ffscriptLibrary C CXX)

SET (PROJECT_SOURCE_FILES 
	./Geometry.h
	./GeometryLib.h
	./MathLib.h
	./RawStringLib.h
	./stdafx.h
	./targetver.h
	./GeometryLib.cpp
	./MathLib.cpp
	./RawStringLib.cpp
	./stdafx.cpp
)

# define project's build target with project's source files
add_library(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ffscript)