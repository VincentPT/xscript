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
#include <OperatorObject.h>

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
	TEST_CLASS(OperatorBracketsUT)
	{
		CompilerSuite compiler;
		const BasicTypes* basicType;
		ScriptCompiler* scriptCompiler;
	public:

		OperatorBracketsUT()
		{
			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			scriptCompiler = compiler.getCompiler().get();
			basicType = &compiler.getTypeManager()->getBasicTypes();			
		}

		TEST_METHOD(ForTypeHasNoBreacketOperator1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			
			const wchar_t scriptCode[] =
				L"void test() {"
				L"	int ret;"
				L"	ret();"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNull(program, L"Compile program should be failed");
		}

		TEST_METHOD(ForTypeHasNoBreacketOperator2)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"void test() {"
				L"	int ret;"
				L"	ret(1);"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNull(program, L"Compile program should be failed");
		}

		static int int_inc(int a) {
			return a + 1;
		}

		static int* int_ref(int& a) {
			return &a;
		}

		TEST_METHOD(ForTypeHasBracketOperator1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			FunctionRegisterHelper fb(scriptCompiler);

			const wchar_t scriptCode[] =
				L"int test() {"
				L"	int ret = 0;"
				L"	return ret();"
				L"}"
				;
			DFunction2* intFunctionOperator = new CdeclFunction2<int, int>(int_inc);
			int functionId = fb.registFunction("int_function_operator", "int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", intFunctionOperator, scriptCompiler));
			Assert::IsTrue(functionId > 0, L"register function failed");

			bool res = scriptCompiler->registFunctionOperator(basicType->TYPE_INT, functionId);
			Assert::IsTrue(res, L"register function operator for integer failed");

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(1, *iRes, L"function operator is run but return value is not correct");
		}

		TEST_METHOD(ForTypeHasBracketOperator2)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			FunctionRegisterHelper fb(scriptCompiler);

			const wchar_t scriptCode[] =
				L"int test() {"
				L"	int ret = 2;"
				L"	return ret(3);"
				L"}"
				;
			int functionId = scriptCompiler->findFunction("*", "int,int");
			Assert::IsTrue(functionId > 0, L"operator * is not registered");

			bool res = scriptCompiler->registFunctionOperator(basicType->TYPE_INT, functionId);
			Assert::IsTrue(res, L"register function operator for integer failed");

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(6, *iRes, L"function operator is run but return value is not correct");
		}

		TEST_METHOD(ForTypeHasBracketOperator3)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			FunctionRegisterHelper fb(scriptCompiler);

			const wchar_t scriptCode[] =
				L"int test1() {"
				L"	return 2;"
				L"}"
				L"int test() {"
				L"	int ret = test1()(3);"
				L"	return ret;"
				L"}"
				;
			int functionId = scriptCompiler->findFunction("*", "int,int");
			Assert::IsTrue(functionId > 0, L"operator * is not registered");

			bool res = scriptCompiler->registFunctionOperator(basicType->TYPE_INT, functionId);
			Assert::IsTrue(res, L"register function operator for integer failed");

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(6, *iRes, L"function operator is run but return value is not correct");
		}

		TEST_METHOD(ForTypeHasBracketOperator4)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			FunctionRegisterHelper fb(scriptCompiler);

			const wchar_t scriptCode[] =
				L"int test() {"
				L"	int ret = 2;"
				L"	e = ref(ret);"
				L"	return (*e)(3);"
				L"}"
				;
			int functionId = scriptCompiler->findFunction("*", "int,int");
			Assert::IsTrue(functionId > 0, L"operator * is not registered");

			bool res = scriptCompiler->registFunctionOperator(basicType->TYPE_INT, functionId);
			Assert::IsTrue(res, L"register function operator for integer failed");

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(6, *iRes, L"function operator is run but return value is not correct");
		}

		TEST_METHOD(ForTypeHasBracketOperator5)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			FunctionRegisterHelper fb(scriptCompiler);

			const wchar_t scriptCode[] =
				L"int test() {"
				L"	int ret = 2;"
				L"	e = ref(ret);"
				L"	return e[0](3);"
				L"}"
				;
			int functionId = scriptCompiler->findFunction("*", "int,int");
			Assert::IsTrue(functionId > 0, L"operator * is not registered");

			bool res = scriptCompiler->registFunctionOperator(basicType->TYPE_INT, functionId);
			Assert::IsTrue(res, L"register function operator for integer failed");

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(6, *iRes, L"function operator is run but return value is not correct");
		}

		TEST_METHOD(ForTypeHasBracketOperator6)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			FunctionRegisterHelper fb(scriptCompiler);

			const wchar_t scriptCode[] =
				L"int test() {"
				L"	int ret = 1;"
				L"	return ret()[0];"
				L"}"
				;
			DFunction2* intFunctionOperator = new CdeclFunction2<int*, int&>(int_ref);
			int functionId = fb.registFunction("forward_ref", "int&", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "ref int", intFunctionOperator, scriptCompiler));
			Assert::IsTrue(functionId > 0, L"register function failed");

			bool res = scriptCompiler->registFunctionOperator(basicType->TYPE_INT, functionId);
			Assert::IsTrue(res, L"register function operator for integer failed");

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(1, *iRes, L"function operator is run but return value is not correct");
		}

		static int sum(SimpleVariantArray* params) {
			int sum = 0;
			for (int i = 0; i < params->size; i++) {
				sum += *((int*)params->elems[i].pData);
			}
			return sum;
		}

		static int sum(int a, int b, int c) {
			return (a + b + c);
		}

		static int sum(int a, int b, int c, int d, int e, int f) {
			return (a + b + c + d + e + f);
		}

		TEST_METHOD(ForTypeHasBracketOperator7)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			FunctionRegisterHelper fb(scriptCompiler);

			const wchar_t scriptCode[] =
				L"int test() {"
				L"	int ret = 1;"
				L"	return ret(2, 3);"
				L"}"
				;
			DFunction2* intFunctionOperator = new CdeclFunction2<int, int, int, int>(sum);
			int functionId = fb.registFunction("forward_ref", "int,int,int", new BasicFunctionFactory<3>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", intFunctionOperator, scriptCompiler));
			Assert::IsTrue(functionId > 0, L"register function failed");

			bool res = scriptCompiler->registFunctionOperator(basicType->TYPE_INT, functionId);
			Assert::IsTrue(res, L"register function operator for integer failed");

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(6, *iRes, L"function operator is run but return value is not correct");
		}

		TEST_METHOD(ForTypeHasBracketOperator8)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			FunctionRegisterHelper fb(scriptCompiler);

			const wchar_t scriptCode[] =
				L"int test() {"
				L"	int ret = 1;"
				L"	return ret(2, 3, 4, 5, 6);"
				L"}"
				;
			DFunction2* intFunctionOperator = new CdeclFunction2<int, int, int, int, int, int, int>(sum);
			int functionId = fb.registFunction("forward_ref", "int,int,int,int,int,int", new BasicFunctionFactory<6>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", intFunctionOperator, scriptCompiler));
			Assert::IsTrue(functionId > 0, L"register function failed");

			bool res = scriptCompiler->registFunctionOperator(basicType->TYPE_INT, functionId);
			Assert::IsTrue(res, L"register function operator for integer failed");

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(21, *iRes, L"function operator is run but return value is not correct");
		}

		TEST_METHOD(ForTypeHasBracketOperator9)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			FunctionRegisterHelper fb(scriptCompiler);

			const wchar_t scriptCode[] =
				L"int test() {"
				L"	int ret = 1;"
				L"	return ret(2, 3, 4, 5, 6);"
				L"}"
				;
			auto theNativeFunction = new CdeclFunction2<int, SimpleVariantArray*>(sum);
			DynamicFunctionFactory dynamicFunctionFactor("int", theNativeFunction, scriptCompiler);
			int functionId = fb.registDynamicFunction("sum", &dynamicFunctionFactor, false);
			Assert::IsTrue(functionId > 0, L"register function failed");

			bool res = scriptCompiler->registFunctionOperator(basicType->TYPE_INT, functionId);
			Assert::IsTrue(res, L"register function operator for integer failed");

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			functionId = scriptCompiler->findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(21, *iRes, L"function operator is run but return value is not correct");
		}
	};
}