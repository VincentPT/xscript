#include "stdafx.h"
#include "CppUnitTest.h"
#include "template\TemplateBlock.hpp"
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ffscriptUT
{		
	TEST_CLASS(MemoryblockChecker)
	{
	public:
		TEST_METHOD(CheckSize1)
		{	
			int size = sizeof(MemoryBlock<1>);
			Assert::AreEqual(1, size);
		}

		TEST_METHOD(CheckSize2)
		{
			int size = sizeof(MemoryBlock<2>);
			Assert::AreEqual(2, size);
		}

		TEST_METHOD(CheckSize4)
		{
			int size = sizeof(MemoryBlock<4>);
			Assert::AreEqual(4, size);
		}

		TEST_METHOD(CheckSize6)
		{
			int size = sizeof(MemoryBlock<6>);
			Assert::AreEqual(6, size);
		}

		TEST_METHOD(CheckSize7)
		{
			int size = sizeof(MemoryBlock<7>);
			Assert::AreEqual(7, size);
		}

		TEST_METHOD(CheckSize8)
		{
			int size = sizeof(MemoryBlock<8>);
			Assert::AreEqual(8, size);
		}

		TEST_METHOD(CheckSize9)
		{
			int size = sizeof(MemoryBlock<9>);
			Assert::AreEqual(9, size);
		}
	};
}