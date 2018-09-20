/******************************************************************
* File:        ExpressionUT.cpp
* Description: Test cases for checking compiling process, compiling
*              linking a single expression of C Lambda scripting
*              language.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

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

namespace ffscriptUT
{
	TEST_CLASS(ExpressionUT)
	{
	public:

		ExpressionUT() {

		}

		TEST_METHOD(testParseFunction1)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1 + 2", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");
			Assert::IsTrue(units.size() == 3, L"the units is not 1 + 2");
		}

		TEST_METHOD(testParseFunction2)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);
			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1 + 2 - (3 - 4)", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");
			Assert::IsTrue(units.size() == 9, L"the units is not '1 + 2 - (3 - 4)'");
		}

		TEST_METHOD(testParseFunction3)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1 + \"hello\"", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");
			Assert::IsTrue(units.size() == 3, L"the units is not '1 + \"hello\"'");
		}

		TEST_METHOD(testParseFunction4)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1 + L\"hello\" + 2", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");
			Assert::IsTrue(units.size() == 5, L"the units is not '1 + L\"hello\" + 2'");
		}

		TEST_METHOD(testCompile1)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);
			
			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1  + 2", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Assert::IsTrue(res, L"compile '1 + 2' failed!");
			Assert::IsTrue(expList.size() == 1, L"compile '1 + 2' and retured more than one expression");
		}

		TEST_METHOD(testCompile2)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1  + 2 + (3 - 4)", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Assert::IsTrue(res, L"compile '1  + 2 + (3 - 4)' failed!");
			Assert::IsTrue(expList.size() == 1, L"compile '1  + 2 + (3 - 4)' and retured more than one expression");
		}

		TEST_METHOD(testCompile2_checkfail1)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1  + 2 + (3 - 4", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Assert::IsFalse(res, L"compile '1  + 2 + (3 - 4' must be failed!");			
		}

		TEST_METHOD(testCompile2_checkfail2)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1  + 2 + 3 - 4)", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Assert::IsFalse(res, L"compile '1  + 2 + 3 - 4)' must be failed!");
		}

		TEST_METHOD(testCompile2_checkfail3)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1  + 2 + )3 - 4)", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Assert::IsFalse(res, L"compile '1  + 2 + )3 - 4)' must be failed!");
		}


		TEST_METHOD(testCompile2_checkfail4)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1  + 2 + (3 - 4(", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Assert::IsFalse(res, L"compile '1  + 2 + (3 - 4(' must be failed!");
		}

		TEST_METHOD(testCompile3)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1+2,3", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Assert::IsTrue(res, L"compile '1+2,3' failed!");
			Assert::IsTrue(expList.size() == 2, L"compile '1+2,3' must return two expressions");
		}

		TEST_METHOD(testCompile4)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1  + 2, 3", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Assert::IsTrue(res, L"compile '1  + 2 , 3' failed!");
			Assert::IsTrue(expList.size() == 2, L"compile '1  + 2, 3' must return two expressions");
		}

		TEST_METHOD(testCompile4_checkfail1)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L",1  + 2, 3", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Assert::IsFalse(res, L"compile ',1  + 2, 3' must be failed!");			
		}

		TEST_METHOD(testCompile4_checkfail2)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1  + 2, 3,", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Assert::IsFalse(res, L"compile '1  + 2, 3,' must be failed!");
		}


		TEST_METHOD(testCompile4_checkfail3)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L",1  + 2, 3,", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Assert::IsFalse(res, L"compile ',1  + 2, 3,' must be failed!");
		}

		TEST_METHOD(testCompile5)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1  + 2, 3 - 4", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Assert::IsTrue(res, L"compile '1  + 2 + (3 - 4)' failed!");
			Assert::IsTrue(expList.size() == 2, L"compile '1  + 2, 3 - 4' must return two expressions");
		}

		TEST_METHOD(testCompile6)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1 + 2 * 3 - 4", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Assert::IsTrue(res, L"compile '1 + 2 * 3 - 4' failed!");
			Assert::IsTrue(expList.size() == 1, L"compile '1 + 2 * 3 - 4' must return one expression");
		}

		TEST_METHOD(testCompile6_checkfail1)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1 + 2 3 * - 4", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Assert::IsFalse(res, L"compile '1 + 2 3 * - 4' must failed!");
		}

		TEST_METHOD(testCompile6_1)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1 + 2 * - 4", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			//because 1 + 2 * - 4 equal to 1 + 2 * (-4)
			//so the expression is valid
			Assert::IsTrue(res, L"compile '1 + 2 * - 4' must success!");

			eResult = parser.link(expList.front().get());
			Assert::IsTrue(eResult == E_SUCCESS, L"link expression '1 + 2 * - 4' failed.");

			Assert::IsTrue(((Function*)expList.front()->getRoot().get())->getChild(0)->toString() == "1", L"first param of root function of '1 + 2 * (- 4)' must be '1'");
			Assert::IsTrue(((Function*)expList.front()->getRoot().get())->getChild(1)->toString() == "*", L"second param of root function of '1 + 2 * (- 4)' must be '*'");
		}

		TEST_METHOD(testCompile6_2)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1 + 2 * (- 4)", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Assert::IsTrue(res, L"compile '1 + 2 * (- 4)' must success!");

			eResult = parser.link(expList.front().get());
			Assert::IsTrue(eResult == E_SUCCESS, L"link expression '1 + 2 * (- 4)' failed.");

			Assert::IsTrue(expList.front()->getRoot()->toString() == "+", L"root unit of '1 + 2 * (- 4)' must be '+'");
			Assert::IsTrue(((Function*)expList.front()->getRoot().get())->getChild(0)->toString() == "1", L"first param of root function of '1 + 2 * (- 4)' must be '1'");
			Assert::IsTrue(((Function*)expList.front()->getRoot().get())->getChild(1)->toString() == "*", L"second param of root function of '1 + 2 * (- 4)' must be '*'");
		}

		TEST_METHOD(testCompile7)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1 * 2 + 3", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Assert::IsTrue(res, L"compile '1 * 2 + 3' failed!");
			Assert::IsTrue(expList.size() == 1, L"compile '1 * 2 + 3' must return one expression");
			Assert::IsTrue(expList.front()->getRoot()->toString() == "+", L"root unit of '1 * 2 + 3' must be '+'");
		}

		TEST_METHOD(testCompile8)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1 * (2 + 3)", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Assert::IsTrue(res, L"compile '1 * 2 + 3' failed!");
			Assert::IsTrue(expList.size() == 1, L"compile '1 * (2 + 3)' must return one expression");
			Assert::IsTrue(expList.front()->getRoot()->toString() == "*", L"root unit of '1 * (2 + 3)' must be '*'");
		}

		TEST_METHOD(testCompile9)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1 + 2", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			Assert::IsTrue(res, L"compile '1 + 2' failed!");

			eResult = parser.link(expList.front().get());
			Assert::IsTrue(eResult == E_SUCCESS, L"link expression '1 + 2' failed.");

			Assert::IsTrue(expList.front()->getRoot()->toString() == "+", L"root unit of '1 + 2' must be '+'");
			Assert::IsTrue( ((Function*)expList.front()->getRoot().get())->getChild(0)->toString() == "1", L"first param of '1 + 2' must be '1'");
		}

		static Function* createSumFunction(const string& name, int id) {
			return new FixParamFunction<2>(name, EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int");
		}

		TEST_METHOD(testCompile11)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			//register a custom function
			FunctionFactoryCdecl sumFactor(createSumFunction, ScriptType(basicType.TYPE_INT, "int"));
			scriptCompiler.registFunction("sum", "int,int", &sumFactor);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"sum(1,2)", units);

			Assert::IsTrue(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			Expression* expressionPtr = expList.front().get();
			parser.link(expressionPtr);

			Assert::IsTrue(res, L"compile 'sum(1,2)' failed!");
			Assert::IsTrue(expList.front()->getRoot()->toString() == "sum", L"root unit of 'sum(1,2)' must be '+'");
			Assert::IsTrue(((Function*)expList.front()->getRoot().get())->getChild(0)->toString() == "1", L"first param of 'sum(1,2)' must be '1'");
			Assert::IsTrue(((Function*)expList.front()->getRoot().get())->getChild(1)->toString() == "2", L"second param of 'sum(1,2)' must be '2'");
		}
	};
}