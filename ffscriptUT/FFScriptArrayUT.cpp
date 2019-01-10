/******************************************************************
* File:        FFScriptArrayUT.cpp
* Description: Test cases of FFScriptArrayUT class
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "fftest.hpp"

#include "FFScriptArray.hpp"
#include "function/FunctionDelegate2.hpp"
#include "function/MemberFunction2Ref.hpp"
#include "function/MemberFunction2.hpp"

using namespace ffscript;
using namespace std;

namespace ffscriptUT
{		
	namespace FFScriptArrayUT
	{
		FF_TEST_FUNCTION(FFScriptArrayTest, CheckSizeOfArray) {
			size_t sizeOfArray = sizeof(FFScriptArray<int>);
			FF_EXPECT_EQ(4*sizeof(void*), sizeOfArray, L"Size of FFScriptArray should be 16");
		}

		FF_TEST_FUNCTION(FFScriptArrayTest, TestConstructFromStaticArray)
		{
			{
				int data[] = { 1, 4, 9, 3 };
				FFScriptArray<int> a(data, sizeof(data) / sizeof(data[0]));

				a[0] = 0;
				a[1] = 2;

				FF_EXPECT_EQ(data[0], a[0], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(data[1], a[1], L"operator [] of FFScriptArray works incorrectly");
			}
		}

		FF_TEST_FUNCTION(FFScriptArrayTest, TestConstructorDynamic)
		{
			{
				FFScriptArray<int> a(5);
				a[0] = 0;
				a[1] = 1;
				a[2] = 2;
				a[3] = 3;
				a[4] = 4;

				FF_EXPECT_EQ(0, a[0], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(1, a[1], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(2, a[2], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(3, a[3], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(4, a[4], L"operator [] of FFScriptArray works incorrectly");
			}
		}

		FF_TEST_FUNCTION(FFScriptArrayTest, TestResize)
		{
			{
				FFScriptArray<int> a(5);
				a[0] = 0;
				a[1] = 1;
				a[2] = 2;
				a[3] = 3;
				a[4] = 4;

				a.resize(6);
				a[5] = 5;

				FF_EXPECT_EQ(0, a[0], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(1, a[1], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(2, a[2], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(3, a[3], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(4, a[4], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(5, a[5], L"operator [] of FFScriptArray works incorrectly");
			}
		}

		FF_TEST_FUNCTION(FFScriptArrayTest, TestSort1)
		{
			{
				function<bool(const int&, const int&)> f;
				FFScriptArray<int> a(5);
				a[0] = 0;
				a[1] = 2;
				a[2] = 1;
				a[3] = 3;
				a[4] = 4;

				a.sort();

				FF_EXPECT_EQ(0, a[0], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(1, a[1], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(2, a[2], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(3, a[3], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(4, a[4], L"operator [] of FFScriptArray works incorrectly");
			}
		}

		FF_TEST_FUNCTION(FFScriptArrayTest, TestSort2)
		{
			{
				FFScriptArray<int> a(5);
				a[0] = 0;
				a[1] = 2;
				a[2] = 1;
				a[3] = 3;
				a[4] = 4;

				a.sort([](const int& a, const int& b) -> bool {
					return b < a;
				});

				FF_EXPECT_EQ(4, a[0], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(3, a[1], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(2, a[2], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(1, a[3], L"operator [] of FFScriptArray works incorrectly");
				FF_EXPECT_EQ(0, a[4], L"operator [] of FFScriptArray works incorrectly");
			}
		}

		FF_TEST_FUNCTION(FFScriptArrayTest, TestPosibilityRunInScript1)
		{
			FFScriptArray<int> a(5);
			a[0] = 0;
			a[1] = 2;
			a[2] = 1;
			a[3] = 3;
			a[4] = 4;

			MFunction2Ref<int&, FFScriptArray<int>, int> mf(&a, &FFScriptArray<int>::operator[]);
			int* ret;
			int i = 1;
			void* params[] = { (void*)(size_t) i};
			mf.call(&ret, params);

			FF_EXPECT_EQ(a[i], *ret, L"MFunction2Ref invoke operator[] incorrectly");
		}

		FF_TEST_FUNCTION(FFScriptArrayTest, TestPosibilityRunInScript2)
		{
			FFScriptArray<double> a(5);
			a[0] = 0;
			a[1] = 2;
			a[2] = 1;
			a[3] = 3;
			a[4] = 4;

			void* pMethod = MFunction2Ref<double&, FFScriptArray<double>, int>::getMethodAddress(&FFScriptArray<double>::operator[]);
			MFunction2<void*, FFScriptArray<int>, int> mf((FFScriptArray<int>*)&a, pMethod);

			double* ret;
			int i = 1;
			void* params[] = {(void*)(size_t)i };
			mf.call(&ret, params);

			FF_EXPECT_EQ(a[i], *ret, L"MFunction2Ref invoke operator[] incorrectly");
		}

		FF_TEST_FUNCTION(FFScriptArrayTest, TestPosibilityRunInScript3)
		{
			FFScriptArray<long long> a(5);
			a[0] = 0;
			a[1] = 2;
			a[2] = 1;
			a[3] = 3;
			a[4] = 4;

			void* pMethod = MFunction2<void, FFScriptArray<long long>>::getMethodAddress(&FFScriptArray<long long>::sort);
			MFunction2<void, FFScriptArray<int>> mf((FFScriptArray<int>*)&a, pMethod);

			mf.call(nullptr, nullptr);

			FF_EXPECT_TRUE((long long)0 == a[0], L"MFunction2Ref invoke operator[] incorrectly");
			FF_EXPECT_TRUE((long long)1 == a[1], L"MFunction2Ref invoke operator[] incorrectly");
			FF_EXPECT_TRUE((long long)2 == a[2], L"MFunction2Ref invoke operator[] incorrectly");
			FF_EXPECT_TRUE((long long)3 == a[3], L"MFunction2Ref invoke operator[] incorrectly");
			FF_EXPECT_TRUE((long long)4 == a[4], L"MFunction2Ref invoke operator[] incorrectly");
		}
	};
}