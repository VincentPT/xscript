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
#include "fftest.hpp"

#include "ExpresionParser.h"
#include <functional>
#include "TemplateForTest.hpp"
#include "Variable.h"
#include "FunctionRegisterHelper.h"
#include "BasicFunction.h"
#include "BasicType.h"
#include "FunctionFactory.h"
#include "function/MemberFunction.hpp"
#include "Context.h"
#include <thread>
#include <future>
#include <Program.h>
#include <ScriptTask.h>

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
#include "function/MemberFunction3.hpp"
#include "expressionunit.h"
#include "DynamicFunctionFactory.h"
#include "MemoryBlock.h"
#include "CompilerSuite.h"

namespace ffscriptUT
{
	FF_TEST_CLASS(CompositeConstructor)
	{
	protected:
		CompilerSuite compiler;
		const BasicTypes* basicType;
		ScriptCompiler* scriptCompiler;

	public:
		CompositeConstructor()
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
	};

	namespace CompositeConstructorUT {
		FF_TEST_METHOD(CompositeConstructor, SimpleUT11)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new FT::FunctionDelegate3<void, int&, int, int>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int, int", new BasicFunctionFactory<3>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			FF_EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			FF_EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	int ret = {1,2};"
				L"	return ret;"
				L"}" 
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(3, *iRes, L"Construtor is run but result is not correct");
		}

		FF_TEST_METHOD(CompositeConstructor, SimpleUT12)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new FT::FunctionDelegate3<void, int&, int, int>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int, int", new BasicFunctionFactory<3>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			FF_EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			FF_EXPECT_TRUE(blRes, L"Register constructor failed");

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
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(3, *iRes, L"Construtor is run but result is not correct");
		}

		FF_TEST_METHOD(CompositeConstructor, SimpleUT13)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new FT::FunctionDelegate3<void, int&, int, int>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int, int", new BasicFunctionFactory<3>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			FF_EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			FF_EXPECT_TRUE(blRes, L"Register constructor failed");

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
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(3, *iRes, L"Construtor is run but result is not correct");
		}

		FF_TEST_METHOD(CompositeConstructor, SimpleUT21)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new FT::FunctionDelegate3<void, int&, double&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, double&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			FF_EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			FF_EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	int ret = 1.0;"
				L"	return ret;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(2, *iRes, L"Construtor is run but result is not correct");
		}

		FF_TEST_METHOD(CompositeConstructor, SimpleUT22)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new FT::FunctionDelegate3<void, int&, double&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, double&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			FF_EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			FF_EXPECT_TRUE(blRes, L"Register constructor failed");

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
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(2, *iRes, L"Construtor is run but result is not correct");
		}

		FF_TEST_METHOD(CompositeConstructor, SimpleUT31)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new FT::FunctionDelegate3<void, int&, double&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, double&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			FF_EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			FF_EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo() {"				
				L"	return 1.0;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(2, *iRes, L"Construtor is run but result is not correct");
		}

		FF_TEST_METHOD(CompositeConstructor, SimpleUT32)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new FT::FunctionDelegate3<void, int&, int, int>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int, int", new BasicFunctionFactory<3>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			FF_EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			FF_EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	return {1,2};"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(3, *iRes, L"Construtor is run but result is not correct");
		}

		FF_TEST_METHOD(CompositeConstructor, SimpleUT40)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			
			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	return 1;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(1, *iRes, L"Construtor is run but result is not correct");
		}

		FF_TEST_METHOD(CompositeConstructor, SimpleUT41)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new FT::FunctionDelegate3<void, int&, int&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			FF_EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			FF_EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	return 1;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(2, *iRes, L"Construtor is run but result is not correct");
		}

		FF_TEST_METHOD(CompositeConstructor, SimpleUT42)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new FT::FunctionDelegate3<void, int&, int&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			FF_EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			FF_EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	int p = 1;"
				L"	return p;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(2, *iRes, L"Construtor is run but result is not correct");
		}

		FF_TEST_METHOD(CompositeConstructor, SimpleUT43)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new FT::FunctionDelegate3<void, int&, int&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			FF_EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			FF_EXPECT_TRUE(blRes, L"Register constructor failed");

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
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(2, *iRes, L"Construtor is run but result is not correct");
		}

		FF_TEST_METHOD(CompositeConstructor, SimpleUT44)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new FT::FunctionDelegate3<void, int&, int&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			FF_EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			FF_EXPECT_TRUE(blRes, L"Register constructor failed");

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
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			// optimizing pass parameter through interger constructor
			// so the constructor is not call to construct int object from constant '1'
			FF_EXPECT_EQ(1, *iRes, L"Construtor is run but result is not correct");
		}

		FF_TEST_METHOD(CompositeConstructor, SimpleUT45)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new FT::FunctionDelegate3<void, int&, int&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			FF_EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			FF_EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	int a = 1;"
				L"	pa = ref(a);"
				L"	return *pa;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(3, *iRes, L"Construtor is run but result is not correct");
		}

		FF_TEST_METHOD(CompositeConstructor, SimpleUT46)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			int type = basicType->TYPE_INT;

			DFunction2* initFunction = new FT::FunctionDelegate3<void, int&, int&>(&IntegerConstructor);
			int functionId = scriptCompiler->registFunction("IntegerConstructor", "ref int, int&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			FF_EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			FF_EXPECT_TRUE(blRes, L"Register constructor failed");

			const wchar_t scriptCode[] =
				L"int foo() {"
				L"	int a = 1;"
				L"	int& ra = &a;"
				L"	return ra;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int* iRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(3, *iRes, L"Construtor is run but result is not correct");
		}
	}
}