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
#include <OperatorObject.h>

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

	TEST_CLASS(CompletedConstructorDestructorsUT)
	{
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
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			Assert::IsTrue(blRes, L"Register constructor failed");
		}

		template <class T>
		void registerCopyConstructor(T* obj, int type) {
			ScriptType stype(type, scriptCompiler->getType(type));
			auto refType = stype.makeRef();
			string args = refType.sType();
			args.append(",");
			args.append(stype.makeSemiRef().sType());

			DFunction2* copyFunction = new MFunction2<void, T, void*, const void*>(obj, &T::operatorFunction);
			int functionId = scriptCompiler->registFunction("copyConstructor", args, new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", copyFunction, scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for copy constructor failed");

			bool blRes = scriptCompiler->registConstructor(type, functionId);
			Assert::IsTrue(blRes, L"Register copy constructor failed");
		}

		template <class T>
		void registerDestructor(T* obj, int type) {
			ScriptType stype(type, scriptCompiler->getType(type));

			DFunction2* initFunction = new MFunction2<void, T, void*>(obj, &T::operatorFunction);
			int functionId = scriptCompiler->registFunction("destructorCount", stype.makeRef().sType(), new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", initFunction, scriptCompiler));
			Assert::IsTrue(functionId >= 0, L"Register function for constructor failed");

			bool blRes = scriptCompiler->registDestructor(type, functionId);
			Assert::IsTrue(blRes, L"Register destructor failed");
		}

	public:
		CompletedConstructorDestructorsUT()
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

		TEST_METHOD(SimpleUT1)
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
				Assert::IsNull(program, L"compile program should failed");
				return;
			}
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			// because integer has no copy constructor, so in a declaration assigment expression.
			// the default constructor is run before operator '=' is run.
			Assert::AreEqual(1, constructorCounter.getCount(), L"Construtor is run but result is not correct");

			// because  operator '=' return an integer value, so after execute the operation int ret = 1;
			// the destructor for integer is executed. So, destructor count must be 2
			Assert::AreEqual(2, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
		}

		TEST_METHOD(SimpleUT2)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			// because integer has copy constructor, so default constructor is not used
			Assert::AreEqual(0, constructorCounter.getCount(), L"Construtor is run but result is not correct");

			Assert::AreEqual(1, copyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			// because integer has copy constructor, so operator '=' is replaced by copy constructor
			// then destructor for return value of operator '=' have no change to run. So, destructor count must be 1
			Assert::AreEqual(1, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
		}
		
		TEST_METHOD(StructUT01)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(2, constructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(2, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, copyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");
		}

		TEST_METHOD(StructUT02)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(4, constructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(4, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, copyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");
		}

		TEST_METHOD(StructUT03)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(1, constructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(1, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, copyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");
		}

		TEST_METHOD(StructUT04)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(2, intConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(2, intDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, intCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			Assert::AreEqual(1, constructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(1, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, copyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");
		}

		TEST_METHOD(StructUT05)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(1, constructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(1, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, copyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");
		}

		TEST_METHOD(StructUT06)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(1, constructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(1, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, copyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");
		}

		TEST_METHOD(StructUT07)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(1, pointConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(1, pointDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, pointCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			Assert::AreEqual(1, constructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(1, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, copyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");
		}

		TEST_METHOD(StructUT08)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(4, intConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(4, intDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, intCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			Assert::AreEqual(1, pointConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(1, pointDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, pointCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			Assert::AreEqual(1, constructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(1, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, copyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");
		}

		TEST_METHOD(StructUT09)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

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

			Assert::AreEqual(2, intConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(expectedDestructorCount, intDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, intCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			Assert::AreEqual(1, pointConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(1, pointDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, pointCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			Assert::AreEqual(0, constructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(0, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, copyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");
		}

		TEST_METHOD(StructUT10)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);			

			Assert::AreEqual(6, intConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(6, intDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, intCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			Assert::AreEqual(2, pointConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(2, pointDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, pointCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			Assert::AreEqual(1, constructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(1, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, copyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");
		}

		TEST_METHOD(StructUT11)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

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

			Assert::AreEqual(6, intConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(expectedDestructorCount, intDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, intCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			Assert::AreEqual(2, pointConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(2, pointDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, pointCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			Assert::AreEqual(1, constructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(1, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, copyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");
		}

		TEST_METHOD(StructUT12)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(6, intConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(6, intDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, intCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			Assert::AreEqual(2, pointConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(2, pointDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, pointCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			Assert::AreEqual(1, constructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(1, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, copyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");
		}

		TEST_METHOD(CopyConstructorStructUT01)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(4, intConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(4, intDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, intCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			Assert::AreEqual(1, pointConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(2, pointDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(1, pointCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");
		}

		TEST_METHOD(CopyConstructorStructUT02)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(8, intConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(8, intDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, intCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			Assert::AreEqual(2, pointConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(2, pointDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, pointCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			Assert::AreEqual(1, constructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(2, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(1, copyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");
		}

		TEST_METHOD(SimpleArrayUT1)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(10, constructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(10, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
		}

		TEST_METHOD(SimpleArray2DUT1)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(100, constructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(100, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
		}

		TEST_METHOD(ComplexArrayUT1)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(20, constructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(20, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
		}

		TEST_METHOD(ComplexArray2DUT1)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(200, constructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(200, destructorCounter.getCount(), L"Destrutor is run but result is not correct");
		}

		TEST_METHOD(StructArrayUT1)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(20, intConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(20, intDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, intCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			Assert::AreEqual(10, pointConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(10, pointDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, pointCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");
		}

		TEST_METHOD(StructArray2DUT1)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(200, intConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(200, intDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, intCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");

			Assert::AreEqual(100, pointConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(100, pointDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, pointCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");
		}

		TEST_METHOD(ArrayInStructUT1)
		{
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			ExcutionCounter intConstructorCounter, intDestructorCounter;
			CopyConstructorCounter intCopyConstructorCounter(scriptCompiler->getTypeSize(basicType->TYPE_INT));
			registerConstructor(&intConstructorCounter, basicType->TYPE_INT);
			registerDestructor(&intDestructorCounter, basicType->TYPE_INT);
			registerCopyConstructor(&intCopyConstructorCounter, basicType->TYPE_INT);

			ScriptType typeInt(basicType->TYPE_INT, "int");
			int iArrayType = scriptCompiler->registArrayType(L"array<int,10>");
			Assert::AreNotEqual(DATA_TYPE_UNKNOWN, iArrayType, L"Register array type failed");
			ScriptType arrayType(iArrayType, scriptCompiler->getType(iArrayType));

			StructClass* structSimpleArray = new StructClass(scriptCompiler, "SimpleArray");
			structSimpleArray->addMember(typeInt, "size");
			structSimpleArray->addMember(arrayType, "data");
			int iSimpleArrayType = scriptCompiler->registStruct(structSimpleArray);
			Assert::AreNotEqual(DATA_TYPE_UNKNOWN, iSimpleArrayType, L"Register struct type failed");

			const wchar_t scriptCode[] =
				L"void foo() {"
				L"	SimpleArray ret;"
				L"}"
				;

			scriptCompiler->beginUserLib();
			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			Assert::AreEqual(11, intConstructorCounter.getCount(), L"Construtor is run but result is not correct");
			Assert::AreEqual(11, intDestructorCounter.getCount(), L"Destrutor is run but result is not correct");
			Assert::AreEqual(0, intCopyConstructorCounter.getCount(), L"copy constructor is run but result is not correct");
		}

		/// check oder of calling operator for single object
		TEST_METHOD(OperatorOrderUT1)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

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

			Assert::AreEqual(expected_order.size(), recorder.size(), L"constructor and destructor does not work properly");

			auto jt = recorder.begin();
			for (auto it = expected_order.begin(); it != expected_order.end(); it++, jt++) {
				if (*jt != *it) {
					break;
				}
			}

			Assert::IsTrue(jt == recorder.end(), L"constructor or destructor is executed in wrong order");
		}

		/// check oder of calling operator for multi object and use copy constructor
		TEST_METHOD(OperatorOrderUT2)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

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

			Assert::AreEqual(expected_order.size(), recorder.size(), L"constructor and destructor does not work properly");

			auto jt = recorder.begin();
			for (auto it = expected_order.begin(); it != expected_order.end(); it++, jt++) {
				if (*jt != *it) {
					break;
				}
			}

			Assert::IsTrue(jt == recorder.end(), L"constructor or destructor is executed in wrong order");
		}

		/// check oder of calling operator for multi object and multi type
		TEST_METHOD(OperatorOrderUT3)
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
			Assert::IsNotNull(program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", {});
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

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

			Assert::AreEqual(expected_order.size(), recorder.size(), L"constructor and destructor does not work properly");

			auto jt = recorder.begin();
			for (auto it = expected_order.begin(); it != expected_order.end(); it++, jt++) {
				if (*jt != *it) {
					break;
				}
			}

			Assert::IsTrue(jt == recorder.end(), L"constructor or destructor is executed in wrong order");
		}
	};
}