/******************************************************************
* File:        AsyncUT.cpp
* Description: Test cases for run program with async code
* Author:      Vincent Pham
*
* Copyright (c) 2023 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "fftest.hpp"

#include <CompilerSuite.h>
#include <AsyncEnabledRunner.h>
#include <Utils.h>
#include <DefaultPreprocessor.h>
#include <RawStringLib.h>
#include <thread>
#include <chrono>

#include "Utils.h"

using namespace std;
using namespace ffscript;

void expensiveCostFunction(int& n) {
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(300ms);

	n = n * (n + 1);
}

void expensiveCostFunctionRunner(int* n) {
	auto context = Context::getCurrent();
	auto task = new ResumeEvent(context);
	auto asyncRunner = AsyncEnabledRunner::current();

	std::thread* pThread = new std::thread([n, task, asyncRunner]() {
		expensiveCostFunction(*n);
		task->setStatus(ResumeEvent::Done);
		asyncRunner->wakeMeUp();
	});	
	
	task->setPostProcessing([pThread]() {
		pThread->join();
		delete pThread;
	});
	asyncRunner->pushEvent(task);

	// tell the runner that the current task associate with this context should
	// be suppend for await operation
	context->awaitInterupt();
}

//CompileSuiteUT
TEST(Async, PesudoSingleAwait)
{
	CompilerSuite compiler;

	//the code does not contain any global scope'code and only a variable
	//so does not need global memory
	compiler.initialize(8);
	GlobalScopeRef rootScope = compiler.getGlobalScope();
	auto scriptCompiler = rootScope->getCompiler();
	includeRawStringToCompiler(scriptCompiler);

	const BasicTypes& basicType = scriptCompiler->getTypeManager()->getBasicTypes();

	DefaultUserFunctionFactory pesudoAwait(createFunctionDelegateRef<void, int*>(expensiveCostFunctionRunner), scriptCompiler,"void", 1);
	scriptCompiler->registFunction("pesudoAwait", "ref int", &pesudoAwait);

	// if not call bellow code, the RawString library will be remove when compile the program
	scriptCompiler->beginUserLib();

	const wchar_t* scriptCode =
		L"int main(int n) {"
		L"	pesudoAwait(&n);"
		L"	return n;"
		L"}"
		;

	Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
	ASSERT_TRUE(program != nullptr);
	int functionId = scriptCompiler->findFunction("main", "int");
	ASSERT_TRUE(functionId >= 0);

	int n = 1;
	ScriptParamBuffer paramBuffer;
	paramBuffer.addParam(n);

	rootScope->runGlobalCode();

	AsyncEnabledRunner runner(program);
	runner.run(functionId, paramBuffer);

	int* res = (int*)runner.getTaskResult();
	expensiveCostFunction(n);

	ASSERT_EQ(n, *res);
}

TEST(Async, PesudoDoubleAwait)
{
	CompilerSuite compiler;

	//the code does not contain any global scope'code and only a variable
	//so does not need global memory
	compiler.initialize(8);
	GlobalScopeRef rootScope = compiler.getGlobalScope();
	auto scriptCompiler = rootScope->getCompiler();
	includeRawStringToCompiler(scriptCompiler);

	const BasicTypes& basicType = scriptCompiler->getTypeManager()->getBasicTypes();

	DefaultUserFunctionFactory pesudoAwait(createFunctionDelegateRef<void, int*>(expensiveCostFunctionRunner), scriptCompiler, "void", 1);
	scriptCompiler->registFunction("pesudoAwait", "ref int", &pesudoAwait);

	// if not call bellow code, the RawString library will be remove when compile the program
	scriptCompiler->beginUserLib();

	const wchar_t* scriptCode =
		L"int main(int n) {"
		L"	pesudoAwait(&n);"
		L"	pesudoAwait(&n);"
		L"	return n;"
		L"}"
		;

	Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
	ASSERT_TRUE(program != nullptr);
	int functionId = scriptCompiler->findFunction("main", "int");
	ASSERT_TRUE(functionId >= 0);

	int n = 1;
	ScriptParamBuffer paramBuffer;
	paramBuffer.addParam(n);

	rootScope->runGlobalCode();

	AsyncEnabledRunner runner(program);
	runner.run(functionId, paramBuffer);

	int* res = (int*)runner.getTaskResult();
	expensiveCostFunction(n);
	expensiveCostFunction(n);

	ASSERT_EQ(n, *res);
}