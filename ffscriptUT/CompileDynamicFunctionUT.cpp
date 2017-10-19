#include "stdafx.h"
#include "CppUnitTest.h"
#include "ExpresionParser.h"
#include <functional>
#include "TemplateForTest.hpp"
#include "Variable.h"
#include "FunctionRegisterHelper.h"
#include "BasicFunction.h"
#include "BasicType.h"
#include "FunctionFactory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace ffscript;

#include "ExpresionParser.h"
#include "ScriptCompiler.h"
#include "ScriptScope.h"
#include "expressionunit.h"
#include "Expression.h"
#include "DynamicFunctionFactory.h"
#include "function\CdeclFunction2.hpp"

namespace ffscriptUT
{
	TEST_CLASS(CompileDynamicFunctionUT)
	{
		static int sum(SimpleVariantArray* params) {
			return params->size;
		}
	public:
		TEST_METHOD(TestRegisterDynamicFunction1)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			//register a dynamic function
			auto theNativeFunction = new CdeclFunction2<int, SimpleVariantArray*>(sum);
			DynamicFunctionFactory dynamicFunctionFactory("int", theNativeFunction,&scriptCompiler);
			int functionId = scriptCompiler.registDynamicFunction("sum", &dynamicFunctionFactory);

			Assert::AreNotEqual(-1, functionId);
		}

		TEST_METHOD(TestRegisterDynamicFunction2)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			//register a dynamic function for twice
			auto theNativeFunction = new CdeclFunction2<int, SimpleVariantArray*>(sum);
			DynamicFunctionFactory dynamicFunctionFactory("int", theNativeFunction,&scriptCompiler);
			int functionId = scriptCompiler.registDynamicFunction("sum", &dynamicFunctionFactory);

			//register twice
			functionId = scriptCompiler.registDynamicFunction(dynamicFunctionFactory.getName(), &dynamicFunctionFactory);

			Assert::AreEqual(-1, functionId, L"Register a dynamic function has same name with an existed function is not allow");
		}

		TEST_METHOD(TestRegisterDynamicFunction3)
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
			auto theNativeFunction = new CdeclFunction2<int, SimpleVariantArray*>(sum);
			DynamicFunctionFactory dynamicFunctionFactory("int", theNativeFunction,&scriptCompiler);
			int functionId2 = scriptCompiler.registDynamicFunction("sum", &dynamicFunctionFactory);

			Assert::AreNotEqual(-1, functionId1);
			Assert::AreNotEqual(-1, functionId2, L"a dynamic function should be registered success event has same name with a exist function");
		}

		TEST_METHOD(TestCompileDynamicFunction1)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			//register a custom function
			auto theNativeFunction = new CdeclFunction2<int, SimpleVariantArray*>(sum);
			DynamicFunctionFactory dynamicFunctionFactory("int", theNativeFunction,&scriptCompiler);
			int functionId = scriptCompiler.registDynamicFunction("sum", &dynamicFunctionFactory);

			Assert::AreNotEqual(-1, functionId);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.stringToExpList(L"sum(1,2)", units);
			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			Assert::IsTrue(res, L"compile dynamic function 'sum(1,2) should be success");

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);
			Assert::IsTrue(eResult == E_SUCCESS, L"link the program with dynamic function 'sum(1,2)' should be succcess");
		}

		TEST_METHOD(TestCompileDynamicFunction2)
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
			auto theNativeFunction = new CdeclFunction2<int, SimpleVariantArray*>(sum);
			DynamicFunctionFactory dynamicFunctionFactory("int", theNativeFunction,&scriptCompiler);
			int functionId = scriptCompiler.registDynamicFunction("sum", &dynamicFunctionFactory);

			Assert::AreNotEqual(-1, functionId);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.stringToExpList(L"sum(1,2,3)", units);
			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			Assert::IsTrue(res, L"compile dynamic function 'sum(1,2,3) should be success");

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);
			Assert::IsTrue(eResult == E_SUCCESS, L"link the program with dynamic function 'sum(1,2,3)' should be succcess");
		}

		TEST_METHOD(TestCompileDynamicFunction3)
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
			auto theNativeFunction = new CdeclFunction2<int, SimpleVariantArray*>(sum);
			DynamicFunctionFactory dynamicFunctionFactory("int", theNativeFunction,&scriptCompiler);
			int functionId = scriptCompiler.registDynamicFunction("sum", &dynamicFunctionFactory);

			Assert::AreNotEqual(-1, functionId);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.stringToExpList(L"sum(1,2,3)", units);
			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			Assert::IsTrue(res, L"compile dynamic function 'sum(1,2,3) should be success");

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);
			Assert::IsTrue(eResult == E_SUCCESS, L"link the program with dynamic function 'sum(1,2,3)' should be succcess");
			Assert::AreEqual(expressionPtr->getRoot()->getType(), EXP_UNIT_ID_DYNAMIC_FUNC, L"the sum(1,2,3) should be compiled as a dynamic function");
		}

		TEST_METHOD(TestCompileDynamicFunction4)
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
			EExpressionResult eResult = parser.stringToExpList(L"sum(1,2,3)", units);
			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			Assert::IsTrue(res, L"compile dynamic function 'sum(1,2,3) should be success");

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);
			Assert::IsTrue (E_SUCCESS != eResult, L"link the program with dynamic function 'sum(1,2,3)' should be failed");
		}
	};
}