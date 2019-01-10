cmake_minimum_required(VERSION 3.2)
project(delegatesUT C CXX)

if (WIN32)
    include(${CMAKE_CURRENT_SOURCE_DIR}/../gtest/conan/conanbuildinfo_multi.cmake)
endif (WIN32)

if (UNIX)
    if(NOT APPLE)
        include(${CMAKE_CURRENT_SOURCE_DIR}/../gtest/conan/conanbuildinfo.cmake)
    endif()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif (UNIX)

conan_basic_setup()

SET (PROJECT_SOURCE_FILES
	MFunctionTUT.cpp
	CdeclFunctionUT.cpp
	MemberFunctionUT.cpp
	FunctionsForTest.h
	MethodsForTest.h
	MethodsForTest.cpp
	FunctionsForTest.cpp
	FunctionDelegate3UT.cpp
	../ffscript/function/DynamicFunction.cpp
	../ffscript/function/DynamicFunction2.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${PROJECT_SOURCE_FILES})
conan_target_link_libraries(${PROJECT_NAME})