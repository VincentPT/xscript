#include "stdafx.h"
#include "CppUnitTest.h"
#include "TemplateForTest.hpp"
#include <functional>
#include "FunctionRegisterHelper.h"
#include "BasicFunction.h"
#include "BasicType.h"
#include "StaticContext.h"
#include "Utility.h"
#include <memory>
#include <thread>
#include "GlobalScope.h"
#include "Utils.h"
#include "Variable.h"
#include "DynamicFunctionFactory.h"
#include "function\CdeclFunction2.hpp"
#include "function\MemberFunction2.hpp"

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
#include "ScriptTask.h"
#include <condition_variable>
#include <mutex>


namespace ffscriptUT
{		
	TEST_CLASS(MultithreadUT)
	{
	private:
		ScriptCompiler scriptCompiler;
		FunctionRegisterHelper funcLibHelper;
		const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
		
	private:
		static int fibonaci(int n) {
			if (n < 2) {
				return n;
			}
			int res = fibonaci(n - 1) + fibonaci(n - 2);
			return res;
		}

		void printResult(SimpleVariantArray* results) {
			string message = "{";
			for (int i = 0; i < results->size; i++) {
				if (results->elems[i].scriptType == basicType.TYPE_INT) {
					message += std::to_string( *((int*)results->elems[i].pData));
				}
				else if (results->elems[i].scriptType == basicType.TYPE_FLOAT) {
					message += std::to_string(*((float*)results->elems[i].pData));
				}
				else if (results->elems[i].scriptType == basicType.TYPE_DOUBLE) {
					message += std::to_string(*((double*)results->elems[i].pData));
				}
				else if (results->elems[i].scriptType == basicType.TYPE_BOOL) {
					message += std::to_string(*((bool*)results->elems[i].pData));
				}
				else if (results->elems[i].scriptType == basicType.TYPE_STRING) {
					message += (char*)results->elems[i].pData;
				}
				else
				{
					message += "[";
					message += std::to_string( (size_t)results->elems[i].pData);
					message += "]";
				}
				message += ",";
			}
			message.erase (message.size() - 1);
			message += "}";

			PRINT_TEST_MESSAGE("function result:");
			PRINT_TEST_MESSAGE(message.c_str());
		}

		mutex _m;
		condition_variable _cv;

		void notifySignal(mutex& m) {
			unique_lock<mutex> lk(m);
			_cv.notify_one();
		}

		void waitSignaled(mutex& m) {
			std::unique_lock<std::mutex> lk(m);
			_cv.wait(lk);
		}

	public:
		MultithreadUT() : funcLibHelper(&scriptCompiler) {
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);

			funcLibHelper.registFunction("fibonaciNative", "int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", new CdeclFunction2<int, int>(fibonaci), &scriptCompiler));
			funcLibHelper.registFunction("signal", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new MFunction2<void, MultithreadUT, mutex&>(this, &MultithreadUT::notifySignal) , &scriptCompiler));
			funcLibHelper.registFunction("waitSignaled", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new MFunction2<void, MultithreadUT, mutex&>(this, &MultithreadUT::waitSignaled), &scriptCompiler));

			importBasicfunction(funcLibHelper);

			//register dynamic functions
			auto theNativeFunction1 = new MFunction2 <void, MultithreadUT, SimpleVariantArray*>(this, &MultithreadUT::printResult);
			auto dynamicFunctionFactory1 = new DynamicFunctionFactory("int", theNativeFunction1, funcLibHelper.getSriptCompiler());
			funcLibHelper.getSriptCompiler()->registDynamicFunction("printResult", dynamicFunctionFactory1);
			funcLibHelper.addFactory(dynamicFunctionFactory1);
		}

		TEST_METHOD(MultithreadIndependent1)
		{
			wstring exp = L"fibonaciNative(20)";
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());
			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			//Assert::AreEqual(product(4, 5) * 6 + 6, *result, (L"result of expression '" + exp + L"' should be 126").c_str());

			int impossibleValue = -1;;

			int result1 = impossibleValue;
			int result2 = impossibleValue;

			thread thread1([&]() {
				Context localContext(1024 * 1024);
				excutor->runCode();
				result1 = *((int*)excutor->getReturnData());
			});

			thread thread2([&]() {
				Context localContext(1024 * 1024);
				excutor->runCode();
				result2 = *((int*)excutor->getReturnData());
			});

			thread1.join();
			thread2.join();

			Assert::AreEqual(result1, result2, L"Run fibonaciNative(20) on two thread returned different values, it's not acceptable");
			Assert::AreEqual(fibonaci(20), result2, L"fibonaciNative(20) return wrong value");
		}

		TEST_METHOD(MultithreadIndependent2)
		{
			int n = 30;
			wstring exp = L"fibonaciNative(" + std::to_wstring(n) + L")";
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());
			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			//Assert::AreEqual(product(4, 5) * 6 + 6, *result, (L"result of expression '" + exp + L"' should be 126").c_str());

			int impossibleValue = -1;;

			int result1 = impossibleValue;
			int result2 = impossibleValue;

			thread thread1([&]() {
				Context localContext(1024 * 1024);
				excutor->runCode();
				result1 = *((int*)excutor->getReturnData());
			});

			thread thread2([&]() {
				Context localContext(1024 * 1024);
				excutor->runCode();
				result2 = *((int*)excutor->getReturnData());
			});

			thread1.join();
			thread2.join();

			Assert::AreEqual(result1, result2, (L"Run " + exp + L"(20) on two thread returned different values, it's not acceptable").c_str());
			Assert::AreEqual(fibonaci(n), result2, (exp + L" returned wrong value").c_str());
		}

		TEST_METHOD(MultithreadIndependent3)
		{
			int n = 30;
			wstring exp = L"fibonaciNative(" + std::to_wstring(n) + L")";
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());
			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			//Assert::AreEqual(product(4, 5) * 6 + 6, *result, (L"result of expression '" + exp + L"' should be 126").c_str());

			int impossibleValue = -1;;

			int result1 = impossibleValue;
			int result2 = impossibleValue;
			int result3 = impossibleValue;
			int result4 = impossibleValue;

			thread thread1([&]() {
				Context localContext(1024 * 1024);
				excutor->runCode();
				result1 = *((int*)excutor->getReturnData());
			});

			thread thread2([&]() {
				Context localContext(1024 * 1024);
				excutor->runCode();
				result2 = *((int*)excutor->getReturnData());
			});

			thread thread3([&]() {
				Context localContext(1024 * 1024);
				excutor->runCode();
				result3 = *((int*)excutor->getReturnData());
			});

			thread thread4([&]() {
				Context localContext(1024 * 1024);
				excutor->runCode();
				result4 = *((int*)excutor->getReturnData());
			});

			thread1.join();
			thread2.join();
			thread3.join();
			thread4.join();

			Assert::AreEqual(result1, result2, (L"Run " + exp + L" on four threads returned different values, it's not acceptable").c_str());
			Assert::AreEqual(result3, result4, (L"Run " + exp + L" on four threads returned different values, it's not acceptable").c_str());
			Assert::AreEqual(result1, result4, (L"Run " + exp + L" on four threads returned different values, it's not acceptable").c_str());
			Assert::AreEqual(fibonaci(n), result2, (exp + L" return wrong value").c_str());
		}

		TEST_METHOD(MultithreadIndependentScriptFunc1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 20;
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

			int fibonaciFunctionid = overLoadingFuncItems->front().functionId;

			int impossibleValue = -1;;

			int result1 = impossibleValue;
			int result2 = impossibleValue;
			int result3 = impossibleValue;
			int result4 = impossibleValue;

			ScriptTask scriptTask1(&theProgram);
			ScriptTask scriptTask2(&theProgram);
			ScriptParamBuffer paramBuffer(n);

			thread thread1([&]() {
				scriptTask1.runFunction(fibonaciFunctionid, &paramBuffer);
				int* funcRes = (int*)scriptTask1.getTaskResult();
				if (funcRes) {
					result1 = *funcRes;
				}
			});

			thread thread2([&]() {
				scriptTask2.runFunction(fibonaciFunctionid, &paramBuffer);
				int* funcRes = (int*)scriptTask2.getTaskResult();
				if (funcRes) {
					result2 = *funcRes;
				}
			});

			thread1.join();
			thread2.join();

			Assert::IsTrue(result1 == cPlusPlusRes, L"thread 1 returned wrong value");
			Assert::IsTrue(result2 == cPlusPlusRes, L"thread 2 returned wrong value");
		}

		TEST_METHOD(MultithreadIndependentScriptFunc2)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 20;
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

			int fibonaciFunctionid = overLoadingFuncItems->front().functionId;

			int impossibleValue = -1;;

			int result1 = impossibleValue;
			int result2 = impossibleValue;
			int result3 = impossibleValue;
			int result4 = impossibleValue;

			ScriptTask scriptTask1(&theProgram);
			ScriptTask scriptTask2(&theProgram);
			ScriptTask scriptTask3(&theProgram);
			ScriptTask scriptTask4(&theProgram);

			ScriptParamBuffer paramBuffer(n);

			thread thread1([&]() {
				scriptTask1.runFunction(fibonaciFunctionid, &paramBuffer);
				int* funcRes = (int*)scriptTask1.getTaskResult();
				if (funcRes) {
					result1 = *funcRes;
				}
			});

			thread thread2([&]() {
				scriptTask2.runFunction(fibonaciFunctionid, &paramBuffer);
				int* funcRes = (int*)scriptTask2.getTaskResult();
				if (funcRes) {
					result2 = *funcRes;
				}
			});

			thread thread3([&]() {
				scriptTask3.runFunction(fibonaciFunctionid, &paramBuffer);
				int* funcRes = (int*)scriptTask3.getTaskResult();
				if (funcRes) {
					result3 = *funcRes;
				}
			});

			thread thread4([&]() {
				scriptTask4.runFunction(fibonaciFunctionid, &paramBuffer);
				int* funcRes = (int*)scriptTask4.getTaskResult();
				if (funcRes) {
					result4 = *funcRes;
				}
			});

			thread1.join();
			thread2.join();
			thread3.join();
			thread4.join();

			Assert::IsTrue(result1 == cPlusPlusRes, L"thread 1 returned wrong value");
			Assert::IsTrue(result2 == cPlusPlusRes, L"thread 2 returned wrong value");
			Assert::IsTrue(result3 == cPlusPlusRes, L"thread 3 returned wrong value");
			Assert::IsTrue(result4 == cPlusPlusRes, L"thread 4 returned wrong value");
		}

		TEST_METHOD(MultithreadIndependentScriptFunc3)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n1 = 25;
			int n2 = 20;
			int n3 = 15;
			int n4 = 10;

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

			int fibonaciFunctionid = overLoadingFuncItems->front().functionId;

			int impossibleValue = -1;;

			int result1 = impossibleValue;
			int result2 = impossibleValue;
			int result3 = impossibleValue;
			int result4 = impossibleValue;

			ScriptTask scriptTask1(&theProgram);
			ScriptTask scriptTask2(&theProgram);
			ScriptTask scriptTask3(&theProgram);
			ScriptTask scriptTask4(&theProgram);

			ScriptParamBuffer paramBuffer1(n1);
			ScriptParamBuffer paramBuffer2(n2);
			ScriptParamBuffer paramBuffer3(n3);
			ScriptParamBuffer paramBuffer4(n4);

			thread thread1([&]() {
				scriptTask1.runFunction(fibonaciFunctionid, &paramBuffer1);
				int* funcRes = (int*)scriptTask1.getTaskResult();
				if (funcRes) {
					result1 = *funcRes;
				}
			});

			thread thread2([&]() {
				scriptTask2.runFunction(fibonaciFunctionid, &paramBuffer2);
				int* funcRes = (int*)scriptTask2.getTaskResult();
				if (funcRes) {
					result2 = *funcRes;
				}
			});

			thread thread3([&]() {
				scriptTask3.runFunction(fibonaciFunctionid, &paramBuffer3);
				int* funcRes = (int*)scriptTask3.getTaskResult();
				if (funcRes) {
					result3 = *funcRes;
				}
			});

			thread thread4([&]() {
				scriptTask4.runFunction(fibonaciFunctionid, &paramBuffer4);
				int* funcRes = (int*)scriptTask4.getTaskResult();
				if (funcRes) {
					result4 = *funcRes;
				}
			});

			thread1.join();
			thread2.join();
			thread3.join();
			thread4.join();

			Assert::IsTrue(result1 == fibonaci(n1), L"thread 1 returned wrong value");
			Assert::IsTrue(result2 == fibonaci(n2), L"thread 2 returned wrong value");
			Assert::IsTrue(result3 == fibonaci(n3), L"thread 3 returned wrong value");
			Assert::IsTrue(result4 == fibonaci(n4), L"thread 4 returned wrong value");
		}

		TEST_METHOD(MultithreadDependentScriptFunc1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 25;
			int cPlusPlusRes = fibonaci(n);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int finResult;"
				L"ref void mutex;"
				L"int fibonaci(int n) {"
				L"	if(n < 2) {"
				L"		return n;"
				L"	}"
				L"	int res = fibonaci(n - 1) + fibonaci(n - 2);"
				L"	return res;"
				L"}"

				L"void function1(int n) {"
				L"	finResult = fibonaci(n);"
				L"	signal(mutex);"
				L"}"

				L"void function2() {"
				L"	waitSignaled(mutex);"
				L"	printResult(finResult);"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			setVariableValue<void*>(*rootScope.findVariable("mutex"), (void*)&_m);

			const list<OverLoadingItem>* overLoadingFuncItems1 = scriptCompiler.findOverloadingFuncRoot("function1");
			Assert::IsTrue(overLoadingFuncItems1->size() > 0, L"cannot find function 'function1'");

			const list<OverLoadingItem>* overLoadingFuncItems2 = scriptCompiler.findOverloadingFuncRoot("function2");
			Assert::IsTrue(overLoadingFuncItems2->size() > 0, L"cannot find function 'function2'");

			int function1 = overLoadingFuncItems1->front().functionId;
			int function2 = overLoadingFuncItems2->front().functionId;
			
			ScriptParamBuffer paramBuffer(n);

			ScriptTask scriptTask1(&theProgram);
			ScriptTask scriptTask2(&theProgram);
			thread thread1([&]() {
				scriptTask1.runFunction(function1, &paramBuffer);
			});

			scriptTask2.runFunction(function2, nullptr);

			int result = *getVaribleRef<int>(*rootScope.findVariable("finResult"));
			
			thread1.join();

			Assert::IsTrue(result == cPlusPlusRes, L"program can run but return wrong value");
		}

		TEST_METHOD(MultithreadDependentScriptFunc2)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);
			int n = 25;
			int cPlusPlusRes = fibonaci(n);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int finResult;"
				L"ref void mutex;"
				L"int fibonaci(int n) {"
				L"	if(n < 2) {"
				L"		return n;"
				L"	}"
				L"	int res = fibonaci(n - 1) + fibonaci(n - 2);"
				L"	return res;"
				L"}"

				L"void function1(int n) {"
				L"	finResult = fibonaci(n);"
				L"	signal(mutex);"
				L"}"

				L"void function2() {"
				L"	waitSignaled(mutex);"
				L"	printResult(finResult);"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			setVariableValue<void*>(*rootScope.findVariable("mutex"), (void*)&_m);

			const list<OverLoadingItem>* overLoadingFuncItems1 = scriptCompiler.findOverloadingFuncRoot("function1");
			Assert::IsTrue(overLoadingFuncItems1->size() > 0, L"cannot find function 'function1'");

			const list<OverLoadingItem>* overLoadingFuncItems2 = scriptCompiler.findOverloadingFuncRoot("function2");
			Assert::IsTrue(overLoadingFuncItems2->size() > 0, L"cannot find function 'function2'");

			int function1 = overLoadingFuncItems1->front().functionId;
			int function2 = overLoadingFuncItems2->front().functionId;

			ScriptParamBuffer paramBuffer(n);

			ScriptTask scriptTask1(&theProgram);
			ScriptTask scriptTask2(&theProgram);
			thread thread1([&]() {
				scriptTask1.runFunction(function1, &paramBuffer);
			});

			int result = -1;
			thread thread2([&]() {
				scriptTask2.runFunction(function2, nullptr);
				result = *getVaribleRef<int>(*rootScope.findVariable("finResult"));
			});

			thread2.join();
			thread1.join();

			Assert::IsTrue(result == cPlusPlusRes, L"program can run but return wrong value");
		}
	};
}