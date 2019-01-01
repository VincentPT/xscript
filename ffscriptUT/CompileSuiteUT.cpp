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
#include <gtest/gtest.h>

#include <CompilerSuite.h>
#include <ScriptTask.h>
#include <Utils.h>
#include <DefaultPreprocessor.h>
#include <RawStringLib.h>

#include "Utils.h"

using namespace std;
using namespace ffscript;

//CompileSuiteUT
TEST(CompileSuite, CompileSimpleProgram)
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
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'square'";

	int n = 10;
	ScriptParamBuffer paramBuffer(n);
	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, &paramBuffer);
	int* funcRes = (int*)scriptTask.getTaskResult();

	EXPECT_TRUE(*funcRes == n * n) << L"program can run but return wrong value";
}

TEST(CompileSuite, CompileSimpleProgram2)
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
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);
	int* funcRes = (int*)scriptTask.getTaskResult();

	EXPECT_TRUE(*funcRes == 3) << L"program can run but return wrong value";
}

TEST(CompileSuite, CompileSimpleProgram3)
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
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'main'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);
}

TEST(CompileSuite, CompileSimpleProgram4)
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
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'main'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);
}

TEST(CompileSuite, CompileSimpleProgram5)
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
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'main'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);
}

TEST(CompileSuite, CompileSimpleProgram6)
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
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'main'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);
}

TEST(CompileSuite, CompileSimpleProgram7)
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
	EXPECT_NE(nullptr, program) << L"compile program should success";
}

TEST(CompileSuite, CompileSimpleProgram8)
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
	EXPECT_EQ(nullptr, program) << L"compile program should be failed due to brackets are mismatch";

	EXPECT_TRUE(scriptCompiler->getLastError().length() > 0) << L"error message should not be empty";
}

TEST(CompileSuite, CompileSimpleProgram9)
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
	EXPECT_EQ(nullptr, program) << L"compile program should be failed due to brackets are mismatch";

	EXPECT_TRUE(scriptCompiler->getLastError().length() > 0) << L"error message should not be empty";
}

TEST(CompileSuite, ProgramIndependent1)
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

	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'square'";

	int n = 10;
	ScriptParamBuffer paramBuffer(n);
	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, &paramBuffer);
	int* funcRes = (int*)scriptTask.getTaskResult();

	EXPECT_TRUE(*funcRes == n * n) << L"program can run but return wrong value";
}

TEST(CompileSuite, ProgramAndTypeIndependent1)
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

	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'square'";

	int n = 10;
	ScriptParamBuffer paramBuffer(n);

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, &paramBuffer);
	int* funcRes = (int*)scriptTask.getTaskResult();

	EXPECT_TRUE(*funcRes == n * n) << L"program can run but return wrong value";
}

TEST(CompileSuite, ProgramAndTypeIndependent2)
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

	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'square'";

	int n = 10;
	ScriptParamBuffer paramBuffer(n);

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, &paramBuffer);
	int* funcRes = (int*)scriptTask.getTaskResult();

	EXPECT_TRUE(*funcRes == n * n) << L"program can run but return wrong value";
}

TEST(CompileSuite, ScriptHasBreakLines_1)
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

		EXPECT_NE(nullptr, program) << (L"compie program failed: " + convertToWstring(scriptCompiler->getLastError())).c_str();

		typeManagerRef = compiler.getTypeManager();
		functionId = scriptCompiler->findFunction("square", "int");
	}

	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'square'";

	int n = 10;
	ScriptParamBuffer paramBuffer(n);

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, &paramBuffer);
	int* funcRes = (int*)scriptTask.getTaskResult();

	EXPECT_TRUE(*funcRes == n * n) << L"program can run but return wrong value";
}

TEST(CompileSuite, ScriptHasBreakLines_2)
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

		EXPECT_NE(nullptr, program) << (L"compie program failed: " + convertToWstring(scriptCompiler->getLastError())).c_str();

		typeManagerRef = compiler.getTypeManager();
		functionId = scriptCompiler->findFunction("square", "int");
	}

	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'square'";

	int n = 10;
	ScriptParamBuffer paramBuffer(n);

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, &paramBuffer);
	int* funcRes = (int*)scriptTask.getTaskResult();

	EXPECT_TRUE(*funcRes == n * n) << L"program can run but return wrong value";
}

TEST(CompileSuite, ScriptHasBreakLines_3)
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

		EXPECT_EQ(nullptr, program) << L"compie program should fail";
	}
}

TEST(CompileSuite, ScriptHasBreakLines_4)
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

		EXPECT_EQ(nullptr, program) << L"compie program should fail";
	}
}

TEST(CompileSuite, ScriptHasBreakLines_5)
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

		EXPECT_EQ(nullptr, program) << L"compie program should fail";
	}
}

TEST(CompileSuite, ApplyPreprocessor1)
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

		EXPECT_NE(nullptr, program) << (L"compie program failed: " + convertToWstring(scriptCompiler->getLastError())).c_str();

		typeManagerRef = compiler.getTypeManager();
		functionId = scriptCompiler->findFunction("square", "int");
	}

	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'square'";

	int n = 10;
	ScriptParamBuffer paramBuffer(n);

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, &paramBuffer);
	int* funcRes = (int*)scriptTask.getTaskResult();

	EXPECT_TRUE(*funcRes == n * n) << L"program can run but return wrong value";
}

TEST(CompileSuite, ApplyPreprocessor2)
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

		EXPECT_NE(nullptr, program) << (L"compie program failed: " + convertToWstring(scriptCompiler->getLastError())).c_str();

		typeManagerRef = compiler.getTypeManager();
		functionId = scriptCompiler->findFunction("square", "int");
	}

	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'square'";

	int n = 10;
	ScriptParamBuffer paramBuffer(n);

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, &paramBuffer);
	int* funcRes = (int*)scriptTask.getTaskResult();

	EXPECT_TRUE(*funcRes == n * n) << L"program can run but return wrong value";
}

static int getOriginalLinePosition(Preprocessor* preprocessor, int idx) {
	int line, column;
	preprocessor->getOriginalPosition(idx, line, column);
	return line;
}

TEST(CompileSuite, ApplyPreprocessor3)
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

	EXPECT_NE(nullptr, program) << (L"compie program failed: " + convertToWstring(scriptCompiler->getLastError())).c_str();

	auto preprocessor = compiler.getPreprocessor().get();

	EXPECT_EQ(-1, getOriginalLinePosition(preprocessor, -1)) << L"line original map failed";
	EXPECT_EQ(-1, getOriginalLinePosition(preprocessor, -2)) << L"line original map failed";

	EXPECT_EQ(0, getOriginalLinePosition(preprocessor, 0)) << L"line original map failed";
	EXPECT_EQ(1, getOriginalLinePosition(preprocessor, 1)) << L"line original map failed";
	EXPECT_EQ(1, getOriginalLinePosition(preprocessor, 14)) << L"line original map failed";
	EXPECT_EQ(1, getOriginalLinePosition(preprocessor, 15)) << L"line original map failed";
	EXPECT_EQ(2, getOriginalLinePosition(preprocessor, 21)) << L"line original map failed";
	EXPECT_EQ(3, getOriginalLinePosition(preprocessor, 22)) << L"line original map failed";
	EXPECT_EQ(3, getOriginalLinePosition(preprocessor, 24)) << L"line original map failed";
	EXPECT_EQ(4, getOriginalLinePosition(preprocessor, 30)) << L"line original map failed";
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

	EXPECT_NE(nullptr, program.get()) << (L"compie program failed: " + convertToWstring(scriptCompiler->getLastError())).c_str();

	functionId = scriptCompiler->findFunction(functionName, "");
	EXPECT_TRUE(functionId >= 0) << (L"cannot find function '" + convertToWstring(functionName) + L"'").c_str();

	return program;
}

TEST(CompileSuite, CreateSimpleString1)
{
	GlobalScopeRef rootScope;
	int functionId;
	char* functionName = "createSampleString";
	auto program = loadProgram(rootScope, "CreateSimpleString1.c955", functionName, "", functionId);
	auto scriptCompiler = rootScope->getCompiler();

	ScriptTask scriptTask(program.get());
	scriptTask.runFunction(functionId, nullptr);

	RawString* rws = (RawString*)scriptTask.getTaskResult();

	int cmpRes = memcmp(rws->elms, L"Simple String", (rws->size + 1) * sizeof(RawChar));
	EXPECT_EQ(0, cmpRes);

	freeRawString(*rws);
}

TEST(CompileSuite, AddString1)
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
	EXPECT_EQ(0, cmpRes);

	freeRawString(*rws);
}

TEST(CompileSuite, ConvertToString1)
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
	EXPECT_EQ(0, cmpRes);

	freeRawString(*rws);
}

TEST(CompileSuite, ConvertToString2)
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
	EXPECT_EQ(0, cmpRes);

	freeRawString(*rws);
}

TEST(CompileSuite, ConvertToString3)
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
	EXPECT_EQ(0, cmpRes);

	freeRawString(*rws);
}

TEST(CompileSuite, ConvertToString4)
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
	EXPECT_EQ(0, cmpRes);

	freeRawString(*rws);
}

TEST(CompileSuite, ConvertToString5)
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
	EXPECT_EQ(0, cmpRes);

	freeRawString(*rws);
}

TEST(CompileSuite, TestHexNumber1)
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
	EXPECT_NE(nullptr, program);
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);
	int* funcRes = (int*)scriptTask.getTaskResult();

	EXPECT_TRUE(*funcRes == 0x1234) << L"program can run but return wrong value";
}

TEST(CompileSuite, TestHexNumber2)
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
	EXPECT_NE(nullptr, program);
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);
	int* funcRes = (int*)scriptTask.getTaskResult();

	EXPECT_TRUE(*funcRes == 0x1234) << L"program can run but return wrong value";
}

TEST(CompileSuite, TestHexNumber3)
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
	EXPECT_NE(nullptr, program);
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);
	int* funcRes = (int*)scriptTask.getTaskResult();

	EXPECT_TRUE(*funcRes == (0x1233 + 1)) << L"program can run but return wrong value";
}

TEST(CompileSuite, TestHexNumber4)
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
	EXPECT_NE(nullptr, program);
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);
	int* funcRes = (int*)scriptTask.getTaskResult();

	EXPECT_TRUE(*funcRes == (0x1233 + 1)) << L"program can run but return wrong value";
}

TEST(CompileSuite, TestHexNumber5)
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
	EXPECT_NE(nullptr, program);
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);
	long long* funcRes = (long long*)scriptTask.getTaskResult();

	EXPECT_TRUE(*funcRes == (0x1233 + 0x123456789)) << L"program can run but return wrong value";
}

TEST(CompileSuite, TestHexNumber6)
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
	EXPECT_NE(nullptr, program);
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);
	int* funcRes = (int*)scriptTask.getTaskResult();

	EXPECT_TRUE(*funcRes == -0x1234) << L"program can run but return wrong value";
}

TEST(CompileSuite, TestHexNumber7)
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
	EXPECT_NE(nullptr, program);
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);
	long long* funcRes = (long long*)scriptTask.getTaskResult();

	EXPECT_TRUE(*funcRes == -(0x1234ll)) << L"program can run but return wrong value";
}

TEST(CompileSuite, TestHexNumber8)
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
	EXPECT_NE(nullptr, program);
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);
	long long* funcRes = (long long*)scriptTask.getTaskResult();

	EXPECT_TRUE(*funcRes == -(0x123456789)) << L"program can run but return wrong value";
}

TEST(CompileSuite, TestTypeCastOperator1)
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
	EXPECT_NE(nullptr, program);
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);

	auto res = *(int**)scriptTask.getTaskResult();
	EXPECT_EQ((size_t)1, (size_t)res);
}

TEST(CompileSuite, TestTypeCastOperator2)
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
	EXPECT_NE(nullptr, program);
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);

	auto res = *(int**)scriptTask.getTaskResult();
	EXPECT_EQ((size_t)1, (size_t)res);
}

TEST(CompileSuite, TestTypeCastOperator3)
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
	EXPECT_NE(nullptr, program);
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);

	auto res = *(int*)scriptTask.getTaskResult();
	EXPECT_EQ(1, res);
}

TEST(CompileSuite, TestTypeCastOperator4)
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
	EXPECT_NE(nullptr, program);
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);

	auto res = *(int*)scriptTask.getTaskResult();
	EXPECT_EQ(0x12345678, res);
}

TEST(CompileSuite, TestTypeCastOperator5)
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
	EXPECT_NE(nullptr, program);
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);

	auto res = *(unsigned long long*)scriptTask.getTaskResult();
	EXPECT_EQ(0x12345678ull, res);
}

TEST(CompileSuite, TestTypeCastOperator6)
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
	EXPECT_NE(nullptr, program);
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);

	auto res = *(unsigned long long*)scriptTask.getTaskResult();
	if (sizeof(void*) == 8) {
		EXPECT_EQ(0x123456789ull, res);
	}
	else {
		EXPECT_EQ(0x23456789ull, res);
	}
}

TEST(CompileSuite, TestTypeCastOperator7)
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
	EXPECT_NE(nullptr, program);
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);

	auto res = *(unsigned long long*)scriptTask.getTaskResult();
	EXPECT_EQ(0x12345678ull, res);
}

TEST(CompileSuite, TestSemiRef01)
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
		L"	val = 0x12345679;"
		L"	return rval;"
		L"}"
		;
	Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
	EXPECT_NE(nullptr, program) << convertToWstring(scriptCompiler->getLastError()).c_str();
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);

	auto res = *(unsigned long long*)scriptTask.getTaskResult();
	EXPECT_EQ(0x12345679ull, res);
}

TEST(CompileSuite, TestSemiRef02)
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
		L"	rval = 0x12345679;"
		L"	return val;"
		L"}"
		;
	Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
	EXPECT_NE(nullptr, program);
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);

	auto res = *(unsigned long long*)scriptTask.getTaskResult();
	EXPECT_EQ(0x12345679ull, res);
}

TEST(CompileSuite, TestSemiRef03)
{
	CompilerSuite compiler;

	//the code does not contain any global scope'code and only a variable
	//so does not need global memory
	compiler.initialize(8);
	GlobalScopeRef rootScope = compiler.getGlobalScope();
	auto scriptCompiler = rootScope->getCompiler();

	const wchar_t* scriptCode =
		L"long foo1(long val) {"
		L"	return val;"
		L"}"
		L"long foo() {"
		L"	int val = 0x12345678;"
		L"	int& rval = val;"
		L"	return foo1(rval);"
		L"}"
		;
	Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
	EXPECT_NE(nullptr, program) << convertToWstring(scriptCompiler->getLastError()).c_str();
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'foo'";

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);

	auto res = *(unsigned long long*)scriptTask.getTaskResult();
	EXPECT_EQ(0x12345678ull, res);
}
#if 0
TEST(CompileSuite, TestSemiRef04)
{
	CompilerSuite compiler;

	//the code does not contain any global scope'code and only a variable
	//so does not need global memory
	compiler.initialize(8);
	GlobalScopeRef rootScope = compiler.getGlobalScope();
	auto scriptCompiler = rootScope->getCompiler();

	const wchar_t* scriptCode =
		L"long foo1(long& val) {"
		L"	return val;"
		L"}"
		L"long foo() {"
		L"	int val = 0x12345678;"
		L"	return foo1(val);"
		L"}"
		;
	Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
	EXPECT_NE(nullptr, program, convertToWstring(scriptCompiler->getLastError()).c_str());
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);

	auto res = *(unsigned long long*)scriptTask.getTaskResult();
	EXPECT_EQ(0x12345678ull, res);
}

TEST(CompileSuite, TestSemiRef05)
{
	CompilerSuite compiler;

	//the code does not contain any global scope'code and only a variable
	//so does not need global memory
	compiler.initialize(8);
	GlobalScopeRef rootScope = compiler.getGlobalScope();
	auto scriptCompiler = rootScope->getCompiler();

	const wchar_t* scriptCode =
		L"long foo1(long& val) {"
		L"	return val;"
		L"}"
		L"long foo() {"
		L"	int val = 0x12345678;"
		L"	int& rval = val;"
		L"	return foo1(rval);"
		L"}"
		;
	Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
	EXPECT_NE(nullptr, program, convertToWstring(scriptCompiler->getLastError()).c_str());
	int functionId = scriptCompiler->findFunction("foo", "");
	EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

	ScriptTask scriptTask(program);
	scriptTask.runFunction(functionId, nullptr);

	auto res = *(unsigned long long*)scriptTask.getTaskResult();
	EXPECT_EQ(0x12345678ull, res);
}

static RawString customTypeToString(int val) {
	RawString rws;
	constantConstructor(rws, std::to_wstring(val));

	return rws;
}

TEST(CompileSuite, TestSemiRef06)
{
	CompilerSuite compiler;

	//the code does not contain any global scope'code and only a variable
	//so does not need global memory
	compiler.initialize(8);
	GlobalScopeRef rootScope = compiler.getGlobalScope();
	auto scriptCompiler = rootScope->getCompiler();
	includeRawStringToCompiler(scriptCompiler);

	FunctionRegisterHelper fb(scriptCompiler);
	int typeId = fb.registerUserType("CustomType", sizeof(int));
	fb.registFunction("String", "CustomType", createUserFunctionFactory(scriptCompiler, "String", customTypeToString));

	scriptCompiler->beginUserLib();

	const wchar_t* scriptCode =
		L"void foo(CustomType val) {"
		L"	String s = \"val =\";"
		L"	s + val;"
		L"}"
		;
	Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
	EXPECT_NE(nullptr, program, convertToWstring(scriptCompiler->getLastError()).c_str());
	int functionId = scriptCompiler->findFunction("foo", "CustomType");
	EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

	ScriptTask scriptTask(program);
	ScriptParamBuffer paramBuffer(3);
	scriptTask.runFunction(functionId, paramBuffer);
}

TEST(CompileSuite, TestSemiRef07)
{
	CompilerSuite compiler;

	//the code does not contain any global scope'code and only a variable
	//so does not need global memory
	compiler.initialize(8);
	GlobalScopeRef rootScope = compiler.getGlobalScope();
	auto scriptCompiler = rootScope->getCompiler();
	includeRawStringToCompiler(scriptCompiler);

	FunctionRegisterHelper fb(scriptCompiler);
	int typeId = fb.registerUserType("CustomType", sizeof(int));
	fb.registFunction("String", "CustomType", createUserFunctionFactory(scriptCompiler, "String", customTypeToString));

	scriptCompiler->beginUserLib();

	const wchar_t* scriptCode =
		L"void foo(CustomType val) {"
		L"	\"val =\" + val;"
		L"}"
		;
	Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
	EXPECT_NE(nullptr, program, convertToWstring(scriptCompiler->getLastError()).c_str());
	int functionId = scriptCompiler->findFunction("foo", "CustomType");
	EXPECT_TRUE(functionId >= 0, L"cannot find function 'foo'");

	ScriptTask scriptTask(program);
	ScriptParamBuffer paramBuffer(3);
	scriptTask.runFunction(functionId, paramBuffer);
}
#endif