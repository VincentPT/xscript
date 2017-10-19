cmake_minimum_required (VERSION 3.2)
project(ffdebugger)

# add cinder dependency interface
add_subdirectory(cinder)

# add ffscript dependency interface
add_subdirectory(ffscript)

# spylib
add_subdirectory(spylib)

# buzz spy application
add_subdirectory(ffdebugger)