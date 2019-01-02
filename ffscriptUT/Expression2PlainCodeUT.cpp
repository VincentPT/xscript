/******************************************************************
* File:        Expression2PlainCodeUT.cpp
* Description: Test cases for compiling and running single expression
*              of C Lambda scripting language.
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
#include "FunctionFactory.h"
#include "function\MemberFunction.hpp"
#include "Context.h"
#include <thread>
#include <future>
#include <Program.h>

using namespace std;
using namespace ffscript;

#include "ExpresionParser.h"
#include "ScriptCompiler.h"
#include "expressionunit.h"
#include "Expression.h"
#include "GlobalScope.h"
#include "ExpUnitExecutor.h"
#include "Utils.h"

namespace ffscriptUT
{	
	namespace Expression2PlainCodeUT
	{
		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode1)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			Context currentContext(1024 * 1024);

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			wstring functionString = L"1 + 2";

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			eResult = parser.link(expList.front().get());
			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link expression '" + functionString + L"' failed.").c_str());

			ExpUnitExecutor excutor( scriptCompiler.currentScope() ) ;
			res = excutor.extractCode(&scriptCompiler, expList.front().get());

			FF_EXPECT_TRUE(res, (L"convert expression '" + functionString + L"' to plain code failed!").c_str());

			excutor.runCode();
			void* result = excutor.getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + functionString + L"' failed!").c_str());
			FF_EXPECT_TRUE( *(int*)result == 3, (L"result of expression '" + functionString + L"' is not correct").c_str());
		}

		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode2)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);

			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			Context currentContext(1024 * 1024);

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			wstring functionString = L"1 * 2";

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			eResult = parser.link(expList.front().get());
			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link expression '" + functionString + L"' failed.").c_str());

			ExpUnitExecutor excutor(scriptCompiler.currentScope());
			res = excutor.extractCode(&scriptCompiler, expList.front().get());

			FF_EXPECT_TRUE(res, (L"convert expression '" + functionString + L"' to plain code failed!").c_str());

			excutor.runCode();
			void* result = excutor.getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + functionString + L"' failed!").c_str());
			FF_EXPECT_TRUE(*(int*)result == 2, (L"result of expression '" + functionString + L"' is not correct").c_str());
		}

		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode3)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			Context currentContext(1024);

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			wstring functionString = L"1 * 2 + 3";

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			eResult = parser.link(expList.front().get());
			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link expression '" + functionString + L"' failed.").c_str());

			ExpUnitExecutor excutor(scriptCompiler.currentScope());
			res = excutor.extractCode(&scriptCompiler, expList.front().get());

			FF_EXPECT_TRUE(res, (L"convert expression '" + functionString + L"' to plain code failed!").c_str());

			excutor.runCode();
			void* result = excutor.getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + functionString + L"' failed!").c_str());
			FF_EXPECT_TRUE(*(int*)result == 5, (L"result of expression '" + functionString + L"' is not correct").c_str());
		}

		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode4)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			Context currentContext(1024 * 1024);

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			wstring functionString = L"1 - 2";

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			eResult = parser.link(expList.front().get());
			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link expression '" + functionString + L"' failed.").c_str());

			ExpUnitExecutor excutor(scriptCompiler.currentScope());
			res = excutor.extractCode(&scriptCompiler, expList.front().get());

			FF_EXPECT_TRUE(res, (L"convert expression '" + functionString + L"' to plain code failed!").c_str());

			excutor.runCode();
			void* result = excutor.getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + functionString + L"' failed!").c_str());
			FF_EXPECT_TRUE(*(int*)result == -1, (L"result of expression '" + functionString + L"' is not correct").c_str());
		}

		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode5)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			wstring functionString = L"1 - 2";

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			Expression* expressionPtr = expList.front().get();
			parser.link(expressionPtr);

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());
			FF_EXPECT_TRUE(((ffscript::Function*)(expressionPtr->getRoot().get()))->getChild(0)->toString().compare("1") == 0);
			FF_EXPECT_TRUE(((ffscript::Function*)(expressionPtr->getRoot().get()))->getChild(1)->toString().compare("2") == 0);
		}

		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode6)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			Context currentContext(1024 * 1024);

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			wstring functionString = L"1 - 2";

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);
			
			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link '" + functionString + L"' failed!").c_str());

			FF_EXPECT_TRUE(((ffscript::Function*)(expressionPtr->getRoot().get()))->getChild(0)->toString().compare("1") == 0);
			FF_EXPECT_TRUE(((ffscript::Function*)(expressionPtr->getRoot().get()))->getChild(1)->toString().compare("2") == 0);

			ExpUnitExecutor excutor(scriptCompiler.currentScope());
			res = excutor.extractCode(&scriptCompiler, expList.front().get());

			excutor.runCode();
			int* iRes = (int*)excutor.getReturnData();

			FF_EXPECT_TRUE(*iRes == -1, (L"resutlt of '" + functionString + L"' is wrong").c_str());
		}

		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode7)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			Context currentContext(1024 * 1024);

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			wstring functionString = L"1 - 2 * 3 + (4 / 2)";

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link '" + functionString + L"' failed!").c_str());

			ExpUnitExecutor excutor(scriptCompiler.currentScope());
			res = excutor.extractCode(&scriptCompiler, expList.front().get());

			excutor.runCode();
			int* iRes = (int*)excutor.getReturnData();

			FF_EXPECT_TRUE(*iRes == (1 - 2 * 3 + (4 / 2)), (L"resutlt of '" + functionString + L"' is wrong").c_str());
		}


		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode8)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();

			Context currentContext(1024 * 1024);

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			importBasicfunction(funcLibHelper);

			wstring functionString = L"1 - (2 + 3) * (4 / 2)";

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link '" + functionString + L"' failed!").c_str());

			ExpUnitExecutor excutor(scriptCompiler.currentScope());
			res = excutor.extractCode(&scriptCompiler, expList.front().get());

			excutor.runCode();
			int* iRes = (int*)excutor.getReturnData();
			FF_EXPECT_TRUE(*iRes == (1 - (2 + 3) * (4 / 2)), (L"resutlt of '" + functionString + L"' is wrong").c_str());
		}

		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode9)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			Context currentContext(1024 * 1024);

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			wstring functionString = L"1 - 2.5";

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link '" + functionString + L"' failed!").c_str());

			ExpUnitExecutor excutor(scriptCompiler.currentScope());
			res = excutor.extractCode(&scriptCompiler, expList.front().get());

			excutor.runCode();
			double* execres = (double*)excutor.getReturnData();

			FF_EXPECT_TRUE(*execres == (1 - 2.5), (L"resutlt of '" + functionString + L"' is wrong").c_str());
		}

		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode10)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			Context currentContext(1024 * 1024);

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			wstring functionString = L"1 - 2 * 3.5";

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link '" + functionString + L"' failed!").c_str());

			ExpUnitExecutor excutor(scriptCompiler.currentScope());
			res = excutor.extractCode(&scriptCompiler, expList.front().get());

			excutor.runCode();
			double* execres = (double*)excutor.getReturnData();

			FF_EXPECT_TRUE(*execres == (1 - 2 * 3.5), (L"resutlt of '" + functionString + L"' is wrong").c_str());
		}

		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode11)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			byte threadData[1024];
			Context currentContext(threadData, sizeof(threadData));

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);
			wstring functionString = L"1 - 2 * 3.5 + 4*(5 + 6)";

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link '" + functionString + L"' failed!").c_str());

			ExpUnitExecutor excutor(scriptCompiler.currentScope());
			res = excutor.extractCode(&scriptCompiler, expList.front().get());

			excutor.runCode();
			double* execres = (double*)excutor.getReturnData();

			FF_EXPECT_TRUE(*execres == (1 - 2 * 3.5 + 4 * (5 + 6)), (L"resutlt of '" + functionString + L"' is wrong").c_str());
		}



		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode12)
		{
			Context* pContext = Context::getCurrent();
			FF_EXPECT_TRUE(pContext == nullptr);

			auto future =  std::async([]()-> Context* {
				Context* secondContext = new Context(1024);
				Context* pSecondContext = Context::getCurrent();
				return pSecondContext;
			});
			
			Context context(1024 * 1024);
			Context* contextInFirstThread = Context::getCurrent();
			Context* contextInSecondThread = future.get();

			FF_EXPECT_TRUE(contextInFirstThread != nullptr);
			FF_EXPECT_TRUE(contextInSecondThread != nullptr);
			FF_EXPECT_TRUE(contextInFirstThread != contextInSecondThread);

			delete contextInSecondThread;			
		}

		//run one code twice
		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode13)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			byte threadData[1024];
			Context currentContext(threadData, sizeof(threadData));

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			wstring functionString = L"1 - 2 * 3.5 + 4*(5 + 6)";

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link '" + functionString + L"' failed!").c_str());

			ExpUnitExecutor excutor(scriptCompiler.currentScope());
			res = excutor.extractCode(&scriptCompiler, expList.front().get());

			excutor.runCode();
			double* execres = (double*)excutor.getReturnData();
			FF_EXPECT_TRUE(*execres == (1 - 2 * 3.5 + 4 * (5 + 6)), (L"resutlt of '" + functionString + L"' is wrong").c_str());

			excutor.runCode();
			execres = (double*)excutor.getReturnData();
			FF_EXPECT_TRUE(*execres == (1 - 2 * 3.5 + 4 * (5 + 6)), (L"resutlt of '" + functionString + L"' is wrong").c_str());
		}

		//run one code many time
		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode14)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			byte threadData[1024];
			Context currentContext(threadData, sizeof(threadData));

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			wstring functionString = L"1 - 2 * 3.5 + 4*(5 + 6)";

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link '" + functionString + L"' failed!").c_str());

			ExpUnitExecutor excutor(scriptCompiler.currentScope());
			res = excutor.extractCode(&scriptCompiler, expList.front().get());

			excutor.runCode();
			double* execres = (double*)excutor.getReturnData();
			double expectedRes = 1 - 2 * 3.5 + 4 * (5 + 6);

			int i;
			int n = 100000;
			for (i = 0; i < n; i++) {

				excutor.runCode();
				execres = (double*)excutor.getReturnData();

				if (*execres != expectedRes) {
					break;
				}
			}
			FF_EXPECT_TRUE(i == n, (L"resutlt of '" + functionString + L"' is wrong at " + std::to_wstring(i)).c_str());
		}

		//test parse function for only one constant value
		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode15)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			byte threadData[1024];
			Context currentContext(threadData, sizeof(threadData));

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			
			wstring functionString = L"12345";

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link '" + functionString + L"' failed!").c_str());

			ExpUnitExecutor excutor(scriptCompiler.currentScope());
			res = excutor.extractCode(&scriptCompiler, expList.front().get());

			excutor.runCode();

			int* execres = (int*)excutor.getReturnData();
			FF_EXPECT_TRUE(*execres == 12345, (L"resutlt of '" + functionString + L"' is wrong").c_str());
		}

		//test parse function for one paramter
		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode16)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			byte threadData[1024];
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			Context currentContext(threadData, sizeof(threadData));

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			GlobalScope aScope(&staticContex,&scriptCompiler);
			Variable* pX = aScope.registVariable("x");
			pX->setDataType(ScriptType(basicType.TYPE_INT,"int"));			

			wstring functionString = L"x + 1";

			scriptCompiler.pushScope(&aScope);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			aScope.updateVariableOffset();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link '" + functionString + L"' failed!").c_str());

			const int x = 100;
			setVariableValue(*pX, x);

			ExpUnitExecutor excutor(scriptCompiler.currentScope());
			res = excutor.extractCode(&scriptCompiler, expList.front().get());

			excutor.runCode();
			int* execres = (int*)excutor.getReturnData();

			FF_EXPECT_TRUE(*execres == (x + 1), (L"resutlt of '" + functionString + L"' is wrong").c_str());
		}

		//test parse function for two paramters
		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode17)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			byte threadData[1024];
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			Context currentContext(threadData, sizeof(threadData));

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			GlobalScope aScope(&staticContex,&scriptCompiler);
			Variable* pX = aScope.registVariable("x");
			Variable* pY = aScope.registVariable("y");
			pX->setDataType(ScriptType(basicType.TYPE_INT,"int"));
			pY->setDataType(ScriptType(basicType.TYPE_DOUBLE,"double"));

			wstring functionString = L"x + 1 - y";

			scriptCompiler.pushScope(&aScope);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			aScope.updateVariableOffset();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link '" + functionString + L"' failed!").c_str());

			const int x = 100;
			const double y = 50;
			setVariableValue(*pX, x);
			setVariableValue(*pY, y);

			ExpUnitExecutor excutor(scriptCompiler.currentScope());
			res = excutor.extractCode(&scriptCompiler, expList.front().get());

			excutor.runCode();
			double* execres = (double*)excutor.getReturnData();

			FF_EXPECT_TRUE(*execres == (x + 1 - y), (L"resutlt of '" + functionString + L"' is wrong").c_str());
		}

		//test parse function for two paramters
		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode18)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			byte threadData[1024];
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			Context currentContext(threadData, sizeof(threadData));

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			GlobalScope aScope(&staticContex,&scriptCompiler);
			Variable* pX = aScope.registVariable("x");
			Variable* pY = aScope.registVariable("y");
			pX->setDataType(ScriptType(basicType.TYPE_INT,"int"));
			pY->setDataType(ScriptType(basicType.TYPE_DOUBLE,"double"));

			wstring functionString = L"(x + 1 - y)*(2.5 - y + x)";

			scriptCompiler.pushScope(&aScope);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();

			aScope.updateVariableOffset();

			FF_EXPECT_TRUE(res, (L"compile '" + functionString + L"' failed!").c_str());

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link '" + functionString + L"' failed!").c_str());

			const int x = 100;
			const double y = 50;
			setVariableValue(*pX, x);
			setVariableValue(*pY, y);

			ExpUnitExecutor excutor(scriptCompiler.currentScope());
			res = excutor.extractCode(&scriptCompiler, expList.front().get());

			excutor.runCode();
			double* execres = (double*)excutor.getReturnData();

			FF_EXPECT_TRUE(*execres == (x + 1 - y)*(2.5 - y + x), (L"resutlt of '" + functionString + L"' is wrong").c_str());
		}

		//test parse function for operator assigment
		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode19)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			byte threadData[1024];
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			Context currentContext(threadData, sizeof(threadData));

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			GlobalScope aScope(&staticContex,&scriptCompiler);
			
			int x;
			wstring functionString1 = L"x = 1";

			scriptCompiler.pushScope(&aScope);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString1.c_str(), units);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			units.clear();
			
			FF_EXPECT_TRUE(res, (L"compile '" + functionString1 + L"' failed!").c_str());

			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);

			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link '" + functionString1 + L"' failed!").c_str());

			aScope.updateVariableOffset();

			ExpUnitExecutor excutor(scriptCompiler.currentScope());
			res = excutor.extractCode(&scriptCompiler, expList.front().get());

			excutor.runCode();
			//int* execres = (int*)excutor.getReturnData();
			//FF_EXPECT_TRUE(*execres == (x = 1), (L"resutlt of '" + functionString1 + L"' is wrong").c_str());
			
			Variable* pX = aScope.findVariable("x");
			FF_EXPECT_TRUE(*(getVaribleRef<int>(*pX)) == (x = 1), (L"resutlt of '" + functionString1 + L"' is wrong").c_str());
		}

		//test run two lines of code
		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode20)
		{
			ScriptCompiler scriptCompiler;
			ExpressionParser parser(&scriptCompiler);
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			byte threadData[1024];
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			Context currentContext(threadData, sizeof(threadData));

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			GlobalScope aScope(&staticContex,&scriptCompiler);
			scriptCompiler.pushScope(&aScope);

			wstring functionString1 = L"x = 1";
			wstring functionString2 = L"x + 2";
			
			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(functionString1.c_str(), units);
			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");
			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			FF_EXPECT_TRUE(res, (L"compile '" + functionString1 + L"' failed!").c_str());
			Expression* expressionPtr1 = expList.front().get();
			eResult = parser.link(expressionPtr1);
			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link '" + functionString1 + L"' failed!").c_str());

			list<ExpUnitRef> units2;
			eResult = parser.tokenize(functionString2.c_str(), units2);
			FF_EXPECT_TRUE(eResult == E_SUCCESS, L"parse string to units failed");
			list<ExpressionRef> expList2;
			res = parser.compile(units2, expList2);
			FF_EXPECT_TRUE(res, (L"compile '" + functionString2 + L"' failed!").c_str());
			Expression* expressionPtr2 = expList2.front().get();
			eResult = parser.link(expressionPtr2);
			FF_EXPECT_TRUE(eResult == E_SUCCESS, (L"link '" + functionString2 + L"' failed!").c_str());

			aScope.updateVariableOffset();

			ExpUnitExecutor excutor1(scriptCompiler.currentScope());
			excutor1.extractCode(&scriptCompiler,expressionPtr1);
			ExpUnitExecutor excutor2(scriptCompiler.currentScope());
			excutor2.extractCode(&scriptCompiler, expressionPtr2);

			excutor1.runCode();
			currentContext.moveOffset(excutor1.getCurrentLocalOffset());
			excutor2.runCode();
			int* execres = (int*)excutor2.getReturnData();
			currentContext.moveOffset(excutor2.getCurrentLocalOffset());

			FF_EXPECT_TRUE(*execres == 3);
		}

		ExpressionRef compileAndLink(ScriptCompiler* scriptCompiler, ScriptScope* ownerScope, const wstring& expString) {
			ExpressionParser parser(scriptCompiler);
	
			scriptCompiler->pushScope(ownerScope);

			list<ExpUnitRef> units;
			EExpressionResult eResult = parser.tokenize(expString.c_str(), units);
			if (eResult != E_SUCCESS) return nullptr;

			list<ExpressionRef> expList;
			bool res = parser.compile(units, expList);
			if (res == false) return nullptr;
			
			Expression* expressionPtr = expList.front().get();
			eResult = parser.link(expressionPtr);
			if (eResult != E_SUCCESS) return nullptr;

			return expList.front();
		}

		ExpUnitExecutor* convertToPlainCode(ScriptScope* ownerScope, ExpressionRef& expresion) {
			ExpUnitExecutor* pExcutor = new ExpUnitExecutor(ownerScope);
			pExcutor->extractCode(ownerScope->getCompiler(), expresion.get());

			//release immediately after done
			expresion.reset();

			return pExcutor;
		}
			

		//test run two lines of code
		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode21)
		{
			ScriptCompiler scriptCompiler;

			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			byte threadData[1024];
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			Context currentContext(threadData, sizeof(threadData));
			GlobalScope aScope(&staticContex,&scriptCompiler);

			wstring functionString1 = L"x = 1";
			wstring functionString2 = L"x + 2";

			ExpressionRef exp1 = compileAndLink(&scriptCompiler, &aScope, functionString1);
			ExpressionRef exp2 = compileAndLink(&scriptCompiler, &aScope, functionString2);

			aScope.updateVariableOffset();

			ExpUnitExecutor* pExpUnitExecutor1 = convertToPlainCode(&aScope, exp1);
			ExpUnitExecutor* pExpUnitExecutor2 = convertToPlainCode(&aScope, exp2);

			pExpUnitExecutor1->runCode();
			currentContext.moveOffset(pExpUnitExecutor1->getCurrentLocalOffset());
			pExpUnitExecutor2->runCode();
			int* execres = (int*)pExpUnitExecutor2->getReturnData();
			currentContext.moveOffset(pExpUnitExecutor2->getCurrentLocalOffset());

			FF_EXPECT_TRUE(*execres == 3);
		}

		//test run two lines of code
		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode22)
		{
			ScriptCompiler scriptCompiler;

			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			byte threadData[1024];
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			Context currentContext(threadData, sizeof(threadData));
			GlobalScope aScope(&staticContex,&scriptCompiler);

			wstring functionString1 = L"x = 1";
			wstring functionString2 = L"x = x + 1";

			ExpressionRef exp1 = compileAndLink(&scriptCompiler, &aScope, functionString1);
			ExpressionRef exp2 = compileAndLink(&scriptCompiler, &aScope, functionString2);

			aScope.updateVariableOffset();

			ExpUnitExecutor* pExpUnitExecutor1 = convertToPlainCode(&aScope, exp1);
			ExpUnitExecutor* pExpUnitExecutor2 = convertToPlainCode(&aScope, exp2);

			pExpUnitExecutor1->runCode();
			//currentContext.moveOffset(pExpUnitExecutor1->getCurrentLocalOffset());
			pExpUnitExecutor2->runCode();
			int interferAssigment = scriptCompiler.findFunction("=", "int&,int");
			int* execres;
			// if operator '=' of interger is not defined
			if (interferAssigment < 0) {
				execres = *(int**)pExpUnitExecutor2->getReturnData();
			}
			else {
				// if operator '=' of interger is defined
				 execres = (int*)pExpUnitExecutor2->getReturnData();
			}
			FF_EXPECT_TRUE(*execres == 2);
		}

		//test run two lines of code
		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode23)
		{
			ScriptCompiler scriptCompiler;

			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			byte threadData[1024];
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			Context currentContext(threadData, sizeof(threadData));
			GlobalScope aScope(&staticContex,&scriptCompiler);

			wstring functionString1 = L"x = 0";
			wstring functionString2 = L"x = x + 1";

			ExpressionRef exp1 = compileAndLink(&scriptCompiler, &aScope, functionString1);
			ExpressionRef exp2 = compileAndLink(&scriptCompiler, &aScope, functionString2);

			aScope.updateVariableOffset();

			ExpUnitExecutor* pExpUnitExecutor1 = convertToPlainCode(&aScope, exp1);
			ExpUnitExecutor* pExpUnitExecutor2 = convertToPlainCode(&aScope, exp2);

			pExpUnitExecutor1->runCode();
			//currentContext.moveOffset(pExpUnitExecutor1->getCurrentLocalOffset());			
			//currentContext.moveOffset(pExpUnitExecutor2->getCurrentLocalOffset());
			int n = 10000;
			for (int i = 0; i < n; i++) {
				pExpUnitExecutor2->runCode();
			}						
			int interferAssigment = scriptCompiler.findFunction("=", "int&,int");
			int* execres;
			// if operator '=' of interger is not defined
			if (interferAssigment < 0) {
				execres = *(int**)pExpUnitExecutor2->getReturnData();
			}
			else {
				// if operator '=' of interger is defined
				execres = (int*)pExpUnitExecutor2->getReturnData();
			}

			FF_EXPECT_TRUE(*execres == n);
		}

		//test run two lines of code
 		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode24)
		{
			ScriptCompiler scriptCompiler;

			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			byte threadData[1024];
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			Context currentContext(threadData, sizeof(threadData));
			GlobalScope aScope(&staticContex,&scriptCompiler);

			wstring functionString1 = L"x = 0";
			wstring functionString2 = L"x = x + 1";

			ExpressionRef exp1 = compileAndLink(&scriptCompiler, &aScope, functionString1);
			ExpressionRef exp2 = compileAndLink(&scriptCompiler, &aScope, functionString2);

			aScope.updateVariableOffset();

			ExpUnitExecutor* pExpUnitExecutor1 = convertToPlainCode(&aScope, exp1);
			ExpUnitExecutor* pExpUnitExecutor2 = convertToPlainCode(&aScope, exp2);

			Program program;
			scriptCompiler.bindProgram(&program);
			program.addExecutor((ExecutorRef)(pExpUnitExecutor1));
			program.addExecutor((ExecutorRef)(pExpUnitExecutor2));
			program.convertToPlainCode();

			currentContext.setCurrentCommand(program.getFirstCommand());
			currentContext.setEndCommand(program.getEndCommand());
			currentContext.run();

			Variable* pX = aScope.findVariable("x");
			int* x = getVaribleRef<int>(*pX);

			std::string message("x =");
			message.append(std::to_string(*x));
			PRINT_TEST_MESSAGE(message.c_str());

			FF_EXPECT_TRUE(*x == 1);
		}

		//test run two lines of code
		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode25)
		{
			ScriptCompiler scriptCompiler;

			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			byte threadData[1024];
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			Context currentContext(threadData, sizeof(threadData));
			GlobalScope aScope(&staticContex,&scriptCompiler);

			wstring functionString1 = L"x = 0";
			wstring functionString2 = L"x = x + 1.5";

			ExpressionRef exp1 = compileAndLink(&scriptCompiler, &aScope, functionString1);
			ExpressionRef exp2 = compileAndLink(&scriptCompiler, &aScope, functionString2);

			aScope.updateVariableOffset();

			ExpUnitExecutor* pExpUnitExecutor1 = convertToPlainCode(&aScope, exp1);
			ExpUnitExecutor* pExpUnitExecutor2 = convertToPlainCode(&aScope, exp2);

			Program program;
			scriptCompiler.bindProgram(&program);

			program.addExecutor((ExecutorRef)(pExpUnitExecutor1));
			program.addExecutor((ExecutorRef)(pExpUnitExecutor2));
			program.convertToPlainCode();

			currentContext.setCurrentCommand(program.getFirstCommand());
			currentContext.setEndCommand(program.getEndCommand());
			currentContext.run();

			Variable* pX = aScope.findVariable("x");
			int* x = getVaribleRef<int>(*pX);
			FF_EXPECT_TRUE(*x == 1);
		}

		//test run two lines of code
		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode26)
		{
			ScriptCompiler scriptCompiler;

			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			byte threadData[1024];
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			Context currentContext(threadData, sizeof(threadData));
			GlobalScope aScope(&staticContex,&scriptCompiler);

			wstring functionString1 = L"x = 0";
			wstring functionString2 = L"y = x = x + 1.5";

			ExpressionRef exp1 = compileAndLink(&scriptCompiler, &aScope, functionString1);
			ExpressionRef exp2 = compileAndLink(&scriptCompiler, &aScope, functionString2);

			FF_EXPECT_TRUE(exp1 != nullptr, (L"compile " + functionString1 + L" failed").c_str() );
			FF_EXPECT_TRUE(exp2 != nullptr, (L"compile " + functionString2 + L" failed").c_str());

			aScope.updateVariableOffset();

			ExpUnitExecutor* pExpUnitExecutor1 = convertToPlainCode(&aScope, exp1);
			ExpUnitExecutor* pExpUnitExecutor2 = convertToPlainCode(&aScope, exp2);

			Program program;
			scriptCompiler.bindProgram(&program);

			program.addExecutor((ExecutorRef)(pExpUnitExecutor1));
			program.addExecutor((ExecutorRef)(pExpUnitExecutor2));
			program.convertToPlainCode();

			currentContext.setCurrentCommand(program.getFirstCommand());
			currentContext.setEndCommand(program.getEndCommand());
			currentContext.run();

			Variable* pX = aScope.findVariable("x");
			int* x = getVaribleRef<int>(*pX);
			FF_EXPECT_TRUE(*x == 1);

			Variable* pY = aScope.findVariable("y");
			int* y = getVaribleRef<int>(*pY);
			FF_EXPECT_TRUE(*y == 1);
		}

		//test run string function
		FF_TEST_FUNCTION(Expression2PlainCode, Expression2PlainCode27)
		{
			ScriptCompiler scriptCompiler;

			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();

			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			byte threadData[1024];
			byte globalData[1024];
			StaticContext staticContex(globalData, sizeof(globalData));
			Context currentContext(threadData, sizeof(threadData));
			GlobalScope aScope(&staticContex,&scriptCompiler);

			wstring stringValue = L"this is a string";
			wstring functionString1 = L"x = length(\"" + stringValue + L"\")" ;			

			ExpressionRef exp1 = compileAndLink(&scriptCompiler, &aScope, functionString1);
			FF_EXPECT_TRUE(exp1.get() != nullptr, L"compile string function failed");

			aScope.updateVariableOffset();

			ExpUnitExecutor* pExpUnitExecutor1 = convertToPlainCode(&aScope, exp1);

			Program program;
			scriptCompiler.bindProgram(&program);

			program.addExecutor((ExecutorRef)(pExpUnitExecutor1));
			program.convertToPlainCode();

			currentContext.setCurrentCommand(program.getFirstCommand());
			currentContext.setEndCommand(program.getEndCommand());
			currentContext.run();

			Variable* pX = aScope.findVariable("x");
			int* x = getVaribleRef<int>(*pX);
			FF_EXPECT_TRUE(*x == stringValue.length());
		}
	};
}