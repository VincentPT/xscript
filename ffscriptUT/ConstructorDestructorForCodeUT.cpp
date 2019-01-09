/******************************************************************
* File:        ConstructorDestructorForCodeUT.cpp
* Description: Test cases for checking behavior of constructor and
*              destructors.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "fftest.hpp"

#include "ExpresionParser.h"
#include <functional>
#include "TemplateForTest.hpp"
#include "Variable.h"
#include "FunctionRegisterHelper.h"
#include "BasicFunction.h"
#include "BasicType.h"
#include "FunctionFactory.h"
#include "function/MemberFunction.hpp"
#include "Context.h"
#include <thread>
#include <future>
#include <Program.h>
#include <ScriptTask.h>

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
#include "function/MemberFunction2.hpp"
#include "expressionunit.h"
#include "DynamicFunctionFactory.h"
#include "MemoryBlock.h"
#include "CompilerSuite.h"

namespace ffscriptUT
{
#pragma pack(push)
#pragma pack(1)
	struct DummyStruct {
		int* iVal;
		double dVal;
	};
#pragma pack(pop)

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
			_count += 2;
		}

		void intCopyOperator1(int* pObject, int value) {
			*pObject = value;
			_count++;
		}

		int getCount() const {
			return _count;
		}

		void resetCount() {
			_count = 0;
		}
	};

	FF_TEST_CLASS(ConstructorDestructorForCode)
	{
	protected:
		CompilerSuite compiler;
		const BasicTypes* basicType;
		ScriptCompiler* scriptCompiler;

	public:
		ConstructorDestructorForCode()
		{
			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			scriptCompiler = compiler.getCompiler().get();
			basicType = &compiler.getTypeManager()->getBasicTypes();
		}

		void registerConstructor(OperatorExecuteCounter* obj, int type) {
			ScriptType stype(type, scriptCompiler->getType(type));

			DFunction2* initFunction = new MFunction2<void, OperatorExecuteCounter, void*>(obj, &OperatorExecuteCounter::operatorFunction);
			int functionId = scriptCompiler->registFunction("constructorCount", stype.makeRef().sType(), new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			FF_EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			FF_EXPECT_TRUE(blRes, L"Register constructor failed");
		}

		void registerDestructor(OperatorExecuteCounter* obj, int type) {
			ScriptType stype(type, scriptCompiler->getType(type));

			DFunction2* initFunction = new MFunction2<void, OperatorExecuteCounter, void*>(obj, &OperatorExecuteCounter::operatorFunction);
			int functionId = scriptCompiler->registFunction("destructorCount", stype.makeRef().sType(), new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			FF_EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registDestructor(type, functionId);
			FF_EXPECT_TRUE(blRes, L"Register destructor failed");
		}


		template <typename T>
		void registerConstructor(T f, int type) {
			ScriptType stype(type, scriptCompiler->getType(type));
			typedef void (*F)(void*);
			DFunction2* initFunction = new CdeclFunction2<void, void*>((F)f);
			int functionId = scriptCompiler->registFunction("constructorCount", stype.makeRef().sType(), new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			FF_EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			FF_EXPECT_TRUE(blRes, L"Register constructor failed");
		}

		template <typename T>
		void registerDestructor(T f, int type) {
			ScriptType stype(type, scriptCompiler->getType(type));

			typedef void (*F)(void*);
			DFunction2* initFunction = new CdeclFunction2<void, void*>((F)f);
			int functionId = scriptCompiler->registFunction("destructorCount", stype.makeRef().sType(), new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			FF_EXPECT_TRUE(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registDestructor(type, functionId);
			FF_EXPECT_TRUE(blRes, L"Register destructor failed");
		}
	};
	namespace ConstructorDestructorForCodeUT {
		FF_TEST_METHOD(ConstructorDestructorForCode, DestructorUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			OperatorExecuteCounter constructorCounter, destructorCounter;
			registerConstructor(&constructorCounter, basicType->TYPE_INT );
			registerDestructor(&destructorCounter, basicType->TYPE_INT);

			const wchar_t scriptCode[] =
				L"int test() {"
				L"	int ret = 1;"
				L"	return ret;" /*destuctor will not run for 'ret' because*/
				L"}"             /*function will copy it directly to return address of the function*/
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);

			int interferAssigment = scriptCompiler->findFunction("=", "int&,int");
			// if operator '=' of interger is not defined...
			if (interferAssigment < 0) {
				// ...then cannot construct object ret in expression int ret = 1;
				FF_EXPECT_EQ(nullptr, program, L"compile program should failed");
				return;
			}
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("test", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* iRes = (int*) scriptTask.getTaskResult();

			FF_EXPECT_EQ(1, *iRes, L"Construtor is run but parameter value is not correct");
			FF_EXPECT_EQ(1, constructorCounter.getCount(), L"Construtor is run but parameter value is not correct");
			FF_EXPECT_EQ(1, destructorCounter.getCount(), L"Construtor is run but parameter value is not correct");
		}
		
		FF_TEST_METHOD(ConstructorDestructorForCode, DestructorForStructUT2)
		{
			ScriptType typeInt(basicType->TYPE_INT, "int");
			ScriptType typeDouble(basicType->TYPE_DOUBLE, "double");

			StructClass* structInfo = new StructClass(scriptCompiler, "DummyStruct");
			structInfo->addMember(typeInt, "a");
			structInfo->addMember(typeDouble, "b");
			int structType = scriptCompiler->registStruct(structInfo);

			OperatorExecuteCounter structConstructorCounter;
			OperatorExecuteCounter structDestructorCounter;
			OperatorExecuteCounter intConstructorCounter;
			OperatorExecuteCounter intDestructorCounter;

			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerConstructor(&structConstructorCounter, structType);
			registerDestructor(&structDestructorCounter, structType);

			const wchar_t scriptCode[] =
				L"DummyStruct createDummy() {" /*when this function run, default constructor and destructor for the struct will be run*/
				L"	DummyStruct dummyObj;"
				L"	return dummyObj;"
				L"}"
				L"void test() {"                          /*when this function run, constructor for dummyObj is not run*/
				L"	DummyStruct dummyObj = createDummy();"/*destructor for return object of 'createDummy' is not run*/
				L"}"                                      /*destructor for return object of '=' is not run because it returns a ref type*/
				;

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("test", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			FF_EXPECT_EQ(1, structConstructorCounter.getCount(), L"Constructor is run but parameter value is not correct");
			FF_EXPECT_EQ(1, structDestructorCounter.getCount(), L"Destrutor is run but parameter value is not correct");
			FF_EXPECT_EQ(1, intConstructorCounter.getCount(), L"Constructor is run but parameter value is not correct");
			FF_EXPECT_EQ(1, intDestructorCounter.getCount(), L"Destrutor is run but parameter value is not correct");
		}

		FF_TEST_METHOD(ConstructorDestructorForCode, DestructorForStructUT3)
		{
			ScriptType typeInt(basicType->TYPE_INT, "int");
			ScriptType typeDouble(basicType->TYPE_DOUBLE, "double");

			StructClass* structInfo = new StructClass(scriptCompiler, "DummyStruct");
			structInfo->addMember(typeInt, "a");
			structInfo->addMember(typeDouble, "b");
			int structType = scriptCompiler->registStruct(structInfo);

			OperatorExecuteCounter structConstructorCounter;
			OperatorExecuteCounter structDestructorCounter;
			OperatorExecuteCounter intConstructorCounter;
			OperatorExecuteCounter intDestructorCounter;

			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerConstructor(&structConstructorCounter, structType);
			registerDestructor(&structDestructorCounter, structType);

			const wchar_t scriptCode[] =
				L"DummyStruct createDummy() {" /*when this function run, default constructor will be run*/
				L"	DummyStruct dummyObj;"     /*but destructor is not run because dummyObj will be copied directly to return address of the function  */
				L"	return dummyObj;"
				L"}"
				L"DummyStruct test() {"    /*when this function run */
				L"	return createDummy();"/*destructor for return object of 'createDummy' is not run*/
				L"}"                      /*because it will be copied to return address directly*/
				;                         /*after used, user should release it*/

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("test", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			FF_EXPECT_EQ(1, structConstructorCounter.getCount(), L"Destrutor is run but parameter value is not correct");
			FF_EXPECT_EQ(0, structDestructorCounter.getCount(), L"Denstrutor is run but parameter value is not correct");
			FF_EXPECT_EQ(1, intConstructorCounter.getCount(), L"Destrutor is run but parameter value is not correct");
			FF_EXPECT_EQ(0, intDestructorCounter.getCount(), L"Denstrutor is run but parameter value is not correct");
		}

		static void DummyStructConstructor(DummyStruct* obj) {
			obj->iVal = new int;
		}

		static void DummyStructDestructor(DummyStruct* obj) {
			if (obj->iVal) delete obj->iVal;
		}

		FF_TEST_METHOD(ConstructorDestructorForCode, DestructorForStructUT4)
		{
			ScriptType typeInt(basicType->TYPE_INT, "int");
			ScriptType typeDouble(basicType->TYPE_DOUBLE, "double");

			StructClass* structInfo = new StructClass(scriptCompiler, "DummyStruct");
			structInfo->addMember(typeInt.makeRef(), "a");
			structInfo->addMember(typeDouble, "b");
			int structType = scriptCompiler->registStruct(structInfo);

			registerConstructor(DummyStructConstructor, structType);
			registerDestructor(DummyStructDestructor, structType);

			const wchar_t scriptCode[] =
				L"DummyStruct createDummy() {" /*when this function run, default constructor will be run*/
				L"	DummyStruct dummyObj;"     /*but destructor is not run because dummyObj will be copied directly to return address of the function  */
				L"	*dummyObj.a = 1;"
				L"	return dummyObj;"
				L"}"
				L"DummyStruct test() {"    /*when this function run */
				L"	return createDummy();"/*destructor for return object of 'createDummy' is not run*/
				L"}"                      /*because it will be copied to return address directly*/
				;                         /*after used, user should release it*/

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("test", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			DummyStruct* dummyStruct = (DummyStruct*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(1, *dummyStruct->iVal, L"Destrutor is run but parameter value is not correct");

			DummyStructDestructor(dummyStruct);
		}
	}
}