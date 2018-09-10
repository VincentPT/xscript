/******************************************************************
* File:        ffscriptUT.cpp
* Description: Test cases of checking some C++ techniques before
*              using it inside C Lambda compiling library. 
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
#include "ScriptCompiler.h"
#include "FunctionFactory.h"
#include "BasicType.h"
#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace ffscript;

namespace ffscriptUT
{
	typedef void (__stdcall *STDFunc)(char*, float);

	void __stdcall stdfunc(char* a, float b) {
	}

	class Internal {
	public:
		void foo() {

		}

		void foo(int a) {

		}

		void* foo(char* a) {
			return (void*)a;
		}

		void* foo(char* a, float b) {
			return (void*)a;
		}
	};

	class DummyFactory : public FunctionFactory {
	public:
		Function* createFunction(const std::string& name, int id) {
			return nullptr;
		}
	};

	class DemoAutoPtr {
		static int g_autoId;
		int id;

	public:
		DemoAutoPtr() {
			char s[256];
			id = g_autoId++;
			sprintf_s(s, "created object 0x%p (%d)", this, id);
			PRINT_TEST_MESSAGE(s);
		}
		~DemoAutoPtr() {
			char s[256];			
			sprintf_s(s, "deleted object 0x%p (%d)", this, id);			
			PRINT_TEST_MESSAGE(s);
		}
	};

	int DemoAutoPtr::g_autoId = 0;

	typedef shared_ptr<DemoAutoPtr> DemoAutoPtrRef;

	TEST_CLASS(ffscriptUT)
	{
	public:

		TEST_METHOD(testAutoPtr)
		{
			list<DemoAutoPtrRef> theList;	

			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));
			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));
			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));	
		}


		TEST_METHOD(testAutoPtr2)
		{
			list<DemoAutoPtrRef> theList;
			list<DemoAutoPtrRef> theSecondList;


			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));
			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));
			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));
		
			theSecondList.push_back(theList.back());
		}

		TEST_METHOD(testAutoPtr3)
		{
			list<DemoAutoPtrRef> theList;
			list<DemoAutoPtrRef>* theSecondList = new list<DemoAutoPtrRef>();


			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));
			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));
			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));

			DemoAutoPtr* aa = new DemoAutoPtr();

			theSecondList->push_back(theList.back());
		}

		TEST_METHOD(testAutoPtr4)
		{
			list<ExpUnitRef> theList;

			theList.push_back(ExpUnitRef(new OpenRoundBracket()));
			theList.push_back(ExpUnitRef(new ClosedRoundBracket()));

			list<ExecutableUnitRef> theSecondList;
			theSecondList.push_back(static_pointer_cast<ExecutableUnit>(theList.back()));
		}


		//TEST_METHOD(testAutoPtr5)
		//{
		//	list<DemoAutoPtrRef> theList;
		//	list<DemoAutoPtrRef> theSecondList;

		//	DemoAutoPtr* a = new DemoAutoPtr();

		//	theList.push_back(DemoAutoPtrRef(a));
		//	theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));
		//	theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));
		//	//it crashs here
		//	theSecondList.push_back(DemoAutoPtrRef(a));
		//}


		TEST_METHOD(testFunctionTemplate1)
		{
			void* theFunction = getFunctionPointer<Internal, void> (&Internal::foo);

			Assert::IsTrue(true);
		}
		
		TEST_METHOD(testFunctionTemplate2)
		{
			void* theFunction = getFunctionPointer<Internal, void, int>(&Internal::foo);

			Assert::IsTrue(true);
		}

		TEST_METHOD(testFunctionTemplate3)
		{
			void* theFunction = getFunctionPointer<Internal, void*, char*>(&Internal::foo);

			Assert::IsTrue(true);
		}

		TEST_METHOD(testFunctionTemplate4)
		{
			void* theFunction = getFunctionPointer<Internal, void*, char*, float>(&Internal::foo);

			Assert::IsTrue(true);
		}

		TEST_METHOD(testSTDFunction)
		{
			STDFunc func = stdfunc;
			func(nullptr, 1.0f);
			Assert::IsTrue(true);
		}

		TEST_METHOD(testTemplateSpecialization1)
		{
			//it shoud call classA for general
			classA<float> aa;
			Assert::IsTrue(aa.f(1.0f) == 0);
		}

		TEST_METHOD(testTemplateSpecialization2)
		{
			//it shoud call classA for general
			classA<float, float, float> aa;
			Assert::IsTrue(aa.f(1.0f, 1, 2) == 3);
		}

		TEST_METHOD(testTemplateSpecialization3)
		{
			//it shoud call classA for specialized int
			classA<int, char, int> aa;
			Assert::IsTrue(aa.f(1, 1, 2) == 4);
		}
		
		TEST_METHOD(testScriptCompiler1)
		{
			DummyFactory dummyFactory;

			ScriptCompiler scriptCompiler;
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			int functionId1 = scriptCompiler.registFunction("sin", "", &dummyFactory);
			int functionId2 = scriptCompiler.registFunction("sin", "double", &dummyFactory);
			Assert::IsTrue(functionId1 >= 0, L"register failed sin()");
			Assert::IsTrue(functionId2 >= 0, L"register failed sin(double)");
			Assert::IsTrue(functionId2 != functionId1, L"register failed: two different function with same function id returned");

			functionId2 = scriptCompiler.registFunction("sin", "double", &dummyFactory);
			Assert::IsTrue(functionId2 < 0, L"register same function but success return ");
		}
	};
}