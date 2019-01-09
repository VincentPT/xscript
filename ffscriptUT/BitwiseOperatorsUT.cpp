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
#include "fftest.hpp"

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
	wstring exp = L"1 & 2";
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();

	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (1 & 2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseAndIntegers2)
{
	wstring exp = L"1 & 0";
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (1 & 0)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseOrIntegers1)
{
	wstring exp = L"1 | 2";
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();

	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (1 | 2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseOrIntegers2)
{
	wstring exp = L"1 | 0";
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (1 | 0)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseXorIntegers1)
{
	wstring exp = L"7 ^ 10";
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();

	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (7 ^ 10)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseXorIntegers2)
{
	wstring exp = L"0 ^ 7";
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (0 ^ 7)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseShiftLeftIntegers)
{
	wstring exp = L"4 << 3";
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (4 << 3)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseShiftRightIntegers)
{
	wstring exp = L"12234 >> 5";
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (12234 >> 5)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseNotIntegers1)
{
	wstring exp = L"~10";
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (~10)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseNotIntegers2)
{
	wstring exp = L"~0";
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (~0)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(BitwiseOperatorsUT, BitwiseCombineIntegers)
{
	wstring exp = L"10 & 11 | 15 ^ ~19";
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();
	int* result = (int*)excutor->getReturnData();

	EXPECT_TRUE(result != nullptr) << (L"run expression '" + exp + L"' failed!").c_str();
	PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
	EXPECT_TRUE(*result == (10 & 11 | 15 ^ ~19)) << (L"result of expression '" + exp + L"' is not correct").c_str();
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

	wstring exp = L"a & b";
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

	wstring exp = L"a | b";
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

	wstring exp = L"a ^ b";
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

	wstring exp = L"a << 2";
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

	wstring exp = L"a >> b";
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

	wstring exp = L"~a";
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

	wstring exp = L"a ^ b | c ^ ~d & e << 2";
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