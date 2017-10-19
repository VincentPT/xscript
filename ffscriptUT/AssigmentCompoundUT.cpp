#include "stdafx.h"
#include "CppUnitTest.h"
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
#include "ReservedContextScope.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace ffscript;


namespace ffscriptUT
{
	TEST_CLASS(AssigmentCompoundUT)
	{
		ScriptCompiler scriptCompiler;
		FunctionRegisterHelper funcLibHelper;
		const BasicTypes* basicType;
		StaticContext* _staticContext;
		unsigned char _buffer[1024];
	public:

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

		TEST_METHOD(AssignmentCompoundAdd1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == 2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundSub1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == -2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundMul1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == 2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundDev1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == 2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundMod1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == 1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundAnd1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == (5&2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundOr1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == (5 | 2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundXor1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == (5 ^ 2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundShiftLeft1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == (5 << 2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundShiftRight1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == (5 >> 2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}
		////////////
		TEST_METHOD(AssignmentCompoundAddLong1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == 2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundSubLong1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == -2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundMulLong1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == 2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundDevLong1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == 2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundModLong1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == 1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundAndLong1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == (5 & 2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundOrLong1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == (5 | 2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundXorLong1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == (5 ^ 2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundShiftLeftLong1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == (5 << 2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundShiftRightLong1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == (5 >> 2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		////////////////////////////////////////////////////////////////////////////
		TEST_METHOD(AssignmentCompoundAddFloat1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == 2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundSubFloat1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == -2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundMulFloat1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == 2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundDevFloat1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == 2.5f, (L"result of expression '" + exp + L"' is not correct").c_str());
		}		
		////////////////////////////////////////////////////////////////////////////
		TEST_METHOD(AssignmentCompoundAddDouble1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == 2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundSubDouble1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == -2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundMulDouble1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == 2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(AssignmentCompoundDevDouble1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();

			PRINT_TEST_MESSAGE((L"run expression '" + exp + L"' then a =" + std::to_wstring(a)).c_str());

			Assert::IsTrue(a == 2.5f, (L"result of expression '" + exp + L"' is not correct").c_str());
		}
		////////////////////////////////////////////////////////////////////////////
	};
}