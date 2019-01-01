/******************************************************************
* File:        BitwiseOperatorsUT.cpp
* Description: Test cases for compiling, running C Lambda scripting
*              language that contains bitwise operators.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include <gtest/gtest.h>

#include "TemplateForTest.hpp"
#include <functional>
#include "FunctionRegisterHelper.h"
#include "BasicFunction.h"
#include "StaticContext.h"
#include "Utility.h"
#include <memory>
#include "GlobalScope.h"
#include "Utils.h"
#include "Variable.h"

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

class BitwiseOperatorsUT : public ::testing::Test {
protected:
	ScriptCompiler scriptCompiler;
	FunctionRegisterHelper funcLibHelper;
	StaticContext* _staticContext;
	const BasicTypes* basicType;
	unsigned char _buffer[1024];

	BitwiseOperatorsUT() : funcLibHelper(&scriptCompiler) {

		auto typeManager = scriptCompiler.getTypeManager();
		basicType = &(typeManager->getBasicTypes());
		typeManager->registerBasicTypes(&scriptCompiler);
		typeManager->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
		importBasicfunction(funcLibHelper);

		_staticContext = new StaticContext(_buffer, sizeof(_buffer));
	}

	~BitwiseOperatorsUT() {
		delete _staticContext;
	}
};

TEST_F(BitwiseOperatorsUT, BitwiseAndIntegers1)
{
	wstring exp = MAKE_WSTRING_X(BITWISEANDINTEGERS1);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();

	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (BITWISEANDINTEGERS1)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseAndIntegers2)
{
	wstring exp = MAKE_WSTRING_X(BITWISEANDINTEGERS2);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (BITWISEANDINTEGERS2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseOrIntegers1)
{
	wstring exp = MAKE_WSTRING_X(BITWISEORINTEGERS1);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();

	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (BITWISEORINTEGERS1)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseOrIntegers2)
{
	wstring exp = MAKE_WSTRING_X(BITWISEORINTEGERS2);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (BITWISEORINTEGERS2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseXorIntegers1)
{
	wstring exp = MAKE_WSTRING_X(BITWISEXORINTEGERS1);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();

	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (BITWISEXORINTEGERS1)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseXorIntegers2)
{
	wstring exp = MAKE_WSTRING_X(BITWISEXORINTEGERS2);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (BITWISEXORINTEGERS2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseShiftLeftIntegers)
{
	wstring exp = MAKE_WSTRING_X(BITWISESHIFTLEFTINTEGERS);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (BITWISESHIFTLEFTINTEGERS)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseShiftRightIntegers)
{
	wstring exp = MAKE_WSTRING_X(BITWISESHIFTRIGHTINTEGERS);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (BITWISESHIFTRIGHTINTEGERS)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseNotIntegers1)
{
	wstring exp = MAKE_WSTRING_X(BITWISENOTINTEGERS1);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (BITWISENOTINTEGERS1)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseNotIntegers2)
{
	wstring exp = MAKE_WSTRING_X(BITWISENOTINTEGERS2);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (BITWISENOTINTEGERS2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseCombineIntegers)
{
	wstring exp = MAKE_WSTRING_X(BITWISECOMBINEINTEGERS);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (BITWISECOMBINEINTEGERS)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseAndLongs)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");
	Variable* pB = globalScope.registVariable("b");

	pA->setDataType(ScriptType(basicType->TYPE_LONG,"long"));
	pB->setDataType(ScriptType(basicType->TYPE_LONG,"long"));

	globalScope.updateVariableOffset();

	__int64& a = *getVaribleRef<__int64>(*pA);
	__int64& b = *getVaribleRef<__int64>(*pB);

	a = 100;
	b = 123;

	wstring exp = MAKE_WSTRING(a & b);
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	__int64* result = (__int64*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (a & b)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseOrLongs)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");
	Variable* pB = globalScope.registVariable("b");

	pA->setDataType(ScriptType(basicType->TYPE_LONG,"long"));
	pB->setDataType(ScriptType(basicType->TYPE_LONG,"long"));

	globalScope.updateVariableOffset();

	__int64& a = *getVaribleRef<__int64>(*pA);
	__int64& b = *getVaribleRef<__int64>(*pB);

	a = 100;
	b = 123;

	wstring exp = MAKE_WSTRING(a | b);
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	__int64* result = (__int64*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (a | b)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseXorLongs)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");
	Variable* pB = globalScope.registVariable("b");

	pA->setDataType(ScriptType(basicType->TYPE_LONG,"long"));
	pB->setDataType(ScriptType(basicType->TYPE_LONG,"long"));

	globalScope.updateVariableOffset();

	__int64& a = *getVaribleRef<__int64>(*pA);
	__int64& b = *getVaribleRef<__int64>(*pB);

	a = 100;
	b = 123;

	wstring exp = MAKE_WSTRING(a ^ b);
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	__int64* result = (__int64*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (a ^ b)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseShitLeftLongs)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_LONG,"long"));

	globalScope.updateVariableOffset();

	__int64& a = *getVaribleRef<__int64>(*pA);
	a = 100;

	wstring exp = MAKE_WSTRING(a << 2);
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	__int64* result = (__int64*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (a << 2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseShitRightLongs)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");
	Variable* pB = globalScope.registVariable("b");

	pA->setDataType(ScriptType(basicType->TYPE_LONG,"long"));
	pB->setDataType(ScriptType(basicType->TYPE_LONG,"long"));

	globalScope.updateVariableOffset();

	__int64& a = *getVaribleRef<__int64>(*pA);
	__int64& b = *getVaribleRef<__int64>(*pB);

	a = 100;
	b = 2;

	wstring exp = MAKE_WSTRING(a >> b);
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	__int64* result = (__int64*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (a >> b)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseNotLongs)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_LONG,"long"));

	globalScope.updateVariableOffset();

	__int64& a = *getVaribleRef<__int64>(*pA);

	a = 100;

	wstring exp = MAKE_WSTRING( ~a);
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	__int64* result = (__int64*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (~a)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseCombineLongs)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");
	Variable* pB = globalScope.registVariable("b");
	Variable* pC = globalScope.registVariable("c");
	Variable* pD = globalScope.registVariable("d");
	Variable* pE = globalScope.registVariable("e");

	pA->setDataType(ScriptType(basicType->TYPE_LONG,"long"));
	pB->setDataType(ScriptType(basicType->TYPE_LONG,"long"));
	pC->setDataType(ScriptType(basicType->TYPE_INT,"int"));
	pD->setDataType(ScriptType(basicType->TYPE_LONG,"long"));
	pE->setDataType(ScriptType(basicType->TYPE_LONG,"long"));

	globalScope.updateVariableOffset();

	__int64& a = *getVaribleRef<__int64>(*pA);
	__int64& b = *getVaribleRef<__int64>(*pB);
	int& c = *getVaribleRef<int>(*pC);
	__int64& d = *getVaribleRef<__int64>(*pD);
	__int64& e = *getVaribleRef<__int64>(*pE);

	a = 100;
	b = 123;
	c = 245;
	d = 245123123123;
	e = 123;

	wstring exp = MAKE_WSTRING(a ^ b | c ^ ~d & e << 2);
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	__int64* result = (__int64*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (a ^ b | c ^ ~d & e << 2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}