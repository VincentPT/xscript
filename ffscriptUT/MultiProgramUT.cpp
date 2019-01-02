/******************************************************************
* File:        MultiProgramUT.cpp
* Description: Test cases for using many program instance
*              at the same time of C Lambda scripting language.
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
	namespace MultiProgramUT
	{
		int fibonaci(int n) {
			if (n < 2) {
				return n;
			}
			int res = fibonaci(n - 1) + fibonaci(n - 2);
			return res;
		}

		FF_TEST_FUNCTION(MultiProgram, MultiInstanceInSameThread)
		{
			//prepare compiler object with some basic types and functions
			ScriptCompiler scriptCompiler;
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			//initialize global scope and its data
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContex, &scriptCompiler);

			//initialize first instance
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
			FF_EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			FF_EXPECT_TRUE(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("fibonaci");
			FF_EXPECT_TRUE(overLoadingFuncItems->size() > 0, L"cannot find function 'fibonaci'");

			ScriptTask scriptTask(&theProgram);
			int n = 10;
			ScriptParamBuffer paramBuffer(n);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();

			int cPlusPlusRes = fibonaci(n);

			FF_EXPECT_TRUE(*funcRes == cPlusPlusRes, L"program can run but return wrong value");

			//second instance
			ScriptCompiler scriptCompiler2;
			FunctionRegisterHelper funclib2(&scriptCompiler2);
			scriptCompiler2.getTypeManager()->registerBasicTypes(&scriptCompiler2);
			scriptCompiler2.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler2, funclib2);
			importBasicfunction(funclib2);

			GlobalScope rootScope2(&staticContex, &scriptCompiler2);

			//initialize an instance of script program
			Program theProgram2;
			scriptCompiler2.bindProgram(&theProgram2);

			res = rootScope2.parse(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_TRUE(res != nullptr, L"compile program failed");

			blRes = rootScope2.extractCode(&theProgram2);
			FF_EXPECT_TRUE(blRes, L"extract code failed");

			overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("fibonaci");
			FF_EXPECT_TRUE(overLoadingFuncItems->size() > 0, L"cannot find function 'fibonaci'");

			ScriptTask scriptTask2(&theProgram2);
			n = 15;
			ScriptParamBuffer paramBuffer2(n);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer2);
			funcRes = (int*)scriptTask.getTaskResult();

			cPlusPlusRes = fibonaci(n);

			FF_EXPECT_TRUE(*funcRes == cPlusPlusRes, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(MultiProgram, MultiInstanceInOtherThreads)
		{
			//prepare compiler object with some basic types and functions
			ScriptCompiler scriptCompiler;
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			//initialize global scope and its data
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContex, &scriptCompiler);

			//initialize first instance
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
			FF_EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			FF_EXPECT_TRUE(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("fibonaci");
			FF_EXPECT_TRUE(overLoadingFuncItems->size() > 0, L"cannot find function 'fibonaci'");

			//second instance
			ScriptCompiler scriptCompiler2;
			FunctionRegisterHelper funclib2(&scriptCompiler2);
			scriptCompiler2.getTypeManager()->registerBasicTypes(&scriptCompiler2);
			scriptCompiler2.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler2, funclib2);
			importBasicfunction(funclib2);

			GlobalScope rootScope2(&staticContex, &scriptCompiler2);

			//initialize an instance of script program
			Program theProgram2;
			scriptCompiler2.bindProgram(&theProgram2);

			res = rootScope2.parse(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_TRUE(res != nullptr, L"compile program failed");

			blRes = rootScope2.extractCode(&theProgram2);
			FF_EXPECT_TRUE(blRes, L"extract code failed");

			overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("fibonaci");
			FF_EXPECT_TRUE(overLoadingFuncItems->size() > 0, L"cannot find function 'fibonaci'");

			int n1 = 10, n2 = 15;
			ScriptParamBuffer paramBuffer1(n1);
			ScriptParamBuffer paramBuffer2(n2);
			ScriptTask scriptTask1(&theProgram);
			ScriptTask scriptTask2(&theProgram2);

			//run function of program 1 in thread 1
			thread thread1([&]() {
				scriptTask1.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer1);
			});
			//run function of program 2 in main thread
			scriptTask2.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer2);
			
			//wait thread 1 completed
			thread1.join();

			int* funcRes = (int*)scriptTask1.getTaskResult();
			int cPlusPlusRes = fibonaci(n1);

			FF_EXPECT_TRUE(*funcRes == cPlusPlusRes, L"program 1 can run but return wrong value");

			funcRes = (int*)scriptTask2.getTaskResult();
			cPlusPlusRes = fibonaci(n2);

			FF_EXPECT_TRUE(*funcRes == cPlusPlusRes, L"program 2 can run but return wrong value");
		}
	};
}