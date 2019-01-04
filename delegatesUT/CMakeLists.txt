cmake_minimum_required(VERSION 3.2)
project(delegatesUT C CXX)

if (WIN32)
    include(${CMAKE_CURRENT_SOURCE_DIR}/conan/conanbuildinfo_multi.cmake)
endif (WIN32)

if (UNIX)
    if(NOT APPLE)
        include(${CMAKE_CURRENT_SOURCE_DIR}/conan/conanbuildinfo.cmake)
    endif()
endif (UNIX)

conan_basic_setup()

SET (PROJECT_SOURCE_FILES
	MFunctionTUT.cpp
	../ffscript/function/DynamicFunction2.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${PROJECT_SOURCE_FILES})
conan_target_link_libraries(${PROJECT_NAME})