/******************************************************************
* File:        SubscriptionUT.cpp
* Description: Test cases focus on checking usage of struct in
*              C Lambda scripting language.
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
#include "RawStringLib.h"
#include "GeometryLib.h"

namespace ffscriptUT
{
#pragma pack(push)
#pragma pack(1)
	struct TestStruct {
		int a;
		int b;
	};
#pragma pack(pop)

	TEST_CLASS(StructUT)
	{
		ScriptCompiler scriptCompiler;
		FunctionRegisterHelper funcLibHelper;
		const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
	public:
		StructUT() : funcLibHelper(&scriptCompiler) {
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
		}

		int testStruct1(TestStruct& obj) {
			return obj.a + obj.b;
		}

		static int sum(SimpleVariantArray* params) {
			int sum = 0;
			auto elm = params->elems;
			auto end = elm + params->size;
			for (; elm < end; elm++) {
				if ((elm->scriptType & DATA_TYPE_POINTER_MASK) || (elm->scriptType & DATA_TYPE_REF_MASK)) {
					sum += **((int**)(elm->pData));
				}
				else {
					sum += *((int*)(elm->pData));
				}
			}
			return sum;
		}

		TEST_METHOD(TestStruct1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			funcLibHelper.registFunction("testStruct", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", new MFunction2<int, StructUT, TestStruct&>(this, &StructUT::testStruct1), &scriptCompiler));

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct TestStruct {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"int invoke(ref TestStruct obj) {"
				L"	return testStruct(obj);"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idTestStruct = scriptCompiler.getType("TestStruct");
			EXPECT_NE(-1, idTestStruct, L"TestStruct should be already registered, but not");

			int functionId = scriptCompiler.findFunction("invoke", "ref TestStruct");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'invoke'");

			TestStruct obj = {1, 2};			
			ScriptParamBuffer paramBuffer(&obj);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("testStruct =" + std::to_string(*funcRes)).c_str());
			EXPECT_TRUE(*funcRes == 3, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStruct2)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			funcLibHelper.registFunction("testStruct", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", new MFunction2<int, StructUT, TestStruct&>(this, &StructUT::testStruct1), &scriptCompiler));
			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct TestStruct {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"int invoke(TestStruct obj) {"
				L"	return testStruct( ref obj );"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idTestStruct = scriptCompiler.getType("TestStruct");
			EXPECT_NE(-1, idTestStruct, L"TestStruct should be already registered, but not");

			int functionId = scriptCompiler.findFunction("invoke", "TestStruct");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'invoke'");

			TestStruct obj = { 1, 2 };
			ScriptParamBuffer paramBuffer(obj);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("testStruct =" + std::to_string(*funcRes)).c_str());
			EXPECT_TRUE(*funcRes == 3, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStruct3)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			funcLibHelper.registFunction("testStruct", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", new MFunction2<int, StructUT, TestStruct&>(this, &StructUT::testStruct1), &scriptCompiler));
			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct TestStruct {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"int test(TestStruct obj) {"
				L"	int ret = obj.a;"
				L"	return ret;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idTestStruct = scriptCompiler.getType("TestStruct");
			EXPECT_NE(-1, idTestStruct, L"TestStruct should be already registered, but not");

			int functionId = scriptCompiler.findFunction("test", "TestStruct");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			TestStruct obj = { 1, 2 };
			ScriptParamBuffer paramBuffer(obj);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("test =" + std::to_string(*funcRes)).c_str());
			EXPECT_TRUE(*funcRes == obj.a, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStruct4)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			funcLibHelper.registFunction("testStruct", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", new MFunction2<int, StructUT, TestStruct&>(this, &StructUT::testStruct1), &scriptCompiler));
			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct TestStruct {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"int test(TestStruct obj) {"
				L"	int ret = obj.b;"
				L"	return ret;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idTestStruct = scriptCompiler.getType("TestStruct");
			EXPECT_NE(-1, idTestStruct, L"TestStruct should be already registered, but not");

			int functionId = scriptCompiler.findFunction("test", "TestStruct");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			TestStruct obj = { 1, 2 };
			ScriptParamBuffer paramBuffer(obj);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("test =" + std::to_string(*funcRes)).c_str());
			EXPECT_TRUE(*funcRes == obj.b, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStruct5)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			funcLibHelper.registFunction("testStruct", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", new MFunction2<int, StructUT, TestStruct&>(this, &StructUT::testStruct1), &scriptCompiler));
			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct TestStruct {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"int test(TestStruct obj) {"
				L"	return obj.b + obj.a;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idTestStruct = scriptCompiler.getType("TestStruct");
			EXPECT_NE(-1, idTestStruct, L"TestStruct should be already registered, but not");

			int functionId = scriptCompiler.findFunction("test", "TestStruct");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			TestStruct obj = { 1, 2 };
			ScriptParamBuffer paramBuffer(obj);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("test =" + std::to_string(*funcRes)).c_str());
			EXPECT_TRUE(*funcRes == obj.b + obj.a, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStruct6)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			funcLibHelper.registFunction("testStruct", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", new MFunction2<int, StructUT, TestStruct&>(this, &StructUT::testStruct1), &scriptCompiler));
			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct TestStruct {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"int test(ref TestStruct obj) {"
				L"	return obj.b + obj.a;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idTestStruct = scriptCompiler.getType("TestStruct");
			EXPECT_NE(-1, idTestStruct, L"TestStruct should be already registered, but not");

			int functionId = scriptCompiler.findFunction("test", "ref TestStruct");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			TestStruct obj = { 1, 2 };
			ScriptParamBuffer paramBuffer(&obj);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("test =" + std::to_string(*funcRes)).c_str());
			EXPECT_TRUE(*funcRes == obj.b + obj.a, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStruct7)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			funcLibHelper.registFunction("testStruct", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", new MFunction2<int, StructUT, TestStruct&>(this, &StructUT::testStruct1), &scriptCompiler));
			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct TestStruct {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"TestStruct test(TestStruct obj) {"
				L"	return obj;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idTestStruct = scriptCompiler.getType("TestStruct");
			EXPECT_NE(-1, idTestStruct, L"TestStruct should be already registered, but not");

			int functionId = scriptCompiler.findFunction("test", "TestStruct");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			TestStruct obj = { 1, 2 };
			ScriptParamBuffer paramBuffer(obj);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			TestStruct* funcRes = (TestStruct*)scriptTask.getTaskResult();
			EXPECT_TRUE(funcRes->a == obj.a, L"program can run but return wrong value");
			EXPECT_TRUE(funcRes->b == obj.b, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStruct8)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			funcLibHelper.registFunction("testStruct", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", new MFunction2<int, StructUT, TestStruct&>(this, &StructUT::testStruct1), &scriptCompiler));
			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct TestStruct {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"TestStruct test(TestStruct obj) {"
				L"	obj.a = obj.a + 2;"
				//L"	obj.b = obj.b + 2;"
				L"	return obj;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idTestStruct = scriptCompiler.getType("TestStruct");
			EXPECT_NE(-1, idTestStruct, L"TestStruct should be already registered, but not");

			int functionId = scriptCompiler.findFunction("test", "TestStruct");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			TestStruct obj = { 1, 2 };
			ScriptParamBuffer paramBuffer(obj);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			TestStruct* funcRes = (TestStruct*)scriptTask.getTaskResult();
			EXPECT_TRUE(funcRes->a == 3, L"program can run but return wrong value");
			//EXPECT_TRUE(funcRes->b == 4, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStruct9)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			funcLibHelper.registFunction("testStruct", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", new MFunction2<int, StructUT, TestStruct&>(this, &StructUT::testStruct1), &scriptCompiler));
			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct TestStruct {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"TestStruct obj;"
				L"int test() {"
				L"	return obj.b + obj.a;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idTestStruct = scriptCompiler.getType("TestStruct");
			EXPECT_NE(-1, idTestStruct, L"TestStruct should be already registered, but not");

			int functionId = scriptCompiler.findFunction("test", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			TestStruct* pObj = getVaribleRef<TestStruct>(*rootScope.findVariable("obj"));
			pObj->a = 1;
			pObj->b = 2;

			TestStruct& obj = *pObj;			

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("test =" + std::to_string(*funcRes)).c_str());
			EXPECT_TRUE(*funcRes == obj.b + obj.a, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStruct10)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			funcLibHelper.registFunction("testStruct", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", new MFunction2<int, StructUT, TestStruct&>(this, &StructUT::testStruct1), &scriptCompiler));
			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct TestStruct {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"TestStruct obj;"
				L"obj.a = 1 + 2;"
				L"obj.b = obj.a + 2;"
				L"int test() {"
				L"	return obj.b + obj.a;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idTestStruct = scriptCompiler.getType("TestStruct");
			EXPECT_NE(-1, idTestStruct, L"TestStruct should be already registered, but not");

			int functionId = scriptCompiler.findFunction("test", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			//the code in global scope should be executed only once
			staticContext.run();

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("test =" + std::to_string(*funcRes)).c_str());
			EXPECT_TRUE(*funcRes == 8, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStructComplex01)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct StructA {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"struct StructB {"
				L"	StructA a;"
				L"	int iVal;"
				L"}"

				L"StructB test() {"
				L"	StructB obj;"
				L"	obj.a.a = 1;"
				L"	obj.a.b = 2;"
				L"	obj.iVal = 3;"
				L"	return obj;"
				L"}"
				;
#pragma pack(push)
#pragma pack(1)
			struct StructA
			{
				int a;
				int b;
			};
			struct StructB
			{
				StructA a;
				int iVal;
			};
#pragma pack(pop)
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("test", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			//the code in global scope should be executed only once
			staticContext.run();

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			StructB* objRes = (StructB*)scriptTask.getTaskResult();
			EXPECT_EQ(1, objRes->a.a, L"program can run but return wrong value");
			EXPECT_EQ(2, objRes->a.b, L"program can run but return wrong value");
			EXPECT_EQ(3, objRes->iVal, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStructComplex02)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct StructA {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"struct StructB {"
				L"	ref StructA a;"
				L"	int iVal;"
				L"}"

				L"StructB test(StructB obj) {"
				L"	obj.a.b = 2;"
				L"	obj.a.a = 1;"
				L"	obj.iVal = 3;"
				L"	return obj;"
				L"}"
				;
#pragma pack(push)
#pragma pack(1)
			struct StructA
			{
				int a;
				int b;
			};
			struct StructB
			{
				StructA* a;
				int iVal;
			};
#pragma pack(pop)
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idStruct = scriptCompiler.getType("StructB");
			EXPECT_NE(-1, idStruct, L"StructB should be already registered, but not");

			int functionId = scriptCompiler.findFunction("test", "StructB");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			StructB obj;
			StructA objA;
			objA.a = 0x1111;
			objA.b = 0x2222;
			obj.a = &objA;

			ScriptParamBuffer paramBuffer(obj);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			StructB* objRes = (StructB*)scriptTask.getTaskResult();
			EXPECT_EQ(1, objRes->a->a, L"program can run but return wrong value");
			EXPECT_EQ(2, objRes->a->b, L"program can run but return wrong value");
			EXPECT_EQ(3, objRes->iVal, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStructComplex03)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct StructA {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"struct StructB {"
				L"	ref StructA a;"
				L"	int iVal;"
				L"}"

				L"int test(StructB obj) {"
				L"	return obj.a.a + obj.a.b + obj.iVal;"
				L"}"
				;
#pragma pack(push)
#pragma pack(1)
			struct StructA
			{
				int a;
				int b;
			};
			struct StructB
			{
				StructA* a;
				int iVal;
			};
#pragma pack(pop)
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idStruct = scriptCompiler.getType("StructB");
			EXPECT_NE(-1, idStruct, L"StructB should be already registered, but not");

			int functionId = scriptCompiler.findFunction("test", "StructB");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			StructB obj;
			StructA objA;
			obj.a = &objA;

			obj.a->a = 1;
			obj.a->b = 2;
			obj.iVal = 3;

			ScriptParamBuffer paramBuffer(obj);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* iRes = (int*)scriptTask.getTaskResult();
			EXPECT_EQ(*iRes, 6, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStructComplex04)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct StructA {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"int test(StructA obj) {"
				L"	return obj.c;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_EQ(nullptr, res, L"compile program should fail");
		}

		TEST_METHOD(TestStructComplex05)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct StructA {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"struct StructB {"
				L"	ref StructA a;"
				L"	int iVal;"
				L"}"

				L"int test(StructB obj) {"
				L"	return obj.a.c;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_EQ(nullptr, res, L"compile program should fail");
		}

		TEST_METHOD(TestStructComplex06)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			importBasicfunction(funcLibHelper);

			//register dynamic functions
			auto theNativeFunction1 = new CdeclFunction2<int, SimpleVariantArray*>(sum);
			auto dynamicFunctionFactory1 = new DynamicFunctionFactory("int", theNativeFunction1, &scriptCompiler);			
			funcLibHelper.getSriptCompiler()->registDynamicFunction("sum", dynamicFunctionFactory1);
			funcLibHelper.addFactory(dynamicFunctionFactory1);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct StructA {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"struct StructB {"
				L"	ref StructA a;"
				L"	int iVal;"
				L"}"

				L"int test(StructB obj) {"
				L"	return sum(obj.a.a, obj.a.b, obj.iVal);"
				L"}"
				;
#pragma pack(push)
#pragma pack(1)
			struct StructA
			{
				int a;
				int b;
			};
			struct StructB
			{
				StructA* a;
				int iVal;
			};
#pragma pack(pop)
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idStruct = scriptCompiler.getType("StructB");
			EXPECT_NE(-1, idStruct, L"StructB should be already registered, but not");

			int functionId = scriptCompiler.findFunction("test", "StructB");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			StructB obj;
			StructA objA;
			obj.a = &objA;

			obj.a->a = 1;
			obj.a->b = 2;
			obj.iVal = 3;
			ScriptParamBuffer paramBuffer(obj);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* iRes = (int*)scriptTask.getTaskResult();
			EXPECT_EQ(6, *iRes, L"program can run but return wrong value");
		}

		//TEST_METHOD(TestStructComplex07)
		//{
		//	byte globalData[1024];
		//	StaticContext staticContext(globalData, sizeof(globalData));
		//	GlobalScope rootScope(&staticContext, &scriptCompiler);

		//	importBasicfunction(funcLibHelper);

		//	//initialize an instance of script program
		//	Program theProgram;
		//	scriptCompiler.bindProgram(&theProgram);

		//	const wchar_t* scriptCode =
		//		L"struct StructA {"
		//		L"	int a;"
		//		L"	int b;"
		//		L"}"

		//		L"struct StructB {"
		//		L"	ref StructA a;"
		//		L"	int iVal;"
		//		L"}"

		//		L"int test(StructB obj) {"
		//		L"	return sum(obj.a.a, obj.a.b, obj.iVal);"
		//		L"}"
		//		;
		//	const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
		//	EXPECT_TRUE(res != nullptr, L"compile program failed");

		//	bool blRes = rootScope.extractCode(&theProgram);
		//	EXPECT_TRUE(blRes, L"extract code failed");

		//	int idStruct = scriptCompiler.getType("StructB");
		//	EXPECT_NE(-1, idStruct, L"StructB should be already registered, but not");

		//	int functionId = scriptCompiler.findFunction("test", "StructB");
		//	EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

		//	StructB obj;
		//	StructA objA;
		//	obj.a = &objA;

		//	obj.a->a = 1;
		//	obj.a->b = 2;
		//	obj.iVal = 3;
		//	ScriptParamBuffer paramBuffer(obj);

		//	ScriptTask scriptTask(&theProgram);
		//	scriptTask.runFunction(functionId, &paramBuffer);
		//	int* iRes = (int*)scriptTask.getTaskResult();
		//	EXPECT_EQ(6, *iRes, L"program can run but return wrong value");
		//}

		TEST_METHOD(TestStructReturn01)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct StructA {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"StructB test(StructA obj) {"
				L"	StructA objTemp = obj;"
				L"	return objTemp;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_EQ(nullptr, res, L"compile program failed");
		}

		TEST_METHOD(TestStructAssigment01)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct StructA {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"StructA test(StructA obj) {"
				L"	StructA objTemp = obj;"
				L"	return objTemp;"
				L"}"
				;
#pragma pack(push)
#pragma pack(1)
			struct StructA
			{
				int a;
				int b;
			};
#pragma pack(pop)
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_NE(nullptr, res, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idStruct = scriptCompiler.getType("StructA");
			EXPECT_NE(-1, idStruct, L"StructA should be already registered, but not");

			int functionId = scriptCompiler.findFunction("test", "StructA");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			StructA objA;
			objA.a = 1;
			objA.b = 2;
			ScriptParamBuffer paramBuffer(objA);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			StructA* objRes = (StructA*)scriptTask.getTaskResult();
			EXPECT_EQ(objA.a, objRes->a, L"program can run but return wrong value");
			EXPECT_EQ(objA.a, objRes->a, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStructAssigment02)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct StructA {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"StructA test(StructA obj) {"
				L"	StructA objTemp1;"
				L"	StructA objTemp2;"
				L"	objTemp2 = objTemp1 = obj;"
				L"	return objTemp2;"
				L"}"
				;
#pragma pack(push)
#pragma pack(1)
			struct StructA
			{
				int a;
				int b;
			};
#pragma pack(pop)
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_NE(nullptr, res, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idStruct = scriptCompiler.getType("StructA");
			EXPECT_NE(-1, idStruct, L"StructA should be already registered, but not");

			int functionId = scriptCompiler.findFunction("test", "StructA");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			StructA objA;
			objA.a = 1;
			objA.b = 2;
			ScriptParamBuffer paramBuffer(objA);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			StructA* objRes = (StructA*)scriptTask.getTaskResult();
			EXPECT_EQ(objA.a, objRes->a, L"program can run but return wrong value");
			EXPECT_EQ(objA.a, objRes->a, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStructInitialize1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct StructA {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"StructA test() {"
				L"	StructA objTemp = {1,2};"
				L"	return objTemp;"
				L"}"
				;
#pragma pack(push)
#pragma pack(1)
			struct StructA
			{
				int a;
				int b;
			};
#pragma pack(pop)
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_NE(nullptr, res, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("test", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			StructA* objRes = (StructA*)scriptTask.getTaskResult();
			EXPECT_EQ(1, objRes->a, L"program can run but return wrong value");
			EXPECT_EQ(2, objRes->b, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStructInitialize2)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct StructA {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"struct StructB {"
				L"	int a;"
				L"	StructA b;"
				L"	int c;"
				L"}"

				L"StructB test() {"
				L"	StructB objTemp = {1,{2,3},4};"
				L"	return objTemp;"
				L"}"
				;
#pragma pack(push)
#pragma pack(1)
			struct StructA
			{
				int a;
				int b;
			};
			struct StructB
			{
				int a;
				StructA b;
				int c;
			};
#pragma pack(pop)
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_NE(nullptr, res, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("test", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			StructB* objRes = (StructB*)scriptTask.getTaskResult();
			EXPECT_EQ(1, objRes->a, L"program can run but return wrong value");
			EXPECT_EQ(2, objRes->b.a, L"program can run but return wrong value");
			EXPECT_EQ(3, objRes->b.b, L"program can run but return wrong value");
			EXPECT_EQ(4, objRes->c, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStructInitialize3)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct StructA {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"struct StructB {"
				L"	int a;"
				L"	ref StructA b;"
				L"	int c;"
				L"}"

				L"StructB test() {"
				L"	StructB objTemp = {1,{2,3},4};"
				L"	return objTemp;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_EQ(nullptr, res, L"compile program should be failed, due to struct assignment's rules");
		}

		TEST_METHOD(TestStructInitialize4)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct StructA {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"struct StructB {"
				L"	int a;"
				L"	StructA b;"
				L"	int c;"
				L"}"

				L"StructB test() {"
				L"	StructB objTemp;"
				L"	StructB objTemp1;"
				L"	objTemp = objTemp1 = {1,{2,3},4};"
				L"	return objTemp;"
				L"}"
				;
#pragma pack(push)
#pragma pack(1)
			struct StructA
			{
				int a;
				int b;
			};
			struct StructB
			{
				int a;
				StructA b;
				int c;
			};
#pragma pack(pop)
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_NE(nullptr, res, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("test", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			StructB* objRes = (StructB*)scriptTask.getTaskResult();
			EXPECT_EQ(1, objRes->a, L"program can run but return wrong value");
			EXPECT_EQ(2, objRes->b.a, L"program can run but return wrong value");
			EXPECT_EQ(3, objRes->b.b, L"program can run but return wrong value");
			EXPECT_EQ(4, objRes->c, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStructInitialize5)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct StructA {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"struct StructB {"
				L"	int a;"
				L"	StructA b;"
				L"	int c;"
				L"}"

				L"StructB test() {"
				L"	StructB objTemp1;"
				L"	return objTemp1 = {1,{2,3},4};"
				L"}"
				;
#pragma pack(push)
#pragma pack(1)
			struct StructA
			{
				int a;
				int b;
			};
			struct StructB
			{
				int a;
				StructA b;
				int c;
			};
#pragma pack(pop)
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_NE(nullptr, res, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("test", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			StructB* objRes = (StructB*)scriptTask.getTaskResult();
			EXPECT_EQ(1, objRes->a, L"program can run but return wrong value");
			EXPECT_EQ(2, objRes->b.a, L"program can run but return wrong value");
			EXPECT_EQ(3, objRes->b.b, L"program can run but return wrong value");
			EXPECT_EQ(4, objRes->c, L"program can run but return wrong value");
		}

		///
		/// Check initializing struct with a dynamic array and function inside the dynamic array
		///
		TEST_METHOD(TestStructInitialize6)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct StructA {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"struct StructB {"
				L"	int a;"
				L"	StructA b;"
				L"	int c;"
				L"}"

				L"int fx(int x) {"
				L"  return x;"
				L"}"

				L"StructB test() {"
				L"	StructB objTemp1;"
				L"	return objTemp1 = {fx(1),{2,fx(3)},4};"
				L"}"
				;
#pragma pack(push)
#pragma pack(1)
			struct StructA
			{
				int a;
				int b;
			};
			struct StructB
			{
				int a;
				StructA b;
				int c;
			};
#pragma pack(pop)
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_NE(nullptr, res, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("test", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			StructB* objRes = (StructB*)scriptTask.getTaskResult();
			EXPECT_EQ(1, objRes->a, L"program can run but return wrong value");
			EXPECT_EQ(2, objRes->b.a, L"program can run but return wrong value");
			EXPECT_EQ(3, objRes->b.b, L"program can run but return wrong value");
			EXPECT_EQ(4, objRes->c, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStructReturnRef)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			funcLibHelper.registFunction("testStruct", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", new MFunction2<int, StructUT, TestStruct&>(this, &StructUT::testStruct1), &scriptCompiler));
			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct TestStruct {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"ref int test(ref TestStruct obj) {"
				L"	return obj.a;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idTestStruct = scriptCompiler.getType("TestStruct");
			EXPECT_NE(-1, idTestStruct, L"TestStruct should be already registered, but not");

			int functionId = scriptCompiler.findFunction("test", "ref TestStruct");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			//the code in global scope should be executed only once
			staticContext.run();

			TestStruct obj;
			obj.a = 1;
			obj.b = 2;
			ScriptParamBuffer paramBuffer(&obj);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int** funcRes = (int**)scriptTask.getTaskResult();
			EXPECT_EQ((size_t)(&obj.a), (size_t)(*funcRes), L"program can run but return wrong value");
		}

		TEST_METHOD(TestStructAndDeref01)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct StructA {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"struct StructB {"
				L"	ref StructA a;"
				L"	int iVal;"
				L"}"

				L"StructA test(StructB obj) {"
				L"	return *(obj.a);"
				L"}"
				;
#pragma pack(push)
#pragma pack(1)
			struct StructA
			{
				int a;
				int b;
			};
			struct StructB
			{
				StructA* a;
				int iVal;
			};
#pragma pack(pop)
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idStruct = scriptCompiler.getType("StructB");
			EXPECT_NE(-1, idStruct, L"StructB should be already registered, but not");

			int functionId = scriptCompiler.findFunction("test", "StructB");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			StructB obj;
			StructA objA;
			objA.a = 1;
			objA.b = 2;
			obj.a = &objA;

			ScriptParamBuffer paramBuffer(obj);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			StructA* objRes = (StructA*)scriptTask.getTaskResult();
			EXPECT_EQ(objA.a, objRes->a, L"program can run but return wrong value");
			EXPECT_EQ(objA.b, objRes->b, L"program can run but return wrong value");
		}

		TEST_METHOD(TestStructAndDeref02)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			importBasicfunction(funcLibHelper);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct StructA {"
				L"	int a;"
				L"	int b;"
				L"}"

				L"struct StructB {"
				L"	ref StructA a;"
				L"	int iVal;"
				L"}"

				L"StructA test(StructB obj) {"
				L"	return *obj.a;"
				L"}"
				;
#pragma pack(push)
#pragma pack(1)
			struct StructA
			{
				int a;
				int b;
			};
			struct StructB
			{
				StructA* a;
				int iVal;
			};
#pragma pack(pop)
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int idStruct = scriptCompiler.getType("StructB");
			EXPECT_NE(-1, idStruct, L"StructB should be already registered, but not");

			int functionId = scriptCompiler.findFunction("test", "StructB");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			StructB obj;
			StructA objA;
			objA.a = 5;
			objA.b = 4;
			obj.a = &objA;
			ScriptParamBuffer paramBuffer(obj);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			StructA* objRes = (StructA*)scriptTask.getTaskResult();
			EXPECT_EQ(objA.a, objRes->a, L"program can run but return wrong value");
			EXPECT_EQ(objA.b, objRes->b, L"program can run but return wrong value");
		}

		TEST_METHOD(AccessPointerInStructAndCheckPassingArgument1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			importBasicfunction(funcLibHelper);
			includeRawStringToCompiler(&scriptCompiler);
			includeGeoLibToCompiler(&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct SimpleArray {"
				L"	int count;"
				L"	ref Point data;"
				L"}"

				L"float test() {"
				L"	Point p = {1.1f, 2.2f};"
				L"	SimpleArray a = {1, ref(p)};"
				L"	points = a.data;"
				L"	x = points[0].x;"
				L"	return x;"
				L"}"
				;

			scriptCompiler.beginUserLib();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("test", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			auto fRes = *(float*)scriptTask.getTaskResult();
			EXPECT_EQ(1.1f, fRes, L"function 'foo' return wrong");
		}

		TEST_METHOD(AccessPointerInStructAndCheckPassingArgument2)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			importBasicfunction(funcLibHelper);
			includeRawStringToCompiler(&scriptCompiler);
			includeGeoLibToCompiler(&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct SimpleArray {"
				L"	int count;"
				L"	ref Point data;"
				L"}"

				L"float test() {"
				L"	Point p = {1.1f, 2.2f};"
				L"	SimpleArray a = {1, ref(p)};"
				L"	points = a.data;"
				L"	return points[0].x;"
				L"}"
				;

			scriptCompiler.beginUserLib();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("test", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			auto fRes = *(float*)scriptTask.getTaskResult();
			EXPECT_EQ(1.1f, fRes, L"function 'foo' return wrong");
		}

		TEST_METHOD(AccessPointerInStructAndCheckPassingArgument3)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			importBasicfunction(funcLibHelper);
			includeRawStringToCompiler(&scriptCompiler);
			includeGeoLibToCompiler(&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct SimpleArray {"
				L"	int count;"
				L"	ref Point data;"
				L"}"

				L"float test() {"
				L"	Point p = {1.1f, 2.2f};"
				L"	SimpleArray a = {1, ref(p)};"
				L"	points = a.data;"
				L"	return points.x;"
				L"}"
				;

			scriptCompiler.beginUserLib();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("test", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			auto fRes = *(float*)scriptTask.getTaskResult();
			EXPECT_EQ(1.1f, fRes, L"function 'foo' return wrong");
		}

		TEST_METHOD(AccessPointerInStructAndCheckPassingArgument4)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			importBasicfunction(funcLibHelper);
			includeRawStringToCompiler(&scriptCompiler);
			includeGeoLibToCompiler(&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct SimpleArray {"
				L"	int count;"
				L"	ref Point data;"
				L"}"

				L"float test(ref Point points) {"
				L"	return points[0].y;"
				L"}"

				L"float test() {"
				L"	Point p = {1.1f, 2.2f};"
				L"	SimpleArray a = {1, ref(p)};"
				L"	points = a.data;"
				L"	return test(points);"
				L"}"
				;

			scriptCompiler.beginUserLib();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("test", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			auto fRes = *(float*)scriptTask.getTaskResult();
			EXPECT_EQ(2.2f, fRes, L"function 'foo' return wrong");
		}

		TEST_METHOD(AccessPointerInStructAndCheckPassingArgument5)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			importBasicfunction(funcLibHelper);
			includeRawStringToCompiler(&scriptCompiler);
			includeGeoLibToCompiler(&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"struct SimpleArray {"
				L"	int count;"
				L"	ref Point data;"
				L"}"

				L"float test(ref Point points) {"
				L"	return points[0].y;"
				L"}"

				L"float test() {"
				L"	Point p = {1.1f, 2.2f};"
				L"	SimpleArray a = {1, ref(p)};"
				L"	return test(a.data);"
				L"}"
				;

			scriptCompiler.beginUserLib();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("test", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			auto fRes = *(float*)scriptTask.getTaskResult();
			EXPECT_EQ(2.2f, fRes, L"function 'foo' return wrong");
		}

		TEST_METHOD(TestGeometryLib01)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			importBasicfunction(funcLibHelper);
			includeGeoLibToCompiler(&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"float test() {"
				L"	Point p1 = {1.1f, 2.2f};"
				L"	Point u = {1, 1};"
				L"	GeneralLine a = {p1, u};"
				L"	return compute(a, p1);"
				L"}"
				;

			scriptCompiler.beginUserLib();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("test", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			auto fRes = *(float*)scriptTask.getTaskResult();
			EXPECT_EQ(0.0f, fRes, L"function 'foo' return wrong");
		}

		TEST_METHOD(TestGeometryLib02)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			importBasicfunction(funcLibHelper);
			includeGeoLibToCompiler(&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"float test() {"
				L"	Point p1 = {1.1f, 2.2f};"
				L"	Point u = {1, 1};"
				L"	GeneralLine a = {p1, u};"
				L"	return distance(a, p1);"
				L"}"
				;

			scriptCompiler.beginUserLib();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("test", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			auto fRes = *(float*)scriptTask.getTaskResult();
			EXPECT_EQ(0.0f, fRes, L"function 'foo' return wrong");
		}
	};
}