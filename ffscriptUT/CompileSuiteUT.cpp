/******************************************************************
* File:        CompileProgramUT.cpp
* Description: Test cases for compiling and running C Lambda scripting
*              language using CompilerSuite.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
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

#include "Utils.h"

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

		TEST_METHOD(CompileSimpleProgram7)
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
				L"	int i = 0;"
				L"	if(true) {;"
				L"	    i = 1;"
				L"	}"
				L"	else {"
				L"	    i = 2;"
				L"	}"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNotNull(program, L"compile program should success");
		}

		TEST_METHOD(CompileSimpleProgram8)
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
				L"	int i = 0;"
				L"	if(true) {"
				L"	    i = 1;"
				L"	}"
				L"	else {"
				L"	    i = 2;"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNull(program, L"compile program should be failed due to brackets are mismatch");

			Assert::IsTrue(scriptCompiler->getLastError().length() > 0, L"error message should not be empty");
		}

		TEST_METHOD(CompileSimpleProgram9)
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
				L"	int i = 0;"
				L"	if(true) {;"
				L"	    i = 1;"
				L"	}"
				L"	else {"
				L"	    i = 2;"
				L"}"
				L"void foo() {"
				L"}"
				;

			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNull(program, L"compile program should be failed due to brackets are mismatch");

			Assert::IsTrue(scriptCompiler->getLastError().length() > 0, L"error message should not be empty");
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

		static int getOriginalLinePosition(Preprocessor* preprocessor, int idx) {
			int line, column;
			preprocessor->getOriginalPosition(idx, line, column);
			return line;
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

			auto preprocessor = compiler.getPreprocessor().get();
			Assert::AreEqual(-1,getOriginalLinePosition(preprocessor,-1), L"line original map failed");
			Assert::AreEqual(-1,getOriginalLinePosition(preprocessor,-2), L"line original map failed");

			Assert::AreEqual(0,getOriginalLinePosition(preprocessor,0), L"line original map failed");
			Assert::AreEqual(1,getOriginalLinePosition(preprocessor,1), L"line original map failed");
			Assert::AreEqual(1,getOriginalLinePosition(preprocessor,14), L"line original map failed");
			Assert::AreEqual(1,getOriginalLinePosition(preprocessor,15), L"line original map failed");
			Assert::AreEqual(2,getOriginalLinePosition(preprocessor,21), L"line original map failed");
			Assert::AreEqual(3,getOriginalLinePosition(preprocessor,22), L"line original map failed");
			Assert::AreEqual(3,getOriginalLinePosition(preprocessor,24), L"line original map failed");
			Assert::AreEqual(4,getOriginalLinePosition(preprocessor,30), L"line original map failed");
		}

		shared_ptr<Program> loadProgram(GlobalScopeRef& rootScope, const char* file,
			const char* functionName, const char* params, int& functionId) {
			shared_ptr<Program> program;
			
			CompilerSuite compiler;
			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();
			// include String library and ...
			includeRawStringToCompiler(scriptCompiler);
			// ... make it as system lib
			scriptCompiler->beginUserLib();

			auto script = readCodeFromUtf8File(file);

			compiler.setPreprocessor(std::make_shared<DefaultPreprocessor>());
			program = shared_ptr<Program>(compiler.compileProgram(script.c_str(), script.c_str() + script.size()));

			Assert::IsNotNull(program.get(), (L"compie program failed: " + convertToWstring(scriptCompiler->getLastError())).c_str());

			functionId = scriptCompiler->findFunction(functionName, "");
			Assert::IsTrue(functionId >= 0, (L"cannot find function '" + convertToWstring(functionName) + L"'").c_str());

			return program; 
		}

		TEST_METHOD(CreateSimpleString1)
		{
			GlobalScopeRef rootScope;
			int functionId;
			char* functionName = "createSampleString";
			auto program = loadProgram(rootScope, "CreateSimpleString1.c955", functionName, "", functionId);
			auto scriptCompiler = rootScope->getCompiler();

			ScriptTask scriptTask(program.get());
			scriptTask.runFunction(functionId, nullptr);

			RawString* rws = (RawString*)scriptTask.getTaskResult();

			int cmpRes = memcmp(rws->elms, L"Simple String", (rws->size + 1)* sizeof(RawChar));
			Assert::AreEqual(0, cmpRes);

			freeRawString(*rws);
		}

		TEST_METHOD(AddString1)
		{
			GlobalScopeRef rootScope;
			int functionId;
			char* functionName = "addString1";
			auto program = loadProgram(rootScope, "StringFunctions.c955", functionName, "", functionId);
			auto scriptCompiler = rootScope->getCompiler();

			ScriptTask scriptTask(program.get());
			scriptTask.runFunction(functionId, nullptr);

			RawString* rws = (RawString*)scriptTask.getTaskResult();

			int cmpRes = memcmp(rws->elms, L"this is a simple string", (rws->size + 1) * sizeof(RawChar));
			Assert::AreEqual(0, cmpRes);

			freeRawString(*rws);
		}

		TEST_METHOD(ConvertToString1)
		{
			GlobalScopeRef rootScope;
			int functionId;
			char* functionName = "convertToString1";
			auto program = loadProgram(rootScope, "StringFunctions.c955", functionName, "", functionId);
			auto scriptCompiler = rootScope->getCompiler();

			ScriptTask scriptTask(program.get());
			scriptTask.runFunction(functionId, nullptr);

			RawString* rws = (RawString*)scriptTask.getTaskResult();

			int cmpRes = memcmp(rws->elms, L"bool value:false", (rws->size + 1) * sizeof(RawChar));
			Assert::AreEqual(0, cmpRes);

			freeRawString(*rws);
		}

		TEST_METHOD(ConvertToString2)
		{
			GlobalScopeRef rootScope;
			int functionId;
			char* functionName = "convertToString2";
			auto program = loadProgram(rootScope, "StringFunctions.c955", functionName, "", functionId);
			auto scriptCompiler = rootScope->getCompiler();

			ScriptTask scriptTask(program.get());
			scriptTask.runFunction(functionId, nullptr);

			RawString* rws = (RawString*)scriptTask.getTaskResult();

			int cmpRes = memcmp(rws->elms, L"integer value:123", (rws->size + 1) * sizeof(RawChar));
			Assert::AreEqual(0, cmpRes);

			freeRawString(*rws);
		}

		TEST_METHOD(ConvertToString3)
		{
			GlobalScopeRef rootScope;
			int functionId;
			char* functionName = "convertToString3";
			auto program = loadProgram(rootScope, "StringFunctions.c955", functionName, "", functionId);
			auto scriptCompiler = rootScope->getCompiler();

			ScriptTask scriptTask(program.get());
			scriptTask.runFunction(functionId, nullptr);

			RawString* rws = (RawString*)scriptTask.getTaskResult();

			int cmpRes = wcsncmp(rws->elms, L"float value:1.0", 15);
			Assert::AreEqual(0, cmpRes);

			freeRawString(*rws);
		}

		TEST_METHOD(ConvertToString4)
		{
			GlobalScopeRef rootScope;
			int functionId;
			char* functionName = "convertToString4";
			auto program = loadProgram(rootScope, "StringFunctions.c955", functionName, "", functionId);
			auto scriptCompiler = rootScope->getCompiler();

			ScriptTask scriptTask(program.get());
			scriptTask.runFunction(functionId, nullptr);

			RawString* rws = (RawString*)scriptTask.getTaskResult();

			int cmpRes = wcscmp(rws->elms, L"long value:1234567890987654321");
			Assert::AreEqual(0, cmpRes);

			freeRawString(*rws);
		}

		TEST_METHOD(ConvertToString5)
		{
			GlobalScopeRef rootScope;
			int functionId;
			char* functionName = "convertToString5";
			auto program = loadProgram(rootScope, "StringFunctions.c955", functionName, "", functionId);
			auto scriptCompiler = rootScope->getCompiler();

			ScriptTask scriptTask(program.get());
			scriptTask.runFunction(functionId, nullptr);

			RawString* rws = (RawString*)scriptTask.getTaskResult();

			int cmpRes = wcscmp(rws->elms, L"the temperature is 28.000000 degree");
			Assert::AreEqual(0, cmpRes);

			freeRawString(*rws);
		}

		TEST_METHOD(TestHexNumber1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	return 0x1234;"
				L"}"
				;
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNotNull(program);
			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == 0x1234, L"program can run but return wrong value");
		}

		TEST_METHOD(TestHexNumber2)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	x = 0x1234;"
				L"	return x;"
				L"}"
				;
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNotNull(program);
			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == 0x1234, L"program can run but return wrong value");
		}

		TEST_METHOD(TestHexNumber3)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	return 0x1233 + 1;"
				L"}"
				;
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNotNull(program);
			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == (0x1233 + 1), L"program can run but return wrong value");
		}

		TEST_METHOD(TestHexNumber4)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int sum(int a, int b) {"
				L"	return a + b;"
				L"}"
				L"int foo() {"
				L"	return sum(0x1233, 1);"
				L"}"
				;
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNotNull(program);
			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == (0x1233 + 1), L"program can run but return wrong value");
		}

		TEST_METHOD(TestHexNumber5)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"long sum(int a, long b) {"
				L"	return a + b;"
				L"}"
				L"long foo() {"
				L"	return sum(0x1233, 0x123456789);"
				L"}"
				;
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNotNull(program);
			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			long long* funcRes = (long long*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == (0x1233 + 0x123456789), L"program can run but return wrong value");
		}

		TEST_METHOD(TestHexNumber6)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	return -0x1234;"
				L"}"
				;
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNotNull(program);
			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == -0x1234, L"program can run but return wrong value");
		}

		TEST_METHOD(TestHexNumber7)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"long foo() {"
				L"	return -0x1234l;"
				L"}"
				;
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNotNull(program);
			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			long long* funcRes = (long long*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == -(0x1234ll), L"program can run but return wrong value");
		}

		TEST_METHOD(TestHexNumber8)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"long foo() {"
				L"	return -0x123456789;"
				L"}"
				;
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNotNull(program);
			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			long long* funcRes = (long long*)scriptTask.getTaskResult();

			Assert::IsTrue(*funcRes == -(0x123456789), L"program can run but return wrong value");
		}

		TEST_METHOD(TestTypeCastOperator1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"ref int foo() {"
				L"	long val = 1;"
				L"	ref void p = (ref void)val;"
				L"	ref int pI = (ref int)p;"
				L"	return pI;"
				L"}"
				;
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNotNull(program);
			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			auto res = *(int**) scriptTask.getTaskResult();
			Assert::AreEqual((size_t)1, (size_t)res);
		}

		TEST_METHOD(TestTypeCastOperator2)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"ref int foo() {"
				L"	int val = 1;"
				L"	ref void p = (ref void)val;"
				L"	ref int pI = (ref int)p;"
				L"	return pI;"
				L"}"
				;
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNotNull(program);
			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			auto res = *(int**)scriptTask.getTaskResult();
			Assert::AreEqual((size_t)1, (size_t)res);
		}

		TEST_METHOD(TestTypeCastOperator3)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	long val = 1;"
				L"	return (int)val;"
				L"}"
				;
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNotNull(program);
			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			auto res = *(int*)scriptTask.getTaskResult();
			Assert::AreEqual(1, res);
		}

		TEST_METHOD(TestTypeCastOperator4)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	ref long val = (ref long) 0x12345678;"
				L"	return (int)val;"
				L"}"
				;
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNotNull(program);
			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			auto res = *(int*)scriptTask.getTaskResult();
			Assert::AreEqual(0x12345678, res);
		}

		TEST_METHOD(TestTypeCastOperator5)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"long foo() {"
				L"	ref long val = (ref long) 0x12345678;"
				L"	return (long)val;"
				L"}"
				;
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNotNull(program);
			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			auto res = *(unsigned long long*)scriptTask.getTaskResult();
			Assert::AreEqual(0x12345678ull, res);
		}

		TEST_METHOD(TestTypeCastOperator6)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"long foo() {"
				L"	ref long val = (ref long) 0x123456789;"
				L"	return (long)val;"
				L"}"
				;
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNotNull(program);
			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			auto res = *(unsigned long long*)scriptTask.getTaskResult();
			if (sizeof(void*) == 8) {
				Assert::AreEqual(0x123456789ull, res);
			}
			else {
				Assert::AreEqual(0x23456789ull, res);
			}
		}

		TEST_METHOD(TestTypeCastOperator7)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"long foo() {"
				L"	int val = 0x12345678;"
				L"	int& rval = val;"
				L"	ref long lval = (ref long) rval;"
				L"	return (long)lval;"
				L"}"
				;
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			Assert::IsNotNull(program);
			int functionId = scriptCompiler->findFunction("foo", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'foo'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);

			auto res = *(unsigned long long*)scriptTask.getTaskResult();
			Assert::AreEqual(0x12345678ull, res);
		}
	};
}