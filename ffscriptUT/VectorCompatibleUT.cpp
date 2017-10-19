#include "stdafx.h"
#include "CppUnitTest.h"
#include "ffscript.h"
#include <vector>
#include <algorithm>

using namespace ffscript;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace ffscriptUT
{
	TEST_CLASS(VectorCompatibleUT)
	{
	public:
		TEST_METHOD(CompatibleValidateInt)
		{
			SimpleArray<int> intArray;
			int arr[] = { 1, 4, 9, 3 };
			intArray.elms = arr;
			intArray.size = sizeof(arr) / sizeof(arr[0]);
			std::sort(intArray.elms, intArray.elms + intArray.size);

			Assert::AreEqual(1, arr[0]);
			Assert::AreEqual(3, arr[1]);
			Assert::AreEqual(4, arr[2]);
			Assert::AreEqual(9, arr[3]);
		}

		TEST_METHOD(CompatibleValidateFloat)
		{
			SimpleArray<float> floatArray;
			float arr[] = { 1, 4, 9, 3 };
			floatArray.elms = arr;
			floatArray.size = sizeof(arr) / sizeof(arr[0]);
			std::sort(floatArray.elms, floatArray.elms + floatArray.size);

			Assert::AreEqual(1.0f, arr[0]);
			Assert::AreEqual(3.0f, arr[1]);
			Assert::AreEqual(4.0f, arr[2]);
			Assert::AreEqual(9.0f, arr[3]);
		}

		TEST_METHOD(CompatibleValidateDouble)
		{
			SimpleArray<double> floatArray;
			double arr[] = { 1, 4, 9, 3 };
			floatArray.elms = arr;
			floatArray.size = sizeof(arr) / sizeof(arr[0]);
			std::sort(floatArray.elms, floatArray.elms + floatArray.size);

			Assert::AreEqual(1.0, arr[0]);
			Assert::AreEqual(3.0, arr[1]);
			Assert::AreEqual(4.0, arr[2]);
			Assert::AreEqual(9.0, arr[3]);
		}
	};
}