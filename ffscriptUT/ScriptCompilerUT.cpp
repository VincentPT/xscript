/******************************************************************
* File:        ScriptCompilerUT.cpp
* Description: Test cases for using ScriptCompiler to register types
*              and functions in a C Lambda application.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "fftest.hpp"

#include "../ffscript/ScriptCompiler.h"
#include "BasicFunction.h"
#include "FunctionFactory.h"
#include "BasicType.h"

using namespace std;
using namespace ffscript;

namespace ffscriptUT
{
	namespace ScriptCompilerUT
	{
		FF_TEST_FUNCTION(ScriptCompilerTest,testScriptCompiler1)
		{
			ScriptCompiler scriptCompiler;
			string type = "int";
			int id = scriptCompiler.registType(type);
			auto registType = scriptCompiler.getType(id);
			
			//regist type must be success
			FF_EXPECT_NE(-1, id);
			//registered type must be same the type using to regist
			FF_EXPECT_EQ(type, registType);
		}

		FF_TEST_FUNCTION(ScriptCompilerTest,testScriptCompiler2)
		{
			ScriptCompiler scriptCompiler;
			string type = "int";
			int id = scriptCompiler.registType(type);
			id = scriptCompiler.registType(type);			

			//regist type must be failed at second time
			FF_EXPECT_TRUE(IS_UNKNOWN_TYPE(id));
		}

		FF_TEST_FUNCTION(ScriptCompilerTest,testScriptCompiler3)
		{
			ScriptCompiler scriptCompiler;
			string type1 = "int";
			string type2 = "float";
			int id1 = scriptCompiler.registType(type1);
			int id2 = scriptCompiler.registType(type2);

			//regist type must be success
			FF_EXPECT_NE(-1, id1);
			FF_EXPECT_NE(-1, id2);
			//registered type must be difference each time
			FF_EXPECT_NE(id1, id2);
		}

		FF_TEST_FUNCTION(ScriptCompilerTest,testScriptCompiler4)
		{
			ScriptCompiler scriptCompiler;
			string type = "int";
			int id1 = scriptCompiler.registType(type);
			int id2 = scriptCompiler.getType(type);

			//regist type must be success
			FF_EXPECT_NE(-1, id1);
			//registered type must be same the type using to regist
			FF_EXPECT_EQ(id1, id2);
		}

		FF_TEST_FUNCTION(ScriptCompilerTest,testRegisterFunction1)
		{
			ScriptCompiler scriptCompiler;
			FunctionFactoryCdecl sumFactor(nullptr, ScriptType());
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);

			int id0 = scriptCompiler.registFunction("test", "double, int", &sumFactor);
			int id1 = scriptCompiler.registFunction("sum", "", &sumFactor);
			int id2 = scriptCompiler.registFunction("sum", "int", &sumFactor);
			int id3 = scriptCompiler.registFunction("sum", "int,double", &sumFactor);
			int id4 = scriptCompiler.registFunction("sum", "int,float", &sumFactor);
			int id5 = scriptCompiler.registFunction("sum", "int,float,float", &sumFactor);

			int id6 = scriptCompiler.registFunction("sum", "", &sumFactor);
			int id7 = scriptCompiler.registFunction("sum", "int,double", &sumFactor);
			int id8 = scriptCompiler.registFunction("sum", "int,float,float", &sumFactor);

			FF_EXPECT_TRUE(id0 != -1, L"function 'test' must be registerd successfully!");
			FF_EXPECT_TRUE(id1 != -1, L"function 'sum()' must be registerd successfully!");
			FF_EXPECT_TRUE(id2 != -1, L"function 'sum(int)' must be registerd successfully!");
			FF_EXPECT_TRUE(id3 != -1, L"function 'sum(int,double)' must be registerd successfully!");
			FF_EXPECT_TRUE(id4 != -1, L"function 'sum(int,float)' must be registerd successfully!");
			FF_EXPECT_TRUE(id5 != -1, L"function 'sum(int,float,float)' must be registerd successfully!");

			FF_EXPECT_TRUE(id0 != id1, L"function 'test' and 'sum' must be registerd with diffrent id");	

			FF_EXPECT_TRUE(id6 == -1, L"function 'sum()' is register twice and should not regiser at second time");
			FF_EXPECT_TRUE(id7 == -1, L"function 'sum(int,double)' is register twice and should not regiser at second time");
			FF_EXPECT_TRUE(id8 == -1, L"function 'sum(int,float,float)' is register twice and should not regiser at second time");
		}
	};
}
