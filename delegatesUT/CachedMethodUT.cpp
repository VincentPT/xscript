/******************************************************************
* File:        MethodContainerUT.cpp
* Description: Test cases invoking member functions using CachedMethodDelegate.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "../ffscriptUT/fftest.hpp"
#include "../ffscript/function/CachedMethod.hpp"
#include "../ffscriptUT/TemplateForTest.hpp"
#include "MethodsForTest.h"
#include <math.h>

using namespace std;
using namespace FT;

namespace ffscriptUT
{
	namespace MethodContainerUT {
		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerVoid1)
		{
			CachedMethodDelegate<MethodContainer, void> memberFunction(this, &MethodContainer::memberFoo);
			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerVoid2)
		{
			CachedMethodDelegate<MethodContainer, void, const float&> memberFunction(this, &MethodContainer::memberFoo);
			memberFunction.setArgs(1.0f);
			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerVoid3)
		{
			CachedMethodDelegate<MethodContainer, void, const float&, const float&> memberFunction(this, &MethodContainer::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;

			memberFunction.setArgs(forRefValue1, forRefValue2);
			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerVoid4)
		{
			CachedMethodDelegate<MethodContainer, void, const float&, const float&, int> memberFunction(this, &MethodContainer::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;	
			int p3 = 3;

			memberFunction.setArgs(forRefValue1, forRefValue2, p3);

			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerVoid5)
		{
			CachedMethodDelegate<MethodContainer, void, const float&, const float&, int, char> memberFunction(this, &MethodContainer::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';

			memberFunction.setArgs(forRefValue1, forRefValue2, p3, p4);

			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerVoid6)
		{
			CachedMethodDelegate<MethodContainer, void, const float&,
				const float&, int, char, const double&> memberFunction(this, &MethodContainer::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;

			memberFunction.setArgs(forRefValue1, forRefValue2, p3, p4, forRefValue3);

			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerVoid7)
		{
			CachedMethodDelegate<MethodContainer, void, const float&,
				const float&, int, char, const double&, void*> memberFunction(this, &MethodContainer::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;

			memberFunction.setArgs(forRefValue1, forRefValue2, p3, p4, forRefValue3, p6);
			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerVoid8)
		{
			CachedMethodDelegate<MethodContainer, void, const float&,
				const float&, int, char, const double&, void*, short> memberFunction(this, &MethodContainer::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;

			memberFunction.setArgs(forRefValue1, forRefValue2, p3, p4, forRefValue3, p6, p7);

			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerVoid9)
		{
			CachedMethodDelegate<MethodContainer, void, const float&,
				const float&, int, char, const double&, void*, short, int> memberFunction(this, &MethodContainer::memberFoo);
			
			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;
			int p8 = 8;

			memberFunction.setArgs(forRefValue1, forRefValue2, p3, p4, forRefValue3, p6, p7, p8);

			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerFloat1)
		{
			CachedMethodDelegate<MethodContainer, float> memberFunction(this, &MethodContainer::memberFoof);

			float expectedVal = 1.0f;
			memberVariable = expectedVal;
			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerFloat2)
		{
			CachedMethodDelegate<MethodContainer, float, const float&> memberFunction(this, &MethodContainer::memberFoof);

			float forRefValue1 = 1.0f;

			memberFunction.setArgs(forRefValue1);

			float expectedVal = 2.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerFloat3)
		{
			CachedMethodDelegate<MethodContainer, float, const float&, const float&> memberFunction(this, &MethodContainer::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;

			memberFunction.setArgs(forRefValue1, forRefValue2);

			float expectedVal = 3.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerFloat4)
		{
			CachedMethodDelegate<MethodContainer, float, const float&, const float&, int> memberFunction(this, &MethodContainer::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;

			memberFunction.setArgs(forRefValue1, forRefValue2, p3);

			float expectedVal = 4.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerFloat5)
		{
			CachedMethodDelegate<MethodContainer, float, const float&, const float&, int, char> memberFunction(this, &MethodContainer::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';

			memberFunction.setArgs(forRefValue1, forRefValue2, p3, p4);

			float expectedVal = 5.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerFloat6)
		{
			CachedMethodDelegate<MethodContainer, float, const float&,
				const float&, int, char, const double&> memberFunction(this, &MethodContainer::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;

			memberFunction.setArgs(forRefValue1, forRefValue2, p3, p4, forRefValue3);

			float expectedVal = 6.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerFloat7)
		{
			CachedMethodDelegate<MethodContainer, float, const float&,
				const float&, int, char, const double&, void*> memberFunction(this, &MethodContainer::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;

			memberFunction.setArgs(forRefValue1, forRefValue2, p3, p4, forRefValue3, p6);

			float expectedVal = 7.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerFloat8)
		{
			CachedMethodDelegate<MethodContainer, float, const float&,
				const float&, int, char, const double&, void*, short> memberFunction(this, &MethodContainer::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;

			memberFunction.setArgs(forRefValue1, forRefValue2, p3, p4, forRefValue3, p6, p7);

			float expectedVal = 8.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerFloat9)
		{
			CachedMethodDelegate<MethodContainer, float, const float&,
				const float&, int, char, const double&, void*, short, int> memberFunction(this, &MethodContainer::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;
			int p8 = 8;

			memberFunction.setArgs(forRefValue1, forRefValue2, p3, p4, forRefValue3, p6, p7, p8);

			float expectedVal = 9.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerGeneral1)
		{
			CachedMethodDelegate<MethodContainer, char> memberFunction(this, &MethodContainer::MemberFooGeneral);

			CachedDelegate* dynamicCaller = &memberFunction;
			dynamicCaller->call();

			FF_EXPECT_EQ((char)1, (char)dynamicCaller->getReturnValAsInt8());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerGeneral2)
		{
			CachedMethodDelegate<MethodContainer, short, int> memberFunction(this, &MethodContainer::MemberFooGeneral);
			memberFunction.setArgs(1);

			CachedDelegate* dynamicCaller = &memberFunction;
			dynamicCaller->call();

			FF_EXPECT_EQ((short)1, (short)dynamicCaller->getReturnValAsInt16());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerGeneral3)
		{
			CachedMethodDelegate<MethodContainer, float, int, char> memberFunction(this, &MethodContainer::MemberFooGeneral);
			memberFunction.setArgs(1, '2');

			CachedDelegate* dynamicCaller = &memberFunction;
			dynamicCaller->call();

			FF_EXPECT_EQ(1.0f, dynamicCaller->getReturnValAsFloat());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerGeneral4)
		{
			CachedMethodDelegate<MethodContainer, int, int, char, const double&> memberFunction(this, &MethodContainer::MemberFooGeneral);

			memberFunction.setArgs(1, '2', 3.0f);

			CachedDelegate* dynamicCaller = &memberFunction;
			dynamicCaller->call();

			FF_EXPECT_EQ(1, dynamicCaller->getReturnValAsInt32());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerGeneral5)
		{
			CachedMethodDelegate<MethodContainer, long, int, char, const double&, const float&> memberFunction(this, &MethodContainer::MemberFooGeneral);

			memberFunction.setArgs(1, '2', 3.0, 4.0f);

			CachedDelegate* dynamicCaller = &memberFunction;
			dynamicCaller->call();

			FF_EXPECT_EQ(1, dynamicCaller->getReturnValAsInt32());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerGeneral6)
		{
			CachedMethodDelegate<MethodContainer, long long, int, char, const double&, const float&, unsigned int> memberFunction(this, &MethodContainer::MemberFooGeneral);

			memberFunction.setArgs(1, '2', 3.0, 4.0f, 5);

			CachedDelegate* dynamicCaller = &memberFunction;
			dynamicCaller->call();

			FF_EXPECT_TRUE(1000000000000000 == dynamicCaller->getReturnValAsInt64());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerGeneral7)
		{
			CachedMethodDelegate<MethodContainer, double, int, char, const double&, const float&, unsigned int, short> memberFunction(this, &MethodContainer::MemberFooGeneral);

			memberFunction.setArgs(1, '2', 3.0, 4.0f, 5, 6);

			CachedDelegate* dynamicCaller = &memberFunction;
			dynamicCaller->call();

			FF_EXPECT_EQ(1000000000000000.0, dynamicCaller->getReturnValAsDouble());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_METHOD(MethodContainer, testCachedMethodContainerGeneral8)
		{
			CachedMethodDelegate<MethodContainer, unsigned short, int, char, const double&, const float&, unsigned int, short, long> memberFunction(this, &MethodContainer::MemberFooGeneral);

			memberFunction.setArgs(1, '2', 3.0, 4.0f, 5, 6, 7);

			CachedDelegate* dynamicCaller = &memberFunction;
			
			dynamicCaller->call();

			FF_EXPECT_EQ(1, (int)dynamicCaller->getReturnValAsInt16());
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
	};
}
