cmake_minimum_required(VERSION 3.2.0)

SET (TARGET_INTERFACE ffscript)

# apply the target to cmake
add_library(${TARGET_INTERFACE} INTERFACE)

# set include directories
SET (I_OPENCV_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../ffscript)
SET (I_OPENCV_LIB_DIR ${CMAKE_SOURCE_DIR}/..)

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET (I_OPENCV_LIB_DIR ${I_OPENCV_LIB_DIR}/x64)
    set (PLATFORM x64)
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set (PLATFORM x86)
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )


target_include_directories(${TARGET_INTERFACE} INTERFACE ${I_OPENCV_INCLUDE_DIR})
target_link_libraries(${TARGET_INTERFACE} INTERFACE debug "${I_OPENCV_LIB_DIR}/Debug/ffscript.lib" INTERFACE optimized "${I_OPENCV_LIB_DIR}/Release/ffscript")