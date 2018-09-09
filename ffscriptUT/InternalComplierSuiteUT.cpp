/******************************************************************
* File:        InternalComplierSuiteUT.cpp
* Description: Test cases for checking behavior of internal compiler.
*              Internal compiler is a compiler use inside the
*              scripting compiler to compile some constant expression.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "CppUnitTest.h"
#include "InternalCompilerSuite.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ffscript;

namespace ffscriptUT
{		
	TEST_CLASS(InternalComplierSuiteUT)
	{
	public:
		TEST_METHOD(SimpleExpression1)
		{
			InternalCompilerSuite internalCompilerSuite;
			internalCompilerSuite.initialize(1024);
			auto excutor = internalCompilerSuite.compileExpression(L"2");
			Assert::IsNotNull(excutor, L"compile simple expression failed");
			excutor->runCode();
			int* iRes = (int*)excutor->getReturnData();
			Assert::AreEqual((int)2, *iRes, L"Simple expression is calculated wrongly");
		}

		TEST_METHOD(SimpleExpression2)
		{
			InternalCompilerSuite internalCompilerSuite;
			internalCompilerSuite.initialize(1024);
			auto excutor = internalCompilerSuite.compileExpression(L"2.5");
			Assert::IsNull(excutor, L"internal compiler should not support double data type");
		}

		//current conditional operator is not support
		TEST_METHOD(ConstantExpression1)
		{
			InternalCompilerSuite internalCompilerSuite;
			internalCompilerSuite.initialize(1024);
			auto excutor = internalCompilerSuite.compileExpression(L"1 ? 2 : 3");
			Assert::IsNotNull(excutor, L"compile simple expression failed");
			excutor->runCode();
			int* iRes = (int*)excutor->getReturnData();
			Assert::AreEqual((int)2, *iRes, L"Simple expression is calculated wrongly");
		}

		TEST_METHOD(ConstantExpression2)
		{
			InternalCompilerSuite internalCompilerSuite;
			internalCompilerSuite.initialize(1024);
			auto excutor = internalCompilerSuite.compileExpression(L"1 + 3 * 2 / 1");
			Assert::IsNotNull(excutor, L"compile simple expression failed");
			excutor->runCode();
			int* iRes = (int*)excutor->getReturnData();
			Assert::AreEqual((int)7, *iRes, L"Simple expression is calculated wrongly");
		}
	};
}