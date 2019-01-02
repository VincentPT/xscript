/******************************************************************
* File:        ExpressionLinkUT.cpp
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
#include "fftest.hpp"

#include "ExpresionParser.h"
#include <functional>
#include "TemplateForTest.hpp"
#include "Variable.h"
#include "FunctionRegisterHelper.h"
#include "BasicFunction.h"
#include "BasicType.h"
#include "FunctionFactory.h"
#include "function\MemberFunction2.hpp"

using namespace std;
using namespace ffscript;

#include "ExpresionParser.h"
#include "ScriptCompiler.h"
#include "ScriptScope.h"
#include "expressionunit.h"
#include "Expression.h"

namespace ffscriptUT
{
	template <int paramSize> 
	class FunctionFactoryForTest : public FunctionFactory
	{
		DFunction2Ref _nativeFunction;
	public:
		FunctionFactoryForTest(ScriptCompiler* pCompiler, int returnType, DFunction2* nativeFunction) :
			_nativeFunction(nativeFunction),
			FunctionFactory(nullptr, pCompiler)
		{
			this->setReturnType(ScriptType(returnType, pCompiler->getType(returnType)));
		}

		virtual ~FunctionFactoryForTest() {}

		ffscript::Function* createFunction(const std::string& name, int id) {
			ffscript::NativeFunction* function = new FixParamFunction<paramSize>(name, EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, this->getReturnType());
			function->setNative(_nativeFunction);
			return function;
		}
	};

	FF_TEST_CLASS(ExpressionLink) {
	public:
		double sum(double a) {
			return a;
		}

		double sum(double a, float b) {
			return a + b;
		}

		double sum(double a, float b, int c) {
			return a + b + c;
		}

		double sum(double a, int b, int c) {
			return a + b + c;
		}
	};

	namespace ExpressionLinkUT
	{
		FF_TEST_METHOD(ExpressionLink, testExpressionLink1)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1 + 2", units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);

			FF_EXPECT_TRUE(res, L"compile '1 + 2' failed!");

			eResult = parser.link(expList.front().get());
			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"link expression '1 + 2' failed!");
		}

		FF_TEST_METHOD(ExpressionLink, testExpressionLink2)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1 + 2.0", units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, L"compile '1 + 2.0' failed!");

			eResult = parser.link(expList.front().get());
			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"link expression '1 + 2.0'  failed!");
		}

		FF_TEST_METHOD(ExpressionLink, testExpressionLink3)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1 + 2.0", units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, L"compile '1 + 2.0' failed!");

			eResult = parser.link(expList.front().get());
			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"link expression '1 + 2.0' failed");
		}

		FF_TEST_METHOD(ExpressionLink, testExpressionLink4)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);			
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1 + 2.0 * 3", units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, L"compile '1 + 2.0 * 3' failed!");

			eResult = parser.link(expList.front().get());
			//FF_EXPECT_TRUE(eResult != E_SUCCESS, L"link expression '1 + 2.0 * 3'  must failed. To link successfully, program must register casting function");
			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"link expression '1 + 2.0 * 3'  should be success. To link successfully, program should implement basic function support all basic parameters without registering casting function");
		}

		FF_TEST_METHOD(ExpressionLink, testExpressionLink5)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1 + 2.0 * 3", units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, L"compile '1 + 2.0 * 3' failed!");

			eResult = parser.link(expList.front().get());
			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"link expression '1 + 2.0 * 3' failed.");

			FF_EXPECT_TRUE(expList.front()->getRoot()->toString() == "+", L"Root function must be '+'");
			FF_EXPECT_TRUE(expList.front()->getRoot()->getReturnType().iType() == basicType.TYPE_DOUBLE , L"operator '+' must return double");
		}

		FF_TEST_METHOD(ExpressionLink, testExpressionLink6)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(L"1 + 2.0 * 3", units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, L"compile '1 + 2.0 * 3' failed!");

			eResult = parser.link(expList.front().get());
			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"link expression '1 + 2.0 * 3' failed.");
		}

		FF_TEST_METHOD(ExpressionLink, testExpressionLink7)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importCoreFunctions(funcLibHelper);

			MFunction2W<double, ExpressionLink, double> sum1(this, &ExpressionLink::sum);
			MFunction2W<double, ExpressionLink, double, float> sum2(this, &ExpressionLink::sum);
			MFunction2W<double, ExpressionLink, double, float, int> sum3(this, &ExpressionLink::sum);
			MFunction2W<double, ExpressionLink, double, int, int> sum4(this, &ExpressionLink::sum);
			
			wstring functionString = L"sum(1, 2.0, 3)";

			int functionId1 = funcLibHelper.registFunction("sum", "double", new FunctionFactoryForTest<1>(&scriptCompiler,basicType.TYPE_DOUBLE, sum1.clone()));
			int functionId2 = funcLibHelper.registFunction("sum", "double,float", new FunctionFactoryForTest<2>(&scriptCompiler, basicType.TYPE_DOUBLE, sum2.clone()));
			int functionId3 = funcLibHelper.registFunction("sum", "double,float,int", new FunctionFactoryForTest<3>(&scriptCompiler, basicType.TYPE_DOUBLE, sum3.clone()));
			int functionId4 = funcLibHelper.registFunction("sum", "double,int,int", new FunctionFactoryForTest<3>(&scriptCompiler, basicType.TYPE_DOUBLE, sum4.clone()));

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			eResult = parser.link(expList.front().get());
			//FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link expression '" + functionString + L"' failed.").c_str());
			//FF_EXPECT_TRUE( dynamic_cast<ffscript::Function*>(expList.front()->getRoot().get())->getId() == functionId3, (L"function '" + functionString + L"' must be linked to sum(double,float,int).").c_str());
			FF_EXPECT_TRUE(eResult != E_SUCCESS, (L"link expression '" + functionString + L"' should failed due tu ambious function call.").c_str());
		}

		FF_TEST_METHOD(ExpressionLink, testExpressionLink8)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importCoreFunctions(funcLibHelper);

			MFunction2W<double, ExpressionLink, double> sum1(this, &ExpressionLink::sum);
			MFunction2W<double, ExpressionLink, double, float> sum2(this, &ExpressionLink::sum);
			MFunction2W<double, ExpressionLink, double, float, int> sum3(this, &ExpressionLink::sum);
			MFunction2W<double, ExpressionLink, double, int, int> sum4(this, &ExpressionLink::sum);

			wstring functionString = L"sum(1.0, 2.0, 3.0)";

			int functionId1 = funcLibHelper.registFunction("sum", "double", new FunctionFactoryForTest<1>(&scriptCompiler, basicType.TYPE_DOUBLE, sum1.clone()));
			int functionId2 = funcLibHelper.registFunction("sum", "double,float", new FunctionFactoryForTest<2>(&scriptCompiler, basicType.TYPE_DOUBLE, sum2.clone()));
			int functionId3 = funcLibHelper.registFunction("sum", "double,float,int", new FunctionFactoryForTest<3>(&scriptCompiler, basicType.TYPE_DOUBLE, sum3.clone()));
			int functionId4 = funcLibHelper.registFunction("sum", "double,int,int", new FunctionFactoryForTest<3>(&scriptCompiler, basicType.TYPE_DOUBLE, sum4.clone()));
			int functionId5 = funcLibHelper.registFunction("sum", "int,int,int", new FunctionFactoryForTest<3>(&scriptCompiler, basicType.TYPE_DOUBLE, nullptr));

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			eResult = parser.link(expList.front().get());
			//FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link expression '" + functionString + L"' failed.").c_str());
			//FF_EXPECT_TRUE(dynamic_cast<ffscript::Function*>(expList.front()->getRoot().get())->getId() == functionId3, (L"function '" + functionString + L"' must be linked to sum(double,float,int).").c_str());
			FF_EXPECT_TRUE(eResult != E_SUCCESS, (L"link expression '" + functionString + L"' should failed due tu ambious function call.").c_str());
		}

		FF_TEST_METHOD(ExpressionLink, testExpressionLink9)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importCoreFunctions(funcLibHelper);

			wstring functionString = L"sum(1, 2.0, 3)";

			int functionId1 = funcLibHelper.registFunction("sum", "int,int,int", new FunctionFactoryForTest<3>(&scriptCompiler, basicType.TYPE_INT , nullptr));
			int functionId3 = funcLibHelper.registFunction("sum", "double,double,double", new FunctionFactoryForTest<3>(&scriptCompiler, basicType.TYPE_DOUBLE, nullptr));

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			eResult = parser.link(expList.front().get());
			//FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link expression '" + functionString + L"' failed.").c_str());
			//FF_EXPECT_TRUE(dynamic_cast<ffscript::Function*>(expList.front()->getRoot().get())->getId() == functionId1, (L"function '" + functionString + L"' must be linked to sum(int,int,int).").c_str());
			FF_EXPECT_TRUE(eResult != E_SUCCESS, (L"link expression '" + functionString + L"' should failed due tu ambious function call.").c_str());
		}
	};
}