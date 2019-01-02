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
#include "fftest.hpp"

#include "InternalCompilerSuite.h"

using namespace ffscript;

namespace ffscriptUT
{		
	namespace InternalComplierSuiteUT
	{
		FF_TEST_FUNCTION(InternalComplierSuiteTest, SimpleExpression1)
		{
			InternalCompilerSuite internalCompilerSuite;
			internalCompilerSuite.initialize(1024);
			auto excutor = internalCompilerSuite.compileExpression(L"2");
			FF_EXPECT_NE(nullptr, excutor, L"compile simple expression failed");
			excutor->runCode();
			int* iRes = (int*)excutor->getReturnData();
			FF_EXPECT_EQ((int)2, *iRes, L"Simple expression is calculated wrongly");
		}

		FF_TEST_FUNCTION(InternalComplierSuiteTest, SimpleExpression2)
		{
			InternalCompilerSuite internalCompilerSuite;
			internalCompilerSuite.initialize(1024);
			auto excutor = internalCompilerSuite.compileExpression(L"2.5");
			FF_EXPECT_EQ(nullptr, excutor, L"internal compiler should not support double data type");
		}

		//current conditional operator is not support
		FF_TEST_FUNCTION(InternalComplierSuiteTest, ConstantExpression1)
		{
			InternalCompilerSuite internalCompilerSuite;
			internalCompilerSuite.initialize(1024);
			auto excutor = internalCompilerSuite.compileExpression(L"1 ? 2 : 3");
			FF_EXPECT_NE(nullptr, excutor, L"compile simple expression failed");
			excutor->runCode();
			int* iRes = (int*)excutor->getReturnData();
			FF_EXPECT_EQ((int)2, *iRes, L"Simple expression is calculated wrongly");
		}

		FF_TEST_FUNCTION(InternalComplierSuiteTest, ConstantExpression2)
		{
			InternalCompilerSuite internalCompilerSuite;
			internalCompilerSuite.initialize(1024);
			auto excutor = internalCompilerSuite.compileExpression(L"1 + 3 * 2 / 1");
			FF_EXPECT_NE(nullptr, excutor, L"compile simple expression failed");
			excutor->runCode();
			int* iRes = (int*)excutor->getReturnData();
			FF_EXPECT_EQ((int)7, *iRes, L"Simple expression is calculated wrongly");
		}
	};
}