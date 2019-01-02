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
#include "fftest.hpp"

#include "template/TemplateBlock.hpp"
#include <functional>

namespace ffscriptUT
{		
	namespace MemoryblockUT
	{
		FF_TEST_FUNCTION(MemoryblockTest, CheckSize1)
		{	
			int size = sizeof(MemoryBlock<1>);
			FF_EXPECT_EQ(1, size);
		}

		FF_TEST_FUNCTION(MemoryblockTest, CheckSize2)
		{
			int size = sizeof(MemoryBlock<2>);
			FF_EXPECT_EQ(2, size);
		}

		FF_TEST_FUNCTION(MemoryblockTest, CheckSize4)
		{
			int size = sizeof(MemoryBlock<4>);
			FF_EXPECT_EQ(4, size);
		}

		FF_TEST_FUNCTION(MemoryblockTest, CheckSize6)
		{
			int size = sizeof(MemoryBlock<6>);
			FF_EXPECT_EQ(6, size);
		}

		FF_TEST_FUNCTION(MemoryblockTest, CheckSize7)
		{
			int size = sizeof(MemoryBlock<7>);
			FF_EXPECT_EQ(7, size);
		}

		FF_TEST_FUNCTION(MemoryblockTest, CheckSize8)
		{
			int size = sizeof(MemoryBlock<8>);
			FF_EXPECT_EQ(8, size);
		}

		FF_TEST_FUNCTION(MemoryblockTest, CheckSize9)
		{
			int size = sizeof(MemoryBlock<9>);
			FF_EXPECT_EQ(9, size);
		}
	};
}