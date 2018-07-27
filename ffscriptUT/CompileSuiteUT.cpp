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
	TEST_CLASS(CompileSuiteUT)
	{
	public:
		TEST_METHOD(CompileSimpleProgram)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int dummy;"
				L"int square(int n) {"
				L"	return n * n;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			auto pVariable = rootScope->findVariable("dummy");

			int functionId = scriptCompiler->findFunction("square", pVariable->getDataType().sType());
			Assert::IsTrue(functionId >= 0, L"cannot find function 'square'");

			int n = 10;
			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == n*n, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileSimpleProgram2)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	return (2.5 + 1.0);"
				L"}"
				;
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == 3, L"program can run but return wrong value");
		}

		TEST_METHOD(CompileSimpleProgram3)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();
			
			includeRawStringToCompiler(scriptCompiler);
			// if not call bellow code, the RawString library will be remove when compile the program
			scriptCompiler->beginUserLib();

			const wchar_t* scriptCode =
				L"void main() {"
				L"	String s;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));

			int functionId = scriptCompiler->findFunction("main", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'main'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
		}

		TEST_METHOD(CompileSimpleProgram4)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();
			includeRawStringToCompiler(scriptCompiler);
			// if not call bellow code, the RawString library will be remove when compile the program
			scriptCompiler->beginUserLib();

			const wchar_t* scriptCode =
				L"void main() {"
				L"	String s = \"123\";"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));

			int functionId = scriptCompiler->findFunction("main", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'main'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
		}

		TEST_METHOD(CompileSimpleProgram5)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();
			includeRawStringToCompiler(scriptCompiler);
			// if not call bellow code, the RawString library will be remove when compile the program
			scriptCompiler->beginUserLib();

			const wchar_t* scriptCode =
				L"void main() {"
				L"	String s = L\"123\";"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));

			int functionId = scriptCompiler->findFunction("main", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'main'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
		}

		TEST_METHOD(CompileSimpleProgram6)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();
			includeRawStringToCompiler(scriptCompiler);
			// if not call bellow code, the RawString library will be remove when compile the program
			scriptCompiler->beginUserLib();

			const wchar_t* scriptCode =
				L"void main() {"
				L"	String s = L\"123\";"
				L"	String r = s;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));

			int functionId = scriptCompiler->findFunction("main", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'main'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
		}

		TEST_METHOD(ProgramIndependent1)
		{
			GlobalScopeRef rootScope;			
			Program* program;
			int functionId = -1;
			{
				CompilerSuite compiler;
				//the code does not contain any global scope'code and only a variable
				//so does not need global memory
				compiler.initialize(8);
				rootScope = compiler.getGlobalScope();
				auto scriptCompiler = rootScope->getCompiler();
				const wchar_t* scriptCode =
					L"int dummy;"
					L"int square(int n) {"
					L"	return n * n;"
					L"}"
					;

				program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
				auto pVariable = rootScope->findVariable("dummy");
				functionId = scriptCompiler->findFunction("square", pVariable->getDataType().sType());
			}
			
			Assert::IsTrue(functionId >= 0, L"cannot find function 'square'");

			int n = 10;
			ScriptParamBuffer paramBuffer(n);
			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == n*n, L"program can run but return wrong value");
		}

		TEST_METHOD(ProgramAndTypeIndependent1)
		{
			GlobalScopeRef rootScope;
			Program* program;
			TypeManagerRef typeManagerRef;
			int functionId = -1;
			{
				CompilerSuite compiler;
				//the code does not contain any global scope'code and only a variable
				//so does not need global memory
				compiler.initialize(8);
				rootScope = compiler.getGlobalScope();
				auto scriptCompiler = rootScope->getCompiler();
				const wchar_t* scriptCode =
					L"int square(int n) {"
					L"	return n * n;"
					L"}"
					;

				program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
				typeManagerRef = compiler.getTypeManager();
				functionId = scriptCompiler->findFunction("square", "int");
			}
 
			Assert::IsTrue(functionId >= 0, L"cannot find function 'square'");

			int n = 10;
			ScriptParamBuffer paramBuffer(n);

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == n*n, L"program can run but return wrong value");
		}

		TEST_METHOD(ProgramAndTypeIndependent2)
		{
			GlobalScopeRef rootScope;
			Program* program;
			TypeManagerRef typeManagerRef;
			int functionId = -1;
			{
				CompilerSuite compiler;
				//the code does not contain any global scope'code and only a variable
				//so does not need global memory
				compiler.initialize(8);
				rootScope = compiler.getGlobalScope();
				auto scriptCompiler = rootScope->getCompiler();

				const wchar_t* scriptCode =
					L"int square(int n) {"
					L"	return n * n;"
					L"}"
					;

				program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
				typeManagerRef = compiler.getTypeManager();
				functionId = scriptCompiler->findFunction("square", "int");
			}

			Assert::IsTrue(functionId >= 0, L"cannot find function 'square'");

			int n = 10;
			ScriptParamBuffer paramBuffer(n);
			
			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == n*n, L"program can run but return wrong value");
		}

		TEST_METHOD(ScriptHasBreakLines_1)
		{
			GlobalScopeRef rootScope;
			Program* program;
			TypeManagerRef typeManagerRef;
			int functionId = -1;
			{
				CompilerSuite compiler;
				//the code does not contain any global scope'code and only a variable
				//so does not need global memory
				compiler.initialize(8);
				rootScope = compiler.getGlobalScope();
				auto scriptCompiler = rootScope->getCompiler();

				const wchar_t* scriptCode =
					L"int square(int n) {\n"
					L"	return n * n;\n"
					L"}\n"
					;

				program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));

				Assert::IsNotNull(program, (L"compie program failed: " + convertToWstring(scriptCompiler->getLastError())).c_str());

				typeManagerRef = compiler.getTypeManager();
				functionId = scriptCompiler->findFunction("square", "int");
			}

			Assert::IsTrue(functionId >= 0, L"cannot find function 'square'");

			int n = 10;
			ScriptParamBuffer paramBuffer(n);

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == n*n, L"program can run but return wrong value");
		}

		TEST_METHOD(ScriptHasBreakLines_2)
		{
			GlobalScopeRef rootScope;
			Program* program;
			TypeManagerRef typeManagerRef;
			int functionId = -1;
			{
				CompilerSuite compiler;
				//the code does not contain any global scope'code and only a variable
				//so does not need global memory
				compiler.initialize(8);
				rootScope = compiler.getGlobalScope();
				auto scriptCompiler = rootScope->getCompiler();

				const wchar_t* scriptCode =
					L"int square(int\n n) {\n"
					L"	return n \n* n;\n"
					L"}\n"
					;

				program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));

				Assert::IsNotNull(program, (L"compie program failed: " + convertToWstring(scriptCompiler->getLastError())).c_str());

				typeManagerRef = compiler.getTypeManager();
				functionId = scriptCompiler->findFunction("square", "int");
			}

			Assert::IsTrue(functionId >= 0, L"cannot find function 'square'");

			int n = 10;
			ScriptParamBuffer paramBuffer(n);

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == n*n, L"program can run but return wrong value");
		}

		TEST_METHOD(ScriptHasBreakLines_3)
		{
			GlobalScopeRef rootScope;
			Program* program;
			TypeManagerRef typeManagerRef;
			int functionId = -1;
			{
				CompilerSuite compiler;
				//the code does not contain any global scope'code and only a variable
				//so does not need global memory
				compiler.initialize(8);
				rootScope = compiler.getGlobalScope();
				auto scriptCompiler = rootScope->getCompiler();

				const wchar_t* scriptCode =
					L"int square(int\n nn) {\n"
					L"	return n\nn * nn;\n"
					L"}\n"
					;

				program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));

				Assert::IsNull(program, L"compie program should fail");
			}
		}

		TEST_METHOD(ScriptHasBreakLines_4)
		{
			GlobalScopeRef rootScope;
			Program* program;
			TypeManagerRef typeManagerRef;
			int functionId = -1;
			{
				CompilerSuite compiler;
				//the code does not contain any global scope'code and only a variable
				//so does not need global memory
				compiler.initialize(8);
				rootScope = compiler.getGlobalScope();
				auto scriptCompiler = rootScope->getCompiler();

				const wchar_t* scriptCode =
					L"int square(i\nnt nn) {\n"
					L"	return nn * nn;\n"
					L"}\n"
					;

				program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));

				Assert::IsNull(program, L"compie program should fail");
			}
		}

		TEST_METHOD(ScriptHasBreakLines_5)
		{
			GlobalScopeRef rootScope;
			Program* program;
			TypeManagerRef typeManagerRef;
			int functionId = -1;
			{
				CompilerSuite compiler;
				//the code does not contain any global scope'code and only a variable
				//so does not need global memory
				compiler.initialize(8);
				rootScope = compiler.getGlobalScope();
				auto scriptCompiler = rootScope->getCompiler();

				const wchar_t* scriptCode =
					L"int squa\nre(int nn) {\n"
					L"	return nn * nn;\n"
					L"}\n"
					;

				program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));

				Assert::IsNull(program, L"compie program should fail");
			}
		}

		TEST_METHOD(ApplyPreprocessor1)
		{
			GlobalScopeRef rootScope;
			Program* program;
			TypeManagerRef typeManagerRef;
			int functionId = -1;
			{
				CompilerSuite compiler;
				//the code does not contain any global scope'code and only a variable
				//so does not need global memory
				compiler.initialize(8);
				rootScope = compiler.getGlobalScope();
				auto scriptCompiler = rootScope->getCompiler();

				const wchar_t* scriptCode =
					L"int square(int\n n) {\n"
					L"	return n \n* n;\n"
					L"}\n"
					;

				compiler.setPreprocessor(std::make_shared<DefaultPreprocessor>());
				program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));

				Assert::IsNotNull(program, (L"compie program failed: " + convertToWstring(scriptCompiler->getLastError())).c_str());

				typeManagerRef = compiler.getTypeManager();
				functionId = scriptCompiler->findFunction("square", "int");
			}

			Assert::IsTrue(functionId >= 0, L"cannot find function 'square'");

			int n = 10;
			ScriptParamBuffer paramBuffer(n);

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == n*n, L"program can run but return wrong value");
		}

		TEST_METHOD(ApplyPreprocessor2)
		{
			GlobalScopeRef rootScope;
			Program* program;
			TypeManagerRef typeManagerRef;
			int functionId = -1;
			{
				CompilerSuite compiler;
				//the code does not contain any global scope'code and only a variable
				//so does not need global memory
				compiler.initialize(8);
				rootScope = compiler.getGlobalScope();
				auto scriptCompiler = rootScope->getCompiler();

				const wchar_t* scriptCode =
					L"//comment in global scope\n"
					L"int square(int\n n) {\n"
					L"  //comment in function scope\n"
					L"	return n * n;// square expression\n"
					L"}\n"
					;

				compiler.setPreprocessor(std::make_shared<DefaultPreprocessor>());
				program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));

				Assert::IsNotNull(program, (L"compie program failed: " + convertToWstring(scriptCompiler->getLastError())).c_str());

				typeManagerRef = compiler.getTypeManager();
				functionId = scriptCompiler->findFunction("square", "int");
			}

			Assert::IsTrue(functionId >= 0, L"cannot find function 'square'");

			int n = 10;
			ScriptParamBuffer paramBuffer(n);

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, &paramBuffer);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == n*n, L"program can run but return wrong value");
		}

		TEST_METHOD(ApplyPreprocessor3)
		{
			GlobalScopeRef rootScope;
			Program* program;
			TypeManagerRef typeManagerRef;
			int functionId = -1;
			
			CompilerSuite compiler;
			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"//comment in global scope\n"
				L"int square(int\n"
				L" n) {\n"
				L"  //comment in function scope\n"
				L"	return n * n;// square expression\n"
				L"}\n"
				;

			compiler.setPreprocessor(std::make_shared<DefaultPreprocessor>());
			program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));

			Assert::IsNotNull(program, (L"compie program failed: " + convertToWstring(scriptCompiler->getLastError())).c_str());

			auto preprocessor = compiler.getPreprocessor();
			Assert::AreEqual(-1, preprocessor->getOriginalLine(-1), L"line original map failed");
			Assert::AreEqual(-1, preprocessor->getOriginalLine(-2), L"line original map failed");

			Assert::AreEqual(0, preprocessor->getOriginalLine(0), L"line original map failed");
			Assert::AreEqual(1, preprocessor->getOriginalLine(1), L"line original map failed");
			Assert::AreEqual(1, preprocessor->getOriginalLine(14), L"line original map failed");
			Assert::AreEqual(1, preprocessor->getOriginalLine(15), L"line original map failed");
			Assert::AreEqual(2, preprocessor->getOriginalLine(21), L"line original map failed");
			Assert::AreEqual(3, preprocessor->getOriginalLine(22), L"line original map failed");
			Assert::AreEqual(3, preprocessor->getOriginalLine(24), L"line original map failed");
			Assert::AreEqual(4, preprocessor->getOriginalLine(30), L"line original map failed");
		}
	};
}