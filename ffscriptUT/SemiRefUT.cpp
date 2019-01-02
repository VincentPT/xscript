/******************************************************************
* File:        SemiRefUT.cpp
* Description: Test cases focus on checking usage of semi-reference
*              operator in C Lambda scripting language.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "fftest.hpp"

#include <CompilerSuite.h>
#include <ScriptTask.h>
#include <Utils.h>

using namespace std;
using namespace ffscript;


namespace ffscriptUT
{		
	namespace SemiRefUT
	{
		FF_TEST_FUNCTION(SemiRef, SemeRefTestCompile1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"void foo() {"
				L"	int n = 0;"
				L"	f = &n;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program should be success");
		}

		FF_TEST_FUNCTION(SemiRef, SemeRefTestCompile2)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	int n = 0;"
				L"	f = &n;"
				L"	return f;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program should be success");
		}

		FF_TEST_FUNCTION(SemiRef, SemeRefTestRun1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	int n = 2;"
				L"	f = &n;"
				L"	return f;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program should be failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 2, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(SemiRef, SemeRefTestRun2)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	int n = 2;"
				L"	f = &n;"
				L"	f = 3;"
				L"	return n;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program should be failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 3, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(SemiRef, SemeRefTestRun3)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"void test(int &n) {"
				L" n = 2;"
				L"}"
				L"int foo() {"
				L"	int n = 0;"
				L"	test(n);"
				L"	return n;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program should be success");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 2, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(SemiRef, SemeRefTestRun4)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int& test(int &n) {"
				L" return n;"
				L"}"
				L"int foo() {"
				L"	int n = 1;"
				L"	int&f = test(n);"
				L"	return f;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program should be success");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 1, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(SemiRef, SemeRefTestRun5)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int& test(ref int n) {"
				L" return *n;"
				L"}"
				L"int foo() {"
				L"	int n = 1;"
				L"	int&f = test(ref(n));"
				L"	return f;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program should be success");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 1, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(SemiRef, SemeRefTestRun6)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int& test(ref int n) {"
				L" return *n;"
				L"}"
				L"int& test2(ref int n) {"
				L" return *n;"
				L"}"
				L"int foo() {"
				L"	int n = 1;"
				L"	int m = 1;"
				L"	int&f = test(ref(n));"
				L"	test2(ref(m));"
				L"	return f;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program should be success");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 1, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(SemiRef, SemeRefTestRun7)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int& test(ref int n) {"
				L" return *n;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program should be success");

			int functionId = scriptCompiler->findFunction("test", "ref int");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			int n = 2;
			ScriptParamBuffer paramBuffer(&n);
			scriptTask.runFunction(functionId, &paramBuffer);
			int** funcRes = (int**)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == &n, L"program can run but return wrong value");
			FF_EXPECT_TRUE(**funcRes == n, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(SemiRef, SemeRefTestRun8)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int& test(ref int n) {"
				L" return *n;"
				L"}"
				L"int foo() {"
				L"	int n = 1;"
				L"	int&f = test(ref(n));"
				L"	return f;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program should be success");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 1, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(SemiRef, SemeRefTestRun9)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int& test(int ref n) {"
				L" return *n;"
				L"}"
				L"int foo() {"
				L"	int n = 1;"
				L"	int&f = test(n);"
				L"	return f;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_EQ(nullptr, program, L"Compile program should be failed");

			FF_EXPECT_NE("", scriptCompiler->getLastError().c_str(), L"Error text should contain a content");
		}

		FF_TEST_FUNCTION(SemiRef, SemeRefTestRun10)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int test(int ref n) {"
				L" return *n;"
				L"}"
				L"int foo() {"
				L"	int n = 1;"
				L"	int&f = & test(n);"
				L"	return f;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_EQ(nullptr, program, L"Compile program should be failed");

			FF_EXPECT_NE("", scriptCompiler->getLastError().c_str(), L"Error text should contain a content");
		}

		FF_TEST_FUNCTION(SemiRef, SemeRefTestRun11)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	int n = 1;"
				L"	int&f = &n;"
				L"	f = 2;"
				L"	return n;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 2, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(SemiRef, SemeRefTestRun12)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"float foo() {"
				L"	int n = 1;"
				L"	int&f = &n;"
				L"	float m = f;"
				L"	return m;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			float* funcRes = (float*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 1.0f, L"program can run but return wrong value");
		}
	};
}