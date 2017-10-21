#include "stdafx.h"
#include "CppUnitTest.h"
#include "ExpresionParser.h"
#include <functional>
#include "TemplateForTest.hpp"
#include "Variable.h"
#include "FunctionRegisterHelper.h"
#include "BasicFunction.h"
#include "BasicType.h"
#include "FunctionFactory.h"
#include "function\MemberFunction.hpp"
#include "Context.h"
#include <thread>
#include <future>
#include <Program.h>
#include <ScriptTask.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
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
#include "function\MemberFunction2.hpp"
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

	TEST_CLASS(DefaultOperatorsUT)
	{
		ScriptCompiler scriptCompiler;
		FunctionRegisterHelper funcLibHelper;
		const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
	public:
		DefaultOperatorsUT() : funcLibHelper(&scriptCompiler) {
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);
		}

		TEST_METHOD(DefaultAssignRef)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			auto newType = scriptCompiler.registType("TestType");
			Assert::AreNotEqual(DATA_TYPE_UNKNOWN, newType, L"Regist new type failed");
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
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			void** funcRes = (void**)scriptTask.getTaskResult();

			auto pVariable = rootScope.findVariable("dummy");
			void* pDummy = getVaribleRef<void>(*pVariable);

			Assert::AreEqual( (size_t) pDummy, (size_t)*funcRes , L"program can run but return wrong value");
		}

		TEST_METHOD(DefaultAssignNewType)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			auto newType = scriptCompiler.registType("TestType");
			Assert::AreNotEqual(DATA_TYPE_UNKNOWN, newType, L"Regist new type failed");
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
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("setVal", "TestType");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			int param = 1;
			ScriptParamBuffer paramBuffer(param);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);

			auto pVariable = rootScope.findVariable("dummy");
			int* pDummy = getVaribleRef<int>(*pVariable);

			Assert::AreEqual(param, *pDummy, L"program can run but return wrong value");
		}

		TEST_METHOD(DefaultAssignForAutoType)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			auto newType = scriptCompiler.registType("TestType");
			Assert::AreNotEqual(DATA_TYPE_UNKNOWN, newType, L"Regist new type failed");
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
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("setVal", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(1, *iRes, L"program can run but return wrong value");
		}

		TEST_METHOD(DefaultAssignForDeclareTypeInExpression)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);

			auto newType = scriptCompiler.registType("TestType");
			Assert::AreNotEqual(DATA_TYPE_UNKNOWN, newType, L"Regist new type failed");
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
			Assert::IsTrue(res != nullptr, L"compile program failed");

			bool blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			int functionId = scriptCompiler.findFunction("setVal", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(1, *iRes, L"program can run but return wrong value");
		}
	};
}