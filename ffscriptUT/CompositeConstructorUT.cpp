/******************************************************************
* File:        CompositeConstructorUT.cpp
* Description: Test cases for checking behavior of composite contructors.
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

	TEST_CLASS(CompositeConstructorUT)
	{
		CompilerSuite compiler;
		const BasicTypes* basicType;
		ScriptCompiler* scriptCompiler;
		
	public:
		CompositeConstructorUT()
		{
			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			scriptCompiler = compiler.getCompiler().get();
			basicType = &compiler.getTypeManager()->getBasicTypes();
		}

		static void IntegerConstructor(int& objRef, int a, int b) {
			objRef = a + b;
		}

		static void IntegerConstructor(int& objRef, double & a) {
			objRef = (int)a;
			objRef++;
		}

		static void IntegerConstructor(int& objRef, int&  a) {
			objRef = a + 1;
		}

		TEST_METHOD(SimpleUT11)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new CdeclFunction2<void, int&, int, int>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int, int", new BasicFunctionFactory<3>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	int ret = {1,2};"
				L"	return ret;"
				L"}" 
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(3, *iRes, L"Construtor is run but result is not correct");
		}

		TEST_METHOD(SimpleUT12)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new CdeclFunction2<void, int&, int, int>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int, int", new BasicFunctionFactory<3>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo(int param) {"
				L"	return param;"
				L"}"
				L"int foo() {"
				L"	int ret = foo({1,2});"
				L"	return ret;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(3, *iRes, L"Construtor is run but result is not correct");
		}

		TEST_METHOD(SimpleUT13)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new CdeclFunction2<void, int&, int, int>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int, int", new BasicFunctionFactory<3>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo(int param) {"
				L"	return param;"
				L"}"
				L"int foo() {"
				L"	return foo({1,2});"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(3, *iRes, L"Construtor is run but result is not correct");
		}

		TEST_METHOD(SimpleUT21)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new CdeclFunction2<void, int&, double&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, double&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	int ret = 1.0;"
				L"	return ret;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(2, *iRes, L"Construtor is run but result is not correct");
		}

		TEST_METHOD(SimpleUT22)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new CdeclFunction2<void, int&, double&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, double&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo(int param) {"
				L"	return param;"
				L"}"
				L"int foo() {"
				L"	int ret = foo(1.0);"
				L"	return ret;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(2, *iRes, L"Construtor is run but result is not correct");
		}

		TEST_METHOD(SimpleUT31)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new CdeclFunction2<void, int&, double&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, double&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo() {"				
				L"	return 1.0;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(2, *iRes, L"Construtor is run but result is not correct");
		}

		TEST_METHOD(SimpleUT32)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new CdeclFunction2<void, int&, int, int>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int, int", new BasicFunctionFactory<3>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	return {1,2};"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(3, *iRes, L"Construtor is run but result is not correct");
		}

		TEST_METHOD(SimpleUT40)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			
			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	return 1;"
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

			EXPECT_EQ(1, *iRes, L"Construtor is run but result is not correct");
		}

		TEST_METHOD(SimpleUT41)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new CdeclFunction2<void, int&, int&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	return 1;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(2, *iRes, L"Construtor is run but result is not correct");
		}

		TEST_METHOD(SimpleUT42)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new CdeclFunction2<void, int&, int&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	int p = 1;"
				L"	return p;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(2, *iRes, L"Construtor is run but result is not correct");
		}

		TEST_METHOD(SimpleUT43)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new CdeclFunction2<void, int&, int&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo2() {"				
				L"	return 1;"
				L"}"
				L"int foo() {"				
				L"	return foo2();"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(2, *iRes, L"Construtor is run but result is not correct");
		}

		TEST_METHOD(SimpleUT44)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new CdeclFunction2<void, int&, int&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo2(int a) {"
				L"	return a;"
				L"}"
				L"int foo() {"
				L"	return foo2(1);"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			// optimizing pass parameter through interger constructor
			// so the constructor is not call to construct int object from constant '1'
			EXPECT_EQ(1, *iRes, L"Construtor is run but result is not correct");
		}

		TEST_METHOD(SimpleUT45)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new CdeclFunction2<void, int&, int&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	int a = 1;"
				L"	pa = ref(a);"
				L"	return *pa;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(3, *iRes, L"Construtor is run but result is not correct");
		}

		TEST_METHOD(SimpleUT46)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new CdeclFunction2<void, int&, int&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	int a = 1;"
				L"	int& ra = &a;"
				L"	return ra;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(3, *iRes, L"Construtor is run but result is not correct");
		}
	};
}