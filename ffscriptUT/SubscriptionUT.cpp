/******************************************************************
* File:        SubscriptionUT.cpp
* Description: Test cases focus on checking usage of operator[]
*              in C Lambda scripting language.
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
#include <RawStringLib.h>
#include <GeometryLib.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace ffscript;

namespace ffscriptUT
{	
	TEST_CLASS(SubscriptionUT)
	{
	public:
		TEST_METHOD(ElementAccess1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = compiler.getCompiler();

			const wchar_t scriptCode[] =
				L"int foo(ref int a) {"
				L"	int ret = a[1];"
				L"	return ret;"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, convertToWstring(scriptCompiler->getLastError()).c_str());

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref int");
			Assert::IsTrue(functionId >= 0, L"can not find function 'foo'");

			int a[] = { 1,2,3 };			

			ScriptParamBuffer paramBuffer(&a[0]);
			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			int* iRes = (int*)task.getTaskResult();

			Assert::AreEqual(a[1], *iRes, L"function 'foo' return wrong");
		}

		TEST_METHOD(ElementAccess2_1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"int foo(ref int a) {"
				L"	return a[1];"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref int");
			Assert::IsTrue(functionId >= 0, L"can not find function 'foo'");

			int a[] = { 1,2,3 };
			ScriptParamBuffer paramBuffer(&a[0]);

			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			int* iRes = (int*)task.getTaskResult();

			Assert::AreEqual(a[1], *iRes, L"function 'foo' return wrong");
		}

		TEST_METHOD(ElementAccess2_2)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"long foo(ref long a) {"
				L"	return a[1];"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref long");
			Assert::IsTrue(functionId >= 0, L"can not find function 'foo'");

			__int64 a[] = { 1,2,3 };
			ScriptParamBuffer paramBuffer(&a[0]);

			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			__int64* iRes = (__int64*)task.getTaskResult();

			Assert::IsTrue(a[1] == *iRes, L"function 'foo' return wrong");
		}

		TEST_METHOD(ElementAccess2_3)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"float foo(ref float a) {"
				L"	return a[1];"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref float");
			Assert::IsTrue(functionId >= 0, L"can not find function 'foo'");

			float a[] = { 1,2,3 };
			ScriptParamBuffer paramBuffer(&a[0]);

			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			float* iRes = (float*)task.getTaskResult();

			Assert::AreEqual(a[1], *iRes, L"function 'foo' return wrong");
		}

		TEST_METHOD(ElementAccess2_4)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"double foo(ref double a) {"
				L"	return a[1];"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref double");
			Assert::IsTrue(functionId >= 0, L"can not find function 'foo'");

			double a[] = { 1,2,3 };
			double* pa = a;
			ScriptParamBuffer paramBuffer(pa);

			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			double* iRes = (double*)task.getTaskResult();

			Assert::AreEqual(a[1], *iRes, L"function 'foo' return wrong");
		}

		TEST_METHOD(ElementAccess3)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"ref int foo(ref int a) {"
				L"	return ref(a[1]);"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref int");
			Assert::IsTrue(functionId >= 0, L"can not find function 'foo'");

			int a[] = { 1,2,3 };
			ScriptParamBuffer paramBuffer(&a[0]);

			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			size_t* pRes = (size_t*)task.getTaskResult();

			Assert::AreEqual(a[1], *((int*)(*pRes)), L"function 'foo' return wrong");
		}

		TEST_METHOD(ElementAccess4)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"ref int foo(ref int a) {"
				L"	ref int ra1 = ref(a[1]);"
				L"	return ra1;"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref int");
			Assert::IsTrue(functionId >= 0, L"can not find function 'foo'");

			int a[] = { 1,2,3 };
			ScriptParamBuffer paramBuffer(&a[0]);

			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			size_t* pRes = (size_t*)task.getTaskResult();

			Assert::AreEqual((size_t)&a[1], *pRes, L"function 'foo' return wrong");
		}

		TEST_METHOD(ElementAccess5)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"ref int foo(ref int a) {"
				L"	return a[1];"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref int");
			Assert::IsTrue(functionId >= 0, L"can not find function 'foo'");

			int a[] = { 1,2,3 };
			ScriptParamBuffer paramBuffer(&a[0]);

			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			size_t* pRes = (size_t*)task.getTaskResult();

			Assert::AreEqual((size_t)&a[1], *pRes, L"function 'foo' return wrong");
		}

		TEST_METHOD(ElementAccess6)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"void foo(ref int a) {"
				L"	a[1] = 1;"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref int");
			Assert::IsTrue(functionId >= 0, L"can not find function 'foo'");

			int a[] = { 1,2,3 };
			ScriptParamBuffer paramBuffer(&a[0]);

			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			Assert::AreEqual(1, a[1], L"function 'foo' return wrong");
		}

		TEST_METHOD(ElementAccess7)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"void foo(ref int a) {"
				L"	int i = 1;"
				L"	a[i++] = 5;"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref int");
			Assert::IsTrue(functionId >= 0, L"can not find function 'foo'");

			int a[] = { 1,2,3 };
			ScriptParamBuffer paramBuffer(&a[0]);

			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			Assert::AreEqual(5, a[1], L"function 'foo' return wrong");
		}

		TEST_METHOD(ElementAccess8)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"void foo(ref int a) {"
				L"	int i = 1;"
				L"	a[++i] = 5;"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref int");
			Assert::IsTrue(functionId >= 0, L"can not find function 'foo'");

			int a[] = { 1,2,3 };
			ScriptParamBuffer paramBuffer(&a[0]);

			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			Assert::AreEqual(5, a[2], L"function 'foo' return wrong");
		}

		TEST_METHOD(ElementMemberAccess1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = compiler.getCompiler();
			includeRawStringToCompiler(scriptCompiler.get());
			includeGeoLibToCompiler(scriptCompiler.get());

			const wchar_t scriptCode[] =
				L"float foo() {"
				L"	Point p = {1.1f , 2.2f};"
				L"	a = ref(p);"
				L"	x = a[0].x;"
				L"	return x;"
				L"}"
				;

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, (L"Compile program failed:" + convertToWstring(scriptCompiler->getLastError())).c_str());

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"can not find function 'foo'");

			ScriptTask task(program);
			task.runFunction(functionId, nullptr);
			auto res = *(float*)task.getTaskResult();

			Assert::AreEqual(1.1f, res, L"function 'foo' return wrong");
		}

		TEST_METHOD(TestSorting)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"void foo(ref int a, int n) {"
				L"	int i = 0;"
				L"	int temp;"
				L"	while(i < n) {"
				L"		int j = i + 1;"
				L"		while( j < n ) {"
				L"			if(a[j] < a[i]) {"
				L"				temp = a[j];"
				L"				a[j] = a[i];"
				L"				a[i] = temp;"
				L"			}"
				L"			j++;"
				L"		}"
				L"		i++;"
				L"	}"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref int, int");
			Assert::IsTrue(functionId >= 0, L"can not find function 'foo'");

			int a[] = { 3,2,5,6,7,1,4 };
			const int n = sizeof(a) / sizeof(a[0]);

			ScriptParamBuffer paramBuffer(&a[0]);
			paramBuffer.addParam(n);

			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			int i;
			for (i = 1; i <= n; i++) {
				if (a[i - 1] != i) {
					break;
				}
			}

			Assert::AreEqual(n + 1, i, L"function 'foo' return wrong");
		}
	};
}