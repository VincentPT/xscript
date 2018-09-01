/******************************************************************
* File:        ShowErrorLineUT.cpp
* Description: Test cases for checking error indicator of script
*              compiler in C Lambda scripting language.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "CppUnitTest.h"
#include <CompilerSuite.h>
#include <ScriptTask.h>
#include <Utils.h>
#include <DefaultPreprocessor.h>
#include <RawStringLib.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace ffscript;


namespace ffscriptUT
{	
	TEST_CLASS(ShowErrorLineUT)
	{
	public:

		void complieErrorProgram(const char* file, int& line, int& column) {
			GlobalScopeRef rootScope;
			CompilerSuite compiler;

			compiler.initialize(1024);
			rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			auto script = readCodeFromUtf8File(file);

			compiler.setPreprocessor(std::make_shared<DefaultPreprocessor>());
			auto program = compiler.compileProgram(script.c_str(), script.c_str() + script.size());

			Assert::IsNull(program, L"compie program should be failed");
			compiler.getLastCompliedPosition(line, column);

			string errorMsg("error at line = ");
			errorMsg.append(std::to_string(line + 1));
			errorMsg.append(", column = ");
			errorMsg.append(std::to_string(column + 1));
			errorMsg.append("\n");
			errorMsg.append(scriptCompiler->getLastError());

			Logger::WriteMessage(errorMsg.c_str());
		}

		TEST_METHOD(ErrorInGlobalScope1)
		{
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInGlobalScope1.c955", line, column);

			// missing ; at the end of file
			Assert::AreEqual(2, line);
			Assert::AreEqual(0, column);
		}

		TEST_METHOD(ErrorInGlobalScope2)
		{
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInGlobalScope2.c955", line, column);

			Assert::AreEqual(1, line);
			Assert::AreEqual(6, column);
		}

		TEST_METHOD(ErrorInStruct1)
		{
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInStruct1.c955", line, column);

			Assert::AreEqual(3, line);
			Assert::AreEqual(7, column);
		}

		TEST_METHOD(ErrorInStruct2)
		{
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInStruct2.c955", line, column);

			Assert::AreEqual(3, line);
			Assert::AreEqual(9, column);
		}

		TEST_METHOD(ErrorInStruct3)
		{
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInStruct3.c955", line, column);

			Assert::AreEqual(5, line);
			Assert::AreEqual(0, column);
		}

		TEST_METHOD(ErrorInFunctionScope1) {
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInFunctionScope1.c955", line, column);

			Assert::AreEqual(3, line);
			Assert::AreEqual(18, column);
		}

		TEST_METHOD(ErrorInFunctionScope2) {
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInFunctionScope2.c955", line, column);

			Assert::AreEqual(4, line);
			Assert::AreEqual(0, column);
		}

		TEST_METHOD(ErrorInFunctionScope3) {
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInFunctionScope3.c955", line, column);

			Assert::AreEqual(4, line);
			Assert::AreEqual(9, column);
		}

		TEST_METHOD(ErrorInFunctionScope4) {
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInFunctionScope4.c955", line, column);

			Assert::AreEqual(5, line);
			Assert::AreEqual(0, column);
		}

		TEST_METHOD(ErrorInFunctionScope5) {
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInFunctionScope5.c955", line, column);

			Assert::AreEqual(8, line);
			Assert::AreEqual(0, column);
		}

		TEST_METHOD(ErrorInFunctionScope6) {
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInFunctionScope6.c955", line, column);

			Assert::AreEqual(8, line);
			Assert::AreEqual(0, column);
		}

		TEST_METHOD(ErrorInFunctionScope7) {
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInFunctionScope7.c955", line, column);

			Assert::AreEqual(8, line);
			Assert::AreEqual(4, column);
		}
	};
}