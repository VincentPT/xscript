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
#include "fftest.hpp"

#include "ExpresionParser.h"
#include <functional>
#include "TemplateForTest.hpp"
#include "Variable.h"
#include "ScriptCompiler.h"
#include "FunctionFactory.h"
#include "BasicType.h"
#include <memory>

using namespace std;
using namespace ffscript;

namespace ffscriptUT
{
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
			SPRINTF_S(s, "created object 0x%p (%d)", this, id);
			PRINT_TEST_MESSAGE(s);
		}
		~DemoAutoPtr() {
			char s[256];			
			SPRINTF_S(s, "deleted object 0x%p (%d)", this, id);			
			PRINT_TEST_MESSAGE(s);
		}
	};

	int DemoAutoPtr::g_autoId = 0;

	typedef shared_ptr<DemoAutoPtr> DemoAutoPtrRef;

	namespace ffscriptUT
	{
		FF_TEST_FUNCTION(ffscriptUTTest, testAutoPtr)
		{
			list<DemoAutoPtrRef> theList;	

			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));
			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));
			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));	
		}


		FF_TEST_FUNCTION(ffscriptUTTest, testAutoPtr2)
		{
			list<DemoAutoPtrRef> theList;
			list<DemoAutoPtrRef> theSecondList;


			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));
			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));
			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));
		
			theSecondList.push_back(theList.back());
		}

		FF_TEST_FUNCTION(ffscriptUTTest, testAutoPtr3)
		{
			list<DemoAutoPtrRef> theList;
			list<DemoAutoPtrRef>* theSecondList = new list<DemoAutoPtrRef>();


			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));
			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));
			theList.push_back(DemoAutoPtrRef(new DemoAutoPtr()));

			DemoAutoPtr* aa = new DemoAutoPtr();

			theSecondList->push_back(theList.back());
		}

		FF_TEST_FUNCTION(ffscriptUTTest, testAutoPtr4)
		{
			list<ExpUnitRef> theList;

			theList.push_back(ExpUnitRef(new OpenRoundBracket()));
			theList.push_back(ExpUnitRef(new ClosedRoundBracket()));

			list<ExecutableUnitRef> theSecondList;
			theSecondList.push_back(static_pointer_cast<ExecutableUnit>(theList.back()));
		}


		//FF_TEST_FUNCTION(ffscriptUTTest, testAutoPtr5)
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


		FF_TEST_FUNCTION(ffscriptUTTest, testFunctionTemplate1)
		{
			void* theFunction = getFunctionPointer<Internal, void> (&Internal::foo);

			FF_EXPECT_TRUE(true);
		}
		
		FF_TEST_FUNCTION(ffscriptUTTest, testFunctionTemplate2)
		{
			void* theFunction = getFunctionPointer<Internal, void, int>(&Internal::foo);

			FF_EXPECT_TRUE(true);
		}

		FF_TEST_FUNCTION(ffscriptUTTest, testFunctionTemplate3)
		{
			void* theFunction = getFunctionPointer<Internal, void*, char*>(&Internal::foo);

			FF_EXPECT_TRUE(true);
		}

		FF_TEST_FUNCTION(ffscriptUTTest, testFunctionTemplate4)
		{
			void* theFunction = getFunctionPointer<Internal, void*, char*, float>(&Internal::foo);

			FF_EXPECT_TRUE(true);
		}


		FF_TEST_FUNCTION(ffscriptUTTest, testTemplateSpecialization1)
		{
			//it shoud call classA for general
			classA<float> aa;
			FF_EXPECT_TRUE(aa.f(1.0f) == 0);
		}

		FF_TEST_FUNCTION(ffscriptUTTest, testTemplateSpecialization2)
		{
			//it shoud call classA for general
			classA<float, float, float> aa;
			FF_EXPECT_TRUE(aa.f(1.0f, 1, 2) == 3);
		}

		FF_TEST_FUNCTION(ffscriptUTTest, testTemplateSpecialization3)
		{
			//it shoud call classA for specialized int
			classA<int, char, int> aa;
			FF_EXPECT_TRUE(aa.f(1, 1, 2) == 4);
		}
		
		FF_TEST_FUNCTION(ffscriptUTTest, testScriptCompiler1)
		{
			DummyFactory dummyFactory;

			ScriptCompiler scriptCompiler;
			const BasicTypes& basicType = scriptCompiler.getTypeManager()->getBasicTypes();
			scriptCompiler.getTypeManager()->registerBasicTypes(&scriptCompiler);
			int functionId1 = scriptCompiler.registFunction("sin", "", &dummyFactory);
			int functionId2 = scriptCompiler.registFunction("sin", "double", &dummyFactory);
			FF_EXPECT_TRUE(functionId1 >= 0, L"register failed sin()");
			FF_EXPECT_TRUE(functionId2 >= 0, L"register failed sin(double)");
			FF_EXPECT_TRUE(functionId2 != functionId1, L"register failed: two different function with same function id returned");

			functionId2 = scriptCompiler.registFunction("sin", "double", &dummyFactory);
			FF_EXPECT_TRUE(functionId2 < 0, L"register same function but success return ");
		}
	};
}