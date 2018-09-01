/******************************************************************
* File:        CompileProgramUT.cpp
* Description: Test cases for compiling, running C Lambda programs
*              read from memory.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
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

namespace ffscriptUT
{	
	TEST_CLASS(CompileProgramUT)
	{
	public:

		CompileProgramUT() {

		}

		TEST_METHOD(CompileProgram1)
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
			Assert::IsTrue(res != nullptr, L"compile a simple code failed!");
		}

		TEST_METHOD(CompileProgram1_failed)
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
			Assert::IsTrue(res == nullptr, L"compile a simple code must failed due to declare a variable twice");
		}

		TEST_METHOD(CompileProgram2)
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
			Assert::IsTrue(res != nullptr);
		}

		TEST_METHOD(CompileProgram3)
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
			Assert::IsTrue(res != nullptr);
		}

		TEST_METHOD(CompileProgram4)
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
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			staticContex.run();

			Variable* pX = rootScope.findVariable("x");
			int* x = getVaribleRef<int>(*pX);

			std::string message("[x] =");
			message.append(std::to_string((size_t)x));
			PRINT_TEST_MESSAGE(message.c_str());

			message = "[data] =";
			message.append(std::to_string((size_t)&globalData[0]));
			PRINT_TEST_MESSAGE(message.c_str());

			Assert::IsTrue(*x == 2, L"the code is run but return incorrect result");
		}

		TEST_METHOD(CompileProgram5)
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
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			staticContex.run();

			Variable* pX = rootScope.findVariable("x");
			int* x = getVaribleRef<int>(*pX);

			std::string message("[x] =");
			message.append(std::to_string((size_t)x));
			PRINT_TEST_MESSAGE(message.c_str());

			message = "[data] =";
			message.append(std::to_string((size_t)&globalData[0]));
			PRINT_TEST_MESSAGE(message.c_str());

			Assert::IsTrue(*x == 4, L"the code is run but return incorrect result");
		}

		TEST_METHOD(CompileProgram6)
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
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");
		}

		int fibonaci(int n) {
			if(n < 2) {
				return n;
			}
			int res = fibonaci(n - 1) + fibonaci(n - 2);
			return res;
		}

		TEST_METHOD(CompileProgram7)
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
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("fibonaci");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'fibonaci'");

			ScriptTask scriptTask(&theProgram);
			int n = 10;
			ScriptParamBuffer paramBuffer(n);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();

			int cPlusPlusRes = fibonaci(n);

			Assert::IsTrue(*funcRes == cPlusPlusRes, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileProgram8)
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
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, nullptr);
		}

		///
		/// Call a function script in global code
		///
		TEST_METHOD(CompileProgram9)
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
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			auto pVariable = rootScope.findVariable("a");
			Assert::IsNotNull(pVariable, L"cannot find variable 'a'");

			//Assert::IsTrue(false, L"Current the engine cannot call a script function in global code");
			//staticContex.run();

			rootScope.runGlobalCode();
			rootScope.cleanupGlobalMemory();

			int* iRes = (int*)staticContex.getAbsoluteAddress(pVariable->getOffset());
			Assert::AreEqual(1, *iRes, L"program does not run properly");
		}
	};
}