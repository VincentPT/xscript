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
#include "../ffscript/function/MemberFunction.hpp"
#include "../ffscriptUT/TemplateForTest.hpp"
#include "MethodsForTest.h"
#include <math.h>

using namespace std;

namespace ffscriptUT
{
	namespace MethodContainerUT {
		FF_TEST_METHOD(MethodContainer, testMethodContainerVoid1)
		{
			MFunction<void, MethodContainer> memberFunction(this, &MethodContainer::memberFoo);
			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerVoid2)
		{
			MFunction<void, MethodContainer, const float&> memberFunction(this, &MethodContainer::memberFoo);

			float forRefValue1 = 1.0f;		

			pushParams(&memberFunction, &forRefValue1);

			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerVoid3)
		{
			MFunction<void, MethodContainer, const float&, const float&> memberFunction(this, &MethodContainer::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2);

			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerVoid4)
		{
			MFunction<void, MethodContainer, const float&, const float&, int> memberFunction(this, &MethodContainer::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3);

			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerVoid5)
		{
			MFunction<void, MethodContainer, const float&, const float&, int, char> memberFunction(this, &MethodContainer::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4);

			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerVoid6)
		{
			MFunction<void, MethodContainer, const float&,
				const float&, int, char, const double&> memberFunction(this, &MethodContainer::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3);

			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerVoid7)
		{
			MFunction<void, MethodContainer, const float&,
				const float&, int, char, const double&, void*> memberFunction(this, &MethodContainer::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6);

			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerVoid8)
		{
			MFunction<void, MethodContainer, const float&,
				const float&, int, char, const double&, void*, short> memberFunction(this, &MethodContainer::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7);

			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerVoid9)
		{
			MFunction<void, MethodContainer, const float&,
				const float&, int, char, const double&, void*, short, int> memberFunction(this, &MethodContainer::memberFoo);
			
			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;
			int p8 = 8;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7, p8);

			memberFunction.call();
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerFloat1)
		{
			MFunction<float, MethodContainer> memberFunction(this, &MethodContainer::memberFoof);

			float expectedVal = 1.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerFloat2)
		{
			MFunction<float, MethodContainer, const float&> memberFunction(this, &MethodContainer::memberFoof);

			float forRefValue1 = 1.0f;

			pushParams(&memberFunction, &forRefValue1);

			float expectedVal = 2.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerFloat3)
		{
			MFunction<float, MethodContainer, const float&, const float&> memberFunction(this, &MethodContainer::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2);

			float expectedVal = 3.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerFloat4)
		{
			MFunction<float, MethodContainer, const float&, const float&, int> memberFunction(this, &MethodContainer::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3);

			float expectedVal = 4.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerFloat5)
		{
			MFunction<float, MethodContainer, const float&, const float&, int, char> memberFunction(this, &MethodContainer::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4);

			float expectedVal = 5.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerFloat6)
		{
			MFunction<float, MethodContainer, const float&,
				const float&, int, char, const double&> memberFunction(this, &MethodContainer::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3);

			float expectedVal = 6.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerFloat7)
		{
			MFunction<float, MethodContainer, const float&,
				const float&, int, char, const double&, void*> memberFunction(this, &MethodContainer::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6);

			float expectedVal = 7.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerFloat8)
		{
			MFunction<float, MethodContainer, const float&,
				const float&, int, char, const double&, void*, short> memberFunction(this, &MethodContainer::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7);

			float expectedVal = 8.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerFloat9)
		{
			MFunction<float, MethodContainer, const float&,
				const float&, int, char, const double&, void*, short, int> memberFunction(this, &MethodContainer::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;
			int p8 = 8;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7, p8);

			float expectedVal = 9.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			FF_EXPECT_EQ(expectedVal, actualVal);
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_METHOD(MethodContainer, testMethodContainerGeneral1)
		{
			MFunction<char, MethodContainer> memberFunction(this, &MethodContainer::MemberFooGeneral);

			DFunction* dynamicCaller = &memberFunction;
			dynamicCaller->call();

			FF_EXPECT_EQ((char)1, (char)dynamicCaller->getReturnValAsInt8());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_METHOD(MethodContainer, testMethodContainerGeneral2)
		{
			MFunction<short, MethodContainer, int> memberFunction(this, &MethodContainer::MemberFooGeneral);

			DFunction* dynamicCaller = &memberFunction;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->call();

			FF_EXPECT_EQ((short)1, (short)dynamicCaller->getReturnValAsInt16());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_METHOD(MethodContainer, testMethodContainerGeneral3)
		{
			MFunction<float, MethodContainer, int, char> memberFunction(this, &MethodContainer::MemberFooGeneral);

			DFunction* dynamicCaller = &memberFunction;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->call();

			FF_EXPECT_EQ(1.0f, dynamicCaller->getReturnValAsFloat());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_METHOD(MethodContainer, testMethodContainerGeneral4)
		{
			MFunction<int, MethodContainer, int, char, const double&> memberFunction(this, &MethodContainer::MemberFooGeneral);

			double p3 = 3.0f;

			DFunction* dynamicCaller = &memberFunction;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->pushParam((void*)&p3);
			dynamicCaller->call();

			FF_EXPECT_EQ(1, dynamicCaller->getReturnValAsInt32());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_METHOD(MethodContainer, testMethodContainerGeneral5)
		{
			MFunction<long, MethodContainer, int, char, const double&, const float&> memberFunction(this, &MethodContainer::MemberFooGeneral);

			double p3 = 3.0f;
			float p4 = 4.0f;

			DFunction* dynamicCaller = &memberFunction;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->pushParam((void*)&p3);
			dynamicCaller->pushParam((void*)&p4);
			dynamicCaller->call();

			FF_EXPECT_EQ(1, dynamicCaller->getReturnValAsInt32());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_METHOD(MethodContainer, testMethodContainerGeneral6)
		{
			MFunction<long long, MethodContainer, int, char, const double&, const float&, unsigned int> memberFunction(this, &MethodContainer::MemberFooGeneral);

			double p3 = 3.0f;
			float p4 = 4.0f;

			DFunction* dynamicCaller = &memberFunction;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->pushParam((void*)&p3);
			dynamicCaller->pushParam((void*)&p4);
			dynamicCaller->pushParam((void*)5);
			dynamicCaller->call();

			FF_EXPECT_TRUE(1000000000000000 == dynamicCaller->getReturnValAsInt64());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_METHOD(MethodContainer, testMethodContainerGeneral7)
		{
			MFunction<double, MethodContainer, int, char, const double&, const float&, unsigned int, short> memberFunction(this, &MethodContainer::MemberFooGeneral);

			double p3 = 3.0f;
			float p4 = 4.0f;

			DFunction* dynamicCaller = &memberFunction;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->pushParam((void*)&p3);
			dynamicCaller->pushParam((void*)&p4);
			dynamicCaller->pushParam((void*)5);
			dynamicCaller->pushParam((void*)6);
			dynamicCaller->call();

			FF_EXPECT_EQ(1000000000000000.0, dynamicCaller->getReturnValAsDouble());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_METHOD(MethodContainer, testMethodContainerGeneral8)
		{
			MFunction<unsigned short, MethodContainer, int, char, const double&, const float&, unsigned int, short, long> memberFunction(this, &MethodContainer::MemberFooGeneral);

			double p3 = 3.0f;
			float p4 = 4.0f;

			DFunction* dynamicCaller = &memberFunction;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->pushParam((void*)&p3);
			dynamicCaller->pushParam((void*)&p4);
			dynamicCaller->pushParam((void*)5);
			dynamicCaller->pushParam((void*)6);
			dynamicCaller->pushParam((void*)7);
			dynamicCaller->call();

			FF_EXPECT_EQ(1, (int)dynamicCaller->getReturnValAsInt16());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_METHOD(MethodContainer, testMethodContainerGeneral9)
		{
			MFunction<unsigned short, MethodContainer, int, char, const double&, const float&, unsigned int, short, long> memberFunction(this, &MethodContainer::MemberFooGeneral);

			double p3 = 3.0f;
			float p4 = 4.0f;

			DFunction* dynamicCaller = &memberFunction;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->pushParam((void*)&p3);
			dynamicCaller->pushParam((void*)&p4);
			dynamicCaller->pushParam((void*)5);
			dynamicCaller->pushParam((void*)6);
			dynamicCaller->pushParam((void*)7);
			dynamicCaller->call();

			FF_EXPECT_EQ(1, (int)dynamicCaller->getReturnValAsInt16());

			DFunction* pNewInsance = dynamicCaller->clone();
			pNewInsance->call();

			FF_EXPECT_EQ(1, (int)pNewInsance->getReturnValAsInt16());
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerCdecl1)
		{
			Function<double, double> function(::sin);
			double p = 1.0f;
			double expect = ::sin(p);

			function.pushParam(&p);
			function.call();

			double val1 = function.getReturnValAsDouble();
			FF_EXPECT_EQ(expect, val1);
		}

		static double cdelfoo(const double& d) {
			return d;
		}


		FF_TEST_METHOD(MethodContainer, testMethodContainerCdecl2)
		{
			Function<double, const double&> function(cdelfoo);
			double p = 1.0f;
			double expect = cdelfoo(p);

			function.pushParam(&p);
			function.call();

			double val1 = function.getReturnValAsDouble();
			FF_EXPECT_EQ(expect, val1);
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerMember1)
		{
			MFunctionW <void, MethodContainer, float> function(this, &MethodContainer::memberFoo);
			double p = 1.0f;
			function.pushParam(&p);
			function.call();
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerMember2)
		{
			MFunctionW <double, MethodContainer, float, double> function(this, &MethodContainer::memberFoo);
			float p = 1.0f;
			double q = 1.0f;
			double expect = memberFoo(p, q);

			function.pushParam(&p);
			function.pushParam(&q);
			function.call();

			double val1 = function.getReturnValAsDouble();
			FF_EXPECT_EQ(expect, val1);
		}

		FF_TEST_METHOD(MethodContainer, testMethodContainerMember3)
		{
			MFunctionW <double, MethodContainer, float, double> function(this, &MethodContainer::memberFoo);
			float p = 1.0f;
			double q = 1.0f;
			double expect = memberFoo(p, q);

			function.pushParam(&p);
			function.pushParam(&q);
			function.call();

			DFunction* newInstance = function.clone();
			newInstance->call();
			double val2 = newInstance->getReturnValAsDouble();

			double val1 = function.getReturnValAsDouble();
			FF_EXPECT_EQ(expect, val1);
			FF_EXPECT_EQ(expect, val2);
		}
	};
}
