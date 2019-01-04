/******************************************************************
* File:        AssigmentCompoundUT.cpp
* Description: Test cases for compiling, running C Lambda scripting
*              language that contains compound assignment operators.
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
#include "ScriptTask.h"

using namespace std;
using namespace ffscript;

class AssigmentCompoundUT : public ::testing::Test {
protected:
	ScriptCompiler scriptCompiler;
	FunctionRegisterHelper funcLibHelper;
	const BasicTypes* basicType;
	StaticContext* _staticContext;
	unsigned char _buffer[1024];

	AssigmentCompoundUT() : funcLibHelper(&scriptCompiler) {
		auto typeManager = scriptCompiler.getTypeManager();
		basicType = &(typeManager->getBasicTypes());
		typeManager->registerBasicTypes(&scriptCompiler);
		typeManager->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
		importBasicfunction(funcLibHelper);

		_staticContext = new StaticContext(_buffer, sizeof(_buffer));
	}

	~AssigmentCompoundUT() {
		delete _staticContext;
	}
};

TEST_F(AssigmentCompoundUT, AssignmentCompoundAdd1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");
			
	pA->setDataType(ScriptType(basicType->TYPE_INT,"int"));

	globalScope.updateVariableOffset();

	int& a = *getVaribleRef<int>(*pA);
	a = 0;

	wstring exp(L"a += 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == 2) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundSub1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_INT,"int"));

	globalScope.updateVariableOffset();

	int& a = *getVaribleRef<int>(*pA);
	a = 0;

	wstring exp(L"a -= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler,exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == -2) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundMul1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_INT,"int"));

	globalScope.updateVariableOffset();

	int& a = *getVaribleRef<int>(*pA);
	a = 1;

	wstring exp(L"a *= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == 2) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundDev1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_INT,"int"));

	globalScope.updateVariableOffset();

	int& a = *getVaribleRef<int>(*pA);
	a = 5;

	wstring exp(L"a /= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == 2) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundMod1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_INT,"int"));

	globalScope.updateVariableOffset();

	int& a = *getVaribleRef<int>(*pA);
	a = 5;

	wstring exp(L"a %= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == 1) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundAnd1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_INT,"int"));

	globalScope.updateVariableOffset();

	int& a = *getVaribleRef<int>(*pA);
	a = 5;

	wstring exp(L"a &= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == (5&2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundOr1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_INT,"int"));

	globalScope.updateVariableOffset();

	int& a = *getVaribleRef<int>(*pA);
	a = 5;

	wstring exp(L"a |= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == (5 | 2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundXor1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_INT,"int"));

	globalScope.updateVariableOffset();

	int& a = *getVaribleRef<int>(*pA);
	a = 5;

	wstring exp(L"a ^= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == (5 ^ 2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundShiftLeft1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_INT,"int"));

	globalScope.updateVariableOffset();

	int& a = *getVaribleRef<int>(*pA);
	a = 5;

	wstring exp(L"a <<= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == (5 << 2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundShiftRight1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_INT,"int"));

	globalScope.updateVariableOffset();

	int& a = *getVaribleRef<int>(*pA);
	a = 5;

	wstring exp(L"a >>= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == (5 >> 2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}
////////////
TEST_F(AssigmentCompoundUT, AssignmentCompoundAddLong1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_LONG,"long"));

	globalScope.updateVariableOffset();

	__int64& a = *getVaribleRef<__int64>(*pA);
	a = 0;

	wstring exp(L"a += 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == 2) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundSubLong1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_LONG,"long"));

	globalScope.updateVariableOffset();

	__int64& a = *getVaribleRef<__int64>(*pA);
	a = 0;

	wstring exp(L"a -= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == -2) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundMulLong1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_LONG,"long"));

	globalScope.updateVariableOffset();

	__int64& a = *getVaribleRef<__int64>(*pA);
	a = 1;

	wstring exp(L"a *= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == 2) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundDevLong1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_LONG,"long"));

	globalScope.updateVariableOffset();

	__int64& a = *getVaribleRef<__int64>(*pA);
	a = 5;

	wstring exp(L"a /= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == 2) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundModLong1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_LONG,"long"));

	globalScope.updateVariableOffset();

	__int64& a = *getVaribleRef<__int64>(*pA);
	a = 5;

	wstring exp(L"a %= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == 1) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundAndLong1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_LONG,"long"));

	globalScope.updateVariableOffset();

	__int64& a = *getVaribleRef<__int64>(*pA);
	a = 5;

	wstring exp(L"a &= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == (5 & 2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundOrLong1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_LONG,"long"));

	globalScope.updateVariableOffset();

	__int64& a = *getVaribleRef<__int64>(*pA);
	a = 5;

	wstring exp(L"a |= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == (5 | 2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundXorLong1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_LONG,"long"));

	globalScope.updateVariableOffset();

	__int64& a = *getVaribleRef<__int64>(*pA);
	a = 5;

	wstring exp(L"a ^= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == (5 ^ 2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundShiftLeftLong1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_LONG,"long"));

	globalScope.updateVariableOffset();

	__int64& a = *getVaribleRef<__int64>(*pA);
	a = 5;

	wstring exp(L"a <<= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == (5 << 2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundShiftRightLong1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_LONG,"long"));

	globalScope.updateVariableOffset();

	__int64& a = *getVaribleRef<__int64>(*pA);
	a = 5;

	wstring exp(L"a >>= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == (5 >> 2)) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

////////////////////////////////////////////////////////////////////////////
TEST_F(AssigmentCompoundUT, AssignmentCompoundAddFloat1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_FLOAT,"float"));

	globalScope.updateVariableOffset();

	float& a = *getVaribleRef<float>(*pA);
	a = 0;

	wstring exp(L"a += 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == 2) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundSubFloat1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_FLOAT,"float"));

	globalScope.updateVariableOffset();

	float& a = *getVaribleRef<float>(*pA);
	a = 0;

	wstring exp(L"a -= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == -2) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundMulFloat1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_FLOAT,"float"));

	globalScope.updateVariableOffset();

	float& a = *getVaribleRef<float>(*pA);
	a = 1;

	wstring exp(L"a *= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == 2) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundDevFloat1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_FLOAT,"float"));

	globalScope.updateVariableOffset();

	float& a = *getVaribleRef<float>(*pA);
	a = 5;

	wstring exp(L"a /= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == 2.5f) << (L"result of expression '" + exp + L"' is not correct").c_str();
}		
////////////////////////////////////////////////////////////////////////////
TEST_F(AssigmentCompoundUT, AssignmentCompoundAddDouble1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_DOUBLE,"double"));

	globalScope.updateVariableOffset();

	double& a = *getVaribleRef<double>(*pA);
	a = 0;

	wstring exp(L"a += 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == 2) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundSubDouble1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_DOUBLE,"double"));

	globalScope.updateVariableOffset();

	double& a = *getVaribleRef<double>(*pA);
	a = 0;

	wstring exp(L"a -= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == -2) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundMulDouble1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_DOUBLE,"double"));

	globalScope.updateVariableOffset();

	double& a = *getVaribleRef<double>(*pA);
	a = 1;

	wstring exp(L"a *= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == 2) << (L"result of expression '" + exp + L"' is not correct").c_str();
}

TEST_F(AssigmentCompoundUT, AssignmentCompoundDevDouble1)
{
	GlobalScope globalScope(_staticContext, &scriptCompiler);
	Variable* pA = globalScope.registVariable("a");

	pA->setDataType(ScriptType(basicType->TYPE_DOUBLE,"double"));

	globalScope.updateVariableOffset();

	double& a = *getVaribleRef<double>(*pA);
	a = 5;

	wstring exp(L"a /= 2");
	scriptCompiler.pushScope(&globalScope);
	ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
	scriptCompiler.popScope();
	EXPECT_TRUE(pExcutor != nullptr) << (L"compile '" + exp + L"' failed!").c_str();

	unique_ptr<ExpUnitExecutor> excutor(pExcutor);

	excutor->runCode();

	PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

	EXPECT_TRUE(a == 2.5f) << (L"result of expression '" + exp + L"' is not correct").c_str();
}