/******************************************************************
* File:        LambdaExpressionUT.cpp
* Description: Test cases for checking lambda function of C Lambda
*              scripting language.
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
#include <thread>

using namespace std;
using namespace ffscript;

namespace ffscriptUT
{		
	namespace LambdaExpressionUT
	{
		FF_TEST_FUNCTION(LambdaExpression, CheckDetachPointer1)
		{
			shared_ptr<int> ptr(nullptr);
			{
				auto ptrTemp = make_shared<int>(1);
				ptrTemp.swap(ptr);
			}

			FF_EXPECT_EQ(1, *ptr);
		}

		//FF_TEST_FUNCTION(LambdaExpression, CheckDetachPointer2)
		//{
		//	shared_ptr<int> ptr(nullptr, [](int*) {});
		//	ptr = make_shared<int>(1);
		//	FF_EXPECT_EQ(1, *ptr);
		//	auto pVal = ptr.get();
		//	ptr.reset();
		//	delete pVal;
		//}

		// set smarty to point to nothing
		// returns old(smarty.get())
		// caller is responsible for the returned pointer (careful)
		template <typename T>
		T* release(shared_ptr<T>& smarty) {
			// sanity check:
			//assert(smarty.unique());
			// only one owner (please don't play games with weak_ptr in another thread)
			// would want to check the total count (shared+weak) here

			// save the pointer:
			T *raw = &*smarty;
			// at this point smarty owns raw, can't return it

			try {
				// an exception here would be quite unpleasant

				// now smash smarty:
				new (&smarty) shared_ptr<T>();
				// REALLY: don't do it!
				// the behaviour is not defined!
				// in practice: at least a memory leak!
			}
			catch (...) {
				// there is no shared_ptr<T> in smarty zombie now
				// can't fix it at this point:
				// the only fix would be to retry, and it would probably throw again
				// sorry, can't do anything
				abort();
			}
			// smarty is a fresh shared_ptr<T> that doesn't own raw

			// at this point, nobody owns raw, can return it
			return raw;
		}
/*
		template <typename T>
		T* detach(shared_ptr<T>& ptr) {
			auto sizePtr = sizeof(ptr);
			char* pptr = (char*)&ptr;
			_Ref_count_base* internalRef = *(_Ref_count_base**)(pptr + sizeof(void*));
			//increase ref one but no increase number of owner
			//so the pointer will not release after all owner are destroyed
			internalRef->_Incref();
			return ptr.get();
		}

		FF_TEST_FUNCTION(LambdaExpression, CheckDetachPointer3)
		{
			int* pVal;
			{
				int* p = new int(100);
				shared_ptr<int> ptr(p);
				pVal = detach(ptr);
			}

			delete pVal;
		}
*/
		FF_TEST_FUNCTION(LambdaExpression, CompileLambdaUT1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();
			
			const wchar_t* scriptCode =
				L"void foo() {"
				L"	f = [1](ref int a) { i = a[0]; };"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_EQ(nullptr, program, L"Compile program should be failed");
		}

		FF_TEST_FUNCTION(LambdaExpression, CompileLambdaUT2)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"void foo() {"
				L"	int n = 0;"
				L"	f = [n](ref int a) { a[0] = n; };"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");
		}

		FF_TEST_FUNCTION(LambdaExpression, CompileLambdaUT3)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	int n = 1;"
				L"	int a;"
				L"	f = [n](ref int a) { a[0] = n; };"
				L"	f(ref(a));"
				L"	return a;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 1, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(LambdaExpression, CompileLambdaUT4)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	int n = 2;"
				L"	int a = 1;"
				L"	f = [&n](ref int a) { n = a[0]; };"
				L"	f(ref(a));"
				L"	return n;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(1, *funcRes, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(LambdaExpression, CompileLambdaUT5)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	int n;"
				L"	int a = 1;"
				L"	function<void(ref int)> f = [&n](ref int a) { n = a[0]; };"
				L"	f(ref(a));"
				L"	return n;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(1, *funcRes, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(LambdaExpression, CompileLambdaUT6)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	int n = 1000;"
				L"	f = [n]() -> int {"
				L"	while(n > 0) {"
				L"   n--;"
				L"  }"
				L"	return n;"
				L"  };"
				L"  n = f();"
				L"	return n;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 0, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(LambdaExpression, CompileLambdaUT7)
		{
			CompilerSuite compiler;

			auto aaaa = []() -> int {return 0; };

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	int n = 1000;"
				L"	n = [n](int a) -> int {"
				L"	while(n > 0) {"
				L"   n--;"
				L"  }"
				L"	return n;"
				L"  }(1);"
				L"	return n;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 0, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(LambdaExpression, CompileThread1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	int n = 1000;"
				L"	f = [&n]() {"
				L"	while(n > 0) {"
				L"   n--;"
				L"  }"
				L"  };"
				L"	t = createThread(f) ;"
				L"	joinThread(t);"
				L"	closeThread(t);"
				L"	return n;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");
#if __APPLE__
            FF_EXPECT_TRUE(false, L"this test is defaultly failed when run on gtest 1.7.0 on Mac");
#else
            ScriptTask scriptTask(program);
            scriptTask.runFunction(functionId, nullptr);
            int* funcRes = (int*)scriptTask.getTaskResult();
            
            FF_EXPECT_TRUE(*funcRes == 0, L"program can run but return wrong value");
#endif //__APPLE__
		}

		FF_TEST_FUNCTION(LambdaExpression, CompileThread2)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	int n = 1000000;"				
				L"	t = createThread([n](){ while(n > 0) { n --;} });"
				L"	joinThread(t);"
				L"	closeThread(t);"
				L"	return n;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

            ScriptTask scriptTask(program);
            scriptTask.runFunction(functionId, nullptr);
            int* funcRes = (int*)scriptTask.getTaskResult();

            FF_EXPECT_TRUE(*funcRes == 1000000, L"program can run but return wrong value");
		}
	};
}
