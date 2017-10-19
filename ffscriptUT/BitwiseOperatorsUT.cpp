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

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
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
	TEST_CLASS(BitwiseOperatorsUT)
	{
		ScriptCompiler scriptCompiler;
		FunctionRegisterHelper funcLibHelper;
		StaticContext* _staticContext;
		const BasicTypes* basicType;
		unsigned char _buffer[1024];
	public:

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

		TEST_METHOD(BitwiseAndIntegers1)
		{
			wstring exp = MAKE_WSTRING_X(BITWISEANDINTEGERS1);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());

			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (BITWISEANDINTEGERS1), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(BitwiseAndIntegers2)
		{
			wstring exp = MAKE_WSTRING_X(BITWISEANDINTEGERS2);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (BITWISEANDINTEGERS2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(BitwiseOrIntegers1)
		{
			wstring exp = MAKE_WSTRING_X(BITWISEORINTEGERS1);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());

			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (BITWISEORINTEGERS1), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(BitwiseOrIntegers2)
		{
			wstring exp = MAKE_WSTRING_X(BITWISEORINTEGERS2);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (BITWISEORINTEGERS2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(BitwiseXorIntegers1)
		{
			wstring exp = MAKE_WSTRING_X(BITWISEXORINTEGERS1);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());

			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (BITWISEXORINTEGERS1), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(BitwiseXorIntegers2)
		{
			wstring exp = MAKE_WSTRING_X(BITWISEXORINTEGERS2);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (BITWISEXORINTEGERS2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(BitwiseShiftLeftIntegers)
		{
			wstring exp = MAKE_WSTRING_X(BITWISESHIFTLEFTINTEGERS);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (BITWISESHIFTLEFTINTEGERS), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(BitwiseShiftRightIntegers)
		{
			wstring exp = MAKE_WSTRING_X(BITWISESHIFTRIGHTINTEGERS);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (BITWISESHIFTRIGHTINTEGERS), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(BitwiseNotIntegers1)
		{
			wstring exp = MAKE_WSTRING_X(BITWISENOTINTEGERS1);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (BITWISENOTINTEGERS1), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(BitwiseNotIntegers2)
		{
			wstring exp = MAKE_WSTRING_X(BITWISENOTINTEGERS2);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (BITWISENOTINTEGERS2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(BitwiseCombineIntegers)
		{
			wstring exp = MAKE_WSTRING_X(BITWISECOMBINEINTEGERS);
			ExpUnitExecutor* pExcutor = compileExpression(&scriptCompiler, exp);
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (BITWISECOMBINEINTEGERS), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(BitwiseAndLongs)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			__int64* result = (__int64*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (a & b), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(BitwiseOrLongs)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			__int64* result = (__int64*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (a | b), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(BitwiseXorLongs)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			__int64* result = (__int64*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (a ^ b), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(BitwiseShitLeftLongs)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			__int64* result = (__int64*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (a << 2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(BitwiseShitRightLongs)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			__int64* result = (__int64*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (a >> b), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(BitwiseNotLongs)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			__int64* result = (__int64*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (~a), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(BitwiseCombineLongs)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			__int64* result = (__int64*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());
			Assert::IsTrue(*result == (a ^ b | c ^ ~d & e << 2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}
	};
}