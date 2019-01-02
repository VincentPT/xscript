/******************************************************************
* File:        PostFixPrefixOperatorsUT.cpp
* Description: Test cases for using postfix and prefix operators
*              in C Lambda scripting language.
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
#include "ScriptTask.h"

using namespace std;
using namespace ffscript;


#define MAKE_WSTRING(a) L#a
#define MAKE_WSTRING_X(a) MAKE_WSTRING(a)

#define BITWISEANDINTEGERS1 1 & 2
#define BITWISEANDINTEGERS2 1 & 0
#define BITWISEORINTEGERS1 1 | 2
#define BITWISEORINTEGERS2 1 | 0
#define BITWISEXORINTEGERS1 7 ^ 10
#define BITWISEXORINTEGERS2 0 ^ 7
#define BITWISENOTINTEGERS1 ~10
#define BITWISENOTINTEGERS2 ~0
#define BITWISESHIFTLEFTINTEGERS 4 << 3
#define BITWISESHIFTRIGHTINTEGERS 12234 >> 5
#define BITWISECOMBINEINTEGERS 10 & 11 | 15 ^ ~19

namespace ffscriptUT
{
	FF_TEST_CLASS(PostFixPrefixOperators)
	{
	protected:
		ScriptCompiler scriptCompiler;
		FunctionRegisterHelper funcLibHelper;
		const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
		StaticContext* _staticContext;
		unsigned char _buffer[1024];
		PostFixPrefixOperators() : funcLibHelper(&scriptCompiler) {
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			_staticContext = new StaticContext(_buffer, sizeof(_buffer));
		}

		~PostFixPrefixOperators() {
			delete _staticContext;
		}
	};

	namespace PostFixPrefixOperatorsUT {
		FF_TEST_METHOD(PostFixPrefixOperators,  PostFixInc1)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");
			
			pA->setDataType(ScriptType(basicType.TYPE_INT, "int"));

			globalScope.updateVariableOffset();

			int& a = *getVaribleRef<int>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(a++);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			a = 0;
			FF_EXPECT_TRUE(*result == (a++), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PostFixInc2)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_INT, "int"));

			globalScope.updateVariableOffset();

			int& a = *getVaribleRef<int>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(a++ * 2);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			a = 0;
			FF_EXPECT_TRUE(*result == (a++ * 2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PostFixInc3)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_INT, "int"));

			globalScope.updateVariableOffset();

			int& a = *getVaribleRef<int>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(a++ + a);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			FF_EXPECT_TRUE(*result == 1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PostFixInc5)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_INT, "int"));

			globalScope.updateVariableOffset();

			int& a = *getVaribleRef<int>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(a + (++ a));
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			FF_EXPECT_TRUE(*result == 1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PostFixInc4)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_INT, "int"));

			globalScope.updateVariableOffset();

			int& a = *getVaribleRef<int>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(3 + a++);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			a = 0;
			FF_EXPECT_TRUE(*result == (3 + a++), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PreFixInc1)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_INT, "int"));

			globalScope.updateVariableOffset();

			int& a = *getVaribleRef<int>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(++a);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			a = 0;
			FF_EXPECT_TRUE(*result == (++a), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PreFixInc2)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_INT, "int"));

			globalScope.updateVariableOffset();

			int& a = *getVaribleRef<int>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(++a * 2);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			a = 0;
			FF_EXPECT_TRUE(*result == (++a * 2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PreFixInc3)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_INT, "int"));

			globalScope.updateVariableOffset();

			int& a = *getVaribleRef<int>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(++a + a);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			a = 0;
			FF_EXPECT_TRUE(*result == (++a + a), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PreFixInc4)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_INT, "int"));

			globalScope.updateVariableOffset();

			int& a = *getVaribleRef<int>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(3 - ++a);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			a = 0;
			FF_EXPECT_TRUE(*result == (3 - ++a), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PostFixInc6)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_INT, "int"));

			globalScope.updateVariableOffset();

			int& a = *getVaribleRef<int>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(a-- + a);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			FF_EXPECT_TRUE(*result == -1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PostFixIncFloat)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_FLOAT, "float"));

			globalScope.updateVariableOffset();

			float& a = *getVaribleRef<float>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(a++);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			float* result = (float*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			FF_EXPECT_TRUE(*result == 0, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PostFixDecFloat)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_FLOAT, "float"));

			globalScope.updateVariableOffset();

			float& a = *getVaribleRef<float>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(a--);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			float* result = (float*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			FF_EXPECT_TRUE(*result == 0, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PreFixIncFloat)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_FLOAT, "float"));

			globalScope.updateVariableOffset();

			float& a = *getVaribleRef<float>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(++a);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			float* result = (float*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			FF_EXPECT_TRUE(*result == 1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PreFixDecFloat)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_FLOAT, "float"));

			globalScope.updateVariableOffset();

			float& a = *getVaribleRef<float>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(--a);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			float* result = (float*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			FF_EXPECT_TRUE(*result == -1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PostFixIncDouble)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_DOUBLE, "double"));

			globalScope.updateVariableOffset();

			double& a = *getVaribleRef<double>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(a++);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			double* result = (double*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			FF_EXPECT_TRUE(*result == 0, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PostFixDecDouble)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_DOUBLE, "double"));

			globalScope.updateVariableOffset();

			double& a = *getVaribleRef<double>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(a--);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			double* result = (double*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			FF_EXPECT_TRUE(*result == 0, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PreFixIncDouble)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_DOUBLE, "double"));

			globalScope.updateVariableOffset();

			double& a = *getVaribleRef<double>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(++a);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			double* result = (double*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			FF_EXPECT_TRUE(*result == 1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PreFixDecDouble)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_DOUBLE, "double"));

			globalScope.updateVariableOffset();

			double& a = *getVaribleRef<double>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(--a);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			double* result = (double*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			FF_EXPECT_TRUE(*result == -1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PostFixIncLong)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_LONG, "long"));

			globalScope.updateVariableOffset();

			__int64& a = *getVaribleRef<__int64>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(a++);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			double* result = (double*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			FF_EXPECT_TRUE(*result == 0, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PostFixDecLong)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_LONG, "long"));

			globalScope.updateVariableOffset();

			__int64& a = *getVaribleRef<__int64>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(a--);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			__int64* result = (__int64*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			FF_EXPECT_TRUE(*result == 0, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PreFixIncLong)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_LONG, "long"));

			globalScope.updateVariableOffset();

			__int64& a = *getVaribleRef<__int64>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(++a);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			__int64* result = (__int64*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			FF_EXPECT_TRUE(*result == 1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PreFixDecLong)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_LONG, "long"));

			globalScope.updateVariableOffset();

			__int64& a = *getVaribleRef<__int64>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(--a);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			__int64* result = (__int64*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			FF_EXPECT_TRUE(*result == -1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}


		FF_TEST_METHOD(PostFixPrefixOperators,  FunctionAndPostFix1)
		{	
			int a = 0;
			GlobalScope globalScope(_staticContext,&scriptCompiler);
			unique_ptr<Context, void(*)(Context*)> reservedContextScope(_staticContext, Context::makeCurrent);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int a) {"
				L"	return a++;"
				L"}"
				;

			const wchar_t* res = globalScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = globalScope.extractCode(&theProgram);
			FF_EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("foo", "int");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(a);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			FF_EXPECT_TRUE(*funcRes == 0, L"program can run but return wrong value");
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  FunctionAndPreFix1)
		{
			int a = 0;
			GlobalScope globalScope(_staticContext,&scriptCompiler);
			unique_ptr<Context, void(*)(Context*)> reservedContextScope(_staticContext, Context::makeCurrent);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int a) {"
				L"	return ++a;"
				L"}"
				;

			const wchar_t* res = globalScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_TRUE(res != nullptr, L"compile program failed");

			bool blRes = globalScope.extractCode(&theProgram);
			FF_EXPECT_TRUE(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("foo", "int");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(a);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			FF_EXPECT_TRUE(*funcRes == 1, L"program can run but return wrong value");			
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PreFixInc5)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_INT,"int"));

			globalScope.updateVariableOffset();

			int& a = *getVaribleRef<int>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(++a + a);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			FF_EXPECT_TRUE(*result == 2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		FF_TEST_METHOD(PostFixPrefixOperators,  PreFixInc8)
		{
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			Variable* pA = globalScope.registVariable("a");

			pA->setDataType(ScriptType(basicType.TYPE_INT, "int"));

			globalScope.updateVariableOffset();

			int& a = *getVaribleRef<int>(*pA);
			a = 0;

			wstring exp = MAKE_WSTRING(--a + a);
			scriptCompiler.pushScope(&globalScope);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			scriptCompiler.popScope();
			FF_EXPECT_TRUE(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			FF_EXPECT_TRUE(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			FF_EXPECT_TRUE(*result == -2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}
	};
}