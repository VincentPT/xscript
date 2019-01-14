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
#include "../ffscript/function/CachedFunction.hpp"
#include "MethodsForTest.h"
#include "FunctionsForTest.h"

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

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(FunctionDelegate, testCachedFunctionDelegate1)
	{
		CachedFunctionDelegate<double, int, char, const double&, const float&, unsigned int, short> funcDelegate(&funcFooGeneral);

		double arg3 = 3.0;
		float arg4 = 4.0f;
		funcDelegate.setArgs(1, '2', arg3, arg4, 5, 6);
		funcDelegate.call();

		EXPECT_EQ(1000000000000000.0, funcDelegate.getReturnValAsDouble());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(FunctionDelegate, testMemberTypeInfo1)
	{
		typedef MemberTypeInfo < 0, sizeof(void*), float, double, int > ArgumentHelper;
		auto offset2 = ArgumentHelper::template offset<2>();
		EXPECT_EQ(3 * sizeof(void*), offset2);
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(FunctionDelegate, testMemberTypeInfo2)
	{
		typedef MemberTypeInfo < 0, sizeof(void*), int, char, const double&, const float&, unsigned int, short > ArgumentHelper;
		auto offset3 = ArgumentHelper::template offset<3>();
		EXPECT_EQ(3 * sizeof(void*), offset3);
	}
}
