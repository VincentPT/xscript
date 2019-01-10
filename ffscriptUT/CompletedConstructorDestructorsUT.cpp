/******************************************************************
* File:        CompletedConstructorDestructorsUT.cpp
* Description: Test cases for checking behavior of contructors and
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
	class ExcutionCounter {
	protected:
		int _count;
		int _size;
	public:
		ExcutionCounter() : _count(0), _size(0) {}

		void operatorFunction(void* pValue) {
			if (_size) {
				memset(pValue, 0, _size);
			}
			_count++;
		}

		int getCount() const {
			return _count;
		}

		void setDataSize(int size) {
			_size = size;
		}

		void resetCount() {
			_count = 0;
		}
	};

	class CopyConstructorCounter : public ExcutionCounter {
	public:
		CopyConstructorCounter(int dataSize) {
			setDataSize(dataSize);
		}

		void operatorFunction(void* pDest, const void* pSrc) {
			_count++;
			memcpy_s(pDest, _size, pSrc, _size);
		}
	};

	class ExcutionOrder {
	protected:
		int _id;
		list<int>* _recorder;
	public:

		ExcutionOrder(list<int>* recorder, int id) : _id(id), _recorder(recorder) {}

		void operatorFunction(void* pValue) {
			_recorder->push_back(_id);
		}

		void operatorFunction(void* pDest, const void* pSrc) {
			_recorder->push_back(_id);
		}
	};

	class CompletedConstructorDestructors : public ::testing::Test {
	protected:
		CompilerSuite compiler;
		const BasicTypes* basicType;
		ScriptCompiler* scriptCompiler;

		ScriptType typePoint;
		ScriptType typeRectangle;

		template <class T>
		void registerConstructor(T* obj, int type) {
			ScriptType stype(type, scriptCompiler->getType(type));

			DFunction2* initFunction = new MFunction2<void, T, void*>(obj, &T::operatorFunction);
			int functionId = scriptCompiler->registFunction("constructorCount", stype.makeRef().sType(), new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0) << L"Register function for constructor failed";

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			EXPECT_TRUE(blRes) << L"Register constructor failed";
		}

		template <class T>
		void registerCopyConstructor(T* obj, int type) {
			ScriptType stype(type, scriptCompiler->getType(type));
			auto refType = stype.makeRef();
			string args = refType.sType();
			args.append(",");
			args.append(stype.makeSemiRef().sType());
			//args.append(stype.sType());

			DFunction2* copyFunction = new MFunction2<void, T, void*, const void*>(obj, &T::operatorFunction);
			int functionId = scriptCompiler->registFunction("copyConstructor", args, new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", copyFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0) << L"Register function for copy constructor failed";

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			EXPECT_TRUE(blRes) << L"Register copy constructor failed";
		}

		template <class T>
		void registerDestructor(T* obj, int type) {
			ScriptType stype(type, scriptCompiler->getType(type));

			DFunction2* initFunction = new MFunction2<void, T, void*>(obj, &T::operatorFunction);
			int functionId = scriptCompiler->registFunction("destructorCount", stype.makeRef().sType(), new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0) << L"Register function for constructor failed";

			bool blRes = scriptCompiler->registDestructor(type, functionId);
			EXPECT_TRUE(blRes) << L"Register destructor failed";
		}

		CompletedConstructorDestructors()
		{
			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			scriptCompiler = compiler.getCompiler().get();
			basicType = &compiler.getTypeManager()->getBasicTypes();

			ScriptType typeInt(basicType->TYPE_INT, "int");

			StructClass* structPoint = new StructClass(scriptCompiler, "Point");
			structPoint->addMember(typeInt, "x");
			structPoint->addMember(typeInt, "y");
			int iPointType = scriptCompiler->registStruct(structPoint);
			typePoint.setType(iPointType);
			typePoint.setTypeStr(structPoint->getName());

			StructClass* structRectangle = new StructClass(scriptCompiler, "Rectangle");
			structRectangle->addMember(typePoint, "location");
			structRectangle->addMember(typeInt, "width");
			structRectangle->addMember(typeInt, "height");
			int iRectangleType = scriptCompiler->registStruct(structRectangle);
			typeRectangle.setType(iRectangleType);
			typeRectangle.setTypeStr(structRectangle->getName());
		}
	};

	namespace CompletedConstructorDestructorsUT
	{
		TEST_F(CompletedConstructorDestructors, SimpleUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter constructorCounter, destructorCounter;
			registerConstructor(&constructorCounter, basicType->TYPE_INT );
			registerDestructor(&destructorCounter, basicType->TYPE_INT);			

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	int ret = 1;"
				L"}" 
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			int interferAssigment = scriptCompiler->findFunction("=", "int&,int");
			// if operator '=' of interger is not defined...
			if (interferAssigment < 0) {
				// ...then cannot construct object ret in expression int ret = 1;
				EXPECT_EQ(nullptr, program) << L"compile program should failed";
				return;
			}
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			// because integer has no copy constructor, so in a declaration assigment expression.
			// the default constructor is run before operator '=' is run.
			EXPECT_EQ(1, constructorCounter.getCount()) << L"Construtor is run but result is not correct";

			// because  operator '=' return an integer value, so after execute the operation int ret = 1;
			// the destructor for integer is executed. So, destructor count must be 2
			EXPECT_EQ(2, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, SimpleUT2)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter constructorCounter, destructorCounter;
			CopyConstructorCounter copyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&constructorCounter, basicType->TYPE_INT);
			registerDestructor(&destructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&copyConstructorCounter, basicType->TYPE_INT);

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	int ret = 1;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			// because integer has copy constructor, so default constructor is not used
			EXPECT_EQ(0, constructorCounter.getCount()) << L"Construtor is run but result is not correct";

			EXPECT_EQ(1, copyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			// because integer has copy constructor, so operator '=' is replaced by copy constructor
			// then destructor for return value of operator '=' have no change to run. So, destructor count must be 1
			EXPECT_EQ(1, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
		}
		
		TEST_F(CompletedConstructorDestructors, StructUT01)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter constructorCounter, destructorCounter;
			CopyConstructorCounter copyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&constructorCounter, basicType->TYPE_INT);
			registerDestructor(&destructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&copyConstructorCounter, basicType->TYPE_INT);

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	Point point;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(2, constructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(2, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, copyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, StructUT02)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter constructorCounter, destructorCounter;
			CopyConstructorCounter copyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&constructorCounter, basicType->TYPE_INT);
			registerDestructor(&destructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&copyConstructorCounter, basicType->TYPE_INT);

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	Rectangle rect;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(4, constructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(4, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, copyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, StructUT03)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter constructorCounter, destructorCounter;
			CopyConstructorCounter copyConstructorCounter(scriptCompiler->getTypeSize(typePoint.iType()));
			registerConstructor(&constructorCounter, typePoint.iType());
			registerDestructor(&destructorCounter, typePoint.iType());
			registerCopyConstructor(&copyConstructorCounter, typePoint.iType());

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	Point point;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(1, constructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(1, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, copyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, StructUT04)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			ExcutionCounter constructorCounter, destructorCounter;
			CopyConstructorCounter copyConstructorCounter(scriptCompiler->getTypeSize(typePoint.iType()));
			registerConstructor(&constructorCounter, typePoint.iType());
			registerDestructor(&destructorCounter, typePoint.iType());
			registerCopyConstructor(&copyConstructorCounter, typePoint.iType());

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	Point point;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(2, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(2, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			EXPECT_EQ(1, constructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(1, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, copyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, StructUT05)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter constructorCounter, destructorCounter;
			CopyConstructorCounter copyConstructorCounter(scriptCompiler->getTypeSize(typePoint.iType()));
			registerConstructor(&constructorCounter, typePoint.iType());
			registerDestructor(&destructorCounter, typePoint.iType());
			registerCopyConstructor(&copyConstructorCounter, typePoint.iType());

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	Rectangle rect;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(1, constructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(1, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, copyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, StructUT06)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter constructorCounter, destructorCounter;
			CopyConstructorCounter copyConstructorCounter(scriptCompiler->getTypeSize(typeRectangle.iType()));
			registerConstructor(&constructorCounter, typeRectangle.iType());
			registerDestructor(&destructorCounter, typeRectangle.iType());
			registerCopyConstructor(&copyConstructorCounter, typeRectangle.iType());

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	Rectangle rect;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(1, constructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(1, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, copyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, StructUT07)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter pointConstructorCounter, pointDestructorCounter;
			CopyConstructorCounter pointCopyConstructorCounter(scriptCompiler->getTypeSize(typePoint.iType()));
			registerConstructor(&pointConstructorCounter, typePoint.iType());
			registerDestructor(&pointDestructorCounter, typePoint.iType());
			registerCopyConstructor(&pointCopyConstructorCounter, typePoint.iType());

			ExcutionCounter constructorCounter, destructorCounter;
			CopyConstructorCounter copyConstructorCounter(scriptCompiler->getTypeSize(typeRectangle.iType()));
			registerConstructor(&constructorCounter, typeRectangle.iType());
			registerDestructor(&destructorCounter, typeRectangle.iType());
			registerCopyConstructor(&copyConstructorCounter, typeRectangle.iType());

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	Rectangle rect;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(1, pointConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(1, pointDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, pointCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			EXPECT_EQ(1, constructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(1, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, copyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, StructUT08)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			ExcutionCounter pointConstructorCounter, pointDestructorCounter;
			CopyConstructorCounter pointCopyConstructorCounter(scriptCompiler->getTypeSize(typePoint.iType()));
			registerConstructor(&pointConstructorCounter, typePoint.iType());
			registerDestructor(&pointDestructorCounter, typePoint.iType());
			registerCopyConstructor(&pointCopyConstructorCounter, typePoint.iType());

			ExcutionCounter constructorCounter, destructorCounter;
			CopyConstructorCounter copyConstructorCounter(scriptCompiler->getTypeSize(typeRectangle.iType()));
			registerConstructor(&constructorCounter, typeRectangle.iType());
			registerDestructor(&destructorCounter, typeRectangle.iType());
			registerCopyConstructor(&copyConstructorCounter, typeRectangle.iType());

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	Rectangle rect;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo","");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(4, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(4, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			EXPECT_EQ(1, pointConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(1, pointDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, pointCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			EXPECT_EQ(1, constructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(1, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, copyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, StructUT09)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			ExcutionCounter pointConstructorCounter, pointDestructorCounter;
			CopyConstructorCounter pointCopyConstructorCounter(scriptCompiler->getTypeSize(typePoint.iType()));
			registerConstructor(&pointConstructorCounter, typePoint.iType());
			registerDestructor(&pointDestructorCounter, typePoint.iType());
			registerCopyConstructor(&pointCopyConstructorCounter, typePoint.iType());

			ExcutionCounter constructorCounter, destructorCounter;
			CopyConstructorCounter copyConstructorCounter(scriptCompiler->getTypeSize(typeRectangle.iType()));
			registerConstructor(&constructorCounter, typeRectangle.iType());
			registerDestructor(&destructorCounter, typeRectangle.iType());
			registerCopyConstructor(&copyConstructorCounter, typeRectangle.iType());

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	Point point;"
				L"	point.x = 0;"
				L"	point.y = 1;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int expectedDestructorCount = 4;
			int interferAssigment = scriptCompiler->findFunction("=", "int&,int");
			// if operator '=' of interger is not defined...
			if (interferAssigment < 0) {
				// ...then operator '=' is replace by default copy operator
				// becuase default copy operator is not return a l-value,
				// the destructor is not run for return object from the default copy operator
				expectedDestructorCount = 2;
			}

			EXPECT_EQ(2, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(expectedDestructorCount, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			EXPECT_EQ(1, pointConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(1, pointDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, pointCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			EXPECT_EQ(0, constructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(0, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, copyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, StructUT10)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			ExcutionCounter pointConstructorCounter, pointDestructorCounter;
			CopyConstructorCounter pointCopyConstructorCounter(scriptCompiler->getTypeSize(typePoint.iType()));
			registerConstructor(&pointConstructorCounter, typePoint.iType());
			registerDestructor(&pointDestructorCounter, typePoint.iType());
			registerCopyConstructor(&pointCopyConstructorCounter, typePoint.iType());

			ExcutionCounter constructorCounter, destructorCounter;
			CopyConstructorCounter copyConstructorCounter(scriptCompiler->getTypeSize(typeRectangle.iType()));
			registerConstructor(&constructorCounter, typeRectangle.iType());
			registerDestructor(&destructorCounter, typeRectangle.iType());
			registerCopyConstructor(&copyConstructorCounter, typeRectangle.iType());

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	Point point;"
				L"	Rectangle rect;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);			

			EXPECT_EQ(6, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(6, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			EXPECT_EQ(2, pointConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(2, pointDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, pointCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			EXPECT_EQ(1, constructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(1, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, copyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, StructUT11)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			ExcutionCounter pointConstructorCounter, pointDestructorCounter;
			CopyConstructorCounter pointCopyConstructorCounter(scriptCompiler->getTypeSize(typePoint.iType()));
			registerConstructor(&pointConstructorCounter, typePoint.iType());
			registerDestructor(&pointDestructorCounter, typePoint.iType());
			registerCopyConstructor(&pointCopyConstructorCounter, typePoint.iType());

			ExcutionCounter constructorCounter, destructorCounter;
			CopyConstructorCounter copyConstructorCounter(scriptCompiler->getTypeSize(typeRectangle.iType()));
			registerConstructor(&constructorCounter, typeRectangle.iType());
			registerDestructor(&destructorCounter, typeRectangle.iType());
			registerCopyConstructor(&copyConstructorCounter, typeRectangle.iType());

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	Point point;"
				L"	point.x = 0;"
				L"	point.y = 1;"
				L"	Rectangle rect;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			int expectedDestructorCount = 8;
			int interferAssigment = scriptCompiler->findFunction("=", "int&,int");
			// if operator '=' of interger is not defined...
			if (interferAssigment < 0) {
				// ...then operator '=' is replace by default copy operator
				// becuase default copy operator is not return a l-value,
				// the destructor is not run for return object from the default copy operator
				expectedDestructorCount = 6;
			}

			EXPECT_EQ(6, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(expectedDestructorCount, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			EXPECT_EQ(2, pointConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(2, pointDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, pointCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			EXPECT_EQ(1, constructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(1, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, copyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, StructUT12)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			ExcutionCounter pointConstructorCounter, pointDestructorCounter;
			CopyConstructorCounter pointCopyConstructorCounter(scriptCompiler->getTypeSize(typePoint.iType()));
			registerConstructor(&pointConstructorCounter, typePoint.iType());
			registerDestructor(&pointDestructorCounter, typePoint.iType());
			registerCopyConstructor(&pointCopyConstructorCounter, typePoint.iType());

			ExcutionCounter constructorCounter, destructorCounter;
			CopyConstructorCounter copyConstructorCounter(scriptCompiler->getTypeSize(typeRectangle.iType()));
			registerConstructor(&constructorCounter, typeRectangle.iType());
			registerDestructor(&destructorCounter, typeRectangle.iType());
			registerCopyConstructor(&copyConstructorCounter, typeRectangle.iType());

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	Point point;"
				L"	Rectangle rect;"
				L"	rect.location = point;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(6, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(6, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			EXPECT_EQ(2, pointConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(2, pointDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, pointCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			EXPECT_EQ(1, constructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(1, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, copyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, CopyConstructorStructUT01)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			intConstructorCounter.setDataSize(scriptCompiler->getTypeSize(basicType->TYPE_INT));

			ExcutionCounter pointConstructorCounter, pointDestructorCounter;
			CopyConstructorCounter pointCopyConstructorCounter(scriptCompiler->getTypeSize(typePoint.iType()));
			registerConstructor(&pointConstructorCounter, typePoint.iType());
			registerDestructor(&pointDestructorCounter, typePoint.iType());
			registerCopyConstructor(&pointCopyConstructorCounter, typePoint.iType());

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	Point point1;"
				L"	Point point2 = point1;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(4, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(4, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			EXPECT_EQ(1, pointConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(2, pointDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(1, pointCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, CopyConstructorStructUT02)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			intConstructorCounter.setDataSize(scriptCompiler->getTypeSize(basicType->TYPE_INT));

			ExcutionCounter pointConstructorCounter, pointDestructorCounter;
			CopyConstructorCounter pointCopyConstructorCounter(scriptCompiler->getTypeSize(typePoint.iType()));
			registerConstructor(&pointConstructorCounter, typePoint.iType());
			registerDestructor(&pointDestructorCounter, typePoint.iType());
			registerCopyConstructor(&pointCopyConstructorCounter, typePoint.iType());

			ExcutionCounter constructorCounter, destructorCounter;
			CopyConstructorCounter copyConstructorCounter(scriptCompiler->getTypeSize(typeRectangle.iType()));
			registerConstructor(&constructorCounter, typeRectangle.iType());
			registerDestructor(&destructorCounter, typeRectangle.iType());
			registerCopyConstructor(&copyConstructorCounter, typeRectangle.iType());

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	Rectangle rect1;"
				L"	Rectangle rect2 = rect1;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(8, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(8, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			EXPECT_EQ(2, pointConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(2, pointDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, pointCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			EXPECT_EQ(1, constructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(2, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(1, copyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, SimpleArrayUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter constructorCounter, destructorCounter;
			registerConstructor(&constructorCounter, basicType->TYPE_INT);
			registerDestructor(&destructorCounter, basicType->TYPE_INT);

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	array<int,10> ret;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(10, constructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(10, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, SimpleArray2DUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter constructorCounter, destructorCounter;
			registerConstructor(&constructorCounter, basicType->TYPE_INT);
			registerDestructor(&destructorCounter, basicType->TYPE_INT);

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	array<int,10,10> ret;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(100, constructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(100, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, ComplexArrayUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter constructorCounter, destructorCounter;
			registerConstructor(&constructorCounter, basicType->TYPE_INT);
			registerDestructor(&destructorCounter, basicType->TYPE_INT);

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	array<Point,10> ret;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(20, constructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(20, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, ComplexArray2DUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter constructorCounter, destructorCounter;
			registerConstructor(&constructorCounter, basicType->TYPE_INT);
			registerDestructor(&destructorCounter, basicType->TYPE_INT);

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	array<Point,10,10> ret;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(200, constructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(200, destructorCounter.getCount()) << L"Destrutor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, StructArrayUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			intConstructorCounter.setDataSize(scriptCompiler->getTypeSize(basicType->TYPE_INT));

			ExcutionCounter pointConstructorCounter, pointDestructorCounter;
			CopyConstructorCounter pointCopyConstructorCounter(scriptCompiler->getTypeSize(typePoint.iType()));
			registerConstructor(&pointConstructorCounter, typePoint.iType());
			registerDestructor(&pointDestructorCounter, typePoint.iType());
			registerCopyConstructor(&pointCopyConstructorCounter, typePoint.iType());

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	array<Point,10> ret;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(20, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(20, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			EXPECT_EQ(10, pointConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(10, pointDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, pointCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, StructArray2DUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			intConstructorCounter.setDataSize(scriptCompiler->getTypeSize(basicType->TYPE_INT));

			ExcutionCounter pointConstructorCounter, pointDestructorCounter;
			CopyConstructorCounter pointCopyConstructorCounter(scriptCompiler->getTypeSize(typePoint.iType()));
			registerConstructor(&pointConstructorCounter, typePoint.iType());
			registerDestructor(&pointDestructorCounter, typePoint.iType());
			registerCopyConstructor(&pointCopyConstructorCounter, typePoint.iType());

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	array<Point,10, 10> ret;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(200, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(200, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";

			EXPECT_EQ(100, pointConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(100, pointDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, pointCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, ArrayInStructUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			ScriptType typeInt(basicType->TYPE_INT, "int");
			int iArrayType = scriptCompiler->registArrayType(L"array<int,10>");
			EXPECT_NE(DATA_TYPE_UNKNOWN, iArrayType) << L"Register array type failed";
			ScriptType arrayType(iArrayType, scriptCompiler->getType(iArrayType));

			StructClass* structSimpleArray = new StructClass(scriptCompiler, "SimpleArray");
			structSimpleArray->addMember(typeInt, "size");
			structSimpleArray->addMember(arrayType, "data");
			int iSimpleArrayType = scriptCompiler->registStruct(structSimpleArray);
			EXPECT_NE(DATA_TYPE_UNKNOWN, iSimpleArrayType) << L"Register struct type failed";

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	SimpleArray ret;"
				L"}"
				;

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(11, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(11, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		// assignment
		TEST_F(CompletedConstructorDestructors, ArrayInStructUT2)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			ScriptType typeInt(basicType->TYPE_INT, "int");
			int iArrayType = scriptCompiler->registArrayType(L"array<int,10>");
			EXPECT_NE(DATA_TYPE_UNKNOWN, iArrayType) << L"Register array type failed";
			ScriptType arrayType(iArrayType, scriptCompiler->getType(iArrayType));

			StructClass* structSimpleArray = new StructClass(scriptCompiler, "SimpleArray");
			structSimpleArray->addMember(typeInt, "size");
			structSimpleArray->addMember(arrayType, "data");
			int iSimpleArrayType = scriptCompiler->registStruct(structSimpleArray);
			EXPECT_NE(DATA_TYPE_UNKNOWN, iSimpleArrayType) << L"Register struct type failed";

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	SimpleArray ret = {2, {1, 2}};"
				L"}"
				;

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(8, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(11, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(3, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		// assignment
		TEST_F(CompletedConstructorDestructors, ArrayInStructUT3)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			ScriptType typeInt(basicType->TYPE_INT, "int");
			int iArrayType = scriptCompiler->registArrayType(L"array<int,10>");
			EXPECT_NE(DATA_TYPE_UNKNOWN, iArrayType) << L"Register array type failed";
			ScriptType arrayType(iArrayType, scriptCompiler->getType(iArrayType));

			StructClass* structSimpleArray = new StructClass(scriptCompiler, "SimpleArray");
			structSimpleArray->addMember(typeInt, "size");
			structSimpleArray->addMember(arrayType, "data");
			int iSimpleArrayType = scriptCompiler->registStruct(structSimpleArray);
			EXPECT_NE(DATA_TYPE_UNKNOWN, iSimpleArrayType) << L"Register struct type failed";

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	SimpleArray ret = {2, {1, 2}};"
				L"}"
				;

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_EQ(nullptr, program) << L"Compile program should failed due to no default constructor for int but copy constructor was defined for int";
		}

		/// check oder of calling operator for single object
		TEST_F(CompletedConstructorDestructors, OperatorOrderUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			list<int> recorder;

			ExcutionOrder intConstructorCounter(&recorder, 1);
			ExcutionOrder intDestructorCounter(&recorder, 2);
			ExcutionOrder intCopyConstructorCounter(&recorder, 3);

			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			ExcutionOrder pointConstructorCounter(&recorder, 4);
			ExcutionOrder pointDestructorCounter(&recorder, 5);
			ExcutionOrder pointCopyConstructorCounter(&recorder, 6);

			registerConstructor(&pointConstructorCounter, typePoint.iType());
			registerDestructor(&pointDestructorCounter, typePoint.iType());
			registerCopyConstructor(&pointCopyConstructorCounter, typePoint.iType());

			ExcutionOrder constructorCounter(&recorder, 7);
			ExcutionOrder destructorCounter(&recorder, 8);
			ExcutionOrder copyConstructorCounter(&recorder, 9);

			registerConstructor(&constructorCounter, typeRectangle.iType());
			registerDestructor(&destructorCounter, typeRectangle.iType());
			registerCopyConstructor(&copyConstructorCounter, typeRectangle.iType());

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	Rectangle rect1;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			list<int> expected_order = {
				1, 1, // default constructor for { location.x, location.y }
				4,    // default constructor for location
				1, 1, // default constructor for width and height
				7,    // default constructor for Rectangle
				8,    // destructor for Rectangle
				2, 2, // destructor for width and height
				5,    // destructor for location
				2, 2  // destructor for { location.x, location.y }
			};

			EXPECT_EQ(expected_order.size(), recorder.size()) << L"constructor and destructor does not work properly";

			auto jt = recorder.begin();
			for (auto it = expected_order.begin(); it != expected_order.end(); it++, jt++) {
				if (*jt != *it) {
					break;
				}
			}

			EXPECT_TRUE(jt == recorder.end()) << L"constructor or destructor is executed in wrong order";
		}

		/// check oder of calling operator for multi object and use copy constructor
		TEST_F(CompletedConstructorDestructors, OperatorOrderUT2)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			list<int> recorder;

			ExcutionOrder intConstructorCounter(&recorder, 1);
			ExcutionOrder intDestructorCounter(&recorder, 2);
			ExcutionOrder intCopyConstructorCounter(&recorder, 3);

			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			ExcutionOrder pointConstructorCounter(&recorder, 4);
			ExcutionOrder pointDestructorCounter(&recorder, 5);
			ExcutionOrder pointCopyConstructorCounter(&recorder, 6);

			registerConstructor(&pointConstructorCounter, typePoint.iType());
			registerDestructor(&pointDestructorCounter, typePoint.iType());
			registerCopyConstructor(&pointCopyConstructorCounter, typePoint.iType());

			ExcutionOrder constructorCounter(&recorder, 7);
			ExcutionOrder destructorCounter(&recorder, 8);
			ExcutionOrder copyConstructorCounter(&recorder, 9);

			registerConstructor(&constructorCounter, typeRectangle.iType());
			registerDestructor(&destructorCounter, typeRectangle.iType());
			registerCopyConstructor(&copyConstructorCounter, typeRectangle.iType());

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	Rectangle rect1;"
				L"	Rectangle rect2 = rect1;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			list<int> expected_order = {
				1, 1, // default constructor for { location.x, location.y }
				4,    // default constructor for location
				1, 1, // default constructor for width and height
				7,    // default constructor for Rectangle
				
				1, 1, // default constructor for { location.x, location.y }
				4,    // default constructor for location
				1, 1, // default constructor for width and height
				9,    // copy constructor for Rectangle

				8,    // destructor for Rectangle
				2, 2, // destructor for width and height
				5,    // destructor for location
				2, 2,  // destructor for { location.x, location.y }

				8,    // destructor for Rectangle
				2, 2, // destructor for width and height
				5,    // destructor for location
				2, 2,  // destructor for { location.x, location.y }
			};

			EXPECT_EQ(expected_order.size(), recorder.size()) << L"constructor and destructor does not work properly";

			auto jt = recorder.begin();
			for (auto it = expected_order.begin(); it != expected_order.end(); it++, jt++) {
				if (*jt != *it) {
					break;
				}
			}

			EXPECT_TRUE(jt == recorder.end()) << L"constructor or destructor is executed in wrong order";
		}

		/// check oder of calling operator for multi object and multi type
		TEST_F(CompletedConstructorDestructors, OperatorOrderUT3)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			list<int> recorder;

			ExcutionOrder intConstructorCounter(&recorder, 1);
			ExcutionOrder intDestructorCounter(&recorder, 2);
			ExcutionOrder intCopyConstructorCounter(&recorder, 3);

			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			ExcutionOrder pointConstructorCounter(&recorder, 4);
			ExcutionOrder pointDestructorCounter(&recorder, 5);
			ExcutionOrder pointCopyConstructorCounter(&recorder, 6);

			registerConstructor(&pointConstructorCounter, typePoint.iType());
			registerDestructor(&pointDestructorCounter, typePoint.iType());
			registerCopyConstructor(&pointCopyConstructorCounter, typePoint.iType());

			ExcutionOrder constructorCounter(&recorder, 7);
			ExcutionOrder destructorCounter(&recorder, 8);
			ExcutionOrder copyConstructorCounter(&recorder, 9);

			registerConstructor(&constructorCounter, typeRectangle.iType());
			registerDestructor(&destructorCounter, typeRectangle.iType());
			registerCopyConstructor(&copyConstructorCounter, typeRectangle.iType());

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	Point point;"
				L"	Rectangle rect;"
				L"}"
				;

			scriptCompiler->beginUserLib();

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'test'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			list<int> expected_order = {
				1, 1, // default constructor for { point.x, point.y }
				4,    // default constructor for point

				1, 1, // default constructor for { location.x, location.y }
				4,    // default constructor for location
				1, 1, // default constructor for width and height
				7,    // default constructor for Rectangle

				8,    // destructor for Rectangle
				2, 2, // destructor for width and height
				5,    // destructor for location
				2, 2,  // destructor for { location.x, location.y }

				5,    // destructor for point
				2, 2, // destructor { point.x, point.y }				
			};

			EXPECT_EQ(expected_order.size(), recorder.size()) << L"constructor and destructor does not work properly";

			auto jt = recorder.begin();
			for (auto it = expected_order.begin(); it != expected_order.end(); it++, jt++) {
				if (*jt != *it) {
					break;
				}
			}

			EXPECT_TRUE(jt == recorder.end()) << L"constructor or destructor is executed in wrong order";
		}

		TEST_F(CompletedConstructorDestructors, CompositeTypeInParametersUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			ScriptType typeInt(basicType->TYPE_INT, "int");

			StructClass* pStructPoint = new StructClass(scriptCompiler, "Point2i");
			pStructPoint->addMember(typeInt, "x");
			pStructPoint->addMember(typeInt, "y");
			int iPoint = scriptCompiler->registStruct(pStructPoint);
			EXPECT_NE(DATA_TYPE_UNKNOWN, iPoint) << L"Register struct type failed";

			const wchar_t scriptCode[] =
				L"void foo(Point2i ret) {"
				L"}"
				L"void foo() {"
				L"    foo({1,2});"
				L"}"
				;

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(0, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(2, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(2, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		// this bellow test case is not released at this time, it will be checked again soon
#ifdef _DEBUG
		TEST_F(CompletedConstructorDestructors, CompositeTypeInParametersUT2)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			ScriptType typeInt(basicType->TYPE_INT, "int");

			StructClass* pStructPoint = new StructClass(scriptCompiler, "Point2i");
			pStructPoint->addMember(typeInt, "x");
			pStructPoint->addMember(typeInt, "y");
			int iPoint = scriptCompiler->registStruct(pStructPoint);
			EXPECT_NE(DATA_TYPE_UNKNOWN, iPoint) << L"Register struct type failed";

			const wchar_t scriptCode[] =
				L"void foo(Point2i ret) {"
				L"}"
				L"void foo() {"
				L"    foo({1,2.0});"
				L"}"
				;

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(0, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(2, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(2, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}
#endif // _DEBUG
		static void constructPoint(void* p, int x, int y) {
			*(int*)p = *(int*)p + x;
			*((int*)p + 1) = *((int*)p + 1) + y;
		}
		
		TEST_F(CompletedConstructorDestructors, CompositeTypeInParametersUT3)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			intConstructorCounter.setDataSize(sizeof(int));

			ScriptType typeInt(basicType->TYPE_INT, "int");

			StructClass* pStructPoint = new StructClass(scriptCompiler, "Point2i");
			pStructPoint->addMember(typeInt, "x");
			pStructPoint->addMember(typeInt, "y");
			int iPoint = scriptCompiler->registStruct(pStructPoint);
			EXPECT_NE(DATA_TYPE_UNKNOWN, iPoint) << L"Register struct type failed";

			DFunction2* constructFunction = new FunctionDelegate2<void, void*, int, int>(constructPoint);
			int functionId = scriptCompiler->registFunction("constructPoint", "ref Point2i, int, int", new BasicFunctionFactory<3>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", constructFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0) << L"Register function for copy constructor failed";

			bool blRes = scriptCompiler->registConstructor(iPoint, functionId);
			EXPECT_TRUE(blRes) << L"Register copy constructor failed";

			const wchar_t scriptCode[] =
				L"int foo(Point2i p) {"
				L" return p.x + p.y;"
				L"}"
				L"int foo() {"
				L"    return foo({1,2});"
				L"}"
				;

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int*iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(3, *iRes) << L"default construtor for children is not run";

			EXPECT_EQ(2, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(2, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}
		// this bellow test case is not released at this time, it will be checked again soon
#ifdef _DEBUG
		TEST_F(CompletedConstructorDestructors, CompositeTypeInParametersUT3_1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			intConstructorCounter.setDataSize(sizeof(int));

			ScriptType typeInt(basicType->TYPE_INT, "int");

			StructClass* pStructPoint = new StructClass(scriptCompiler, "Point2i");
			pStructPoint->addMember(typeInt, "x");
			pStructPoint->addMember(typeInt, "y");
			int iPoint = scriptCompiler->registStruct(pStructPoint);
			EXPECT_NE(DATA_TYPE_UNKNOWN, iPoint) << L"Register struct type failed";

			DFunction2* constructFunction = new FunctionDelegate2<void, void*, int, int>(constructPoint);
			int functionId = scriptCompiler->registFunction("constructPoint", "ref Point2i, int, int", new BasicFunctionFactory<3>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", constructFunction, scriptCompiler));
			EXPECT_TRUE(functionId >= 0) << L"Register function for copy constructor failed";

			bool blRes = scriptCompiler->registConstructor(iPoint, functionId);
			EXPECT_TRUE(blRes) << L"Register copy constructor failed";

			const wchar_t scriptCode[] =
				L"int foo(Point2i& p) {"
				L" return p.x + p.y;"
				L"}"
				L"int foo() {"
				L"    return foo({1,2});"
				L"}"
				;

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int*iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(3, *iRes) << L"default construtor for children is not run";

			EXPECT_EQ(2, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(2, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}
#endif // _DEBUG

		TEST_F(CompletedConstructorDestructors, CompositeTypeInParametersUT3_2_PreTest)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ScriptType typeInt(basicType->TYPE_INT, "int");

			StructClass* pStructPoint = new StructClass(scriptCompiler, "Point2i");
			pStructPoint->addMember(typeInt.makeSemiRef(), "x");
			pStructPoint->addMember(typeInt, "y");
			int iPoint = scriptCompiler->registStruct(pStructPoint);
			EXPECT_NE(DATA_TYPE_UNKNOWN, iPoint) << L"Register struct type failed";

			const wchar_t scriptCode[] =
				L"int foo() {"
				L"    int n = 1;"
				L"    Point2i p;"
				L"    p.x = &n;"
				L"    return p.x;"
				L"}"
				;

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int*iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(1, *iRes) << L"accessing member in struct does not works properly";
		}

		TEST_F(CompletedConstructorDestructors, CompositeTypeInParametersUT3_2)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			intConstructorCounter.setDataSize(sizeof(int));

			ScriptType typeInt(basicType->TYPE_INT, "int");

			StructClass* pStructPoint = new StructClass(scriptCompiler, "Point2i");
			pStructPoint->addMember(typeInt.makeSemiRef(), "x");
			pStructPoint->addMember(typeInt, "y");
			int iPoint = scriptCompiler->registStruct(pStructPoint);
			EXPECT_NE(DATA_TYPE_UNKNOWN, iPoint) << L"Register struct type failed";			
			
			const wchar_t scriptCode[] =
				L"int foo(Point2i p) {"
				L" return p.x + p.y;"
				L"}"
				L"int foo() {"
				L"    return foo({1,2});"
				L"}"
				;

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int*iRes = (int*)scriptTask.getTaskResult();

			EXPECT_EQ(3, *iRes) << L"constructor for ref type does not work properly";
/*
			EXPECT_EQ(2, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(2, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(0, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";*/
		}

		TEST_F(CompletedConstructorDestructors, CompositeTypeInParametersUT4)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			intConstructorCounter.setDataSize(sizeof(int));

			const wchar_t scriptCode[] =
				L"int test() {"
				L" return 1;" /* copy integer constructor will be call to construct int object from 1 */
				L"}"
				L"void foo() {"
				L"    int x = test();" /* destructor will be called for x, destructor for return object from test will not be called */
				L"}"
				;

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(0, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(1, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(1, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}

		TEST_F(CompletedConstructorDestructors, CompositeTypeInParametersUT5)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			intConstructorCounter.setDataSize(sizeof(int));

			const wchar_t scriptCode[] =
				L"int test() {"
				L" int i = 1;" /* copy constructor of integer will be call here*/
				L" ref int pi = ref(i);"
				L" return *pi;" /* copy constructor of integer will be called here*/
				L"}"
				L"void foo() {"
				L"    int x = test();" /* destructor will be called for x, destructor for return object from test will not be called */
				L"}"
				;

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			EXPECT_NE(nullptr, program) << L"Compile program failed";

			int functionId = scriptCompiler->findFunction("foo", "");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			EXPECT_EQ(0, intConstructorCounter.getCount()) << L"Construtor is run but result is not correct";
			EXPECT_EQ(2, intDestructorCounter.getCount()) << L"Destrutor is run but result is not correct";
			EXPECT_EQ(2, intCopyConstructorCounter.getCount()) << L"copy constructor is run but result is not correct";
		}
	}
}