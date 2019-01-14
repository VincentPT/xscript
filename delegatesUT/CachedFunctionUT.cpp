/******************************************************************
* File:        CdeclFunctionUT.cpp
* Description: Test cases invoking functions using CachedFunction,
*              CachedFunctionDelegate2, CdeclFunction3 function objects.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "../ffscriptUT/fftest.hpp"

#include <functional>
#include "../ffscript/function/CachedFunction.hpp"
#include "../ffscriptUT/TemplateForTest.hpp"
#include "FunctionsForTest.h"
#include <math.h>

using namespace std;
using namespace FT;

namespace ffscriptUT
{
	TEST(CachedFunction, testCachedFunctionVoid1)
	{
		CachedFunctionDelegate<void> funcDelegate(&funcFoo);
		funcDelegate.call();
	}

	TEST(CachedFunction, testCachedFunctionVoid2)
	{
		CachedFunctionDelegate<void, const float&> funcDelegate(&funcFoo);

		funcDelegate.setArgs(1.0f);

		funcDelegate.call();
	}

	TEST(CachedFunction, testCachedFunctionVoid3)
	{
		CachedFunctionDelegate<void, const float&, const float&> funcDelegate(&funcFoo);

		funcDelegate.setArgs(1.0f, 2.0f);

		funcDelegate.call();
	}

	TEST(CachedFunction, testCachedFunctionVoid4)
	{
		CachedFunctionDelegate<void, const float&, const float&, int> funcDelegate(&funcFoo);

		funcDelegate.setArgs(1.0f, 2.0f, 3);

		funcDelegate.call();
	}

	TEST(CachedFunction, testCachedFunctionVoid5)
	{
		CachedFunctionDelegate<void, const float&, const float&, int, char> funcDelegate(&funcFoo);

		funcDelegate.setArgs(1.0f, 2.0f, 3, '4');

		funcDelegate.call();
	}

	TEST(CachedFunction, testCachedFunctionVoid6)
	{
		CachedFunctionDelegate<void, const float&,
			const float&, int, char, const double&> funcDelegate(&funcFoo);

		funcDelegate.setArgs(1.0f, 2.0f, 3, '4', 5.0);

		funcDelegate.call();
	}

	TEST(CachedFunction, testCachedFunctionVoid7)
	{
		CachedFunctionDelegate<void, const float&,
			const float&, int, char, const double&, void*> funcDelegate(&funcFoo);

		funcDelegate.setArgs(1.0f, 2.0f, 3, '4', 5.0, (void*)6);

		funcDelegate.call();
	}

	TEST(CachedFunction, testCachedFunctionVoid8)
	{
		CachedFunctionDelegate<void, const float&,
			const float&, int, char, const double&, void*, short> funcDelegate(&funcFoo);

		funcDelegate.setArgs(1.0f, 2.0f, 3, '4', 5.0, (void*)6, 7);

		funcDelegate.call();
	}

	TEST(CachedFunction, testCachedFunctionVoid9)
	{
		CachedFunctionDelegate<void, const float&,
			const float&, int, char, const double&, void*, short, int> funcDelegate(&funcFoo);
			
		funcDelegate.setArgs(1.0f, 2.0f, 3, '4', 5.0, (void*)6, 7, 8);

		funcDelegate.call();
	}

	TEST(CachedFunction, testCachedFunctionFloat1)
	{
		CachedFunctionDelegate<float> funcDelegate(&funcFoof);
		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(CachedFunction, testCachedFunctionFloat2)
	{
		CachedFunctionDelegate<float, const float&> funcDelegate(&funcFoof);

		funcDelegate.setArgs(1.0f);

		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(CachedFunction, testCachedFunctionFloat3)
	{
		CachedFunctionDelegate<float, const float&, const float&> funcDelegate(&funcFoof);

		funcDelegate.setArgs(1.0f, 2.0f);

		float expectedVal = 3.0f;

		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(CachedFunction, testCachedFunctionFloat4)
	{
		CachedFunctionDelegate<float, const float&, const float&, int> funcDelegate(&funcFoof);

		funcDelegate.setArgs(1.0f, 2.0f, 3);
		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(CachedFunction, testCachedFunctionFloat5)
	{
		CachedFunctionDelegate<float, const float&, const float&, int, char> funcDelegate(&funcFoof);

		funcDelegate.setArgs(1.0f, 2.0f, 3, '4');

		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(CachedFunction, testCachedFunctionFloat6)
	{
		CachedFunctionDelegate<float, const float&,
			const float&, int, char, const double&> funcDelegate(&funcFoof);

		funcDelegate.setArgs(1.0f, 2.0f, 3, '4', 5.0);

		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(CachedFunction, testCachedFunctionFloat7)
	{
		CachedFunctionDelegate<float, const float&,
			const float&, int, char, const double&, void*> funcDelegate(&funcFoof);

		funcDelegate.setArgs(1.0f, 2.0f, 3, '4', 5.0, (void*)6);

		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(CachedFunction, testCachedFunctionFloat8)
	{
		CachedFunctionDelegate<float, const float&,
			const float&, int, char, const double&, void*, short> funcDelegate(&funcFoof);

		funcDelegate.setArgs(1.0f, 2.0f, 3, '4', 5.0, (void*)6, 7);

		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(CachedFunction, testCachedFunctionFloat9)
	{
		CachedFunctionDelegate<float, const float&,
			const float&, int, char, const double&, void*, short, int> funcDelegate(&funcFoof);

		funcDelegate.setArgs(1.0f, 2.0f, 3, '4', 5.0, (void*)6, 7, 8);
			
		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CachedFunction, testCachedFunctionGeneral1)
	{
		CachedFunctionDelegate<char> funcDelegate(&funcFooGeneral);

		CachedDelegate* dynamicCaller = &funcDelegate;
		dynamicCaller->call();

		EXPECT_EQ((char)1, (char)dynamicCaller->getReturnValAsInt8());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CachedFunction, testCachedFunctionGeneral2)
	{
		CachedFunctionDelegate<short, int> funcDelegate(&funcFooGeneral);
		funcDelegate.setArgs(1);

		CachedDelegate* dynamicCaller = &funcDelegate;
		dynamicCaller->call();

		EXPECT_EQ((short)1, (short)dynamicCaller->getReturnValAsInt16());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CachedFunction, testCachedFunctionGeneral3)
	{
		CachedFunctionDelegate<float, int, char> funcDelegate(&funcFooGeneral);
		funcDelegate.setArgs(1, '2');

		CachedDelegate* dynamicCaller = &funcDelegate;
		dynamicCaller->call();

		EXPECT_EQ(1.0f, dynamicCaller->getReturnValAsFloat());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CachedFunction, testCachedFunctionGeneral4)
	{
		CachedFunctionDelegate<int, int, char, const double&> funcDelegate(&funcFooGeneral);

		funcDelegate.setArgs(1, '2', 3.0);

		CachedDelegate* dynamicCaller = &funcDelegate;
		dynamicCaller->call();

		EXPECT_EQ(1, dynamicCaller->getReturnValAsInt32());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CachedFunction, testCachedFunctionGeneral5)
	{
		CachedFunctionDelegate<long, int, char, const double&, const float&> funcDelegate(&funcFooGeneral);

		funcDelegate.setArgs(1, '2', 3.0, 4.0f);

		CachedDelegate* dynamicCaller = &funcDelegate;
		dynamicCaller->call();

		EXPECT_EQ(1, dynamicCaller->getReturnValAsInt32());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CachedFunction, testCachedFunctionGeneral6)
	{
		CachedFunctionDelegate<long long, int, char, const double&, const float&, unsigned int> funcDelegate(&funcFooGeneral);

		funcDelegate.setArgs(1, '2', 3.0, 4.0f, 5);

		CachedDelegate* dynamicCaller = &funcDelegate;
		dynamicCaller->call();

		EXPECT_TRUE(1000000000000000 == dynamicCaller->getReturnValAsInt64());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CachedFunction, testCachedFunctionGeneral7)
	{
		CachedFunctionDelegate<double, int, char, const double&, const float&, unsigned int, short> funcDelegate(&funcFooGeneral);

		funcDelegate.setArgs(1, '2', 3.0, 4.0f, 5, 6);

		CachedDelegate* dynamicCaller = &funcDelegate;
		dynamicCaller->call();

		EXPECT_EQ(1000000000000000.0, dynamicCaller->getReturnValAsDouble());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CachedFunction, testCachedFunctionGeneral8)
	{
		CachedFunctionDelegate<unsigned short, int, char, const double&, const float&, unsigned int, short, long> funcDelegate(&funcFooGeneral);

		funcDelegate.setArgs(1, '2', 3.0, 4.0f, 5, 6, 7);

		CachedDelegate* dynamicCaller = &funcDelegate;
		dynamicCaller->call();

		EXPECT_EQ(1, (int)dynamicCaller->getReturnValAsInt16());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CachedFunction, testCachedFunctionSum)
	{
		CachedFunctionDelegate<double, int&, double> funcDelegate(&sum);

		int arg1 = 1;
		double arg2 = 2.0;
		funcDelegate.setArgs(arg1, arg2);

		CachedDelegate* dynamicCaller = &funcDelegate;
		dynamicCaller->call();

		EXPECT_EQ( sum(arg1, arg2), dynamicCaller->getReturnValAsDouble());
	}
}
