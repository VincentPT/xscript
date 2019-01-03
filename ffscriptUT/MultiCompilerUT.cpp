/******************************************************************
* File:        MultiCompilerUT.cpp
* Description: Test cases for using many compiler instance
*              at the same time to compile C Lambda scripting language.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "fftest.hpp"

#include "TemplateForTest.hpp"
#include <functional>
#include "FunctionRegisterHelper.h"
#include "BasicFunction.h"
#include "BasicType.h"
#include "StaticContext.h"
#include "Utility.h"
#include <memory>
#include <thread>
#include "GlobalScope.h"
#include "Utils.h"
#include "Variable.h"
#include "DynamicFunctionFactory.h"
#include "function/CdeclFunction2.hpp"
#include "function/MemberFunction2.hpp"

using namespace std;
using namespace ffscript;

#include "ExpresionParser.h"
#include "ScriptCompiler.h"
#include "expressionunit.h"
#include "Expression.h"
#include "GlobalScope.h"
#include "Executor.h"
#include "Utils.h"
#include "BasicFunctionFactory.hpp"
#include "ScriptTask.h"
#include <condition_variable>
#include <mutex>

namespace ffscriptUT
{

	template <class T>
	T add(T a, T b) { return a + b; }

	namespace MultiCompilerUT
	{
		FF_TEST_FUNCTION(MultiCompiler, TwoCompilerInstanceSuccessCase1)
		{
			//create first compiler and register basic function to it
			ScriptCompiler scriptCompiler;
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);

			importBasicfunction(funcLibHelper);

			unsigned char _buffer[1024];
			StaticContext _staticContext(_buffer, sizeof(_buffer));
			GlobalScope globalScope(&_staticContext, &scriptCompiler);
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_INT,"int"));

			globalScope.updateVariableOffset();

			int& a = *getVaribleRef<int>(*pA);
			a = 0;

			wstring exp(L"a += 2");
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			FF_EXPECT_TRUE(a == 2, (L"result of expression '" + exp + L"' is not correct").c_str());

			//create second compiler
			ScriptCompiler scriptCompiler2;
			FunctionRegisterHelper functionLib2(&scriptCompiler2);
			int typeInt = scriptCompiler2.registType("int");
			scriptCompiler2.setTypeSize(typeInt, 4);

			DFunction2* addFunc = new CdeclFunction2<int, int, int>(add<int>);


			PredefinedOperator operators[] = {
				{ "+", "int,int", "int", addFunc }
			};

			functionLib2.registPredefinedOperators(operators, sizeof(operators) / sizeof(operators[0]));

			scriptCompiler2.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor2 = compileExpression(&scriptCompiler2, L"1 + 2");
			scriptCompiler2.popScope();
			FF_EXPECT_TRUE(pExcutor2 != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor2(pExcutor2);
			excutor2->runCode();
			int* res = (int*)excutor2->getReturnData();
			FF_EXPECT_TRUE(*res == 3, L"result of expression '1 + 2' is not correct when compile with second compiler");
		}

		FF_TEST_FUNCTION(MultiCompiler, TwoCompilerInstanceSuccessCase2)
		{
			//create first compiler and register basic function to it
			ScriptCompiler scriptCompiler;
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);

			importBasicfunction(funcLibHelper);

			unsigned char _buffer[1024];
			StaticContext _staticContext(_buffer, sizeof(_buffer));
			GlobalScope globalScope(&_staticContext, &scriptCompiler);
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_INT,"int"));

			globalScope.updateVariableOffset();

			int& a = *getVaribleRef<int>(*pA);
			a = 0;

			wstring exp(L"a += 2");
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());
			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			//create second compiler
			ScriptCompiler scriptCompiler2;
			FunctionRegisterHelper functionLib2(&scriptCompiler2);
			int typeInt = scriptCompiler2.registType("int");
			scriptCompiler2.setTypeSize(typeInt, 4);

			DFunction2* addFunc = new CdeclFunction2<int, int, int>(add<int>);


			PredefinedOperator operators[] = {
				{ "+", "int,int", "int", addFunc }
			};

			functionLib2.registPredefinedOperators(operators, sizeof(operators) / sizeof(operators[0]));

			scriptCompiler2.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor2 = compileExpression(&scriptCompiler2, L"1 + 2");
			scriptCompiler2.popScope();
			FF_EXPECT_TRUE(pExcutor2 != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			//run fist code of compiler 1
			excutor->runCode();
			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());
			FF_EXPECT_TRUE(a == 2, (L"result of expression '" + exp + L"' is not correct").c_str());

			//run second code of compiler 2
			unique_ptr<ExpUnitExecutor> excutor2(pExcutor2);
			excutor2->runCode();
			int* res = (int*)excutor2->getReturnData();
			FF_EXPECT_TRUE(*res == 3, L"result of expression '1 + 2' is not correct when compile with second compiler");
		}

		FF_TEST_FUNCTION(MultiCompiler, TwoCompilerInstanceFailedCase)
		{
			//create first compiler and register basic function to it
			ScriptCompiler scriptCompiler;
			FunctionRegisterHelper funcLibHelper(&scriptCompiler);
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);

			importBasicfunction(funcLibHelper);

			unsigned char _buffer[1024];
			StaticContext _staticContext(_buffer, sizeof(_buffer));
			GlobalScope globalScope(&_staticContext, &scriptCompiler);
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_INT,"int"));

			globalScope.updateVariableOffset();

			int& a = *getVaribleRef<int>(*pA);
			a = 0;

			wstring exp(L"a += 2");
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			FF_EXPECT_TRUE(a == 2, (L"result of expression '" + exp + L"' is not correct").c_str());

			//create second compiler
			ScriptCompiler scriptCompiler2;
			FunctionRegisterHelper functionLib2(&scriptCompiler2);
			int typeInt = scriptCompiler2.registType("int");
			scriptCompiler2.setTypeSize(typeInt, 4);

			DFunction2* addFunc = new CdeclFunction2<int, int, int>(add<int>);
			PredefinedOperator operators[] = {
				{ "+", "int,int", "int", addFunc }
			};

			functionLib2.registPredefinedOperators(operators, sizeof(operators) / sizeof(operators[0]));

			scriptCompiler2.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor2 = compileExpression(&scriptCompiler2, L"1 * 2");
			scriptCompiler2.popScope();
			FF_EXPECT_TRUE(pExcutor2 == nullptr, (L"compile '" + exp + L"' must failed when compile with second compiler!").c_str());
		}
	};
}