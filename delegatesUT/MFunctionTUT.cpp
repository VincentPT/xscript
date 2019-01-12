/******************************************************************
* File:        MethodContainerUT.cpp
* Description: Test cases invoking member functions using MFunction.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "../ffscriptUT/fftest.hpp"
#include "../ffscript/ffscript.h"
#include "../ffscriptUT/TemplateForTest.hpp"
#include "../ffscript/function/CachedMethod.hpp"
#include "MethodsForTest.h"

using namespace std;
using namespace ffscript;
using namespace FT;

namespace ffscriptUT
{
	FF_TEST_METHOD(MethodContainer, methodArgumentBufferTestMethodContainerVoid1)
	{
		MFunctionT<MethodContainer, void> memberFunction(this, &MethodContainer::memberFoo);
		memberFunction.call(nullptr, nullptr);
	}

	FF_TEST_METHOD(MethodContainer, methodArgumentBufferTestCachedDelegate1)
	{
		CachedMethodDelegate<MethodContainer, void> memberFunction(this, &MethodContainer::memberFoo);
	}

	FF_TEST_METHOD(MethodContainer, methodArgumentBufferTestCachedDelegate2)
	{
		CachedMethodDelegate<MethodContainer, double, float, double> memberFunction(this, &MethodContainer::memberFoo);

		float arg1 = 0;
		double arg2 = 1;
		memberFunction.setArgs(arg1, arg2);
		memberFunction.call();

		auto ret = memberFunction.getReturnValAsDouble();

		FF_EXPECT_EQ(MethodContainer::memberFoo(arg1, arg2), ret);
	}
}
