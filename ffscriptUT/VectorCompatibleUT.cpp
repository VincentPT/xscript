/******************************************************************
* File:        VectorCompatibleUT.cpp
* Description: Test cases check using SimpleArray with some standard
*              c++ functions.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "fftest.hpp"

#include "ffscript.h"
#include <vector>
#include <algorithm>

using namespace ffscript;
using namespace std;

namespace ffscriptUT
{
	namespace VectorCompatibleUT
	{
		FF_TEST_FUNCTION(VectorCompatible, CompatibleValidateInt)
		{
			SimpleArray<int> intArray;
			int arr[] = { 1, 4, 9, 3 };
			intArray.elms = arr;
			intArray.size = sizeof(arr) / sizeof(arr[0]);
			std::sort(intArray.elms, intArray.elms + intArray.size);

			FF_EXPECT_EQ(1, arr[0]);
			FF_EXPECT_EQ(3, arr[1]);
			FF_EXPECT_EQ(4, arr[2]);
			FF_EXPECT_EQ(9, arr[3]);
		}

		FF_TEST_FUNCTION(VectorCompatible, CompatibleValidateFloat)
		{
			SimpleArray<float> floatArray;
			float arr[] = { 1, 4, 9, 3 };
			floatArray.elms = arr;
			floatArray.size = sizeof(arr) / sizeof(arr[0]);
			std::sort(floatArray.elms, floatArray.elms + floatArray.size);

			FF_EXPECT_EQ(1.0f, arr[0]);
			FF_EXPECT_EQ(3.0f, arr[1]);
			FF_EXPECT_EQ(4.0f, arr[2]);
			FF_EXPECT_EQ(9.0f, arr[3]);
		}

		FF_TEST_FUNCTION(VectorCompatible, CompatibleValidateDouble)
		{
			SimpleArray<double> floatArray;
			double arr[] = { 1, 4, 9, 3 };
			floatArray.elms = arr;
			floatArray.size = sizeof(arr) / sizeof(arr[0]);
			std::sort(floatArray.elms, floatArray.elms + floatArray.size);

			FF_EXPECT_EQ(1.0, arr[0]);
			FF_EXPECT_EQ(3.0, arr[1]);
			FF_EXPECT_EQ(4.0, arr[2]);
			FF_EXPECT_EQ(9.0, arr[3]);
		}
	};
}