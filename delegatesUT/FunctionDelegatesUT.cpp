/******************************************************************
* File:        CdeclFunctionUT.cpp
* Description: Test cases invoking functions using FunctionDelegate,
*              FunctionDelegate2, CdeclFunction3 function objects.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "../ffscriptUT/fftest.hpp"

#include <functional>
#include "../ffscript/function/FunctionDelegate.hpp"
#include "../ffscript/function/FunctionDelegate2.hpp"
#include "../ffscriptUT/TemplateForTest.hpp"
#include "FunctionsForTest.h"
#include <math.h>

using namespace std;
using namespace FT;

namespace ffscriptUT
{
	TEST(FunctionDelegate, testFunctionVoid1)
	{
		FunctionDelegate<void> funcDelegate(&funcFoo);
		funcDelegate.call();
	}

	TEST(FunctionDelegate, testFunctionVoid2)
	{
		FunctionDelegate<void, const float&> funcDelegate(&funcFoo);

		float forRefValue1 = 1.0f;		

		pushParams(&funcDelegate, &forRefValue1);

		funcDelegate.call();
	}

	TEST(FunctionDelegate, testFunctionVoid3)
	{
		FunctionDelegate<void, const float&, const float&> funcDelegate(&funcFoo);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;

		pushParams(&funcDelegate, &forRefValue1, &forRefValue2);

		funcDelegate.call();
	}

	TEST(FunctionDelegate, testFunctionVoid4)
	{
		FunctionDelegate<void, const float&, const float&, int> funcDelegate(&funcFoo);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;

		pushParams(&funcDelegate, &forRefValue1, &forRefValue2, p3);

		funcDelegate.call();
	}

	TEST(FunctionDelegate, testFunctionVoid5)
	{
		FunctionDelegate<void, const float&, const float&, int, char> funcDelegate(&funcFoo);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';

		pushParams(&funcDelegate, &forRefValue1, &forRefValue2, p3, p4);

		funcDelegate.call();
	}

	TEST(FunctionDelegate, testFunctionVoid6)
	{
		FunctionDelegate<void, const float&,
			const float&, int, char, const double&> funcDelegate(&funcFoo);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';
		double forRefValue3 = 5.0f;

		pushParams(&funcDelegate, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3);

		funcDelegate.call();
	}

	TEST(FunctionDelegate, testFunctionVoid7)
	{
		FunctionDelegate<void, const float&,
			const float&, int, char, const double&, void*> funcDelegate(&funcFoo);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';
		double forRefValue3 = 5.0f;
		void* p6 = (void*)6;

		pushParams(&funcDelegate, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6);

		funcDelegate.call();
	}

	TEST(FunctionDelegate, testFunctionVoid8)
	{
		FunctionDelegate<void, const float&,
			const float&, int, char, const double&, void*, short> funcDelegate(&funcFoo);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';
		double forRefValue3 = 5.0f;
		void* p6 = (void*)6;
		short p7 = 7;

		pushParams(&funcDelegate, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7);

		funcDelegate.call();
	}

	TEST(FunctionDelegate, testFunctionVoid9)
	{
		FunctionDelegate<void, const float&,
			const float&, int, char, const double&, void*, short, int> funcDelegate(&funcFoo);
			
		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';
		double forRefValue3 = 5.0f;
		void* p6 = (void*)6;
		short p7 = 7;
		int p8 = 8;

		pushParams(&funcDelegate, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7, p8);

		funcDelegate.call();
	}

	TEST(FunctionDelegate, testFunctionFloat1)
	{
		FunctionDelegate<float> funcDelegate(&funcFoof);
		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(FunctionDelegate, testFunctionFloat2)
	{
		FunctionDelegate<float, const float&> funcDelegate(&funcFoof);

		float forRefValue1 = 1.0f;

		pushParams(&funcDelegate, &forRefValue1);

		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(FunctionDelegate, testFunctionFloat3)
	{
		FunctionDelegate<float, const float&, const float&> funcDelegate(&funcFoof);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;

		pushParams(&funcDelegate, &forRefValue1, &forRefValue2);

		float expectedVal = 3.0f;

		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(FunctionDelegate, testFunctionFloat4)
	{
		FunctionDelegate<float, const float&, const float&, int> funcDelegate(&funcFoof);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;

		pushParams(&funcDelegate, &forRefValue1, &forRefValue2, p3);
		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(FunctionDelegate, testFunctionFloat5)
	{
		FunctionDelegate<float, const float&, const float&, int, char> funcDelegate(&funcFoof);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';

		pushParams(&funcDelegate, &forRefValue1, &forRefValue2, p3, p4);

		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(FunctionDelegate, testFunctionFloat6)
	{
		FunctionDelegate<float, const float&,
			const float&, int, char, const double&> funcDelegate(&funcFoof);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';
		double forRefValue3 = 5.0f;

		pushParams(&funcDelegate, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3);

		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(FunctionDelegate, testFunctionFloat7)
	{
		FunctionDelegate<float, const float&,
			const float&, int, char, const double&, void*> funcDelegate(&funcFoof);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';
		double forRefValue3 = 5.0f;
		void* p6 = (void*)6;

		pushParams(&funcDelegate, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6);

		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(FunctionDelegate, testFunctionFloat8)
	{
		FunctionDelegate<float, const float&,
			const float&, int, char, const double&, void*, short> funcDelegate(&funcFoof);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';
		double forRefValue3 = 5.0f;
		void* p6 = (void*)6;
		short p7 = 7;

		pushParams(&funcDelegate, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7);

		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(FunctionDelegate, testFunctionFloat9)
	{
		FunctionDelegate<float, const float&,
			const float&, int, char, const double&, void*, short, int> funcDelegate(&funcFoof);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';
		double forRefValue3 = 5.0f;
		void* p6 = (void*)6;
		short p7 = 7;
		int p8 = 8;

		pushParams(&funcDelegate, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7, p8);
			
		funcDelegate.call();

		float actualVal = funcDelegate.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(FunctionDelegate, testFunctionGeneral1)
	{
		FunctionDelegate<char> funcDelegate(&funcFooGeneral);

		DFunction* dynamicCaller = &funcDelegate;
		dynamicCaller->call();

		EXPECT_EQ((char)1, (char)dynamicCaller->getReturnValAsInt8());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(FunctionDelegate, testFunctionGeneral2)
	{
		FunctionDelegate<short, int> funcDelegate(&funcFooGeneral);

		DFunction* dynamicCaller = &funcDelegate;
		dynamicCaller->pushParam((void*)1);
		dynamicCaller->call();

		EXPECT_EQ((short)1, (short)dynamicCaller->getReturnValAsInt16());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(FunctionDelegate, testFunctionGeneral3)
	{
		FunctionDelegate<float, int, char> funcDelegate(&funcFooGeneral);

		DFunction* dynamicCaller = &funcDelegate;
		dynamicCaller->pushParam((void*)1);
		dynamicCaller->pushParam((void*)'2');
		dynamicCaller->call();

		EXPECT_EQ(1.0f, dynamicCaller->getReturnValAsFloat());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(FunctionDelegate, testFunctionGeneral4)
	{
		FunctionDelegate<int, int, char, const double&> funcDelegate(&funcFooGeneral);

		double p3 = 3.0f;

		DFunction* dynamicCaller = &funcDelegate;
		dynamicCaller->pushParam((void*)1);
		dynamicCaller->pushParam((void*)'2');
		dynamicCaller->pushParam((void*)&p3);
		dynamicCaller->call();

		EXPECT_EQ(1, dynamicCaller->getReturnValAsInt32());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(FunctionDelegate, testFunctionGeneral5)
	{
		FunctionDelegate<long, int, char, const double&, const float&> funcDelegate(&funcFooGeneral);

		double p3 = 3.0f;
		float p4 = 4.0f;

		DFunction* dynamicCaller = &funcDelegate;
		dynamicCaller->pushParam((void*)1);
		dynamicCaller->pushParam((void*)'2');
		dynamicCaller->pushParam((void*)&p3);
		dynamicCaller->pushParam((void*)&p4);
		dynamicCaller->call();

		EXPECT_EQ(1, dynamicCaller->getReturnValAsInt32());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(FunctionDelegate, testFunctionGeneral6)
	{
		FunctionDelegate<long long, int, char, const double&, const float&, unsigned int> funcDelegate(&funcFooGeneral);

		double p3 = 3.0f;
		float p4 = 4.0f;

		DFunction* dynamicCaller = &funcDelegate;
		dynamicCaller->pushParam((void*)1);
		dynamicCaller->pushParam((void*)'2');
		dynamicCaller->pushParam((void*)&p3);
		dynamicCaller->pushParam((void*)&p4);
		dynamicCaller->pushParam((void*)5);
		dynamicCaller->call();

		EXPECT_TRUE(1000000000000000 == dynamicCaller->getReturnValAsInt64());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(FunctionDelegate, testFunctionGeneral7)
	{
		FunctionDelegate<double, int, char, const double&, const float&, unsigned int, short> funcDelegate(&funcFooGeneral);

		double p3 = 3.0f;
		float p4 = 4.0f;

		DFunction* dynamicCaller = &funcDelegate;
		dynamicCaller->pushParam((void*)1);
		dynamicCaller->pushParam((void*)'2');
		dynamicCaller->pushParam((void*)&p3);
		dynamicCaller->pushParam((void*)&p4);
		dynamicCaller->pushParam((void*)5);
		dynamicCaller->pushParam((void*)6);
		dynamicCaller->call();

		EXPECT_EQ(1000000000000000.0, dynamicCaller->getReturnValAsDouble());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(FunctionDelegate, testFunctionGeneral8)
	{
		FunctionDelegate<unsigned short, int, char, const double&, const float&, unsigned int, short, long> funcDelegate(&funcFooGeneral);

		double p3 = 3.0f;
		float p4 = 4.0f;

		DFunction* dynamicCaller = &funcDelegate;
		dynamicCaller->pushParam((void*)1);
		dynamicCaller->pushParam((void*)'2');
		dynamicCaller->pushParam((void*)&p3);
		dynamicCaller->pushParam((void*)&p4);
		dynamicCaller->pushParam((void*)5);
		dynamicCaller->pushParam((void*)6);
		dynamicCaller->pushParam((void*)7);
		dynamicCaller->call();

		EXPECT_EQ(1, (int)dynamicCaller->getReturnValAsInt16());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(FunctionDelegate, testFunctionGeneral9)
	{
		FunctionDelegate<unsigned short, int, char, const double&, const float&, unsigned int, short, long> funcDelegate(&funcFooGeneral);

		double p3 = 3.0f;
		float p4 = 4.0f;

		DFunction* dynamicCaller = &funcDelegate;
		dynamicCaller->pushParam((void*)1);
		dynamicCaller->pushParam((void*)'2');
		dynamicCaller->pushParam((void*)&p3);
		dynamicCaller->pushParam((void*)&p4);
		dynamicCaller->pushParam((void*)5);
		dynamicCaller->pushParam((void*)6);
		dynamicCaller->pushParam((void*)7);
		dynamicCaller->call();

		EXPECT_EQ(1, (int)dynamicCaller->getReturnValAsInt16());

		DFunction* pNewInsance = dynamicCaller->clone();
		pNewInsance->call();

		EXPECT_EQ(1, (int)pNewInsance->getReturnValAsInt16());
	}

	/*static double sin(const double& v) {
		return ::sin(v);
	}*/

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(FunctionDelegate, testFunctionExternal1)
	{
		/*FunctionDelegate<double, double> funcDelegate(funcForward<double, double>();

		double p = 3.0f;

		double expect = ::sin(p);

		funcDelegate.pushParam(&p);

		funcDelegate.call();

		double val1 = funcDelegate.getReturnValAsDouble();
		const void* val2 = funcDelegate.getReturnValAsReference();

		EXPECT_EQ(expect, val1);
		EXPECT_EQ(expect, *((double*)val2));*/

		double p = 3.0f;
		double val1 = funcForward<double, double>(&sin, p);
		double expect = ::sin(p);
		EXPECT_EQ(expect, val1);
	}
		

	TEST(FunctionDelegate, testFunction2_1)
	{
		float p1 = 1.0f;
		float p2 = 2.0f;
		int p3 = 3;
		FunctionDelegate2<float, const float&, const float&, int> cdelFunction(funcFoof);
		DFunction2* nativeFunction2 = &cdelFunction;

		float returnVal;
		void* params[] = {&p1, &p2, (void*)(size_t)p3};
		nativeFunction2->call(&returnVal, params);
		EXPECT_EQ(funcFoof(p1, p2, p3), returnVal);
	}
#ifndef USE_EXTERNAL_PARAMS_ONLY
	TEST(FunctionDelegate, testFunction2_2)
	{
		float fixedParam = 1.0f;
		float p2 = 2.0f;
		int p3 = 3;
		FunctionDelegate2<float, const float&, const float&, int> cdelFunction(funcFoof);
		cdelFunction.bind<float*>(&fixedParam);

		DFunction2* nativeFunction2 = &cdelFunction;

		float returnVal;
		void* params[] = { &p2, (void*)(size_t)p3 };
		nativeFunction2->call(&returnVal, params);
		EXPECT_EQ(funcFoof(fixedParam, p2, p3), returnVal);
	}
#endif
}
