cmake_minimum_required(VERSION 3.27)
project(delegatesUT C CXX)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
find_package(GTest)

SET (PROJECT_SOURCE_FILES
	MFunctionTUT.cpp
	FunctionDelegatesUT.cpp
	MemberFunctionUT.cpp
	FunctionsForTest.h
	MethodsForTest.h
	MethodsForTest.cpp
	FunctionsForTest.cpp
	FunctionDelegate3UT.cpp
	CachedMethodUT.cpp
	CachedFunctionUT.cpp
	../ffscript/function/DynamicFunction.cpp
	../ffscript/function/DynamicFunction2.cpp
	../ffscript/function/CachedDelegate.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${PROJECT_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} gtest::gtest)

if (UNIX)
    if(NOT APPLE)
        target_link_libraries(${PROJECT_NAME} pthread)
    endif()
endif (UNIX)