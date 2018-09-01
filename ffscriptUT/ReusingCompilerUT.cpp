/******************************************************************
* File:        ReusingCompilerUT.cpp
* Description: Test cases for checking reusing comipiler object to
*              compile many programs in C Lambda scripting language
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

namespace ffscriptUT
{	
	TEST_CLASS(ReusingCompilerUT)
	{
	public:
		static int fibonaci(int n) {
			if(n < 2) {
				return n;
			}
			int res = fibonaci(n - 1) + fibonaci(n - 2);
			return res;
		}

		TEST_METHOD(CompileFibonaciTwice1)
		{
			ScriptCompiler scriptCompiler;
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			scriptCompiler.beginUserLib();

			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 10;
			int cPlusPlusRes = fibonaci(n);

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

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);			
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == cPlusPlusRes, L"program can run but return wrong value");			
			PRINT_TEST_MESSAGE(("fibonaci =" + std::to_string(*funcRes)).c_str());

			scriptCompiler.clearUserLib();
			rootScope.clear();

			Program theProgram2;
			scriptCompiler.bindProgram(&theProgram2);
			
			res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram2);
			Assert::IsTrue(blRes, L"extract code failed");

			overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("fibonaci");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'fibonaci'");

			ScriptTask scriptTask2(&theProgram2);
			scriptTask2.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			funcRes = (int*)scriptTask2.getTaskResult();
			Assert::IsTrue(*funcRes == cPlusPlusRes, L"program can run but return wrong value");
			PRINT_TEST_MESSAGE(("fibonaci(2nd) =" + std::to_string(*funcRes)).c_str());
		}

		TEST_METHOD(CompileFibonaciTwice2)
		{
			ScriptCompiler scriptCompiler;
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			scriptCompiler.beginUserLib();

			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			int n = 10;
			int cPlusPlusRes = fibonaci(n);

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

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);			
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == cPlusPlusRes, L"program can run but return wrong value");
			PRINT_TEST_MESSAGE(("fibonaci =" + std::to_string(*funcRes)).c_str());
			
			//reuse compiler at second time
			scriptCompiler.clearUserLib();
			rootScope.clear();

			Program theProgram2;
			scriptCompiler.bindProgram(&theProgram2);

			const wchar_t* fibonaciFake =
				L"int fibonaci(int n) {"
				L"	return n;"
				L"}"
				;

			res = rootScope.parse(fibonaciFake, fibonaciFake + wcslen(fibonaciFake));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram2);
			Assert::IsTrue(blRes, L"extract code failed");

			overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("fibonaci");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'fibonaci'");

			n = 200;
			ScriptParamBuffer paramBuffer2(n);
			ScriptTask scriptTask2(&theProgram2);
			scriptTask2.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer2);
			funcRes = (int*)scriptTask2.getTaskResult();
			Assert::IsTrue(*funcRes == n, L"program can run but return wrong value");
			PRINT_TEST_MESSAGE(("fibonaci(fake) =" + std::to_string(*funcRes)).c_str());
		}
	};
}