cmake_minimum_required(VERSION 3.2)
project(ffscriptUT C CXX)

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
	./fftest.hpp
	./CompileSuiteUT.cpp
	./AssigmentCompoundUT.cpp
	./Utility.cpp
	./BitwiseOperatorsUT.cpp
	./CdeclFunctionUT.cpp
	./CompileDynamicFunctionUT.cpp
	./CompileProgramInFile.cpp
	./CompileProgramUT.cpp
	./CompileProgramUT2.cpp
	./CompletedConstructorDestructorsUT.cpp
	./CompositeConstructorUT.cpp
	ConditionalOperatorUT.cpp
	ConstructorDestructorForCodeUT.cpp
	ConstructorDestructorUT.cpp
	DefaultOperatorsUT.cpp
	Expression2PlainCodeUT.cpp
	ExpressionLinkUT.cpp
	ExpressionUT.cpp
	FFScriptArrayUT.cpp
	FunctionHelperUT.cpp
	FuntionPointerUT.cpp
	InternalComplierSuiteUT.cpp
	LambdaExpressionUT.cpp
	MakePathsUT.cpp
	MemberFunctionUT.cpp
	MemoryblockUT.cpp
	MultiCompilerUT.cpp
	MultiProgramUT.cpp
	MultithreadUT.cpp
	OperatorBracketsUT.cpp
	PostFixPrefixOperatorsUT.cpp
	ReusingCompilerUT.cpp
	RunDynamicFunctionUT.cpp
	ScriptCompilerUT.cpp
	ScriptTypeUT.cpp
	SemiRefUT.cpp
	ShowErrorLineUT.cpp
	StaticArrayUT.cpp
	StructUT.cpp
	SubscriptionUT.cpp
	UserLibraryUT.cpp
	VectorCompatibleUT.cpp
	ffscriptUT.cpp
	MethodUT.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${PROJECT_SOURCE_FILES})
conan_target_link_libraries(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ffscriptLibrary)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/scripts/CreateSimpleString1.c955" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/scripts/ErrorInFunctionScope1.c955" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/scripts/ErrorInFunctionScope2.c955" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/scripts/ErrorInFunctionScope3.c955" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/scripts/ErrorInFunctionScope4.c955" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/scripts/ErrorInFunctionScope5.c955" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/scripts/ErrorInFunctionScope6.c955" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/scripts/ErrorInFunctionScope7.c955" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/scripts/ErrorInGlobalScope1.c955" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/scripts/ErrorInGlobalScope2.c955" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/scripts/ErrorInStruct1.c955" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/scripts/ErrorInStruct2.c955" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/scripts/ErrorInStruct3.c955" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/scripts/StringFunctions.c955" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/testfunctions.hpp" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")