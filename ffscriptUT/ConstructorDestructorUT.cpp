/******************************************************************
* File:        ConstructorDestructorUT.cpp
* Description: Test cases for checking behavior of constructor and
*              destructors.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

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
#include "MemoryBlock.h"

namespace ffscriptUT
{

#pragma pack(push)
#pragma pack(1)
	struct DummyStruct1 {
		int iVal;
		double dVal;
	};

	struct DummyStruct2 {
		DummyStruct1 structVal;
		int iVal;
	};
#pragma pack(pop)

	class CustomIntegerInitor {
		int _customData;
	public:
		CustomIntegerInitor(int initValue) : _customData(initValue) {}

		void initFunction(int* pValue) {
			*pValue = _customData;
		}
	};

	class CustomIntegerUninitor {
		int _customData;
	public:
		void uninitFunction(int* pValue) {
			_customData = *pValue;
		}
		int getData() {
			return _customData;
		}
		void setData(int data) {
			_customData = data;
		}
	};

	class OperatorExecuteCounter {
		int _count;
	public:
		OperatorExecuteCounter() : _count(0) {}
		void operatorFunction(void* pValue) {
			_count++;
		}
		void doubleOperatorFunction(double& dValue) {
			_count++;
			dValue = 1.0;
		}

		void intOperatorFunction1(int* pValue) {
			*pValue = 0;
			_count++;
		}
		void intOperatorFunction2(int* pValue) {
			*pValue = 2;
			_count++;
		}
		void intOperatorFunction3(int* pValue) {
			*pValue = 2;
			_count+=2;
		}

		void intCopyOperator1(int* pObject, int& value) {
			*pObject = value;
			_count ++;
		}

		void structCopyOperator1(DummyStruct2* pObject, DummyStruct2* value) {
			*pObject = *value;
			_count++;
		}

		void structCopyOperator2(DummyStruct2* pObject, DummyStruct2* value) {
			_count++;
		}

		void structCopyOperator3(DummyStruct2* pObject, int value) {
			pObject->iVal = value;
			_count++;
		}

		int getCount() const {
			return _count;
		}

		void resetCount() {
			_count = 0;
		}
	};

	TEST_CLASS(ConstructorDestructorUT)
	{
		ScriptCompiler scriptCompiler;
		FunctionRegisterHelper funcLibHelper;
		const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();

	public:
		ConstructorDestructorUT() : funcLibHelper(&scriptCompiler){
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			scriptCompiler.getTypeManager()->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);
		}

		TEST_METHOD(ConstructorUT1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const int checkVal = 1;
			CustomIntegerInitor initor(checkVal);

			DFunction2* initFunction = new MFunction2<void, CustomIntegerInitor, int*>(&initor, &CustomIntegerInitor::initFunction);
			int functionId = scriptCompiler.registFunction("DefaultInteger", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");
			
			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");

			const wchar_t* scriptCode =
				L"int test() {"
				L"	int ret;"
				L"	return ret;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(checkVal, *iRes, L"Construtor is run but parameter value is not correct");
		}

		TEST_METHOD(DestructorUT1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			CustomIntegerInitor initor(0);

			DFunction2* initFunction = new MFunction2<void, CustomIntegerInitor, int*>(&initor, &CustomIntegerInitor::initFunction);
			int functionId = scriptCompiler.registFunction("DefaultInteger", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");

			const int checkVal = 0;
			CustomIntegerUninitor uninitor;
			uninitor.setData(checkVal);

			DFunction2* uninitFunction = new MFunction2<void, CustomIntegerUninitor, int*>(&uninitor, &CustomIntegerUninitor::uninitFunction);
			functionId = scriptCompiler.registFunction("UninitInteger", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", uninitFunction, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			/*in bellow function, destuctor will not run for 'ret' because*/
			/*function will copy it directly to return address of the function*/
			const wchar_t* scriptCode =
				L"int test() {"
				L"	int ret = 1;"
				L"	return ret;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));

			int interferAssigment = scriptCompiler.findFunction("=", "int&,int");
			// if operator '=' of interger is not defined...
			if (interferAssigment < 0) {
				// ...then cannot construct object ret in expression int ret = 1;
				Assert::AreEqual(nullptr, res, L"compile program should failed");
				return;
			}
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(0, uninitor.getData(), L"Construtor is run but parameter value is not correct");
		}

		TEST_METHOD(ConstructorDestructorUT1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			OperatorExecuteCounter constuctorCounter;
			OperatorExecuteCounter destuctorCounter;

			DFunction2* constuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, void*>(&constuctorCounter, &OperatorExecuteCounter::operatorFunction);
			int functionId = scriptCompiler.registFunction("IntegerInitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", constuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");
			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");

			DFunction2* destuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, void*>(&destuctorCounter, &OperatorExecuteCounter::operatorFunction);
			functionId = scriptCompiler.registFunction("IntegerUninitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", destuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			/*in bellow function, destuctor will not run for 'ret' because*/
			/*function will copy it directly to return address of the function*/
			const wchar_t* scriptCode =
				L"int test() {"
				L"	int ret = 1;"
				L"	return ret;"
				L"}"
				;

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));			

			int interferAssigment = scriptCompiler.findFunction("=", "int&,int");
			// if operator '=' of interger is not defined...
			if (interferAssigment < 0) {
				// ...then cannot construct object ret in expression int ret = 1;
				Assert::AreEqual(nullptr, res, L"compile program should failed");
				return;
			}			
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(1, constuctorCounter.getCount(), L"Construtor is not executed");
			Assert::AreEqual(0, destuctorCounter.getCount(), L"Construtor and destrutor is not working properly");
		}

		TEST_METHOD(ConstructorDestructorUT2)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			OperatorExecuteCounter constuctorCounter;
			OperatorExecuteCounter destuctorCounter;

			DFunction2* constuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction1);
			int functionId = scriptCompiler.registFunction("IntegerInitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", constuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");
			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");

			DFunction2* destuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, void*>(&destuctorCounter, &OperatorExecuteCounter::operatorFunction);
			functionId = scriptCompiler.registFunction("IntegerUninitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", destuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");
			
			/*in bellow function, destuctor will not run for 'ret' because*/
			/*function will copy it directly to return address of the function*/
			const wchar_t* scriptCode =
				L"int test(int param) {"
				L"	int ret = param;"
				L"	return ret;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			
			int interferAssigment = scriptCompiler.findFunction("=", "int&,int");
			// if operator '=' of interger is not defined...
			if (interferAssigment < 0) {
				// ...then cannot construct object ret in expression int ret = 1;
				Assert::AreEqual(nullptr, res, L"compile program should failed");
				return;
			}
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "int");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			int param = 1;
			ScriptParamBuffer paramBuffer(param);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(1, param, L"Construtor is not working properly(1)");
			Assert::AreEqual(param, *iRes, L"Construtor is not working properly(2)");

			Assert::AreEqual(1, constuctorCounter.getCount(), L"Construtor is not properly(3)");
			Assert::AreEqual(0, destuctorCounter.getCount(), L"Construtor and destrutor is not working properly");
		}

		TEST_METHOD(ConstructorDestructorUT3)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			OperatorExecuteCounter constuctorCounter;
			OperatorExecuteCounter destuctorCounter;

			DFunction2* constuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction1);
			int functionId = scriptCompiler.registFunction("IntegerInitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", constuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");
			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");

			DFunction2* destuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*>(&destuctorCounter, &OperatorExecuteCounter::intOperatorFunction2);
			functionId = scriptCompiler.registFunction("IntegerUninitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", destuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			/*in bellow function, destuctor will not run for 'ret' because*/
			/*function will copy it directly to return address of the function*/
			const wchar_t* scriptCode =
				L"int test(int param) {"
				L"	int ret = param;"
				L"	return ret;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			
			int interferAssigment = scriptCompiler.findFunction("=", "int&,int");
			// if operator '=' of interger is not defined...
			if (interferAssigment < 0) {
				// ...then cannot construct object ret in expression int ret = 1;
				Assert::AreEqual(nullptr, res, L"compile program should failed");
				return;
			}
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "int");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			int param = 1;
			ScriptParamBuffer paramBuffer(param);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(1, param, L"Construtor does not working properly(1)");
			Assert::AreEqual(param, *iRes, L"Construtor does not working properly(2)");

			Assert::AreEqual(1, constuctorCounter.getCount(), L"Construtor does not work properly(3)");
			Assert::AreEqual(0, destuctorCounter.getCount(), L"Construtor and destrutor is not working properly");
		}

		TEST_METHOD(ConstructorDestructorUT4)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			OperatorExecuteCounter constuctorCounter;

			DFunction2* constuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction1);
			int functionId = scriptCompiler.registFunction("IntegerInitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", constuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");
			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");

			DFunction2* destuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction3);
			functionId = scriptCompiler.registFunction("IntegerUninitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", destuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			/*in bellow function, destuctor will not run for 'ret' because*/
			/*function will copy it directly to return address of the function*/
			const wchar_t* scriptCode =
				L"int test(int param) {"
				L"	int ret = param;"
				L"	return ret;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			
			int interferAssigment = scriptCompiler.findFunction("=", "int&,int");
			// if operator '=' of interger is not defined...
			if (interferAssigment < 0) {
				// ...then cannot construct object ret in expression int ret = 1;
				Assert::AreEqual(nullptr, res, L"compile program should failed");
				return;
			}
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "int");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			int param = 1;
			ScriptParamBuffer paramBuffer(param);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			scriptTask.getTaskResult();

			Assert::AreEqual(1, constuctorCounter.getCount(), L"Construtor and destructor does not work properly");
		}

		TEST_METHOD(ConstructorDestructorUT5)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			OperatorExecuteCounter constuctorCounter;

			DFunction2* constuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction1);
			int functionId = scriptCompiler.registFunction("IntegerInitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", constuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");
			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");

			DFunction2* destuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction3);
			functionId = scriptCompiler.registFunction("IntegerUninitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", destuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			/*in 'test' function, destuctor will not run for 'ret' because*/
			/*function will copy it directly to return address of the function*/
			const wchar_t* scriptCode =
				L"int test2() {"
				L"	return 1;"
				L"}"
				L"int test(int param) {"
				L"	int ret = test2();"
				L"	return ret;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			int iRes = rootScope.correctAndOptimize(&theProgram);
			Assert::AreEqual(0, iRes, L"correct and optimize program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "int");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			int param = 1;
			ScriptParamBuffer paramBuffer(param);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			scriptTask.getTaskResult();

			Assert::AreEqual(0, constuctorCounter.getCount(), L"Construtor and destructor does not work properly");
		}

		TEST_METHOD(ConstructorDestructorUT6)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			OperatorExecuteCounter constuctorCounter;
			OperatorExecuteCounter copyConstructorCounter;
			OperatorExecuteCounter destuctorCounter;

			DFunction2* constuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction1);
			int functionId = scriptCompiler.registFunction("IntegerInitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", constuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");
			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");

			DFunction2* destuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, void*>(&destuctorCounter, &OperatorExecuteCounter::operatorFunction);
			functionId = scriptCompiler.registFunction("IntegerUninitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", destuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			DFunction2* copyConstructorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*, int&>(&copyConstructorCounter, &OperatorExecuteCounter::intCopyOperator1);
			functionId = scriptCompiler.registFunction("IntegerCopier", "ref int, int&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", copyConstructorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registConstructor (basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register copy constructor failed");

			/*in bellow function, destuctor will not run for 'ret' because*/
			/*function will copy it directly to return address of the function*/
			const wchar_t* scriptCode =
				L"int test(int param) {"
				L"	int ret = param;"
				L"	return ret;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "int");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			int param = 1;
			ScriptParamBuffer paramBuffer(param);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(1, *iRes, L"Construtor and destructor does not work properly");
			Assert::AreEqual(0, constuctorCounter.getCount(), L"Construtor and destructor does not work properly");
			Assert::AreEqual(1, copyConstructorCounter.getCount(), L"Construtor and destructor does not work properly");
			Assert::AreEqual(constuctorCounter.getCount() + copyConstructorCounter.getCount() - 1, destuctorCounter.getCount(), L"Construtor and destructor does not work properly");
		}

		TEST_METHOD(ConstructorDestructorUT7)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			//register struct type
			ScriptType typeInt(basicType.TYPE_INT, "int");
			ScriptType typeDouble(basicType.TYPE_DOUBLE, "double");

			StructClass* structInfo1 = new StructClass(&scriptCompiler, "DummyStruct1");
			structInfo1->addMember(typeInt, "iVal");
			structInfo1->addMember(typeDouble, "dVal");
			int structType1 = scriptCompiler.registStruct(structInfo1);
			ScriptType typeStruct1(structType1, structInfo1->getName());

			StructClass* structInfo2 = new StructClass(&scriptCompiler, "DummyStruct2");
			structInfo2->addMember(typeStruct1, "structVal");
			structInfo2->addMember(typeInt, "iVal");
			int structType2 = scriptCompiler.registStruct(structInfo2);

			OperatorExecuteCounter constuctorCounter;
			OperatorExecuteCounter copyConstructorCounter;
			OperatorExecuteCounter copyStructConstructorCounter;
			OperatorExecuteCounter destuctorCounter;
			//register constructor/destructor
			DFunction2* constuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction1);
			int functionId = scriptCompiler.registFunction("IntegerInitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", constuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");
			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");

			DFunction2* destuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, void*>(&destuctorCounter, &OperatorExecuteCounter::operatorFunction);
			functionId = scriptCompiler.registFunction("IntegerUninitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", destuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			DFunction2* copyConstructorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*, int&>(&copyConstructorCounter, &OperatorExecuteCounter::intCopyOperator1);
			functionId = scriptCompiler.registFunction("IntegerCopier", "ref int, int&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", copyConstructorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register copy constructor failed");

			DFunction2* structCopyConstructorCounterFunc = new MFunction2<void, OperatorExecuteCounter, DummyStruct2*, DummyStruct2*>(&copyConstructorCounter, &OperatorExecuteCounter::structCopyOperator1);
			functionId = scriptCompiler.registFunction("StructCopier", "ref DummyStruct2, DummyStruct2&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", structCopyConstructorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registConstructor(structType2, functionId);
			Assert::IsTrue(blRes, L"Register copy constructor failed");

			DFunction2* structDestuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, void*>(&destuctorCounter, &OperatorExecuteCounter::operatorFunction);
			functionId = scriptCompiler.registFunction("StructUninitor", "ref DummyStruct2", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", structDestuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(structType2, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			//compile code and run code
			const wchar_t* scriptCode =
				L"int test(DummyStruct2 param) {"
				L"	DummyStruct2 ret = param;"
				L"	return ret.structVal.iVal;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "DummyStruct2");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			DummyStruct2 param;
			param.structVal.iVal = 1;
			ScriptParamBuffer paramBuffer(param);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(param.structVal.iVal, *iRes, L"Construtor and destructor does not work properly");
			Assert::AreEqual(2, constuctorCounter.getCount(), L"Construtor and destructor does not work properly");
			// one for initialize ret, one for initialize return value from ret.structVal.iVal
			Assert::AreEqual(2, copyConstructorCounter.getCount(), L"Construtor and destructor does not work properly");
			// destructor for return value is not called because its scope is outside of test function
			Assert::AreEqual(constuctorCounter.getCount() + copyConstructorCounter.getCount() - 1, destuctorCounter.getCount(), L"Construtor and destructor does not work properly");
		}

		TEST_METHOD(ConstructorDestructorUT8)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			//register struct type
			ScriptType typeInt(basicType.TYPE_INT, "int");
			ScriptType typeDouble(basicType.TYPE_DOUBLE, "double");

			StructClass* structInfo1 = new StructClass(&scriptCompiler, "DummyStruct1");
			structInfo1->addMember(typeInt, "iVal");
			structInfo1->addMember(typeDouble, "dVal");
			int structType1 = scriptCompiler.registStruct(structInfo1);
			ScriptType typeStruct1(structType1, structInfo1->getName());

			StructClass* structInfo2 = new StructClass(&scriptCompiler, "DummyStruct2");
			structInfo2->addMember(typeStruct1, "structVal");
			structInfo2->addMember(typeInt, "iVal");
			int structType2 = scriptCompiler.registStruct(structInfo2);

			OperatorExecuteCounter constuctorCounter;
			OperatorExecuteCounter copyConstructorCounter;
			OperatorExecuteCounter copyStructConstructorCounter;
			OperatorExecuteCounter destuctorCounter;
			//register constructor/destructor
			DFunction2* constuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction1);
			int functionId = scriptCompiler.registFunction("IntegerInitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", constuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");
			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");

			DFunction2* destuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, void*>(&destuctorCounter, &OperatorExecuteCounter::operatorFunction);
			functionId = scriptCompiler.registFunction("IntegerUninitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", destuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			DFunction2* copyConstructorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*, int&>(&copyConstructorCounter, &OperatorExecuteCounter::intCopyOperator1);
			functionId = scriptCompiler.registFunction("IntegerCopier", "ref int, int&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", copyConstructorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register copy constructor failed");

			DFunction2* structCopyConstructorCounterFunc = new MFunction2<void, OperatorExecuteCounter, DummyStruct2*, DummyStruct2*>(&copyConstructorCounter, &OperatorExecuteCounter::structCopyOperator2);
			functionId = scriptCompiler.registFunction("StructCopier", "ref DummyStruct2, DummyStruct2&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", structCopyConstructorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registConstructor(structType2, functionId);
			Assert::IsTrue(blRes, L"Register copy constructor failed");

			DFunction2* structDestuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, void*>(&destuctorCounter, &OperatorExecuteCounter::operatorFunction);
			functionId = scriptCompiler.registFunction("StructUninitor", "ref DummyStruct2", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", structDestuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(structType2, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			//compile code and run code
			const wchar_t* scriptCode =
				L"int test(DummyStruct2 param) {"
				L"	DummyStruct2 ret = param;"
				L"	return ret.structVal.iVal;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "DummyStruct2");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			DummyStruct2 param;
			param.structVal.iVal = 1;
			ScriptParamBuffer paramBuffer(param);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* iRes = (int*)scriptTask.getTaskResult();

			//function structCopyOperator2 do nothing
			//so integer value is still keep value when it is constructed
			Assert::AreEqual(0, *iRes, L"Construtor and destructor does not work properly");
			Assert::AreEqual(2, constuctorCounter.getCount(), L"Construtor and destructor does not work properly");
			// one for initialize ret, one for initialize return value from ret.structVal.iVal
			Assert::AreEqual(2, copyConstructorCounter.getCount(), L"Construtor and destructor does not work properly");
			// destructor for return value is not called because its scope is outside of test function
			Assert::AreEqual(constuctorCounter.getCount() + copyConstructorCounter.getCount() - 1, destuctorCounter.getCount(), L"Construtor and destructor does not work properly");
		}

		TEST_METHOD(ConstructorDestructorUT9)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			//register struct type
			ScriptType typeInt(basicType.TYPE_INT, "int");
			ScriptType typeDouble(basicType.TYPE_DOUBLE, "double");

			StructClass* structInfo1 = new StructClass(&scriptCompiler, "DummyStruct1");
			structInfo1->addMember(typeInt, "iVal");
			structInfo1->addMember(typeDouble, "dVal");
			int structType1 = scriptCompiler.registStruct(structInfo1);
			ScriptType typeStruct1(structType1, structInfo1->getName());

			StructClass* structInfo2 = new StructClass(&scriptCompiler, "DummyStruct2");
			structInfo2->addMember(typeStruct1, "structVal");
			structInfo2->addMember(typeInt, "iVal");
			int structType2 = scriptCompiler.registStruct(structInfo2);

			OperatorExecuteCounter constuctorCounter;
			OperatorExecuteCounter copyConstructorCounter;
			OperatorExecuteCounter copyStructConstructorCounter;
			OperatorExecuteCounter destuctorCounter;
			//register constructor/destructor
			DFunction2* constuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction1);
			int functionId = scriptCompiler.registFunction("IntegerInitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", constuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");
			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");

			DFunction2* destuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, void*>(&destuctorCounter, &OperatorExecuteCounter::operatorFunction);
			functionId = scriptCompiler.registFunction("IntegerUninitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", destuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			DFunction2* copyConstructorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*, int&>(&copyConstructorCounter, &OperatorExecuteCounter::intCopyOperator1);
			functionId = scriptCompiler.registFunction("IntegerCopier", "ref int, int&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", copyConstructorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register copy constructor failed");

			DFunction2* structCopyConstructorCounterFunc = new MFunction2<void, OperatorExecuteCounter, DummyStruct2*, int>(&copyConstructorCounter, &OperatorExecuteCounter::structCopyOperator3);
			functionId = scriptCompiler.registFunction("StructCopier", "ref DummyStruct2, int", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", structCopyConstructorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registConstructor(structType2, functionId);
			Assert::IsTrue(blRes, L"Register copy constructor failed");

			DFunction2* structDestuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, void*>(&destuctorCounter, &OperatorExecuteCounter::operatorFunction);
			functionId = scriptCompiler.registFunction("StructUninitor", "ref DummyStruct2", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", structDestuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(structType2, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			//compile code and run code
			const wchar_t* scriptCode =
				L"int test(int param) {"
				L"	DummyStruct2 ret = param;"
				L"	return ret.iVal;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "int");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			int param;
			ScriptParamBuffer paramBuffer(param);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* iRes = (int*)scriptTask.getTaskResult();

			//function structCopyOperator2 do nothing
			//so integer value is still keep value when it is constructed
			Assert::AreEqual(param, *iRes, L"Construtor and destructor does not work properly");
			Assert::AreEqual(2, constuctorCounter.getCount(), L"Construtor and destructor does not work properly");
			// one for initialize ret, one for initialize return value from ret.iVal
			Assert::AreEqual(2, copyConstructorCounter.getCount(), L"Construtor and destructor does not work properly");
			// destructor for return value is not called because its scope is outside of test function
			Assert::AreEqual(constuctorCounter.getCount() + copyConstructorCounter.getCount() - 1, destuctorCounter.getCount(), L"Construtor and destructor does not work properly");
		}

		TEST_METHOD(ConstructorDestructorWithIf)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			OperatorExecuteCounter constuctorCounter;
			OperatorExecuteCounter destuctorCounter;

			DFunction2* constuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction1);
			int functionId = scriptCompiler.registFunction("IntegerInitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", constuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");
			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");

			DFunction2* destuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, void*>(&destuctorCounter, &OperatorExecuteCounter::operatorFunction);
			functionId = scriptCompiler.registFunction("IntegerUninitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", destuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			/*in bellow function, destuctor will not run for 'ret' because*/
			/*function will copy it directly to return address of the function*/
			const wchar_t* scriptCode =
				L"int test(int param) {"
				L"	if(param % 2 == 0) {"
				L"		int ret;"
				L"		return ret;"
				L"	}"
				L"	int ret = 1;"				
				L"	return ret;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			
			int interferAssigment = scriptCompiler.findFunction("=", "int&,int");
			// if operator '=' of interger is not defined...
			if (interferAssigment < 0) {
				// ...then cannot construct object ret in expression int ret = 1;
				Assert::AreEqual(nullptr, res, L"compile program should failed");
				return;
			}
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "int");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);

			int param = 2;
			ScriptParamBuffer paramBuffer(param);

			scriptTask.runFunction(functionId, &paramBuffer);
			int ret1 = *(int*)(scriptTask.getTaskResult());

			Assert::AreEqual(1, constuctorCounter.getCount(), L"Construtor does not work properly");
			Assert::AreEqual(0, destuctorCounter.getCount(), L"Destructor does not work properly");
			Assert::AreEqual(0, ret1, L"Construtor does not work properly");

			param = 1;
			ScriptParamBuffer paramBuffer2(param);

			scriptTask.runFunction(functionId, &paramBuffer2);
			int ret2 = *(int*)(scriptTask.getTaskResult());

			Assert::AreEqual(2, constuctorCounter.getCount(), L"Construtor does not work properly");
			Assert::AreEqual(0, destuctorCounter.getCount(), L"Destructor does not work properly");
			Assert::AreEqual(1, ret2, L"Construtor does not work properly");
		}

		TEST_METHOD(ConstructorDestructorWithIfElse)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			OperatorExecuteCounter constuctorCounter;
			OperatorExecuteCounter destuctorCounter;

			DFunction2* constuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction1);
			int functionId = scriptCompiler.registFunction("IntegerInitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", constuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");
			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");

			DFunction2* destuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, void*>(&destuctorCounter, &OperatorExecuteCounter::operatorFunction);
			functionId = scriptCompiler.registFunction("IntegerUninitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", destuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			/*in bellow function, destuctor will not run for 'ret' because*/
			/*function will copy it directly to return address of the function*/
			const wchar_t* scriptCode =
				L"int test(int param) {"
				L"	if(param % 2 == 0) {"
				L"		int ret;"
				L"		return ret;"
				L"	}"
				L"	else {"
				L"		int a;"
				L"		int b;"
				L"	}"
				L"	int ret = 1;"
				L"	return ret;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			
			int interferAssigment = scriptCompiler.findFunction("=", "int&,int");
			// if operator '=' of interger is not defined...
			if (interferAssigment < 0) {
				// ...then cannot construct object ret in expression int ret = 1;
				Assert::AreEqual(nullptr, res, L"compile program should failed");
				return;
			}
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "int");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);

			int param = 2;
			ScriptParamBuffer paramBuffer(param);

			scriptTask.runFunction(functionId, &paramBuffer);
			int ret1 = *(int*)(scriptTask.getTaskResult());

			Assert::AreEqual(1, constuctorCounter.getCount(), L"Construtor does not work properly");
			Assert::AreEqual(0, destuctorCounter.getCount(), L"Destructor does not work properly");
			Assert::AreEqual(0, ret1, L"Construtor does not work properly");

			param = 1;
			ScriptParamBuffer paramBuffer2(param);
			scriptTask.runFunction(functionId, &paramBuffer2);
			int ret2 = *(int*)(scriptTask.getTaskResult());

			Assert::AreEqual(1 + 3, constuctorCounter.getCount(), L"Construtor does not work properly");
			Assert::AreEqual(2, destuctorCounter.getCount(), L"Destructor does not work properly");
			Assert::AreEqual(1, ret2, L"Construtor does not work properly");
		}

		TEST_METHOD(ConstructorDestructorWithLoop)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			OperatorExecuteCounter constuctorCounter;
			OperatorExecuteCounter destuctorCounter;

			DFunction2* constuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction1);
			int functionId = scriptCompiler.registFunction("IntegerInitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", constuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");
			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");

			DFunction2* destuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, void*>(&destuctorCounter, &OperatorExecuteCounter::operatorFunction);
			functionId = scriptCompiler.registFunction("IntegerUninitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", destuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			const wchar_t* scriptCode =
				L"void test(int param) {"
				L"	while(param-- > 0) {"
				L"		int ret;"
				L"	}"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "int");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);

			int param = 5;
			ScriptParamBuffer paramBuffer(param);
			scriptTask.runFunction(functionId, &paramBuffer);	

			Assert::AreEqual(param, constuctorCounter.getCount(), L"Construtor does not work properly in loop");
			Assert::AreEqual(param, destuctorCounter.getCount(), L"Destructor does not work properly in loop");
		}

		TEST_METHOD(ConstructorDestructorWithLoopAndBreak)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			OperatorExecuteCounter constuctorCounter;
			OperatorExecuteCounter destuctorCounter;

			DFunction2* constuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction1);
			int functionId = scriptCompiler.registFunction("IntegerInitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", constuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");
			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");

			DFunction2* destuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, void*>(&destuctorCounter, &OperatorExecuteCounter::operatorFunction);
			functionId = scriptCompiler.registFunction("IntegerUninitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", destuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			const wchar_t* scriptCode =
				L"void test(int param) {"
				L"	while(param-- > 0) {"
				L"		int ret;"
				L"		if(param == 2) {"
				L"			int ret;"
				L"			break;"
				L"		}"
				L"	}"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "int");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);

			int param = 5;
			ScriptParamBuffer paramBuffer(param);

			scriptTask.runFunction(functionId, &paramBuffer);

			Assert::AreEqual( param < 2 ? param : param - 1, constuctorCounter.getCount(), L"Construtor does not work properly in loop");
			Assert::AreEqual(param < 2 ? param : param - 1, destuctorCounter.getCount(), L"Destructor does not work properly in loop");
		}

		TEST_METHOD(ConstructorDestructorWithLoopAndContinue)
		{
			byte globalData[1024];
			//register constants, so we can use 'true' and 'false' for bool
			basicType.registerConstants(&scriptCompiler);
			
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			OperatorExecuteCounter constuctorCounter;
			OperatorExecuteCounter destuctorCounter;

			DFunction2* constuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction1);
			int functionId = scriptCompiler.registFunction("IntegerInitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", constuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");
			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");

			DFunction2* destuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, void*>(&destuctorCounter, &OperatorExecuteCounter::operatorFunction);
			functionId = scriptCompiler.registFunction("IntegerUninitor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", destuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			const wchar_t* scriptCode =
				L"void test(int param) {"
				L"	bool tried = false;"
				L"	while(param-- > 0) {"
				L"		int ret;"
				L"		if(param == 0 && tried == false) {"
				L"			int ret;"
				L"			tried = true;"
				L"			param = 1;"
				L"			continue;"
				L"		}"
				L"	}"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "int");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);

			int param = 5;
			ScriptParamBuffer paramBuffer(param);

			scriptTask.runFunction(functionId, &paramBuffer);

			Assert::AreEqual(param + 2, constuctorCounter.getCount(), L"Construtor does not work properly in loop");
			Assert::AreEqual(param + 2, destuctorCounter.getCount(), L"Destructor does not work properly in loop");
		}

		/* case: struct type has constructor and member has*/
		TEST_METHOD(ConstructorForStructUT1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			ScriptType typeInt( basicType.TYPE_INT, "int");
			ScriptType typeDouble(basicType.TYPE_DOUBLE, "double");

			StructClass* structInfo = new StructClass(&scriptCompiler, "DummyStruct");
			structInfo->addMember(typeInt, "a");
			structInfo->addMember(typeDouble, "b");
			int structType = scriptCompiler.registStruct(structInfo);

			OperatorExecuteCounter structConstuctorCounter;
			OperatorExecuteCounter doubleConstuctorCounter;

			DFunction2* structInitFunction = new MFunction2<void, OperatorExecuteCounter, void*>(&structConstuctorCounter, &OperatorExecuteCounter::operatorFunction);
			int functionId = scriptCompiler.registFunction("constructorCounter", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", structInitFunction, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler.registConstructor(structType, functionId);
			Assert::IsTrue(blRes, L"Register constructor for struct failed");

			DFunction2* doubleInitFunction = new MFunction2<void, OperatorExecuteCounter, double&>(&doubleConstuctorCounter, &OperatorExecuteCounter::doubleOperatorFunction);
			functionId = scriptCompiler.registFunction("DdoubleConstructorCounter", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", doubleInitFunction, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");
			
			blRes = scriptCompiler.registConstructor(basicType.TYPE_DOUBLE, functionId);
			Assert::IsTrue(blRes, L"Register constructor for double failed");

			const wchar_t* scriptCode =
				L"double test() {"
				L"	DummyStruct dummyObj;"
				L"	return dummyObj.b;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			double* dRes = (double*)scriptTask.getTaskResult();

			//Assert::AreEqual(1, *iRes, L"Construtor is run but parameter value is not correct");
			Assert::AreEqual(1, structConstuctorCounter.getCount(), L"Construtor is run but parameter value is not correct");
			Assert::AreEqual(1, doubleConstuctorCounter.getCount(), L"Construtor is run but parameter value is not correct");
			Assert::AreEqual(1.0, *dRes, L"Construtor is run but parameter value is not correct");
		}

		/* case: struct type don't has destructor but member has*/
		TEST_METHOD(ConstructorForStructUT2)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			ScriptType typeInt(basicType.TYPE_INT, "int");
			ScriptType typeDouble(basicType.TYPE_DOUBLE, "double");

			StructClass* structInfo = new StructClass(&scriptCompiler, "DummyStruct");
			structInfo->addMember(typeInt, "a");
			structInfo->addMember(typeDouble, "b");
			int structType = scriptCompiler.registStruct(structInfo);

			OperatorExecuteCounter doubleConstuctorCounter;

			DFunction2* doubleInitFunction = new MFunction2<void, OperatorExecuteCounter, double&>(&doubleConstuctorCounter, &OperatorExecuteCounter::doubleOperatorFunction);
			int functionId = scriptCompiler.registFunction("DdoubleConstructorCounter", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", doubleInitFunction, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_DOUBLE, functionId);
			Assert::IsTrue(blRes, L"Register constructor for double failed");

			const wchar_t* scriptCode =
				L"double test() {"
				L"	DummyStruct dummyObj;"
				L"	return dummyObj.b;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			double* dRes = (double*)scriptTask.getTaskResult();

			//Assert::AreEqual(1, *iRes, L"Construtor is run but parameter value is not correct");
			Assert::AreEqual(1, doubleConstuctorCounter.getCount(), L"Construtor is run but parameter value is not correct");
			Assert::AreEqual(1.0, *dRes, L"Construtor is run but parameter value is not correct");
		}

		/* case: struct type doesn't have constructor and member either, but member of member has*/
		TEST_METHOD(ConstructorForStructUT3)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			ScriptType typeInt(basicType.TYPE_INT, "int");
			ScriptType typeDouble(basicType.TYPE_DOUBLE, "double");

			StructClass* structInfo1 = new StructClass(&scriptCompiler, "DummyStruct1");
			structInfo1->addMember(typeInt, "iVal");
			structInfo1->addMember(typeDouble, "dVal");
			int structType1 = scriptCompiler.registStruct(structInfo1);
			ScriptType typeStruct1(structType1, structInfo1->getName());

			StructClass* structInfo2 = new StructClass(&scriptCompiler, "DummyStruct2");
			structInfo2->addMember(typeStruct1, "structVal");
			structInfo2->addMember(typeDouble, "dVal");
			int structType2 = scriptCompiler.registStruct(structInfo2);
			
			OperatorExecuteCounter constuctorCounter;

			DFunction2* inInitFunction = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction2);
			int functionId = scriptCompiler.registFunction("what_ever_you_want", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", inInitFunction, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor for integer failed");

			const wchar_t* scriptCode =
				L"int test() {"
				L"	DummyStruct2 dummyObj;"
				L"	return dummyObj.structVal.iVal;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(1, constuctorCounter.getCount(), L"Construtor is run but parameter value is not correct");
			Assert::AreEqual(2, *iRes, L"function return a wrong value");
		}

		/* case: struct type has constructor and member either, but member of member has*/
		TEST_METHOD(ConstructorForStructUT4)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			ScriptType typeInt(basicType.TYPE_INT, "int");
			ScriptType typeDouble(basicType.TYPE_DOUBLE, "double");

			StructClass* structInfo1 = new StructClass(&scriptCompiler, "DummyStruct1");
			structInfo1->addMember(typeInt, "iVal");
			structInfo1->addMember(typeDouble, "dVal");
			int structType1 = scriptCompiler.registStruct(structInfo1);
			ScriptType typeStruct1(structType1, structInfo1->getName());

			StructClass* structInfo2 = new StructClass(&scriptCompiler, "DummyStruct2");
			structInfo2->addMember(typeStruct1, "structVal");
			structInfo2->addMember(typeDouble, "dVal");
			int structType2 = scriptCompiler.registStruct(structInfo2);

			OperatorExecuteCounter constuctorCounter;

			DFunction2* structInitFunction = new MFunction2<void, OperatorExecuteCounter, void*>(&constuctorCounter, &OperatorExecuteCounter::operatorFunction);
			int functionId = scriptCompiler.registFunction("structInitor", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", structInitFunction, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler.registConstructor(structType2, functionId);
			Assert::IsTrue(blRes, L"Register constructor for integer failed");

			DFunction2* inInitFunction = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction2);
			functionId = scriptCompiler.registFunction("what_ever_you_want", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", inInitFunction, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");

			blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor for integer failed");

			const wchar_t* scriptCode =
				L"int test() {"
				L"	DummyStruct2 dummyObj;"
				L"	return dummyObj.structVal.iVal;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(2, constuctorCounter.getCount(), L"Construtor is run but parameter value is not correct");
			Assert::AreEqual(2, *iRes, L"function return a wrong value");
		}

		/* case: struct type has constructor and member either, but member of member has*/
		TEST_METHOD(ConstructorForStructUT5)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			ScriptType typeInt(basicType.TYPE_INT, "int");
			ScriptType typeDouble(basicType.TYPE_DOUBLE, "double");

			StructClass* structInfo1 = new StructClass(&scriptCompiler, "DummyStruct1");
			structInfo1->addMember(typeInt, "iVal");
			structInfo1->addMember(typeDouble, "dVal");
			int structType1 = scriptCompiler.registStruct(structInfo1);
			ScriptType typeStruct1(structType1, structInfo1->getName());

			StructClass* structInfo2 = new StructClass(&scriptCompiler, "DummyStruct2");
			structInfo2->addMember(typeStruct1, "structVal");
			structInfo2->addMember(typeDouble, "dVal");
			int structType2 = scriptCompiler.registStruct(structInfo2);

			OperatorExecuteCounter constuctorCounter;

			DFunction2* structInitFunction = new MFunction2<void, OperatorExecuteCounter, void*>(&constuctorCounter, &OperatorExecuteCounter::operatorFunction);
			int functionId = scriptCompiler.registFunction("structInitor", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", structInitFunction, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler.registConstructor(structType2, functionId);
			Assert::IsTrue(blRes, L"Register constructor for integer failed");

			blRes = scriptCompiler.registConstructor(structType1, functionId);
			Assert::IsTrue(blRes, L"Register constructor for integer failed");

			DFunction2* inInitFunction = new MFunction2<void, OperatorExecuteCounter, int*>(&constuctorCounter, &OperatorExecuteCounter::intOperatorFunction2);
			functionId = scriptCompiler.registFunction("what_ever_you_want", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", inInitFunction, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");

			blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor for integer failed");

			const wchar_t* scriptCode =
				L"int test() {"
				L"	DummyStruct2 dummyObj;"
				L"	return dummyObj.structVal.iVal;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*)scriptTask.getTaskResult();

			Assert::AreEqual(3, constuctorCounter.getCount(), L"Construtor is run but parameter value is not correct");
			Assert::AreEqual(2, *iRes, L"function return a wrong value");
		}

		/* case: struct type has destructor and member has*/
		TEST_METHOD(DestructorForStructUT1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			ScriptType typeInt(basicType.TYPE_INT, "int");
			ScriptType typeDouble(basicType.TYPE_DOUBLE, "double");

			StructClass* structInfo = new StructClass(&scriptCompiler, "DummyStruct");
			structInfo->addMember(typeInt, "a");
			structInfo->addMember(typeDouble, "b");
			int structType = scriptCompiler.registStruct(structInfo);

			OperatorExecuteCounter structConstuctorCounter;
			OperatorExecuteCounter doubleConstuctorCounter;

			DFunction2* structInitFunction = new MFunction2<void, OperatorExecuteCounter, void*>(&structConstuctorCounter, &OperatorExecuteCounter::operatorFunction);
			int functionId = scriptCompiler.registFunction("constructorCounter", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", structInitFunction, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler.registDestructor(structType, functionId);
			Assert::IsTrue(blRes, L"Register destructor for struct failed");

			DFunction2* doubleInitFunction = new MFunction2<void, OperatorExecuteCounter, double&>(&doubleConstuctorCounter, &OperatorExecuteCounter::doubleOperatorFunction);
			functionId = scriptCompiler.registFunction("DoubleDestructorCounter", "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", doubleInitFunction, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");

			blRes = scriptCompiler.registDestructor(basicType.TYPE_DOUBLE, functionId);
			Assert::IsTrue(blRes, L"Register constructor for double failed");
			
			const wchar_t* scriptCode =
				L"void test() {"
				L"	DummyStruct dummyObj;"				
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);
			scriptTask.getTaskResult();

			//Assert::AreEqual(1, *iRes, L"Construtor is run but parameter value is not correct");
			Assert::AreEqual(1, structConstuctorCounter.getCount(), L"Destrutor is run but parameter value is not correct");
			Assert::AreEqual(1, doubleConstuctorCounter.getCount(), L"Denstrutor is run but parameter value is not correct");			
		}

		TEST_METHOD(ConstructorDestructorUT10)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext, &scriptCompiler);
			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			OperatorExecuteCounter constuctorCounter;
			OperatorExecuteCounter destuctorCounter;

			DFunction2* constuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, void*>(&constuctorCounter, &OperatorExecuteCounter::operatorFunction);
			int functionId = scriptCompiler.registFunction("ctor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", constuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");
			bool blRes = scriptCompiler.registConstructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");

			DFunction2* destuctorCounterFunc = new MFunction2<void, OperatorExecuteCounter, void*>(&destuctorCounter, &OperatorExecuteCounter::operatorFunction);
			functionId = scriptCompiler.registFunction("dtor", "ref int", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", destuctorCounterFunc, &scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for destructor failed");

			blRes = scriptCompiler.registDestructor(basicType.TYPE_INT, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");

			const wchar_t* scriptCode =
				L"int test() {"
				L"	int ret = 1;"
				L"	return ret;"
				L"}"
				;
			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));

			int interferAssigment = scriptCompiler.findFunction("=", "int&,int");
			// if operator '=' of interger is not defined...
			if (interferAssigment < 0) {
				// ...then cannot construct object ret in expression int ret = 1;
				Assert::AreEqual(nullptr, res, L"compile program should failed");
				return;
			}
			Assert::AreNotEqual(nullptr, res, L"compile program failed");

			blRes = rootScope.extractCode(&theProgram);
			Assert::IsTrue(blRes, L"extract code failed");

			functionId = scriptCompiler.findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, nullptr);

			//Assert::AreEqual(1, constuctorCounter.getCount(), L"Construtor is not executed");
			//Assert::AreEqual(0, destuctorCounter.getCount(), L"Construtor and destrutor is not working properly");
		}
	};
}