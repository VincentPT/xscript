/******************************************************************
* File:        CompileDynamicFunctionUT.cpp
* Description: Test cases for compiling C Lambda scripting
*              language that contains dynamic parameter functions.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "fftest.hpp"

#include "ExpresionParser.h"
#include <functional>
#include "TemplateForTest.hpp"
#include "Variable.h"
#include "FunctionRegisterHelper.h"
#include "BasicFunction.h"
#include "BasicType.h"
#include "FunctionFactory.h"

using namespace std;
using namespace ffscript;

#include "ExpresionParser.h"
#include "ScriptCompiler.h"
#include "ScriptScope.h"
#include "expressionunit.h"
#include "Expression.h"
#include "DynamicFunctionFactory.h"
#include "function/FunctionDelegate2.hpp"

namespace ffscriptUT
{
	namespace CompileDynamicFunctionUT
	{
		static int sum(SimpleVariantArray* params) {
			return params->size;
		}
		TEST(CompileDynamicFunction, TestRegisterDynamicFunction1)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			//register a dynamic function
			auto theNativeFunction = new FunctionDelegate2<int, SimpleVariantArray*>(sum);
			DynamicFunctionFactory dynamicFunctionFactory("int", theNativeFunction,&scriptCompiler);
			int functionId = scriptCompiler.registDynamicFunction("sum", &dynamicFunctionFactory);

			EXPECT_NE(-1, functionId);
		}

		TEST(CompileDynamicFunction, TestRegisterDynamicFunction2)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			//register a dynamic function for twice
			auto theNativeFunction = new FunctionDelegate2<int, SimpleVariantArray*>(sum);
			DynamicFunctionFactory dynamicFunctionFactory("int", theNativeFunction,&scriptCompiler);
			int functionId = scriptCompiler.registDynamicFunction("sum", &dynamicFunctionFactory);

			//register twice
			functionId = scriptCompiler.registDynamicFunction(dynamicFunctionFactory.getName(), &dynamicFunctionFactory);

			EXPECT_EQ(-1, functionId) << L"Register a dynamic function has same name with an existed function is not allow";
		}

		TEST(CompileDynamicFunction, TestRegisterDynamicFunction3)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			//register a custom function
			FunctionFactoryCdecl sumFactor(NULL, ScriptType());
			int functionId1 = scriptCompiler.registFunction("sum", "int,int", &sumFactor);

			//register a dynamic function with same name
			auto theNativeFunction = new FunctionDelegate2<int, SimpleVariantArray*>(sum);
			DynamicFunctionFactory dynamicFunctionFactory("int", theNativeFunction,&scriptCompiler);
			int functionId2 = scriptCompiler.registDynamicFunction("sum", &dynamicFunctionFactory);

			EXPECT_NE(-1, functionId1);
			EXPECT_NE(-1, functionId2) << L"a dynamic function should be registered success event has same name with a exist function";
		}

		TEST(CompileDynamicFunction, TestCompileDynamicFunction1)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			//register a custom function
			auto theNativeFunction = new FunctionDelegate2<int, SimpleVariantArray*>(sum);
			DynamicFunctionFactory dynamicFunctionFactory("int", theNativeFunction,&scriptCompiler);
			int functionId = scriptCompiler.registDynamicFunction("sum", &dynamicFunctionFactory);

			EXPECT_NE(-1, functionId);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"sum(1,2)", units);
			EXPECT_TRUE(eResult == E_SUCCESS) << L"parse string to units failed";

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			EXPECT_TRUE(res) << L"compile dynamic function 'sum(1,2) should be success";

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);
			EXPECT_TRUE(eResult == E_SUCCESS) << L"link the program with dynamic function 'sum(1,2)' should be succcess";
		}

		TEST(CompileDynamicFunction, TestCompileDynamicFunction2)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			//register a custom function
			FunctionFactoryCdecl sumFactor(NULL, ScriptType());
			int functionId1 = scriptCompiler.registFunction("sum", "int,int", &sumFactor);

			//register a dynamic function with same name
			auto theNativeFunction = new FunctionDelegate2<int, SimpleVariantArray*>(sum);
			DynamicFunctionFactory dynamicFunctionFactory("int", theNativeFunction,&scriptCompiler);
			int functionId = scriptCompiler.registDynamicFunction("sum", &dynamicFunctionFactory);

			EXPECT_NE(-1, functionId);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"sum(1,2,3)", units);
			EXPECT_TRUE(eResult == E_SUCCESS) << L"parse string to units failed";

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			EXPECT_TRUE(res) << L"compile dynamic function 'sum(1,2,3) should be success";

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);
			EXPECT_TRUE(eResult == E_SUCCESS) << L"link the program with dynamic function 'sum(1,2,3)' should be succcess";
		}

		TEST(CompileDynamicFunction, TestCompileDynamicFunction3)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			//register a custom function
			FunctionFactoryCdecl sumFactor(NULL, ScriptType());
			int functionId1 = scriptCompiler.registFunction("sum", "int,int", &sumFactor);

			//register a dynamic function with same name
			auto theNativeFunction = new FunctionDelegate2<int, SimpleVariantArray*>(sum);
			DynamicFunctionFactory dynamicFunctionFactory("int", theNativeFunction,&scriptCompiler);
			int functionId = scriptCompiler.registDynamicFunction("sum", &dynamicFunctionFactory);

			EXPECT_NE(-1, functionId);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"sum(1,2,3)", units);
			EXPECT_TRUE(eResult == E_SUCCESS) << L"parse string to units failed";

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			EXPECT_TRUE(res) << L"compile dynamic function 'sum(1,2,3) should be success";

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);
			EXPECT_TRUE(eResult == E_SUCCESS) << L"link the program with dynamic function 'sum(1,2,3)' should be succcess";
			EXPECT_EQ(expressionPtr->getRoot()->getType(), EXP_UNIT_ID_DYNAMIC_FUNC) << L"the sum(1,2,3) should be compiled as a dynamic function";
		}

		TEST(CompileDynamicFunction, TestCompileDynamicFunction4)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			//register a custom function
			FunctionFactoryCdecl sumFactor(NULL, ScriptType());
			int functionId1 = scriptCompiler.registFunction("sum", "int,int", &sumFactor);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"sum(1,2,3)", units);
			EXPECT_TRUE(eResult == E_SUCCESS) << L"parse string to units failed";

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			EXPECT_TRUE(res) << L"compile dynamic function 'sum(1,2,3) should be success";

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);
			EXPECT_TRUE (E_SUCCESS != eResult) << L"link the program with dynamic function 'sum(1,2,3)' should be failed";
		}
	}
}