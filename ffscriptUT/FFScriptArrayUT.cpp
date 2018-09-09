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

#include "stdafx.h"
#include "CppUnitTest.h"
#include "FFScriptArray.hpp"
#include "function\CdeclFunction2.hpp"
#include "function\MemberFunction2Ref.hpp"
#include "function\MemberFunction2.hpp"

using namespace ffscript;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace ffscriptUT
{		
	TEST_CLASS(FFScriptArrayUT)
	{
	public:
		TEST_METHOD(CheckSizeOfArray) {
			size_t sizeOfArray = sizeof(FFScriptArray<int>);
			Assert::AreEqual(4*sizeof(void*), sizeOfArray, L"Size of FFScriptArray should be 16");
		}

		TEST_METHOD(TestConstructFromStaticArray)
		{
			{
				int data[] = { 1, 4, 9, 3 };
				FFScriptArray<int> a(data, sizeof(data) / sizeof(data[0]));

				a[0] = 0;
				a[1] = 2;

				Assert::AreEqual(data[0], a[0], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(data[1], a[1], L"operator [] of FFScriptArray works incorrectly");
			}
		}

		TEST_METHOD(TestConstructorDynamic)
		{
			{
				FFScriptArray<int> a(5);
				a[0] = 0;
				a[1] = 1;
				a[2] = 2;
				a[3] = 3;
				a[4] = 4;

				Assert::AreEqual(0, a[0], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(1, a[1], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(2, a[2], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(3, a[3], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(4, a[4], L"operator [] of FFScriptArray works incorrectly");
			}
		}

		TEST_METHOD(TestResize)
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

				Assert::AreEqual(0, a[0], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(1, a[1], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(2, a[2], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(3, a[3], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(4, a[4], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(5, a[5], L"operator [] of FFScriptArray works incorrectly");
			}
		}

		TEST_METHOD(TestSort1)
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

				Assert::AreEqual(0, a[0], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(1, a[1], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(2, a[2], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(3, a[3], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(4, a[4], L"operator [] of FFScriptArray works incorrectly");
			}
		}

		TEST_METHOD(TestSort2)
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

				Assert::AreEqual(4, a[0], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(3, a[1], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(2, a[2], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(1, a[3], L"operator [] of FFScriptArray works incorrectly");
				Assert::AreEqual(0, a[4], L"operator [] of FFScriptArray works incorrectly");
			}
		}

		TEST_METHOD(TestPosibilityRunInScript1)
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

			Assert::AreEqual(a[i], *ret, L"MFunction2Ref invoke operator[] incorrectly");
		}

		TEST_METHOD(TestPosibilityRunInScript2)
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

			Assert::AreEqual(a[i], *ret, L"MFunction2Ref invoke operator[] incorrectly");
		}

		TEST_METHOD(TestPosibilityRunInScript3)
		{
			FFScriptArray<__int64> a(5);
			a[0] = 0;
			a[1] = 2;
			a[2] = 1;
			a[3] = 3;
			a[4] = 4;

			void* pMethod = MFunction2<void, FFScriptArray<__int64>>::getMethodAddress(&FFScriptArray<__int64>::sort);
			MFunction2<void, FFScriptArray<int>> mf((FFScriptArray<int>*)&a, pMethod);

			mf.call(nullptr, nullptr);

			Assert::IsTrue((__int64)0 == a[0], L"MFunction2Ref invoke operator[] incorrectly");
			Assert::IsTrue((__int64)1 == a[1], L"MFunction2Ref invoke operator[] incorrectly");
			Assert::IsTrue((__int64)2 == a[2], L"MFunction2Ref invoke operator[] incorrectly");
			Assert::IsTrue((__int64)3 == a[3], L"MFunction2Ref invoke operator[] incorrectly");
			Assert::IsTrue((__int64)4 == a[4], L"MFunction2Ref invoke operator[] incorrectly");
		}
	};
}