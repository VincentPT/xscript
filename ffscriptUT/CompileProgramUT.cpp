/******************************************************************
* File:        CompileProgramUT.cpp
* Description: Test cases for compiling, running C Lambda programs
*              read from memory.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include <gtest/gtest.h>

#include "ExpresionParser.h"
#include <functional>
#include "TemplateForTest.hpp"
#include "Variable.h"
#include "FunctionRegisterHelper.h"
#include "BasicFunction.h"
#include "FunctionFactory.h"
#include "function\MemberFunction.hpp"
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

namespace ffscriptUT
{	
	namespace CompileProgramUT
	{
		TEST(CompileProgramUT, CompileProgram1)
		{
			//prepare compiler object with some basic types and functions
			ScriptCompiler scriptCompiler;
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
						
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			//initialize global scope and its data
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContex,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = 
				L"int x;"
				L"x = 1;"				
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile a simple code failed!";
		}

		TEST(CompileProgramUT, CompileProgram1_failed)
		{
			//prepare compiler object with some basic types and functions
			ScriptCompiler scriptCompiler;
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			//initialize global scope and its data
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContex,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int x;"
				L"int x = 1;"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res == nullptr) << L"compile a simple code must failed due to declare a variable twice";
		}

		TEST(CompileProgramUT, CompileProgram2)
		{
			//prepare compiler object with some basic types and functions
			ScriptCompiler scriptCompiler;
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			//initialize global scope and its data
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContex,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int x;"
				L"x = 1;"
				L"x = x + 1;"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr);
		}

		TEST(CompileProgramUT, CompileProgram3)
		{
			//prepare compiler object with some basic types and functions
			ScriptCompiler scriptCompiler;
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			//initialize global scope and its data
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContex,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int x;"
				L"x = 1 * 2.5;"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr);
		}

		TEST(CompileProgramUT, CompileProgram4)
		{
			//prepare compiler object with some basic types and functions
			ScriptCompiler scriptCompiler;
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			//initialize global scope and its data
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContex,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int x;"
				L"x = 1 * 2.5;"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			staticContex.run();

			Variable* pX = rootScope.findVariable("x");
			int* x = getVaribleRef<int>(*pX);

			std::string message("[x] =");
			message.append(std::to_string((size_t)x));
			PRINT_TEST_MESSAGE(message.c_str());

			message = "[data] =";
			message.append(std::to_string((size_t)&globalData[0]));
			PRINT_TEST_MESSAGE(message.c_str());

			EXPECT_TRUE(*x == 2) << L"the code is run but return incorrect result";
		}

		TEST(CompileProgramUT, CompileProgram5)
		{
			//prepare compiler object with some basic types and functions
			ScriptCompiler scriptCompiler;
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			//initialize global scope and its data
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContex,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int x;"
				L"x = 1 * (2.5 + 1.5);"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			staticContex.run();

			Variable* pX = rootScope.findVariable("x");
			int* x = getVaribleRef<int>(*pX);

			std::string message("[x] =");
			message.append(std::to_string((size_t)x));
			PRINT_TEST_MESSAGE(message.c_str());

			message = "[data] =";
			message.append(std::to_string((size_t)&globalData[0]));
			PRINT_TEST_MESSAGE(message.c_str());

			EXPECT_TRUE(*x == 4) << L"the code is run but return incorrect result";
		}

		TEST(CompileProgramUT, CompileProgram6)
		{
			//prepare compiler object with some basic types and functions
			ScriptCompiler scriptCompiler;
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			//initialize global scope and its data
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContex,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int fibonaci(int n) {"
				L"	if(n < 2) {"
				L"		return n;"
				L"	}"
				L"	int res = fibonaci(n - 1) + fibonaci(n - 2);"
				L"	return res;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";
		}

		int fibonaci(int n) {
			if(n < 2) {
				return n;
			}
			int res = fibonaci(n - 1) + fibonaci(n - 2);
			return res;
		}

		TEST(CompileProgramUT, CompileProgram7)
		{
			//prepare compiler object with some basic types and functions
			ScriptCompiler scriptCompiler;
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			//initialize global scope and its data
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContex,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int fibonaci(int n) {"
				L"	if(n < 2) {"
				L"		return n;"
				L"	}"
				L"	int res = fibonaci(n - 1) + fibonaci(n - 2);"
				L"	return res;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("fibonaci");
			EXPECT_TRUE(overLoadingFuncItems->size() > 0) << L"cannot find function 'fibonaci'";

			ScriptTask scriptTask(&theProgram);
			int n = 10;
			ScriptParamBuffer paramBuffer(n);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();

			int cPlusPlusRes = fibonaci(n);

			EXPECT_TRUE(*funcRes == cPlusPlusRes) << L"program can run but return wrong value";
		}

		TEST(CompileProgramUT, CompileProgram8)
		{
			//prepare compiler object with some basic types and functions
			ScriptCompiler scriptCompiler;
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			//initialize global scope and its data
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContex,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"void foo() {"
				L"	a = {1,2,3};"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			EXPECT_TRUE(overLoadingFuncItems->size() > 0) << L"cannot find function 'foo'";

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, nullptr);
		}

		///
		/// Call a function script in global code
		///
		TEST(CompileProgramUT, CompileProgram9)
		{
			//prepare compiler object with some basic types and functions
			ScriptCompiler scriptCompiler;
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			//initialize global scope and its data
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContex, &scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int test() {"
				L"	return 1;"
				L"}"
				L"int a = test();"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			auto pVariable = rootScope.findVariable("a");
			EXPECT_NE(nullptr, pVariable) << L"cannot find variable 'a'";

			//EXPECT_TRUE(false, L"Current the engine cannot call a script function in global code");
			//staticContex.run();

			rootScope.runGlobalCode();
			rootScope.cleanupGlobalMemory();

			int* iRes = (int*)staticContex.getAbsoluteAddress(pVariable->getOffset());
			EXPECT_EQ(1, *iRes) << L"program does not run properly";
		}
	}
}