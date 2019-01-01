/******************************************************************
* File:        MemoryblockUT.cpp
* Description: Test cases focus on checking size of MemoryBlock
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "CppUnitTest.h"
#include "template\TemplateBlock.hpp"
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ffscriptUT
{		
	TEST_CLASS(MemoryblockUT)
	{
	public:
		TEST_METHOD(CheckSize1)
		{	
			int size = sizeof(MemoryBlock<1>);
			EXPECT_EQ(1, size);
		}

		TEST_METHOD(CheckSize2)
		{
			int size = sizeof(MemoryBlock<2>);
			EXPECT_EQ(2, size);
		}

		TEST_METHOD(CheckSize4)
		{
			int size = sizeof(MemoryBlock<4>);
			EXPECT_EQ(4, size);
		}

		TEST_METHOD(CheckSize6)
		{
			int size = sizeof(MemoryBlock<6>);
			EXPECT_EQ(6, size);
		}

		TEST_METHOD(CheckSize7)
		{
			int size = sizeof(MemoryBlock<7>);
			EXPECT_EQ(7, size);
		}

		TEST_METHOD(CheckSize8)
		{
			int size = sizeof(MemoryBlock<8>);
			EXPECT_EQ(8, size);
		}

		TEST_METHOD(CheckSize9)
		{
			int size = sizeof(MemoryBlock<9>);
			EXPECT_EQ(9, size);
		}
	};
}