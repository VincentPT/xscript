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
	TEST_CLASS(CompileProgramUT2)
	{
	public:
		ScriptCompiler scriptCompiler;
		FunctionRegisterHelper funcLibHelper;
		const BasicTypes* basicType;
		
		CompileProgramUT2() : funcLibHelper(&scriptCompiler) {
			auto typeManager = scriptCompiler.getTypeManager();
			basicType = &(typeManager->getBasicTypes());
			typeManager->registerBasicTypes(&scriptCompiler);
			typeManager->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);

			funcLibHelper.registFunction("printFibonaci", "int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new CdeclFunction2<void, int>(printFibonaci),&scriptCompiler));

			importBasicfunction(funcLibHelper);
		}
		
		static int fibonaci(int n) {
			if(n < 2) {
				return n;
			}
			int res = fibonaci(n - 1) + fibonaci(n - 2);
			return res;
		}

		static void printFibonaci(int n) {
			Logger::WriteMessage(("run fibonaci with n =" + std::to_string(n)).c_str());
		}

		TEST_METHOD(CompileFibonaci)
		{
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
				//L"	printFibonaci(n);"
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
		}

		TEST_METHOD(CompileSubScope1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			int n = 50;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	{"
				L"		n = n - 1;"
				L"	}"
				L"	return n;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::AreEqual(n-1, *funcRes, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileSubScope2)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			int n = 50;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	int m = n;"
				L"	{"
				L"		n = m - 1;"
				L"	}"
				L"	return n;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");
			
			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::AreEqual(n - 1, *funcRes, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileSubScope3)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			int n = 50;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	int m = n;"
				L"	{"
				L"	    if(n > 0) {"
				L"		    n = m - 1;"
				L"	    }"
				L"	}"
				L"	return n;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::AreEqual(n - 1, *funcRes, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileLoop)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 50;
	
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	while(n > 0) {"
				L"		n = n - 1;"
				L"	}"
				L"	return n;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 0, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileLoop2)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 50;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	while(n > 0) {"
				L"		n = n - 1;"
				L"	}"
				L"	while(n < 0) {"
				L"		n = n + 1;"
				L"	}"
				L"	return n;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 0, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileLoop3)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = -55;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	while(n > 0) {"
				L"		n = n - 1;"
				L"	}"
				L"	while(n < 0) {"
				L"		n = n + 1;"
				L"	}"
				L"	return n;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 0, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileIfElse)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 2;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	if(n > 0) {"
				L"		return -1;"
				L"	}"
				L"	else {"
				L"		return 0;"
				L"	}"
				"	return 1;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == -1, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileIfElse1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 0;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	if(n > 0) {"
				L"		return -1;"
				L"	}"
				L"	else {"
				L"		return 0;"
				L"	}"
				"	return 1;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 0, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileIfElse2)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 0;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	if(n > 0) {"
				L"		return 1;"
				L"	}"
				L"	else if(n == 0){"
				L"		return 0;"
				L"	}"
				"	return -1;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 0, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileIfElse3)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = -2;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	if(n > 0) {"
				L"		return 1;"
				L"	}"
				L"	else if(n == 0){"
				L"		return 0;"
				L"	}"
				"	return -1;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == -1, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileIfElse4)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = -2;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	if(n > 0) {"
				L"		return 1;"
				L"	}"
				L"	else if(n == 0){"
				L"		return 0;"
				L"	}"
				L"	else if(n == -2){"
				L"		return -2;"
				L"	}"
				"	return -1;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == -2, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileIfElse5)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = -3;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	if(n > 0) {"
				L"		return 1;"
				L"	}"
				L"	else if(n == 0){"
				L"		return 0;"
				L"	}"
				L"	else if(n == -2){"
				L"		return -2;"
				L"	}"
				"	return -1;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == -1, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileIfElse10)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			int n = 1;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	if(n > 0) {"
				L"		int a = 0;"
				L"	}"				
				"	return 1;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 1, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileLoopAndIf1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 1;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	int res = 0;"
				L"	while(n > 0) {"
				L"		if( n == 5 ) {"
				L"			res = 1;"
				L"		}"
				L"		n = n - 1;"
				L"	}"
				L"	return res;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 0, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileLoopAndIf2)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 6;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	int res = 0;"
				L"	while(n > 0) {"
				L"		if( n == 5 ) {"
				L"			res = 1;"
				L"		}"
				L"		n = n - 1;"
				L"	}"
				L"	return res;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 1, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileLoopAndIfElse1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 6;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	int res = 0;"
				L"	while(n > 0) {"
				L"		if( n == 5 ) {"
				L"			res = 2;"
				L"		}"
				L"		else if(n == 4) {"
				L"			res = 1;"
				L"		}"
				L"		n = n - 1;"
				L"	}"
				L"	return res;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 1, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileLoopAndIfElse2)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 4;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	int res = 0;"
				L"	while(n > 0) {"
				L"		if( n == 5 ) {"
				L"			res = 2;"
				L"		}"
				L"		else if(n == 4) {"
				L"			res = 1;"
				L"		}"
				L"		n = n - 1;"
				L"	}"
				L"	return res;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 1, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileLoopAndIfElse3)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 4;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"				
				L"	if( n > 0 ) {"
				L"		while(n > 0) {"
				L"			n = n - 1;"
				L"		}"
				L"	}"
				L"	else if(n < 0) {"
				L"		while(n < 0) {"
				L"			n = n + 1;"
				L"		}"
				L"	}"				
				L"	return n;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 0, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileLoopAndIfElse4)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = -40;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	if( n > 0 ) {"
				L"		while(n > 0) {"
				L"			n = n - 1;"
				L"		}"
				L"	}"
				L"	else if(n < 0) {"
				L"		while(n < 0) {"
				L"			n = n + 1;"
				L"		}"
				L"	}"
				L"	return n;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 0, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileIfElse6)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 2;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	int res;"
				L"	if(n > 0) {"
				L"		res = 1;"
				L"	}"
				L"	else {"
				L"		res = 0;"
				L"	}"
				"	return res;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 1, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileIfElse7)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = -1;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	int res = -1;"
				L"	if(n > 0) {"
				L"		res = 1;"
				L"	}"
				L"	else {"
				L"		res = 0;"
				L"	}"
				"	return res;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 0, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileIfElse8)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = -3;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"double foo(int n) {"
				L"	double res;"
				L"	if(n > 0) {"
				L"		res = 2.5 * n;"
				L"	}"
				L"	else {"
				L"		res = -2.5 * n;"
				L"	}"
				"	return res;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			double* funcRes = (double*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 7.5, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileIfElse9)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 3;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"double foo(int n) {"
				L"	double res;"
				L"	if(n > 0) {"
				L"		res = 2.5 * n;"
				L"	}"
				L"	else {"
				L"		res = -2.5 * n;"
				L"	}"
				"	return res;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			double* funcRes = (double*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 7.5, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileFunctionHasRefParam1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = -1;
			
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"void foo(ref int n) {"
				L"	*n = 0;"				
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(&n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			scriptTask.getTaskResult();			
			Assert::IsTrue(n == 0, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileFunctionHasRefParam2)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = -1;			

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"void foo(ref int n) {"
				L"	*n = 9;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(&n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			scriptTask.getTaskResult();
			Assert::IsTrue(n == 9, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileFunctionBool1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 0;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"bool foo(int n) {"
				L"	return n == 0;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();
			Assert::IsTrue(*funcRes == true, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileFunctionBool2)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 1;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"bool foo(int n) {"
				L"	return n == 0;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();
			Assert::IsTrue(*funcRes == false, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileLooAndBreak1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 6;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	while(n > 0) {"
				L"		if( n == 5 ) {"
				L"			break;"
				L"		}"
				L"		n = n - 1;"
				L"	}"
				L"	return n;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 5, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileLooAndBreak2)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 100;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	int res = 0;"
				L"	int m;"
				L"	while(n > 0) {"
				L"		m = n;"
				L"		while(m > 0) {"
				L"			if( m == 2) {"
				L"				break;"
				L"			}"
				L"			m = m - 1;"
				L"		}"
				L"		if(m == 2) {"
				L"			res = res + 1;"
				L"		}"
				L"		n = n - 1;"
				L"	}"
				L"	return res;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == (n - 2 + 1), L"program can run but return wrong value");
		}

		TEST_METHOD(CompileLooAndBreak3)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 100;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	while(n > 0) {"
				L"		break;"
				L"		n = n - 1;"
				L"	}"
				L"	return n;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == n, L"program can run but return wrong value");
		}

		int fibonacci(int n) {
				if(n < 2) {
					return n;
				}
				int d = 0;
				int e = 1;
				int fn;
				int i = 1;
				while(i < n) {
					fn = e + d;
					d = e;
					e = fn;
					i = i + 1;
				}
				return e;
			}

		TEST_METHOD(CompileFibonacciLoop)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 50;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int fibonacci(int n) {"
				L"	if(n < 2) {"
				L"		return n;"
				L"	}"
				L"	int d = 0;"
				L"	int e = 1;"
				L"	int fn;"
				L"	int i = 1;"
				L"	while(i < n) {"
				L"		fn = e + d;"
				L"		d = e;"
				L"		e = fn;"
				L"		i = i + 1;"
				L"	}"
				L"	return e;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("fibonacci");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'fibonacci'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("fibonacci =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == fibonacci(n), L"program can run but return wrong value");
		}

		TEST_METHOD(CompileLoopContinue)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 5000;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	int i = 0;"
				L"	while(n > 0) {"
				L"		if(n == 2) {"
				L"			n = n - 2;"
				L"			continue;"
				L"		}"
				L"		i = i + 1;"
				L"		n = n - 1;"
				L"	}"
				L"	return i;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("foo");
			Assert::IsTrue(overLoadingFuncItems->size() > 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == (n - 2), L"program can run but return wrong value");
		}

		TEST_METHOD(CompileLoopContinue2)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 5000;

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int n) {"
				L"	int i = 0;"
				L"	while(n > 0) {"
				L"		i = i + 1;"
				L"		n = n - 1;"
				L"		continue;"
				L"		n = n - 2;"
				L"	}"
				L"	return i;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("foo", "int");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == n, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileFunctionTwoParams)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo2(bool a, int n) {"				
				L"	return a ? n : -n;"
				L"}"
				L"int foo() {"				
				L"	return foo2(1, 2);"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::AreEqual(2, *funcRes, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileFunctionOneParams)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(bool a) {"
				L"	return a ? 0 : 1;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("foo", "bool");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, true);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::AreEqual(0, *funcRes, L"program can run but return wrong value");
		}
	};
}