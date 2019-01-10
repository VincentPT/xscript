/******************************************************************
* File:        FunctionPointerUT.cpp
* Description: Test cases for checking function pointer in C Lambda
*              scripting language.
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

using namespace std;
using namespace ffscript;

namespace ffscriptUT
{		
	namespace FunctionPointerUT
	{
		FF_TEST_FUNCTION(FunctionPointer, ParseNormalTypeUT1)
		{
			CompilerSuite compiler;
			compiler.initialize(8);

			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode = L"int";
			const wchar_t* end = scriptCode + wcslen(scriptCode);
			ScriptType stype;
			auto c = scriptCompiler->readType(scriptCode, end, stype);

			FF_EXPECT_NE(nullptr, c, L"parse type should be success");
			FF_EXPECT_STREQ("int", stype.sType().c_str(), L"Parse type is incorrect");
		}

		FF_TEST_FUNCTION(FunctionPointer, ParseNormalTypeUT2)
		{
			CompilerSuite compiler;
			compiler.initialize(8);

			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode = L" int";
			const wchar_t* end = scriptCode + wcslen(scriptCode);
			ScriptType stype;
			auto c = scriptCompiler->readType(scriptCode, end, stype);

			FF_EXPECT_NE(nullptr, c, L"parse type should be success");
			FF_EXPECT_STREQ("int", stype.sType().c_str(), L"Parse type is incorrect");
		}

		FF_TEST_FUNCTION(FunctionPointer, ParseRefTypeUT1)
		{
			CompilerSuite compiler;
			compiler.initialize(8);

			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode = L"ref float ";
			const wchar_t* end = scriptCode + wcslen(scriptCode);
			ScriptType stype;
			auto c = scriptCompiler->readType(scriptCode, end, stype);

			FF_EXPECT_NE(nullptr, c, L"parse type should be success");
			FF_EXPECT_STREQ("ref float", stype.sType().c_str(), L"Parse type is incorrect");
		}

		FF_TEST_FUNCTION(FunctionPointer, ParseFunctionTypeUT1)
		{
			CompilerSuite compiler;
			compiler.initialize(8);

			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode = L"function<double()>";
			const wchar_t* end = scriptCode + wcslen(scriptCode);
			ScriptType stype;
			auto c = scriptCompiler->readType(scriptCode, end, stype);

			FF_EXPECT_NE(nullptr, c, L"parse type should be success");
			FF_EXPECT_STREQ("function<double()>", stype.sType().c_str(), L"Parse type is incorrect");
		}

		FF_TEST_FUNCTION(FunctionPointer, ParseFunctionTypeUT2)
		{
			CompilerSuite compiler;
			compiler.initialize(8);

			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode = L"function < double ( ) >";
			const wchar_t* end = scriptCode + wcslen(scriptCode);
			ScriptType stype;
			auto c = scriptCompiler->readType(scriptCode, end, stype);

			FF_EXPECT_NE(nullptr, c, L"parse type should be success");
			FF_EXPECT_STREQ("function<double()>", stype.sType().c_str(), L"Parse type is incorrect");
		}

		FF_TEST_FUNCTION(FunctionPointer, ParseFunctionTypeUT3)
		{
			CompilerSuite compiler;
			compiler.initialize(8);

			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode = L"function < ref double ( int ) >";
			const wchar_t* end = scriptCode + wcslen(scriptCode);
			ScriptType stype;
			auto c = scriptCompiler->readType(scriptCode, end, stype);

			FF_EXPECT_NE(nullptr, c, L"parse type should be success");
			FF_EXPECT_STREQ("function<ref double(int)>", stype.sType().c_str(), L"Parse type is incorrect");
		}

		FF_TEST_FUNCTION(FunctionPointer, ParseFunctionTypeUT4)
		{
			CompilerSuite compiler;
			compiler.initialize(8);

			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode = L"function < ref double ( int, ref float ) >";
			const wchar_t* end = scriptCode + wcslen(scriptCode);
			ScriptType stype;
			auto c = scriptCompiler->readType(scriptCode, end, stype);

			FF_EXPECT_NE(nullptr, c, L"parse type should be success");
			FF_EXPECT_STREQ("function<ref double(int,ref float)>", stype.sType().c_str(), L"Parse type is incorrect");
		}

		FF_TEST_FUNCTION(FunctionPointer, ParseFunctionTypeUT5)
		{
			CompilerSuite compiler;
			compiler.initialize(8);

			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode = L"function<function< ref int(int a)>( int a)>";
			const wchar_t* end = scriptCode + wcslen(scriptCode);
			ScriptType stype;
			auto c = scriptCompiler->readType(scriptCode, end, stype);

			FF_EXPECT_NE(nullptr, c, L"parse type should be success");
			FF_EXPECT_STREQ("function<function<ref int(int)>(int)>", stype.sType().c_str(), L"Parse type is incorrect");
		}

		FF_TEST_FUNCTION(FunctionPointer, ParseFunctionTypeUT6)
		{
			CompilerSuite compiler;
			compiler.initialize(8);

			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode = L"function<int(function< ref int(int a)>, int a)>";
			const wchar_t* end = scriptCode + wcslen(scriptCode);
			ScriptType stype;
			auto c = scriptCompiler->readType(scriptCode, end, stype);

			FF_EXPECT_NE(nullptr, c, L"parse type should be success");
			FF_EXPECT_STREQ("function<int(function<ref int(int)>,int)>", stype.sType().c_str(), L"Parse type is incorrect");
		}

		FF_TEST_FUNCTION(FunctionPointer, ParseFunctionTypeUT7)
		{
			CompilerSuite compiler;
			compiler.initialize(8);

			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode = L"function<int(ref float)>";
			const wchar_t* end = scriptCode + wcslen(scriptCode);
			ScriptType stype;
			auto c = scriptCompiler->readType(scriptCode, end, stype);

			FF_EXPECT_NE(nullptr, c, L"parse type should be success");
			FF_EXPECT_STREQ("function<int(ref float)>", stype.sType().c_str(), L"Parse type is incorrect");
		}

		static int test() {
			return 0;
		}

		static int test(int a) {
			return a + 1;
		}

		FF_TEST_FUNCTION(FunctionPointer, NativeUT_NoParam1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();
			FunctionRegisterHelper funcLibHelper(scriptCompiler);
			funcLibHelper.registFunction("test", "", new BasicFunctionFactory<4>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", new FunctionDelegate2<int>(test), scriptCompiler), true);

			const wchar_t* scriptCode =
				L"int foo() {"
				L"	f = test;"
				L"	return f();"
				L"}"
				;
			
			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'square'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 0, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(FunctionPointer, NativeUT_NoParam2)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();
			FunctionRegisterHelper funcLibHelper(scriptCompiler);
			funcLibHelper.registFunction("test", "", new BasicFunctionFactory<4>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", new FunctionDelegate2<int>(test), scriptCompiler), true);

			const wchar_t* scriptCode =
				L"function<int()> getFn() {"
				L"	f = test;"
				L"	return f;"
				L"}"
				L"int foo() {"
				L"	f = getFn();"
				L"	return f();"
				L"}"
				;

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'square'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 0, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(FunctionPointer, NativeUT_OneParam1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();
			FunctionRegisterHelper funcLibHelper(scriptCompiler);
			funcLibHelper.registFunction("test", "int", new BasicFunctionFactory<4>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", new FunctionDelegate2<int, int>(test), scriptCompiler), true);

			const wchar_t* scriptCode =
				L"function<int(int)> getFn() {"				
				L"	return test;"
				L"}"
				L"int foo() {"
				L"	f = getFn();"
				L"	return f(1);"
				L"}"
				;

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'square'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 2, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(FunctionPointer, NativeUT_OneParam2)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();
			FunctionRegisterHelper funcLibHelper(scriptCompiler);
			funcLibHelper.registFunction("test", "int", new BasicFunctionFactory<4>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", new FunctionDelegate2<int, int>(test), scriptCompiler), true);

			const wchar_t* scriptCode =
				L"void getFn(function<int(int)>& f) {"
				L"	f = test;"
				L"}"
				L"int foo() {"
				L"	function<int(int)> f;"
				L"	getFn(f);"
				L"	return f(1);"
				L"}"
				;

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'square'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 2, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(FunctionPointer, ScriptUT_NoParam1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();
			
			const wchar_t* scriptCode =
				L"int test() {"				
				L"	return 0;"
				L"}"
				L"int foo() {"
				L"	f = test;"
				L"	return f();"
				L"}"
				;

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'square'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 0, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(FunctionPointer, ScriptUT_OneParam1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int test(float a) {"
				L"	return (a + 1);"
				L"}"
				L"function<int(float)> getFn() {"
				L"	return test;"
				L"}"
				L"int foo() {"
				L"	f = getFn();"
				L"	return f(1);"
				L"}"
				;

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'square'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 2, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(FunctionPointer, ScriptUT_OneParam2)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int test(ref float a) {"
				L"	return (*a + 1);"
				L"}"
				L"function<int(ref float)> getFn() {"
				L"	return test;"
				L"}"
				L"int foo() {"
				L"	f = getFn();"
				L"	float a = 1;"
				L"	ra = ref a;"
				L"	return f(ra);"
				L"}"
				;

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'square'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			int* funcRes = (int*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes == 2, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(FunctionPointer, setToNull1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"function<int(ref float)> foo() {"
				L"	function<int(ref float)> f = nullptr;"
				L"	return f;"
				L"}"
				;

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'square'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			RuntimeFunctionInfo* funcRes = (RuntimeFunctionInfo*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(nullptr, funcRes->address, L"program can run but return wrong value");
			FF_EXPECT_EQ((int)RuntimeFunctionType::Null, (int)funcRes->info.type, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(FunctionPointer, setToNull2)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"function<int(ref float)> foo() {"
				L"	function<int(ref float)> f;"
				L"	f = nullptr;"
				L"	return f;"
				L"}"
				;

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'square'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			RuntimeFunctionInfo* funcRes = (RuntimeFunctionInfo*)scriptTask.getTaskResult();

			FF_EXPECT_EQ(nullptr, funcRes->address, L"program can run but return wrong value");
			FF_EXPECT_EQ((int)RuntimeFunctionType::Null, (int)funcRes->info.type, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(FunctionPointer, CompareToNull1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int test(ref float) {"
				L"	return 0;"
				L"}"
				L"bool foo() {"
				L"	function<int(ref float)> f1;"
				L"	function<int(ref float)> f2 = test;"
				L"	f1 = nullptr;"
				L"	return f1 == f2;"
				L"}"
				;

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'square'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			bool* funcRes = (bool*)scriptTask.getTaskResult();
			char* s = (char*)funcRes;

			FF_EXPECT_FALSE(*funcRes, (L"program can run but return wrong value: " + std::to_wstring(*s)).c_str());
		}

		FF_TEST_FUNCTION(FunctionPointer, CompareToNull2)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"bool foo() {"
				L"	function<int(ref float)> f1;"
				L"	function<int(ref float)> f2;"
				L"	f1 = nullptr;"
				L"	f2 = nullptr;"
				L"	return f1 == f2;"
				L"}"
				;

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'square'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			bool* funcRes = (bool*)scriptTask.getTaskResult();

			FF_EXPECT_TRUE(*funcRes, L"program can run but return wrong value");
		}

		FF_TEST_FUNCTION(FunctionPointer, CompareToFunc1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int test(ref float) {"
				L"	return 0;"
				L"}"
				L"bool foo() {"
				L"	function<int(ref float)> f1 = test;"
				L"	function<int(ref float)> f2 = test;"
				L"	return f1 == f2;"
				L"}"
				;

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'square'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			bool* funcRes = (bool*)scriptTask.getTaskResult();
			char* s = (char*)funcRes;

			FF_EXPECT_TRUE(*funcRes, (L"program can run but return wrong value: " + std::to_wstring(*s)).c_str());
		}

		FF_TEST_FUNCTION(FunctionPointer, CompareToFunc2)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"int test1(ref float) {"
				L"	return 0;"
				L"}"
				L"int test2(ref float) {"
				L"	return 0;"
				L"}"
				L"bool foo() {"
				L"	function<int(ref float)> f1 = test1;"
				L"	function<int(ref float)> f2 = test2;"
				L"	return f1 == f2;"
				L"}"
				;

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			int functionId = scriptCompiler->findFunction("foo", "");
			FF_EXPECT_TRUE(functionId >= 0, L"cannot find function 'square'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
			bool* funcRes = (bool*)scriptTask.getTaskResult();
			char* s = (char*)funcRes;

			FF_EXPECT_FALSE(*funcRes, (L"program can run but return wrong value: " + std::to_wstring(*s)).c_str());
		}
	};
}