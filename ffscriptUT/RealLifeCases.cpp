/******************************************************************
* File:        RealLifeCases.cpp
* Description: Test cases collected from real life.
* Author:      Vincent Pham
*
* Copyright (c) 2019 VincentPT.
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
#include <CLamdaProg.h>

#include "Utils.h"

using namespace std;
using namespace ffscript;

//RealLifeCases
TEST(RealLifeCases, IntegerReverse1)
{
	CompilerSuite compiler;
	compiler.initialize(1024);
	GlobalScopeRef rootScope = compiler.getGlobalScope();
	auto scriptCompiler = rootScope->getCompiler();

	const wchar_t* scriptCode =
        L"int reverse(int value) {"
            L"int b1 = value & 0xFF;"
            L"int b2 = (value & 0xFF00) >> 8;"
            L"int b3 = (value & 0xFF0000) >> 16;"
            L"int b4 = (value & 0xFF000000) >> 24;"
            L"return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;"
        L"}"
		;

	auto rawProgram = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
	int functionId = scriptCompiler->findFunction("reverse", "int");
	EXPECT_TRUE(functionId >= 0) << L"cannot find function 'reverse'";
    
    auto program = std::unique_ptr<CLamdaProg>(rootScope->detachScriptProgram(rawProgram));

    program->runGlobalCode();
    
	int n = 0x11223344;
	ScriptParamBuffer paramBuffer(n);
    ScriptRunner scriptRunner(program->getProgram(), functionId);
    scriptRunner.runFunction(&paramBuffer);
    auto funcRes = (int*)scriptRunner.getTaskResult();
    
    program->cleanupGlobalMemory();

    EXPECT_EQ(*funcRes,0x44332211);
}

//RealLifeCases
TEST(RealLifeCases, IntegerReverse2)
{
    CompilerSuite compiler;
    compiler.initialize(1024);
    GlobalScopeRef rootScope = compiler.getGlobalScope();
    auto scriptCompiler = rootScope->getCompiler();
    
    const wchar_t* scriptCode =
    L"int reverse(int value) {"
    L"int b1 = value & 0xFF;"
    L"int b2 = (value & 0xFF00) >> 8;"
    L"int b3 = (value & 0xFF0000) >> 16;"
    L"int b4 = (value & 0xFF000000) >> 24;"
    L"return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;"
    L"}"
    L"int main(){"
    L"int val = 0x11223344;"
    L"return reverse(val);"
    L"}"
    ;
    
    Program* rawProgram = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
    int functionId = scriptCompiler->findFunction("main", "");
    EXPECT_TRUE(functionId >= 0) << L"cannot find function 'main'";
    auto program = std::unique_ptr<CLamdaProg>(rootScope->detachScriptProgram(rawProgram));
    program->runGlobalCode();
    
    ScriptRunner scriptRunner(program->getProgram(), functionId);
    scriptRunner.runFunction(nullptr);
    auto funcRes = (int*)scriptRunner.getTaskResult();
    
    program->cleanupGlobalMemory();
    
    EXPECT_EQ(*funcRes,0x44332211);
}

TEST(RealLifeCases, IntegerReverse3)
{
    CompilerSuite compiler;
    compiler.initialize(1024);
    GlobalScopeRef rootScope = compiler.getGlobalScope();
    auto scriptCompiler = rootScope->getCompiler();
    
    const wchar_t* scriptCode =
    L"int b1;"
    L"int b2;"
    L"int b3;"
    L"int b4;"
    L"int reverse(int value) {"
    L"b1 = value & 0xFF;"
    L"b2 = (value & 0xFF00) >> 8;"
    L"b3 = (value & 0xFF0000) >> 16;"
    L"b4 = (value & 0xFF000000) >> 24;"
    L"return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;"
    L"}"
    ;
    
    Program* rawProgram = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));
    int functionId = scriptCompiler->findFunction("reverse", "int");
    EXPECT_TRUE(functionId >= 0) << L"cannot find function 'reverse'";
    
    auto program = std::unique_ptr<CLamdaProg>(rootScope->detachScriptProgram(rawProgram));
    program->runGlobalCode();
    
    auto& b1 = getGlobalVariable<int>(program.get(), "b1");
    auto& b2 = getGlobalVariable<int>(program.get(), "b2");
    auto& b3 = getGlobalVariable<int>(program.get(), "b3");
    auto& b4 = getGlobalVariable<int>(program.get(), "b4");
    
    int n = 0x11223344;
    ScriptParamBuffer paramBuffer(n);
    
    ScriptRunner scriptRunner(program->getProgram(), functionId);
    scriptRunner.runFunction(&paramBuffer);
    auto funcRes = (int*)scriptRunner.getTaskResult();
    
    program->cleanupGlobalMemory();
    
    EXPECT_EQ(b1, (n & 0xFF));
    EXPECT_EQ(b2,(n & 0xFF00) >> 8);
    EXPECT_EQ(b3,(n & 0xFF0000) >> 16);
    EXPECT_EQ(b4,(n & 0xFF000000) >> 24);
    EXPECT_EQ(*funcRes,0x44332211);
}