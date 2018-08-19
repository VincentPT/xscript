#include "stdafx.h"
#include "CppUnitTest.h"
#include <CompilerSuite.h>
#include <ScriptTask.h>
#include <Utils.h>
#include <DefaultPreprocessor.h>
#include <RawStringLib.h>

#include "Utils.h"

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
		}

		TEST_METHOD(ErrorInGlobalScope1)
		{
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInGlobalScope1.c955", line, column);

			Assert::AreEqual(1, line);
			Assert::AreEqual(9, column);
		}

		TEST_METHOD(ErrorInGlobalScope2)
		{
			int line = -1;
			int column = -1;

			complieErrorProgram("ErrorInGlobalScope2.c955", line, column);

			Assert::AreEqual(1, line);
			Assert::AreEqual(8, column);
		}
	};
}