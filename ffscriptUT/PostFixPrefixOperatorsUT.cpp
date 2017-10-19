#include "stdafx.h"
#include "CppUnitTest.h"
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
#include "ReservedContextScope.h"

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
	TEST_CLASS(PostFixPrefixOperatorsUT)
	{
		ScriptCompiler scriptCompiler;
		FunctionRegisterHelper funcLibHelper;
		const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
		StaticContext* _staticContext;
		unsigned char _buffer[1024];
	public:

		PostFixPrefixOperatorsUT() : funcLibHelper(&scriptCompiler) {
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);

			_staticContext = new StaticContext(_buffer, sizeof(_buffer));
		}

		~PostFixPrefixOperatorsUT() {
			delete _staticContext;
		}		

		TEST_METHOD(PostFixInc1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			a = 0;
			Assert::IsTrue(*result == (a++), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PostFixInc2)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			a = 0;
			Assert::IsTrue(*result == (a++ * 2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PostFixInc3)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			Assert::IsTrue(*result == 1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PostFixInc5)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			Assert::IsTrue(*result == 1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PostFixInc4)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			a = 0;
			Assert::IsTrue(*result == (3 + a++), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PreFixInc1)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			a = 0;
			Assert::IsTrue(*result == (++a), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PreFixInc2)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			a = 0;
			Assert::IsTrue(*result == (++a * 2), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PreFixInc3)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			a = 0;
			Assert::IsTrue(*result == (++a + a), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PreFixInc4)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			a = 0;
			Assert::IsTrue(*result == (3 - ++a), (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PostFixInc6)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			Assert::IsTrue(*result == -1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PostFixIncFloat)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			float* result = (float*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			Assert::IsTrue(*result == 0, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PostFixDecFloat)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			float* result = (float*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			Assert::IsTrue(*result == 0, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PreFixIncFloat)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			float* result = (float*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			Assert::IsTrue(*result == 1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PreFixDecFloat)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			float* result = (float*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			Assert::IsTrue(*result == -1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PostFixIncDouble)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			double* result = (double*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			Assert::IsTrue(*result == 0, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PostFixDecDouble)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			double* result = (double*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			Assert::IsTrue(*result == 0, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PreFixIncDouble)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			double* result = (double*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			Assert::IsTrue(*result == 1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PreFixDecDouble)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			double* result = (double*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			Assert::IsTrue(*result == -1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PostFixIncLong)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			double* result = (double*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			Assert::IsTrue(*result == 0, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PostFixDecLong)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			__int64* result = (__int64*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			Assert::IsTrue(*result == 0, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PreFixIncLong)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			__int64* result = (__int64*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			Assert::IsTrue(*result == 1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PreFixDecLong)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			__int64* result = (__int64*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			Assert::IsTrue(*result == -1, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(FunctionAndPostFix1)
		{	
			int a = 0;
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			ReservedContextScope reservedContextScope(_staticContext);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int a) {"
				L"	return a++;"
				L"}"
				;

			const wchar_t* res = globalScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = globalScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("foo", "int");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(a);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 0, L"program can run but return wrong value");
		}

		TEST_METHOD(FunctionAndPreFix1)
		{
			int a = 0;
			GlobalScope globalScope(_staticContext,&scriptCompiler);;
			ReservedContextScope reservedContextScope(_staticContext);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode =
				L"int foo(int a) {"
				L"	return ++a;"
				L"}"
				;

			const wchar_t* res = globalScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = globalScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("foo", "int");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptParamBuffer paramBuffer(a);
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("foo =" + std::to_string(*funcRes)).c_str());
			Assert::IsTrue(*funcRes == 1, L"program can run but return wrong value");			
		}

		TEST_METHOD(PreFixInc5)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			Assert::IsTrue(*result == 2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}

		TEST_METHOD(PreFixInc8)
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
			Assert::IsTrue(pExcutor != nullptr, (L"compile '" + exp + L"' failed!").c_str());

			unique_ptr<ExpUnitExecutor> excutor(pExcutor);

			excutor->runCode();
			int* result = (int*)excutor->getReturnData();

			Assert::IsTrue(result != nullptr, (L"run expression '" + exp + L"' failed!").c_str());
			PRINT_TEST_MESSAGE((exp + L" = " + std::to_wstring(*result)).c_str());

			Assert::IsTrue(*result == -2, (L"result of expression '" + exp + L"' is not correct").c_str());
		}
	};
}