#include "stdafx.h"
#include "CppUnitTest.h"
#include "function/FunctionHelper.hpp"
#include "TemplateForTest.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace FT;

namespace ffscriptUT
{
	TEST_CLASS(FunctionHelperUT)
	{
	public:
		TEST_METHOD(CheckHasReference_0)
		{
			auto val = FT::has_reference<>::value;
			Assert::AreEqual(val, false);
		}

		TEST_METHOD(CheckHasReference_1)
		{
			auto val = FT::has_reference<int, int, float, char>::value;
			Assert::AreEqual(val, false);
		}

		TEST_METHOD(CheckHasReference_2)
		{
			auto val = FT::has_reference<int, int, float*, char>::value;
			Assert::AreEqual(val, false);
		}

		TEST_METHOD(CheckHasReference_3)
		{
			auto val = FT::has_reference<int, int, float&, char>::value;
			Assert::AreEqual(val, true);
		}

		TEST_METHOD(CheckHasReference_4)
		{
			auto val = FT::has_reference<int, int&&, float*, char>::value;
			Assert::AreEqual(val, true);
		}

		TEST_METHOD(CheckHasReference_5)
		{
			auto val = FT::has_reference<int*&, int, float, char>::value;
			Assert::AreEqual(val, true);
		}

		TEST_METHOD(CheckHasType_0)
		{
			auto val = FT::has_type<int>::value;
			Assert::AreEqual(val, false);
		}

		TEST_METHOD(CheckHasType_1)
		{
			auto val = FT::has_type<int, int&>::value;
			Assert::AreEqual(val, false);
		}

		TEST_METHOD(CheckHasType_2)
		{
			auto val = FT::has_type<int, int>::value;
			Assert::AreEqual(val, true);
		}

		TEST_METHOD(CheckHasType_3)
		{
			auto val = FT::has_type<float, char, float>::value;
			Assert::AreEqual(val, true);
		}

		TEST_METHOD(CheckHasType_4)
		{
			auto val = FT::has_type<double, char, float, double*>::value;
			Assert::AreEqual(val, false);
		}

		TEST_METHOD(CheckHasType_5)
		{
			auto val = FT::has_type<double, char, float, double, int>::value;
			Assert::AreEqual(val, true);
		}

		TEST_METHOD(CheckNoExceedStackUnitSize_1)
		{
			auto val = FT::no_exceed_uts<int>::value;
			Assert::AreEqual(val, true);
		}

		TEST_METHOD(CheckNoEceedStackUnitSize_2)
		{
			auto val = FT::no_exceed_uts<float*>::value;
			Assert::AreEqual(val, true);
		}

		TEST_METHOD(CheckNoEceedStackUnitSize_3)
		{
			struct AStruct {
				double a;
				int b;
			};
			auto val = FT::no_exceed_uts<double*, AStruct>::value;
			Assert::AreEqual(val, false);
		}

		TEST_METHOD(CheckNoEceedStackUnitSize_4)
		{
			struct AStruct {
				double a;
				int b;
			};
			auto val = FT::no_exceed_uts<double*, int, AStruct>::value;
			Assert::AreEqual(val, false);
		}

		TEST_METHOD(CheckMakePointerType_1)
		{
			auto val = std::is_same<int*, make_pointer<int>::ptr_type>::value;
			Assert::AreEqual(val, true);
		}

		TEST_METHOD(CheckMakePointerType_2)
		{
			auto val = std::is_same<double*, make_pointer<double&>::ptr_type>::value;
			Assert::AreEqual(val, true);
		}

		TEST_METHOD(CheckMakePointerType_3)
		{
			auto val = std::is_same<double&, make_pointer<double&>::ptr_type>::value;
			Assert::AreEqual(val, false);
		}

		TEST_METHOD(CheckMakePointerType_4)
		{
			auto val = std::is_same<long long, make_pointer<long long>::ptr_type>::value;
			Assert::AreEqual(val, false);
		}
	};
}
