#include "stdafx.h"
#include "CppUnitTest.h"
#include <CompilerSuite.h>
#include <ScriptTask.h>
#include <Utils.h>


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
	};
}