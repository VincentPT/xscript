/******************************************************************
* File:        ScriptTypeUT.cpp
* Description: Test cases focus on checking usage of scripting types
*              in C Lambda scripting language.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "CppUnitTest.h"
#include <CompilerSuite.h>
#include <ScriptTask.h>
#include <Utils.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace ffscript;

namespace ffscriptUT
{	
	TEST_CLASS(ScriptTypeUT)
	{
	public:
		TEST_METHOD(RefFunctionUT1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			auto pX = rootScope->registVariable("x");
			pX->setDataType(ScriptType(compiler.getTypeManager()->getBasicTypes().TYPE_INT,"int"));
			auto excutor = compiler.compileExpression(L"ref(x)");
			Assert::IsNotNull(excutor, L"compile expression with ref operator failed");

			int* addressX = (int*)rootScope->getGlobalAddress(pX->getOffset());

			excutor->runCode();
			void* res = excutor->getReturnData();
			int* iRes = *((int**)res);

			Assert::AreEqual(addressX, iRes, L"ref function return wrong");
		}

		TEST_METHOD(RefFunctionUT2)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			auto pX = rootScope->registVariable("x");
			pX->setDataType(ScriptType(compiler.getTypeManager()->getBasicTypes().TYPE_INT, "int"));
			auto excutor = compiler.compileExpression(L"a = ref x");
			Assert::IsNotNull(excutor, L"compile expression with ref operator failed");

			int* addressX = (int*)rootScope->getGlobalAddress(pX->getOffset());
			auto pA = rootScope->findVariable("a");

			size_t* addressA = (size_t*)rootScope->getGlobalAddress(pA->getOffset());

			excutor->runCode();
			void* res = *(void**)excutor->getReturnData();
			int* iRes = *((int**)res);

			Assert::AreEqual(addressX, iRes, L"ref function return wrong");
			Assert::AreEqual(*addressA, (size_t)iRes, L"ref function return wrong");
		}

		TEST_METHOD(RefFunctionUT4)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			auto X = rootScope->registVariable("x");
			X->setDataType(ScriptType(compiler.getTypeManager()->getBasicTypes().TYPE_INT, "int"));
			auto excutor1 = compiler.compileExpression(L"a = ref x");
			Assert::IsNotNull(excutor1, L"compile expression with ref operator failed");
			auto excutor2 = compiler.compileExpression(L"b = ref a");
			Assert::IsNotNull(excutor2, L"compile expression with ref operator failed");

			int* pX = (int*)rootScope->getGlobalAddress(X->getOffset());
			*pX = 1;
			auto A = rootScope->findVariable("a");
			auto B = rootScope->findVariable("b");

			size_t* addressA = (size_t*)rootScope->getGlobalAddress(A->getOffset());
			size_t* addressB = (size_t*)rootScope->getGlobalAddress(B->getOffset());

			excutor1->runCode();			
			void* res = *((void**)excutor1->getReturnData());
			int* iRes = *((int**)res);

			Assert::AreEqual(pX, iRes, L"ref function return wrong");
			Assert::AreEqual(*addressA, (size_t)iRes, L"ref function return wrong");

			int* pA = (int*)*addressA;
			Assert::AreEqual(*pX, *pA, L"value of A is wrong");

			excutor2->runCode();
			Assert::AreEqual((size_t)addressA, *addressB, L"ref of ref gone wrong");
			int** pB = (int**)*addressB;
			Assert::AreEqual(*pX, *(*pB), L"value of B is wrong");
		}

		TEST_METHOD(RefFunctionUT5)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"void foo(ref ref int ppn, ref int pn) {"
				L"	*ppn = pn;"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref ref int, ref int");
			Assert::IsTrue(functionId >= 0, L"can not find function 'foo'");

			int n = 1;
			int* pn;
			ScriptParamBuffer paramBuffer(&pn);
			paramBuffer.addParam(&n);

			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			Assert::AreEqual(n, *pn, L"function 'foo' return wrong");
		}

		TEST_METHOD(RefFunctionUT6)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"void foo(ref ref int ppa, ref ref int ppb) {"
				L"	ppa = ppb;"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref ref int, ref ref int");
			Assert::IsTrue(functionId >= 0, L"can not find function 'foo'");

			int a = 1;
			int b = 2;
			int* pa = &a;
			int* pb = &b;
			
			ScriptParamBuffer paramBuffer(&pa);
			paramBuffer.addParam(&pb);

			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			Assert::AreEqual(a, *pa, L"function 'foo' return wrong");
			Assert::AreEqual(b, *pb, L"function 'foo' return wrong");
		}

		TEST_METHOD(RefFunctionUT7)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"int foo(ref int pn) {"
				L"	return 2 * *pn;"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref int");
			Assert::IsTrue(functionId >= 0, L"can not find function 'foo'");

			int n = 2;
			ScriptParamBuffer paramBuffer(&n);
			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			int* res = (int*)task.getTaskResult();
			Assert::AreEqual(4, *res, L"function 'foo' return wrong");
		}
	};
}