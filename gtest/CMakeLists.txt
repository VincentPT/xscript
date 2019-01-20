cmake_minimum_required(VERSION 3.0)
get_filename_component(GTEST_INCLUDE_DIR_ABSOLUTE ${GTEST_INCLUDE_DIR} ABSOLUTE)
get_filename_component(GTEST_LIB_DIR_ABSOLUTE ${GTEST_LIB_DIR} ABSOLUTE)

configure_file(gtest.cmake.in gtest.cmake)