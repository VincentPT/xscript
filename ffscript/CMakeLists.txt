cmake_minimum_required (VERSION 3.2)
project(ffscript C CXX)

SET (DELEGATES
	./function/CachedFunction.hpp
	./function/CachedDelegate.h
	./function/CachedDelegate.cpp
	./function/CachedMethod.hpp
	./function/FunctionDelegate.hpp
	./function/FunctionDelegate3.hpp
	./function/DynamicFunction.h
	./function/DynamicFunction2.h
	./function/FunctionHelper.hpp
	./function/MemberFunction.hpp
	./function/MemberFunction3.hpp
	./function/MemberTypeInfo.hpp
	./function/functioninternal.hpp
	./function/functioninternal2.hpp
	./function/MemberFunction4.hpp
)

SET (HEADERS
	./BasicFunction.h
	./BasicFunctionFactory.hpp
	./BasicOperators.hpp
	./BasicType.h
	./CLamdaProg.h
	./CodeUpdater.h
	./CommandTree.h
	./CommandUnitBuilder.h
	./CompilerSuite.h
	./CompositeConstrutorUnit.h
	./ConditionalOperator.h
	./Context.h
	./ContextScope.h
	./ControllerExecutor.h
	./DefaultCommands.h
	./DefaultPreprocessor.h
	./DestructorContextScope.h
	./DynamicFunctionFactory.h
	./Executor.h
	./ExpUnitExecutor.h
	./ExpresionParser.h
	./Expression.h
	./FFScriptArray.hpp
	./FFStack.h
	./FactoryTree.h
	./FuncLibrary.h
	./Function.h
	./FunctionFactory.h
	./FunctionRegisterHelper.h
	./FunctionScope.h
	./FwdCompositeConstrutorUnit.h
	./GlobalScope.h
	./InstructionCommand.h
	./Internal.h
	./InternalCompilerSuite.h
	./LogicCommands.hpp
	./LoopScope.h
	./MemberVariableAccessors.h
	./MemoryBlock.h
	./ObjectBlock.hpp
	./Preprocessor.h
	./Program.h
	./RefFunction.h
	./ScopeRuntimeData.h
	./ScopedCompilingScope.h
	./ScopedContext.h
	./ScriptCompiler.h
	./ScriptFunction.h
	./ScriptParamBuffer.hpp
	./ScriptRunner.h
	./ScriptScope.h
	./ScriptTask.h
	./ScriptType.h
	./SingleList.h
	./StaticContext.h
	./StructClass.h
	./Supportfunctions.h
	./Template.h
	./TypeManager.h
	./Utility.hpp
	./Utils.h
	./Variable.h
	./expresion_defs.h
	./expressionunit.h
	./ffscript.h
	./unitType.h
	./template/TemplateBlock.hpp
	./template/TemplateTypeManager.h
)

SET (SOURCES
	./BasicFunction.cpp
	./BasicType.cpp
	./CLamdaProg.cpp
	./CodeUpdater.cpp
	./CommandTree.cpp
	./CommandUnitBuilder.cpp
	./CompilerSuite.cpp
	./CompositeConstrutorUnit.cpp
	./ConditionalOperator.cpp
	./Context.cpp
	./ContextScope.cpp
	./ControllerExecutor.cpp
	./DefaultCommands.cpp
	./DefaultPreprocessor.cpp
	./DestructorContextScope.cpp
	./DynamicFunctionFactory.cpp
	./Executor.cpp
	./ExpUnitExecutor.cpp
	./ExpUnitExecutor2.cpp
	./ExpresionParser.cpp
	./Expression.cpp
	./FactoryTree.cpp
	./FuncLibrary.cpp
	./FunctionFactory.cpp
	./FunctionRegisterHelper.cpp
	./FunctionScope.cpp
	./FwdCompositeConstrutorUnit.cpp
	./GlobalScope.cpp
	./GlobalScopeParser.cpp
	./InstructionCommand.cpp
	./Internal.cpp
	./InternalCompilerSuite.cpp
	./LoopScope.cpp
	./MemberVariableAccessors.cpp
	./MemoryBlock.cpp
	./Preprocessor.cpp
	./Program.cpp
	./RefFunction.cpp
	./ScopeRuntimeData.cpp
	./ScopedCompilingScope.cpp
	./ScopedContext.cpp
	./ScriptCompiler.cpp
	./ScriptFunction.cpp
	./ScriptRunner.cpp
	./ScriptScope.cpp
	./ScriptScopeParser.cpp
	./ScriptTask.cpp
	./ScriptType.cpp
	./StaticContext.cpp
	./StructClass.cpp
	./Supportfunctions.cpp
	./Template.cpp
	./TypeManager.cpp
	./Utils.cpp
	./Variable.cpp
	./expressionunit.cpp
	./function/DynamicFunction.cpp
	./function/DynamicFunction2.cpp
	./template/TemplateTypeManager.cpp
)

source_group(delegates FILES ${DELEGATES})

set (PROJECT_SOURCE_FILES ${HEADERS} ${SOURCES} ${DELEGATES})

# define project's build target with project's source files
add_library(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if (UNIX)
    if(NOT APPLE)
        target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
    endif()
endif (UNIX)