/******************************************************************
* File:        CompileDynamicFunctionUT.cpp
* Description: Test cases for compiling, running C Lambda scripting
*              language that contains dynamic parameter functions.
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
#include "GlobalScope.h"
#include "Utils.h"
#include "Variable.h"
#include "DynamicFunctionFactory.h"
#include "function/FunctionDelegate3.hpp"

using namespace std;
using namespace ffscript;	

namespace ffscriptUT
{		
	FF_TEST_CLASS(RunDynamicFunction)
	{
	protected:
		ScriptCompiler scriptCompiler;
		FunctionRegisterHelper funcLibHelper;
		const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
		StaticContext* _staticContext;
		unsigned char _buffer[1024];

		static int sum(SimpleVariantArray* params) {
			int sum = 0;
			for (int i = 0; i < params->size; i++) {
				sum += *((int*)params->elems[i].pData);
			}
			return sum;
		}

		static int sumTypes(SimpleVariantArray* params) {
			int sum = 0;
			for (int i = 0; i < params->size; i++) {
				sum += params->elems[i].scriptType;
			}
			return sum;
		}

		static int productTypes(SimpleVariantArray* params) {
			int prod = 1;
			for (int i = 0; i < params->size; i++) {
				prod *= params->elems[i].scriptType;
			}
			return prod;
		}

		RunDynamicFunction() :funcLibHelper(&scriptCompiler) {
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			//register dynamic functions
			auto theNativeFunction1 = new FT::FunctionDelegate3<int, SimpleVariantArray*>(sum);
			auto dynamicFunctionFactory1 = new DynamicFunctionFactory("int", theNativeFunction1, &scriptCompiler);
			funcLibHelper.getSriptCompiler()->registDynamicFunction("sum", dynamicFunctionFactory1);
			funcLibHelper.addFactory(dynamicFunctionFactory1);

			auto theNativeFunction2 = new FT::FunctionDelegate3<int, SimpleVariantArray*>(sumTypes);
			auto dynamicFunctionFactory2 = new DynamicFunctionFactory("int", theNativeFunction2, &scriptCompiler);
			funcLibHelper.getSriptCompiler()->registDynamicFunction("sumTypes", dynamicFunctionFactory2);
			funcLibHelper.addFactory(dynamicFunctionFactory2);

			auto theNativeFunction3 = new FT::FunctionDelegate3<int, SimpleVariantArray*>(productTypes);
			auto dynamicFunctionFactory3 = new DynamicFunctionFactory("int", theNativeFunction3, &scriptCompiler);
			funcLibHelper.getSriptCompiler()->registDynamicFunction("productTypes", dynamicFunctionFactory3);
			funcLibHelper.addFactory(dynamicFunctionFactory3);

			//create a static context
			_staticContext = new StaticContext(_buffer, sizeof(_buffer));
		}

		~RunDynamicFunction() {
			delete _staticContext;
		}
	};

	namespace RunDynamicFunctionUT {
		FF_TEST_METHOD(RunDynamicFunction, RunDynamicFunction1)
		{
			wstring exp = L"sum(1,2,3)";
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			
			FF_EXPECT_EQ(6, *result, (L"result of expression '" + exp + L"' should be 6").c_str());
		}

		FF_TEST_METHOD(RunDynamicFunction, RunDynamicFunction2)
		{
			wstring exp = L"sumTypes(1,2,3)";
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());

			FF_EXPECT_EQ(3* basicType.TYPE_INT, *result, (L"result of expression '" + exp + L"' should be 3").c_str());
		}

		FF_TEST_METHOD(RunDynamicFunction, RunDynamicFunction3)
		{
			wstring exp = L"productTypes(1,2,3)";
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());

			FF_EXPECT_EQ(basicType.TYPE_INT, *result, (L"result of expression '" + exp + L"' should be 1").c_str());
		}

		FF_TEST_METHOD(RunDynamicFunction, RunDynamicFunction4)
		{
			wstring exp = L"sumTypes(1,2,3.5)";
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());

			FF_EXPECT_EQ(basicType.TYPE_INT*2 + basicType.TYPE_DOUBLE, *result, (L"result of expression '" + exp + L"' should be 1").c_str());
		}

		FF_TEST_METHOD(RunDynamicFunction, RunDynamicFunction5)
		{
			wstring exp = L"productTypes(1,2.5,3)";
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());

			FF_EXPECT_EQ(basicType.TYPE_INT*basicType.TYPE_DOUBLE, *result, (L"result of expression '" + exp + L"' should be 1").c_str());
		}

		FF_TEST_METHOD(RunDynamicFunction, RunDynamicCombine1)
		{
			wstring exp = L"4 + 5 * sum(1,2,3)";
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());

			FF_EXPECT_EQ(4 + 5*6, *result, (L"result of expression '" + exp + L"' should be 34").c_str());
		}

		static int product(int a, int b) {
			return a * b;
		}

		static Function* createSumFunction(const string& name, int id) {
			auto pFunc = new FixParamFunction<2>(name, EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int");
			static FT::FunctionDelegate3<int, int, int> productNative(product);
			pFunc->setNative( (DFunction2Ref)(productNative.clone()));
			return pFunc;
		}

		FF_TEST_METHOD(RunDynamicFunction, RunDynamicCombine2)
		{
			//register a custom function
			auto sumFactor = new FunctionFactoryCdecl(createSumFunction, ScriptType(basicType.TYPE_INT, "int"));
			funcLibHelper.registFunction("sum", "int,int", sumFactor);

			wstring exp = L"sum(4, 5) * sum(1,2,3) + 6";
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());

			FF_EXPECT_EQ(product(4,5) * 6 + 6, *result, (L"result of expression '" + exp + L"' should be 126").c_str());
		}
	};
}