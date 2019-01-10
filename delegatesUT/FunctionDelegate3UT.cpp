/******************************************************************
* File:        FunctionDelegate3UT.cpp
* Description: Test cases invoking functions using FunctionDelegate3
* Author:      Vincent Pham
*
* Copyright (c) 2019 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "../ffscriptUT/fftest.hpp"

#include <functional>
#include "../ffscript/function/CdeclFunction3.hpp"
#include "../ffscriptUT/TemplateForTest.hpp"
#include "FunctionsForTest.h"
#include <math.h>

using namespace std;
using namespace FT;

namespace ffscriptUT
{

#pragma pack(push)
#pragma pack(1)
	struct SampleStruct {
		int a;
		double b;
	};
#pragma pack(pop)

	static void sum2(SampleStruct a, int b) {
		a.b += b;
	}

	static void sum21(SampleStruct a, int b, SampleStruct& c) {
		c.b = a.b + b;
	}

	TEST(FunctionDelegate3, testFunction3_0)
	{
		typedef MemberTypeInfo<0, sizeof(void*), int, SampleStruct, float> AMemberTypeInfo;
		int offset = 0;
		EXPECT_EQ(offset, AMemberTypeInfo::offset<0>());

		offset += sizeof(void*);
		EXPECT_EQ(offset, AMemberTypeInfo::offset<1>());

		constexpr auto alignedSizeOfStruct = sizeof(void*) == 8 ? 16 : 12;
		offset += alignedSizeOfStruct;

		EXPECT_EQ(offset, AMemberTypeInfo::offset<2>());
		offset += sizeof(void*);

		EXPECT_EQ((int)sizeof(void*), AMemberTypeInfo::getSize<0>());
		EXPECT_EQ(alignedSizeOfStruct, AMemberTypeInfo::getSize<1>());
		EXPECT_EQ((int)sizeof(void*), AMemberTypeInfo::getSize<2>());
		EXPECT_EQ(offset, AMemberTypeInfo::totalSize());
	}

	static double sum(int a, float b) {
		return (double)(a + b);
	}

	TEST(FunctionDelegate3, testFunction3_1)
	{
		int p1 = 123;
		float p2 = 456.0f;
		CdelFunction3<double, int, float> cdelFunction(sum);
		DFunction2* nativeFunction2 = &cdelFunction;

		char paramData[sizeof(void*) * 2];
		// argument 1
		*((int*)&paramData[0]) = p1;
		// argument 2
		*((float*)&paramData[sizeof(void*)]) = p2;

		double returnVal;

		nativeFunction2->call(&returnVal, (void**)&paramData[0]);
		EXPECT_EQ(sum(p1, p2), returnVal);
	}

	TEST(FunctionDelegate3, testFunction3_2)
	{
		SampleStruct p1 = { 456, 789.0f };
		int p2 = 123;
		CdelFunction3<void, SampleStruct, int> cdelFunction(sum2);
		DFunction2* nativeFunction2 = &cdelFunction;

		constexpr auto alignedSizeOfStruct = sizeof(void*) == 8 ? 16 : 12;

		char paramData[sizeof(void*) + alignedSizeOfStruct];
		// argument 1
		*((SampleStruct*)&paramData[0]) = p1;
		// argument 2
		*((int*)&paramData[alignedSizeOfStruct]) = p2;
		nativeFunction2->call(nullptr, (void**)&paramData[0]);
	}

	TEST(FunctionDelegate3, testFunction3_3)
	{
		SampleStruct p1 = { 456, 789.0f };
		int p2 = 123;
		SampleStruct p3;
		constexpr auto alignedSizeOfStruct = sizeof(void*) == 8 ? 16 : 12;

		typedef CdelFunction3<void, SampleStruct, int, SampleStruct*> AFunc;

		AFunc cdelFunction((AFunc::Fx)sum21);

		DFunction2* nativeFunction2 = &cdelFunction;

		char paramData[sizeof(void*) * 2 + alignedSizeOfStruct];
		// argument 1
		*((SampleStruct*)&paramData[0]) = p1;
		// argument 2
		*((int*)&paramData[alignedSizeOfStruct]) = p2;
		// argument 3
		*((SampleStruct**)&paramData[alignedSizeOfStruct + sizeof(void*)]) = &p3;

		nativeFunction2->call(nullptr, (void**)&paramData[0]);

		SampleStruct p31;
		sum21(p1, p2, p31);

		EXPECT_EQ(p31.b, p3.b);
	}

	TEST(FunctionDelegate3, testFunction3_new_1)
	{
		double p2 = 789;
		int p1 = 123;
		CdelFunction3<double, int&, double> cdelFunction(sum);
		DFunction2* nativeFunction2 = &cdelFunction;

		char paramData[sizeof(void*) + sizeof(double)];
		// argument 1
		*((int**)&paramData[0]) = &p1;
		// argument 2
		*((double*)&paramData[sizeof(void*)]) = p2;

		double ret;
		nativeFunction2->call(&ret, (void**)&paramData[0]);

		EXPECT_EQ(sum(p1, p2), ret);
	}

	TEST(FunctionDelegate3, testFunction3_new_2)
	{	
		CdelFunction3<void> cdelFunction(doNothing);
		DFunction2* nativeFunction2 = &cdelFunction;
		nativeFunction2->call(nullptr, nullptr);
	}

	TEST(FunctionDelegate3, testFunction3_new_3)
	{
		CdelFunction3<int> cdelFunction(returnConstant);
		DFunction2* nativeFunction2 = &cdelFunction;

		int ret;
		nativeFunction2->call(&ret, nullptr);
		EXPECT_EQ(returnConstant(), ret);
	}
}
