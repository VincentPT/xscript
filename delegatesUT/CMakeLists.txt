cmake_minimum_required(VERSION 3.2)
project(delegatesUT C CXX)

if (WIN32)
    include(${CMAKE_CURRENT_SOURCE_DIR}/../gtest/conan/conanbuildinfo_multi.cmake)
	conan_basic_setup()
endif (WIN32)

if (GTEST_CMAKE)
    include(${CMAKE_CURRENT_SOURCE_DIR}/../gtest/gtest.cmake)
endif ()


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
if (WIN32)
    conan_target_link_libraries(${PROJECT_NAME})
else (WIN32)
	target_link_libraries(${PROJECT_NAME} gtest gtest_main)
endif (WIN32)
if (UNIX)
    if(NOT APPLE)
        target_link_libraries(${PROJECT_NAME} pthread)
    endif()
endif (UNIX)