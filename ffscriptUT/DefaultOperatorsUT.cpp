/******************************************************************
* File:        DefaultOperatorsUT.cpp
* Description: Test cases for checking behavior of default operators.
*              Default operators are operators is supported automatically
*              by the scripting library for any types such as
*              operator '=', 'ref'
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
#include "function/MemberFunction.hpp"
#include "Context.h"
#include <thread>
#include <future>
#include <Program.h>
#include <ScriptTask.h>

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
#include "function/MemberFunction3.hpp"
#include "expressionunit.h"
#include "DynamicFunctionFactory.h"

namespace ffscriptUT
{
#pragma pack(push)
#pragma pack(1)
	struct TestStruct {
		int a;
		int b;
	};
#pragma pack(pop)

	FF_TEST_CLASS(DefaultOperators)
	{
	protected:
		ScriptCompiler scriptCompiler;
		FunctionRegisterHelper funcLibHelper;
		const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
	public:
		DefaultOperators() : funcLibHelper(&scriptCompiler) {
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);
		}
	};

	namespace DefaultOperatorsUT {
		FF_TEST_METHOD(DefaultOperators, DefaultAssignRef)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			auto newType = scriptCompiler.registType("TestType");
			FF_EXPECT_NE(DATA_TYPE_UNKNOWN, newType, L"Regist new type failed");
			scriptCompiler.setTypeSize(newType, 4);
			
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"TestType dummy;"
				L"ref TestType test() {"
				L"	ref TestType refOfDummy = ref(dummy);"
				L"	return refOfDummy;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			FF_EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("test", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			void** funcRes = (void**)scriptTask.getTaskResult();

			auto pVariable = rootScope.findVariable("dummy");
			void* pDummy = getVaribleRef<void>(*pVariable);

			FF_EXPECT_EQ( (size_t) pDummy, (size_t)*funcRes , L"program can run but return wrong value");
		}

		FF_TEST_METHOD(DefaultOperators, DefaultAssignNewType)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			auto newType = scriptCompiler.registType("TestType");
			FF_EXPECT_NE(DATA_TYPE_UNKNOWN, newType, L"Regist new type failed");
			scriptCompiler.setTypeSize(newType, 4);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"TestType dummy;"
				L"ref TestType getValRef() {"
				L"	return ref(dummy);"
				L"}"
				L"void setVal(TestType val) {"
				L"	*getValRef() = val;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			FF_EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("setVal", "TestType");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			int param = 1;
			ScriptParamBuffer paramBuffer(param);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);

			auto pVariable = rootScope.findVariable("dummy");
			int* pDummy = getVaribleRef<int>(*pVariable);

			FF_EXPECT_EQ(param, *pDummy, L"program can run but return wrong value");
		}

		FF_TEST_METHOD(DefaultOperators, DefaultAssignForAutoType)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			auto newType = scriptCompiler.registType("TestType");
			FF_EXPECT_NE(DATA_TYPE_UNKNOWN, newType, L"Regist new type failed");
			scriptCompiler.setTypeSize(newType, 4);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int setVal() {"
				L"	return i = 1;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			FF_EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("setVal", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(1, *iRes, L"program can run but return wrong value");
		}

		FF_TEST_METHOD(DefaultOperators, DefaultAssignForDeclareTypeInExpression)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			auto newType = scriptCompiler.registType("TestType");
			FF_EXPECT_NE(DATA_TYPE_UNKNOWN, newType, L"Regist new type failed");
			scriptCompiler.setTypeSize(newType, 4);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int setVal() {"
				L"	int i = 1;"
				L"	return i;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			FF_EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("setVal", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(1, *iRes, L"program can run but return wrong value");
		}
	};
}