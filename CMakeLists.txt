cmake_minimum_required (VERSION 3.2)
project(ffscript)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ffscript core project
add_subdirectory(ffscript)
# ffscript library project
add_subdirectory(ffscriptLibrary)
# ffscript unit test project
add_subdirectory(ffscriptUT)
#tutorial projects
add_subdirectory(tutorials)