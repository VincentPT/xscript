#include "fftest.hpp"

#include "function/FunctionHelper.hpp"
#include "TemplateForTest.hpp"

using namespace std;
using namespace FT;

namespace ffscriptUT
{
	namespace FunctionHelperUT
	{
		FF_TEST_FUNCTION(FunctionHelperTest, CheckHasReference_0)
		{
			auto val = FT::has_reference<>::value;
			FF_EXPECT_EQ(val, false);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckHasReference_1)
		{
			auto val = FT::has_reference<int, int, float, char>::value;
			FF_EXPECT_EQ(val, false);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckHasReference_2)
		{
			auto val = FT::has_reference<int, int, float*, char>::value;
			FF_EXPECT_EQ(val, false);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckHasReference_3)
		{
			auto val = FT::has_reference<int, int, float&, char>::value;
			FF_EXPECT_EQ(val, true);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckHasReference_4)
		{
			auto val = FT::has_reference<int, int&&, float*, char>::value;
			FF_EXPECT_EQ(val, true);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckHasReference_5)
		{
			auto val = FT::has_reference<int*&, int, float, char>::value;
			FF_EXPECT_EQ(val, true);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckHasType_0)
		{
			auto val = FT::has_type<int>::value;
			FF_EXPECT_EQ(val, false);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckHasType_1)
		{
			auto val = FT::has_type<int, int&>::value;
			FF_EXPECT_EQ(val, false);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckHasType_2)
		{
			auto val = FT::has_type<int, int>::value;
			FF_EXPECT_EQ(val, true);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckHasType_3)
		{
			auto val = FT::has_type<float, char, float>::value;
			FF_EXPECT_EQ(val, true);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckHasType_4)
		{
			auto val = FT::has_type<double, char, float, double*>::value;
			FF_EXPECT_EQ(val, false);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckHasType_5)
		{
			auto val = FT::has_type<double, char, float, double, int>::value;
			FF_EXPECT_EQ(val, true);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckNoExceedStackUnitSize_1)
		{
			auto val = FT::no_exceed_uts<int>::value;
			FF_EXPECT_EQ(val, true);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckNoEceedStackUnitSize_2)
		{
			auto val = FT::no_exceed_uts<float*>::value;
			FF_EXPECT_EQ(val, true);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckNoEceedStackUnitSize_3)
		{
			struct AStruct {
				double a;
				int b;
			};
			auto val = FT::no_exceed_uts<double*, AStruct>::value;
			FF_EXPECT_EQ(val, false);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckNoEceedStackUnitSize_4)
		{
			struct AStruct {
				double a;
				int b;
			};
			auto val = FT::no_exceed_uts<double*, int, AStruct>::value;
			FF_EXPECT_EQ(val, false);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckMakePointerType_1)
		{
			auto val = std::is_same<int*, make_pointer<int>::ptr_type>::value;
			FF_EXPECT_EQ(val, true);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckMakePointerType_2)
		{
			auto val = std::is_same<double*, make_pointer<double&>::ptr_type>::value;
			FF_EXPECT_EQ(val, true);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckMakePointerType_3)
		{
			auto val = std::is_same<double&, make_pointer<double&>::ptr_type>::value;
			FF_EXPECT_EQ(val, false);
		}

		FF_TEST_FUNCTION(FunctionHelperTest, CheckMakePointerType_4)
		{
			auto val = std::is_same<long long, make_pointer<long long>::ptr_type>::value;
			FF_EXPECT_EQ(val, false);
		}
	};
}
