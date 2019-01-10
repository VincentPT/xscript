/******************************************************************
* File:        CompileProgramUT.cpp
* Description: Test cases for compiling and running C Lambda scripting
*              language using CompilerSuite.
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
#include <DefaultPreprocessor.h>
#include <RawStringLib.h>

#include "Utils.h"

using namespace std;
using namespace ffscript;


namespace ffscriptUT
{	
	namespace MethodUT
	{
		static int inc(int* thiz) {
			return ++ *thiz;
		}

		static int inc(int thiz) {
			return ++ thiz;
		}

		static int inc(int& thiz) {
			return ++thiz;
		}

		static void jump(int* thiz, int step) {
			*thiz += step;
		}

		FF_TEST_FUNCTION(MethodTest, TestMethod01)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	int val = 1;"
				L"	res = val.inc();"
				L"	return res;"
				L"}"
				;
			
			int functionId = scriptCompiler->registFunction("inc", "ref int", new DefaultUserFunctionFactory(
				createFunctionDelegateRef<int, int*>(inc), scriptCompiler, "int", 1)
			);

			FF_EXPECT_TRUE(functionId > 0, L"register function failed");
			
			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, convertToWstring(scriptCompiler->getLastError()).c_str());
			functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			auto res = *(int*)scriptTask.getTaskResult();
			FF_EXPECT_EQ(2, res);
		}

		FF_TEST_FUNCTION(MethodTest, TestMethod02)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	int val = 1;"
				L"	res = val.inc();"
				L"	return res;"
				L"}"
				;

			int functionId = scriptCompiler->registFunction("inc", "int&", new DefaultUserFunctionFactory(
				createFunctionDelegateRef<int, int&>(inc), scriptCompiler, "int", 1)
			);

			FF_EXPECT_TRUE(functionId > 0, L"register function failed");

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, convertToWstring(scriptCompiler->getLastError()).c_str());
			functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			auto res = *(int*)scriptTask.getTaskResult();
			FF_EXPECT_EQ(2, res);
		}

		FF_TEST_FUNCTION(MethodTest, TestMethod03)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	int val = 1;"
				L"	res = val.inc();"
				L"	return res;"
				L"}"
				;

			int functionId = scriptCompiler->registFunction("inc", "int", new DefaultUserFunctionFactory(
				createFunctionDelegateRef<int, int>(inc), scriptCompiler, "int", 1)
			);

			FF_EXPECT_TRUE(functionId > 0, L"register function failed");

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, convertToWstring(scriptCompiler->getLastError()).c_str());
			functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			auto res = *(int*)scriptTask.getTaskResult();
			FF_EXPECT_EQ(2, res);
		}

		FF_TEST_FUNCTION(MethodTest, TestMethod04)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	int val = 0;"
				L"	val.jump(3);"
				L"	return val;"
				L"}"
				;

			int functionId = scriptCompiler->registFunction("jump", "ref int, int", new DefaultUserFunctionFactory(
				createFunctionDelegateRef<void, int*, int>(jump), scriptCompiler, "void", 2)
			);

			FF_EXPECT_TRUE(functionId > 0, L"register function failed");

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, convertToWstring(scriptCompiler->getLastError()).c_str());
			functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			auto res = *(int*)scriptTask.getTaskResult();
			FF_EXPECT_EQ(3, res);
		}

		class ContextMethodContainer {
			int _val;
		public:
			ContextMethodContainer(int val) : _val(val) {}

			int multiply(int val) {
				return _val * val;
			}
		};

		int multiply(ContextMethodContainer* p, int val) {
			return 1 + p->multiply(val);
		}

		FF_TEST_FUNCTION(MethodTest, TestMethod05)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			int type = scriptCompiler->registType("Caculator");
			FF_EXPECT_TRUE(!IS_UNKNOWN_TYPE(type));
			scriptCompiler->setTypeSize(type, sizeof(ContextMethodContainer));

			const wchar_t* scriptCode =
				L"int testMultiply(ref Caculator calculator) {"
				L"	return calculator.multiply(3);"
				L"}"
				;

			int functionId = scriptCompiler->registFunction("multiply", "ref Caculator, int", 
				createUserFunctionFactoryContext<ContextMethodContainer, int, int>(scriptCompiler, "int", &ContextMethodContainer::multiply)
			);

			FF_EXPECT_TRUE(functionId > 0, L"register function failed");

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, convertToWstring(scriptCompiler->getLastError()).c_str());
			functionId = scriptCompiler->findFunction("testMultiply", "ref Caculator");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'testMultiply'");

			ScriptTask scriptTask(program);

			ContextMethodContainer aInstance(2);
			ScriptParamBuffer argBuffer(&aInstance);
			scriptTask.runFunction(functionId, &argBuffer);

			auto res = *(int*)scriptTask.getTaskResult();
			FF_EXPECT_EQ(6, res);
		}

		FF_TEST_FUNCTION(MethodTest, TestMethod06)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			int type = scriptCompiler->registType("Caculator");
			FF_EXPECT_TRUE(!IS_UNKNOWN_TYPE(type));
			scriptCompiler->setTypeSize(type, sizeof(ContextMethodContainer));

			const wchar_t* scriptCode =
				L"int testMultiply(ref Caculator calculator) {"
				L"	return calculator.multiply(3);"
				L"}"
				;

			int functionId = scriptCompiler->registFunction("multiply", "ref Caculator, int",
				createUserFunctionFactory<int, ContextMethodContainer*, int>(scriptCompiler, "int", multiply)
			);

			FF_EXPECT_TRUE(functionId > 0, L"register function failed");

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, convertToWstring(scriptCompiler->getLastError()).c_str());
			functionId = scriptCompiler->findFunction("testMultiply", "ref Caculator");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'testMultiply'");

			ScriptTask scriptTask(program);

			ContextMethodContainer aInstance(2);
			ScriptParamBuffer argBuffer(&aInstance);
			scriptTask.runFunction(functionId, &argBuffer);

			auto res = *(int*)scriptTask.getTaskResult();
			FF_EXPECT_EQ(7, res);
		}
	};
}