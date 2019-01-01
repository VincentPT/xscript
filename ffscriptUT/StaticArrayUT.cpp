/******************************************************************
* File:        StaticArrayUT.cpp
* Description: Test cases focus on checking usage of static array
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
#include "ExpresionParser.h"
#include <functional>
#include "TemplateForTest.hpp"
#include "Variable.h"
#include "FunctionRegisterHelper.h"
#include "BasicFunction.h"
#include "BasicType.h"
#include "FunctionFactory.h"
#include "function\MemberFunction.hpp"
#include "Context.h"
#include <thread>
#include <future>
#include <Program.h>
#include <ScriptTask.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace ffscript;

#include "ExpresionParser.h"
#include "ScriptCompiler.h"
#include "expressionunit.h"
#include "Expression.h"
#include "GlobalScope.h"
#include "Executor.h"
#include "Utils.h"
#include "BasicFunctionFactory.hpp"
#include "function\MemberFunction2.hpp"
#include "expressionunit.h"
#include "DynamicFunctionFactory.h"
#include "MemoryBlock.h"
#include "CompilerSuite.h"

namespace ffscriptUT
{
	TEST_CLASS(StaticArrayUT)
	{
		CompilerSuite compiler;
		const BasicTypes* basicType;
		ScriptCompiler* scriptCompiler;

		ScriptType typePoint;
		ScriptType typeRectangle;	

	public:
		StaticArrayUT()
		{
			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(1024);
			scriptCompiler = compiler.getCompiler().get();
			basicType = &compiler.getTypeManager()->getBasicTypes();

			ScriptType typeInt(basicType->TYPE_INT, "int");

			StructClass* structPoint = new StructClass(scriptCompiler, "Point");
			structPoint->addMember(typeInt, "x");
			structPoint->addMember(typeInt, "y");
			int iPointType = scriptCompiler->registStruct(structPoint);
			typePoint.setType(iPointType);
			typePoint.setTypeStr(structPoint->getName());

			StructClass* structRectangle = new StructClass(scriptCompiler, "Rectangle");
			structRectangle->addMember(typePoint, "location");
			structRectangle->addMember(typeInt, "width");
			structRectangle->addMember(typeInt, "height");
			int iRectangleType = scriptCompiler->registStruct(structRectangle);
			typeRectangle.setType(iRectangleType);
			typeRectangle.setTypeStr(structRectangle->getName());
		}		

		TEST_METHOD(StaticArrayUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	array<int,10> ret;"
				L"	ret[0] = 1;"
				L"	return ret[0];"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(1, *iRes);
		}

		TEST_METHOD(StaticArrayUT2)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			const wchar_t scriptCode[] =
				L"int test(ref int a) {"
				L"	return a[0];"
				L"}"
				L"int foo() {"
				L"	array <int,10> ret;"
				L"	ret[0] = 1;"
				L"	return test(ret);"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(1, *iRes);
		}

		TEST_METHOD(StaticArrayUT3)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			const wchar_t scriptCode[] =
				L"int test(array<int,10> a) {"
				L"	return a[0];"
				L"}"
				L"int foo() {"
				L"	array <int,10> ret;"
				L"	ret[0] = 1;"
				L"	return test(ret);"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(1, *iRes);
		}

		TEST_METHOD(StaticArrayUT4)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			const wchar_t scriptCode[] =
				L"int test(array<int,10>& a) {"
				L"	return a[0];"
				L"}"
				L"int foo() {"
				L"	array <int,10> ret;"
				L"	ret[0] = 1;"
				L"	return test(ret);"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(1, *iRes);
		}

		TEST_METHOD(StaticArray2DUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	array< int,10,10> ret;"
				L"	ret[0][0] = 1;"
				L"	return ret[0][0];"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(1, *iRes);
		}

		TEST_METHOD(StaticArray2DUT2)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	array< int ,10, 10 > ret;"
				L"	ret[1][0] = 1;"
				L"	return ret[1][0];"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(1, *iRes);
		}

		TEST_METHOD(StaticArray2DUT3)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	array  <int,  10  ,10> ret;"
				L"	ret[1][1] = 1;"
				L"	return ret[1][1];"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(1, *iRes);
		}

		TEST_METHOD(StaticArray3DUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	array<int,10,10,2>   ret;"
				L"	ret[1][1][1] = 1;"
				L"	return ret[1][1][1];"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(1, *iRes);
		}

		TEST_METHOD(StaticArray1DSortingUT)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			
			const wchar_t scriptCode[] =
				L"void foo(array<int, 10>& a) {"
				L"	int i = 0;"
				L"	int n = 10;"
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

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "array<int, 10>&");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			int a[] = { 3,2,5,6,7,1,4,0,9,8 };
			const int n = sizeof(a) / sizeof(a[0]);
			ScriptParamBuffer paramBuffer(&a[0]);

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, &paramBuffer);

			int i;
			for (i = 0; i < n; i++) {
				if (a[i] != i) {
					break;
				}
			}

			EXPECT_EQ(n, i, L"function 'foo' return wrong");
		}

		TEST_METHOD(GlobalStaticArray1DUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			const wchar_t scriptCode[] =
				L"array<int,10>   ret;"
				L"int foo() {"
				L"	return ret[5];"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			auto pVariable = rootScope->findVariable("ret");
			EXPECT_NE(nullptr, pVariable, L"cannot find variable ret");

			constexpr int iVal = 1;

			int* pRet = (int*)rootScope->getGlobalAddress(pVariable->getOffset());
			pRet[5] = iVal;

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(iVal, *iRes);
		}

		TEST_METHOD(GlobalStaticArray1DUT2)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			const wchar_t scriptCode[] =
				L"array<int,10>   ret;"
				L"ret[5] = 1;"
				L"int foo() {"
				L"	return ret[5];"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			rootScope->runGlobalCode();

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			rootScope->cleanupGlobalMemory();

			EXPECT_EQ(1, *iRes);
		}

		TEST_METHOD(GlobalStaticArray3DUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			const wchar_t scriptCode[] =
				L"array<int,10, 5, 2>   ret;"
				L"ret[5][4][1] = 1;"
				L"int foo() {"
				L"	return ret[5][4][1];"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			rootScope->runGlobalCode();

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			rootScope->cleanupGlobalMemory();

			EXPECT_EQ(1, *iRes);
		}

		TEST_METHOD(StaticArray1DInitializeUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	array<int,3> ret = {1, 2};"
				L"	return ret[0];"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(1, *iRes);
		}

		TEST_METHOD(StaticArray1DInitializeUT2)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"array<int,3> ret = {1, 2};"
				L"int foo() {"
				L"	int res = ret[0];"
				L"	return res;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			rootScope->runGlobalCode();

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			rootScope->cleanupGlobalMemory();

			EXPECT_EQ(1, *iRes);
		}
	};
}