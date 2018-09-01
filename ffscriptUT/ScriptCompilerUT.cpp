/******************************************************************
* File:        ScriptCompilerUT.cpp
* Description: Test cases for using ScriptCompiler to register types
*              and functions in a C Lambda application.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\ffscript\ScriptCompiler.h"
#include "BasicFunction.h"
#include "FunctionFactory.h"
#include "BasicType.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace ffscript;

namespace ffscriptUT
{
	TEST_CLASS(ScriptCompilerUT)
	{
	public:

		TEST_METHOD(testScriptCompiler1)
		{
			ScriptCompiler scriptCompiler;
			string type = "int";
			int id = scriptCompiler.registType(type);
			auto registType = scriptCompiler.getType(id);
			
			//regist type must be success
			Assert::AreNotEqual(-1, id);
			//registered type must be same the type using to regist
			Assert::AreEqual(type, registType);
		}

		TEST_METHOD(testScriptCompiler2)
		{
			ScriptCompiler scriptCompiler;
			string type = "int";
			int id = scriptCompiler.registType(type);
			id = scriptCompiler.registType(type);			

			//regist type must be failed at second time
			Assert::IsTrue(IS_UNKNOWN_TYPE(id));
		}

		TEST_METHOD(testScriptCompiler3)
		{
			ScriptCompiler scriptCompiler;
			string type1 = "int";
			string type2 = "float";
			int id1 = scriptCompiler.registType(type1);
			int id2 = scriptCompiler.registType(type2);

			//regist type must be success
			Assert::AreNotEqual(-1, id1);
			Assert::AreNotEqual(-1, id2);
			//registered type must be difference each time
			Assert::AreNotEqual(id1, id2);
		}

		TEST_METHOD(testScriptCompiler4)
		{
			ScriptCompiler scriptCompiler;
			string type = "int";
			int id1 = scriptCompiler.registType(type);
			int id2 = scriptCompiler.getType(type);

			//regist type must be success
			Assert::AreNotEqual(-1, id1);
			//registered type must be same the type using to regist
			Assert::AreEqual(id1, id2);
		}

		TEST_METHOD(testRegisterFunction1)
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

			Assert::IsTrue(id0 != -1, L"function 'test' must be registerd successfully!");
			Assert::IsTrue(id1 != -1, L"function 'sum()' must be registerd successfully!");
			Assert::IsTrue(id2 != -1, L"function 'sum(int)' must be registerd successfully!");
			Assert::IsTrue(id3 != -1, L"function 'sum(int,double)' must be registerd successfully!");
			Assert::IsTrue(id4 != -1, L"function 'sum(int,float)' must be registerd successfully!");
			Assert::IsTrue(id5 != -1, L"function 'sum(int,float,float)' must be registerd successfully!");

			Assert::IsTrue(id0 != id1, L"function 'test' and 'sum' must be registerd with diffrent id");	

			Assert::IsTrue(id6 == -1, L"function 'sum()' is register twice and should not regiser at second time");
			Assert::IsTrue(id7 == -1, L"function 'sum(int,double)' is register twice and should not regiser at second time");
			Assert::IsTrue(id8 == -1, L"function 'sum(int,float,float)' is register twice and should not regiser at second time");
		}
	};
}
