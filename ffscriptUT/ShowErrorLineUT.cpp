/******************************************************************
* File:        ShowErrorLineUT.cpp
* Description: Test cases for checking error indicator of script
*              compiler in C Lambda scripting language.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "fftest.hpp"

#include <CompilerSuite.h>
#include <ScriptTask.h>
#include <Utils.h>
#include <DefaultPreprocessor.h>
#include <RawStringLib.h>

using namespace std;
using namespace ffscript;


namespace ffscriptUT
{	
	namespace ShowErrorLineUT
	{
		void complieErrorProgram(const char* file, int& line, int& column) {
			GlobalScopeRef rootScope;
			CompilerSuite compiler;

			compiler.initialize(1024);
			rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			auto script = readCodeFromUtf8File(file);

			compiler.setPreprocessor(std::make_shared<DefaultPreprocessor>());
			auto program = compiler.compileProgram(script.c_str(), script.c_str() + script.size());

			FF_EXPECT_EQ(nullptr, program, L"compie program should be failed");
			compiler.getLastCompliedPosition(line, column);

			string errorMsg("error at line = ");
			errorMsg.append(std::to_string(line + 1));
			errorMsg.append(", column = ");
			errorMsg.append(std::to_string(column + 1));
			errorMsg.append("\n");
			errorMsg.append(scriptCompiler->getLastError());

			//Logger::WriteMessage(errorMsg.c_str());
		}

		FF_TEST_FUNCTION(ShowErrorLine, ErrorInGlobalScope1)
		{
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInGlobalScope1.c955", line, column);

			// missing ; at the end of file
			FF_EXPECT_EQ(2, line);
			FF_EXPECT_EQ(0, column);
		}

		FF_TEST_FUNCTION(ShowErrorLine, ErrorInGlobalScope2)
		{
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInGlobalScope2.c955", line, column);

			FF_EXPECT_EQ(1, line);
			FF_EXPECT_EQ(6, column);
		}

		FF_TEST_FUNCTION(ShowErrorLine, ErrorInStruct1)
		{
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInStruct1.c955", line, column);

			FF_EXPECT_EQ(3, line);
			FF_EXPECT_EQ(7, column);
		}

		FF_TEST_FUNCTION(ShowErrorLine, ErrorInStruct2)
		{
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInStruct2.c955", line, column);

			FF_EXPECT_EQ(3, line);
			FF_EXPECT_EQ(9, column);
		}

		FF_TEST_FUNCTION(ShowErrorLine, ErrorInStruct3)
		{
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInStruct3.c955", line, column);

			FF_EXPECT_EQ(5, line);
			FF_EXPECT_EQ(0, column);
		}

		FF_TEST_FUNCTION(ShowErrorLine, ErrorInFunctionScope1) {
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInFunctionScope1.c955", line, column);

			FF_EXPECT_EQ(3, line);
			FF_EXPECT_EQ(18, column);
		}

		FF_TEST_FUNCTION(ShowErrorLine, ErrorInFunctionScope2) {
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInFunctionScope2.c955", line, column);

			FF_EXPECT_EQ(4, line);
			FF_EXPECT_EQ(0, column);
		}

		FF_TEST_FUNCTION(ShowErrorLine, ErrorInFunctionScope3) {
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInFunctionScope3.c955", line, column);

			FF_EXPECT_EQ(4, line);
			FF_EXPECT_EQ(9, column);
		}

		FF_TEST_FUNCTION(ShowErrorLine, ErrorInFunctionScope4) {
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInFunctionScope4.c955", line, column);

			FF_EXPECT_EQ(5, line);
			FF_EXPECT_EQ(0, column);
		}

		FF_TEST_FUNCTION(ShowErrorLine, ErrorInFunctionScope5) {
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInFunctionScope5.c955", line, column);

			FF_EXPECT_EQ(8, line);
			FF_EXPECT_EQ(0, column);
		}

		FF_TEST_FUNCTION(ShowErrorLine, ErrorInFunctionScope6) {
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInFunctionScope6.c955", line, column);

			FF_EXPECT_EQ(8, line);
			FF_EXPECT_EQ(0, column);
		}

		FF_TEST_FUNCTION(ShowErrorLine, ErrorInFunctionScope7) {
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInFunctionScope7.c955", line, column);

			FF_EXPECT_EQ(8, line);
			FF_EXPECT_EQ(4, column);
		}
	};
}