cmake_minimum_required(VERSION 3.2)
project(delegatesUT C CXX)

if (WIN32)
    include(${CMAKE_CURRENT_SOURCE_DIR}/../gtest/conan/conanbuildinfo_multi.cmake)
else (WIN32)
	include(${CMAKE_CURRENT_SOURCE_DIR}/../gtest/conan/conanbuildinfo.cmake)
endif (WIN32)
conan_basic_setup()

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
conan_target_link_libraries(${PROJECT_NAME})

if (UNIX)
    if(NOT APPLE)
        target_link_libraries(${PROJECT_NAME} pthread)
    endif()
endif (UNIX)