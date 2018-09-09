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

#include "stdafx.h"
#include "CppUnitTest.h"
#include <CompilerSuite.h>
#include <ScriptTask.h>
#include <Utils.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace ffscript;


namespace ffscriptUT
{		
	TEST_CLASS(SemiRefUT)
	{
	public:
		TEST_METHOD(SemeRefTestCompile1)
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
			Assert::IsNotNull(program, L"Compile program should be success");
		}

		TEST_METHOD(SemeRefTestCompile2)
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
			Assert::IsNotNull(program, L"Compile program should be success");
		}

		TEST_METHOD(SemeRefTestRun1)
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
			Assert::IsNotNull(program, L"Compile program should be failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == 2, L"program can run but return wrong value");
		}

		TEST_METHOD(SemeRefTestRun2)
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
			Assert::IsNotNull(program, L"Compile program should be failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == 3, L"program can run but return wrong value");
		}

		TEST_METHOD(SemeRefTestRun3)
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
			Assert::IsNotNull(program, L"Compile program should be success");

			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == 2, L"program can run but return wrong value");
		}

		TEST_METHOD(SemeRefTestRun4)
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
			Assert::IsNotNull(program, L"Compile program should be success");

			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == 1, L"program can run but return wrong value");
		}

		TEST_METHOD(SemeRefTestRun5)
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
			Assert::IsNotNull(program, L"Compile program should be success");

			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == 1, L"program can run but return wrong value");
		}

		TEST_METHOD(SemeRefTestRun6)
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
			Assert::IsNotNull(program, L"Compile program should be success");

			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == 1, L"program can run but return wrong value");
		}

		TEST_METHOD(SemeRefTestRun7)
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
			Assert::IsNotNull(program, L"Compile program should be success");

			int functionId = scriptCompiler->findFunction("test", "ref int");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			int n = 2;
			ScriptParamBuffer paramBuffer(&n);
			scriptTask.runFunction(functionId, &paramBuffer);
			int** funcRes = (int**)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == &n, L"program can run but return wrong value");
			Assert::IsTrue(**funcRes == n, L"program can run but return wrong value");
		}

		TEST_METHOD(SemeRefTestRun8)
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
			Assert::IsNotNull(program, L"Compile program should be success");

			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == 1, L"program can run but return wrong value");
		}

		TEST_METHOD(SemeRefTestRun9)
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
			Assert::IsNull(program, L"Compile program should be failed");

			Assert::AreNotEqual("", scriptCompiler->getLastError().c_str(), L"Error text should contain a content");
		}

		TEST_METHOD(SemeRefTestRun10)
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
			Assert::IsNull(program, L"Compile program should be failed");

			Assert::AreNotEqual("", scriptCompiler->getLastError().c_str(), L"Error text should contain a content");
		}

		TEST_METHOD(SemeRefTestRun11)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == 2, L"program can run but return wrong value");
		}

		TEST_METHOD(SemeRefTestRun12)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			float* funcRes = (float*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == 1.0f, L"program can run but return wrong value");
		}
	};
}