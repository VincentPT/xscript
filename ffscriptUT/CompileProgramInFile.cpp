/******************************************************************
* File:        CompileDynamicFunctionUT.cpp
* Description: Test cases for compiling, running C Lambda programs
*              that read from files.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include <gtest/gtest.h>

#include "ExpresionParser.h"
#include <functional>
#include "TemplateForTest.hpp"
#include "Variable.h"
#include "FunctionRegisterHelper.h"
#include "BasicFunction.h"
#include "FunctionFactory.h"
#include "function\MemberFunction.hpp"
#include "Context.h"
#include <thread>
#include <future>
#include <Program.h>
#include <ScriptTask.h>
#include <sstream>
#include <fstream>
#include "testfunctions.hpp"

using namespace std;
using namespace ffscript;

#include "ExpresionParser.h"
#include "ScriptCompiler.h"
#include "expressionunit.h"
#include "Expression.h"
#include "GlobalScope.h"
#include "Executor.h"
#include "Utils.h"
#include "BasicFunctionFactory.hpp"
#include <windows.h>

namespace ffscriptUT
{
	class CompileProgramInFile : public ::testing::Test {
	protected:
		ScriptCompiler scriptCompiler;
		FunctionRegisterHelper funcLibHelper;
		const BasicTypes* basicType;
		wstring fileContent;

		CompileProgramInFile() : funcLibHelper(&scriptCompiler) {
			auto typeManager = scriptCompiler.getTypeManager();
			basicType = &(typeManager->getBasicTypes());
			typeManager->registerBasicTypes(&scriptCompiler);
			typeManager->registerBasicTypeCastFunctions(&scriptCompiler, funcLibHelper);
			importBasicfunction(funcLibHelper);
			fileContent = readFile(L"testfunctions.hpp");
		}

		static wstring readFile(const wstring& fileName) {
			std::wstringstream wsStream;
			std::wifstream iFileStream;
			wchar_t buffer[512];;
			iFileStream.open(fileName);
			if (iFileStream.is_open()) {
				while (!iFileStream.eof()) {
					iFileStream.getline(buffer, sizeof(buffer) / sizeof(buffer[0]));
					wsStream << buffer;
				}
				iFileStream.close();
			}
			return wsStream.str();
		}
	};

	namespace CompileProgramInFileUT
	{
		TEST_F(CompileProgramInFile, CompileFile1)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("fx");
			EXPECT_TRUE(overLoadingFuncItems->size() > 0) << L"cannot find function 'fx'";

			double x = 5.0;
			ScriptParamBuffer paramBuffer(x);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			double* funcRes = (double*)scriptTask.getTaskResult();

			EXPECT_TRUE(*funcRes == fx(x)) << L"program can run but return wrong value";

			PRINT_TEST_MESSAGE(("fx =" + std::to_string(*funcRes)).c_str());
		}

		TEST_F(CompileProgramInFile, CompileFile2)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);


			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("fx1");
			EXPECT_TRUE(overLoadingFuncItems->size() > 0) << L"cannot find function 'fx1'";

			double x = 5.0;
			ScriptParamBuffer paramBuffer(x);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			double* funcRes = (double*)scriptTask.getTaskResult();

			EXPECT_TRUE(*funcRes == fx1(x)) << L"program can run but return wrong value";

			PRINT_TEST_MESSAGE(("fx1 =" + std::to_string(*funcRes)).c_str());
		}

		TEST_F(CompileProgramInFile, CompileFile3)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);


			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("fa");
			EXPECT_TRUE(overLoadingFuncItems->size() > 0) << L"cannot find function 'fa'";

			double x = 5.0;
			int n = 10;
			ScriptParamBuffer paramBuffer(x);
			paramBuffer.addParam(n);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			double* funcRes = (double*)scriptTask.getTaskResult();

			EXPECT_TRUE(*funcRes == fa(x, n)) << L"program can run but return wrong value";

			PRINT_TEST_MESSAGE(("fa =" + std::to_string(*funcRes)).c_str());
		}

		TEST_F(CompileProgramInFile, CompileFile4)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);


			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("fa");
			EXPECT_TRUE(overLoadingFuncItems->size() > 0) << L"cannot find function 'fa'";

			double x = 5.0;
			int n = -10;

			ScriptParamBuffer paramBuffer(x);
			paramBuffer.addParam(n);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			double* funcRes = (double*)scriptTask.getTaskResult();

			EXPECT_TRUE(*funcRes == fa(x, n)) << L"program can run but return wrong value";

			PRINT_TEST_MESSAGE(("fa =" + std::to_string(*funcRes)).c_str());
		}

		TEST_F(CompileProgramInFile, CompileFile5)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);


			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			const list<OverLoadingItem>* overLoadingFuncItems = scriptCompiler.findOverloadingFuncRoot("fa");
			EXPECT_TRUE(overLoadingFuncItems->size() > 0) << L"cannot find function 'fa'";

			double x = 5.0;
			int n = 0;
			ScriptParamBuffer paramBuffer(x);
			paramBuffer.addParam(n);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(overLoadingFuncItems->front().functionId, &paramBuffer);
			double* funcRes = (double*)scriptTask.getTaskResult();

			EXPECT_TRUE(*funcRes == fa(x, n)) << L"program can run but return wrong value";

			PRINT_TEST_MESSAGE(("fa =" + std::to_string(*funcRes)).c_str());
		}

		TEST_F(CompileProgramInFile, CompileFile6)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);


			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx2", "int");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx2'";

			int n = 2;
			ScriptParamBuffer paramBuffer(n);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			float* funcRes = (float*)scriptTask.getTaskResult();

			EXPECT_TRUE(*funcRes == fx2(n)) << L"program can run but return wrong value";

			PRINT_TEST_MESSAGE(("fx2 =" + std::to_string(*funcRes)).c_str());
		}

		TEST_F(CompileProgramInFile, CompileFile7)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);


			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx3", "int");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx3'";

			int n = 3;
			ScriptParamBuffer paramBuffer(n);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			__int64* funcRes = (__int64*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("fx3 =" + std::to_string(*funcRes)).c_str());

			EXPECT_TRUE(*funcRes == fx3(n)) << L"program can run but return wrong value";
		}

		TEST_F(CompileProgramInFile, CompileFile8)
		{
			byte globalData[1024];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);


			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx4", "int,bool");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx4'";

			int a = 100;
			bool b = true;
			ScriptParamBuffer paramBuffer(a);
			paramBuffer.addParam(b);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();

			EXPECT_TRUE(*funcRes == fx4(a, b)) << L"program can run but return wrong value";

			PRINT_TEST_MESSAGE(("fx4 =" + std::to_string(*funcRes)).c_str());
		}

		TEST_F(CompileProgramInFile, CompileFile9)
		{
			byte globalData[1];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);


			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx4", "int,bool");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx4'";

			int a = 100;
			bool b = false;
			ScriptParamBuffer paramBuffer(a);
			paramBuffer.addParam(b);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();

			EXPECT_TRUE(*funcRes == fx4(a, b)) << L"program can run but return wrong value";

			PRINT_TEST_MESSAGE(("fx4 =" + std::to_string(*funcRes)).c_str());
		}

		TEST_F(CompileProgramInFile, CompileFile10)
		{
			byte globalData[1];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx4","int,bool");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx4'";

			int a = 0;
			bool b = true;
			ScriptParamBuffer paramBuffer(a);
			paramBuffer.addParam(b);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();

			EXPECT_TRUE(*funcRes == fx4(a, b)) << L"program can run but return wrong value";

			PRINT_TEST_MESSAGE(("fx4 =" + std::to_string(*funcRes)).c_str());
		}

		TEST_F(CompileProgramInFile, CompileFile11)
		{
			byte globalData[1];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx5", "int");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx5'";

			int	a = 0;
			ScriptParamBuffer paramBuffer(a);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();

			EXPECT_TRUE(*funcRes == fx5(a)) << L"program can run but return wrong value";

			PRINT_TEST_MESSAGE(("fx5 =" + std::to_string(*funcRes)).c_str());
		}

		TEST_F(CompileProgramInFile, CompileFile12)
		{
			byte globalData[1];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx5", "int");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx5'";

			int	a = 1213;
			ScriptParamBuffer paramBuffer(a);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();

			EXPECT_TRUE(*funcRes == fx5(a)) << L"program can run but return wrong value";

			PRINT_TEST_MESSAGE(("fx5 =" + std::to_string(*funcRes)).c_str());
		}

		TEST_F(CompileProgramInFile, CompileFile13)
		{
			byte globalData[1];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx6", "bool,float,double");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx6'";

			bool a = 0;
			float b = 1.0f;
			double c = 2.0;
			ScriptParamBuffer paramBuffer(a);
			paramBuffer.addParam(b);
			paramBuffer.addParam(c);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();

			EXPECT_TRUE(*funcRes == fx6(a, b, c)) << L"program can run but return wrong value";

			PRINT_TEST_MESSAGE(("fx6 =" + std::to_string(*funcRes)).c_str());
		}

		TEST_F(CompileProgramInFile, CompileFile14)
		{
			byte globalData[1];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx6", "bool,float,double");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx6'";

			bool a = 1;
			float b = 0.0f;
			double c = 2.0;
			ScriptParamBuffer paramBuffer(a);
			paramBuffer.addParam(b);
			paramBuffer.addParam(c);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();

			EXPECT_TRUE(*funcRes == fx6(a, b, c)) << L"program can run but return wrong value";

			PRINT_TEST_MESSAGE(("fx6 =" + std::to_string(*funcRes)).c_str());
		}

		TEST_F(CompileProgramInFile, CompileFile15)
		{
			byte globalData[1];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx6", "bool,float,double");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx6'";

			bool a = true;
			float b = 2.0f;
			double c = 0.0;
			ScriptParamBuffer paramBuffer;
			paramBuffer.addParam(a);
			paramBuffer.addParam(b);
			paramBuffer.addParam(c);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();

			PRINT_TEST_MESSAGE(("fx6 =" + std::to_string(*funcRes)).c_str());
			EXPECT_TRUE(*funcRes == fx6(a, b, c)) << L"program can run but return wrong value";
		}

		TEST_F(CompileProgramInFile, CompileFile16)
		{
			byte globalData[1];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx6", "bool,float,double");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx6'";

			bool a = true;
			float b = 2.0f;
			double c = 3.0;
			ScriptParamBuffer paramBuffer;
			paramBuffer.addParam(a);
			paramBuffer.addParam(b);
			paramBuffer.addParam(c);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("fx6 =" + std::to_string(*funcRes)).c_str());
			EXPECT_TRUE(*funcRes == fx6(a, b, c)) << L"program can run but return wrong value";
		}

		TEST_F(CompileProgramInFile, CompileFile17)
		{
			byte globalData[1];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx6", "bool,float,double");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx6'";

			bool a = false;
			float b = 0.0f;
			double c = 0.0;
			ScriptParamBuffer paramBuffer;
			paramBuffer.addParam(a);
			paramBuffer.addParam(b);
			paramBuffer.addParam(c);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("fx6 =" + std::to_string(*funcRes)).c_str());

			EXPECT_TRUE(*funcRes == fx6(a, b, c)) << L"program can run but return wrong value";
		}

		TEST_F(CompileProgramInFile, CompileFile18)
		{
			byte globalData[1];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx7", "bool,float,double");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx7'";

			bool a = 0;
			float b = 0.0f;
			double c = 10.0;
			ScriptParamBuffer paramBuffer;
			paramBuffer.addParam(a);
			paramBuffer.addParam(b);
			paramBuffer.addParam(c);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("fx7 =" + std::to_string(*funcRes)).c_str());

			EXPECT_TRUE(*funcRes == fx7(a, b, c)) << L"program can run but return wrong value";
		}

		TEST_F(CompileProgramInFile, CompileFile19)
		{
			byte globalData[1];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx7", "bool,float,double");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx7'";

			bool a = 0;
			float b = 0.0f;
			double c = 50.0;
			ScriptParamBuffer paramBuffer;
			paramBuffer.addParam(a);
			paramBuffer.addParam(b);
			paramBuffer.addParam(c);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("fx7 =" + std::to_string(*funcRes)).c_str());

			EXPECT_TRUE(*funcRes == fx7(a, b, c)) << L"program can run but return wrong value";
		}

		TEST_F(CompileProgramInFile, CompileFile20)
		{
			byte globalData[1];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx8", "bool,float,double");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx8'";

			bool a = 0;
			float b = 10.0f;
			double c = 50.0;
			ScriptParamBuffer paramBuffer;
			paramBuffer.addParam(a);
			paramBuffer.addParam(b);
			paramBuffer.addParam(c);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("fx8 =" + std::to_string(*funcRes)).c_str());

			EXPECT_TRUE(*funcRes == fx8(a, b, c)) << L"program can run but return wrong value";
		}

		TEST_F(CompileProgramInFile, CompileFile21)
		{
			byte globalData[1];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx8", "bool,float,double");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx8'";

			bool a = 0;
			float b = 10.0f;
			double c = 5.0;
			ScriptParamBuffer paramBuffer;
			paramBuffer.addParam(a);
			paramBuffer.addParam(b);
			paramBuffer.addParam(c);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("fx8 =" + std::to_string(*funcRes)).c_str());

			EXPECT_TRUE(*funcRes == fx8(a, b, c)) << L"program can run but return wrong value";
		}

		TEST_F(CompileProgramInFile, CompileFile22)
		{
			byte globalData[1];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx8", "bool,float,double");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx8'";

			bool a = 1;
			float b = 10.0f;
			double c = 5.0;
			ScriptParamBuffer paramBuffer;
			paramBuffer.addParam(a);
			paramBuffer.addParam(b);
			paramBuffer.addParam(c);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("fx8 =" + std::to_string(*funcRes)).c_str());

			EXPECT_TRUE(*funcRes == fx8(a, b, c)) << L"program can run but return wrong value";
		}

		TEST_F(CompileProgramInFile, CompileFile23)
		{
			byte globalData[1];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx8", "bool,float,double");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx8'";

			bool a = 1;
#ifdef NDEBUG
			float b = 40.0f;
#else
			float b = 10.0f;
#endif // NDEBUG
			double c = 5.0;
			ScriptParamBuffer paramBuffer;
			paramBuffer.addParam(a);
			paramBuffer.addParam(b);
			paramBuffer.addParam(c);
			 
			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("fx8 =" + std::to_string(*funcRes)).c_str());

			EXPECT_TRUE(*funcRes == fx8(a, b, c)) << L"program can run but return wrong value";
		}

		TEST_F(CompileProgramInFile, CompileFile24)
		{
			byte globalData[1];
			StaticContext staticContext(globalData, sizeof(globalData));
			GlobalScope rootScope(&staticContext,&scriptCompiler);

			//initialize an instance of script program
			Program theProgram;
			scriptCompiler.bindProgram(&theProgram);

			const wchar_t* scriptCode = fileContent.c_str();

			const wchar_t* res = rootScope.parse(scriptCode, scriptCode + wcslen(scriptCode));
			EXPECT_TRUE(res != nullptr) << L"compile program failed";

			bool blRes = rootScope.extractCode(&theProgram);
			EXPECT_TRUE(blRes) << L"extract code failed";

			int functionId = scriptCompiler.findFunction("fx8", "bool,float,double");
			EXPECT_TRUE(functionId >= 0) << L"cannot find function 'fx8'";

			bool a = 0;
			float b = 30.0f;
			double c = 5.0;
			ScriptParamBuffer paramBuffer;
			paramBuffer.addParam(a);
			paramBuffer.addParam(b);
			paramBuffer.addParam(c);

			ScriptTask scriptTask(&theProgram);
			scriptTask.runFunction(functionId, &paramBuffer);
			bool* funcRes = (bool*)scriptTask.getTaskResult();
			PRINT_TEST_MESSAGE(("fx8 =" + std::to_string(*funcRes)).c_str());

			EXPECT_TRUE(*funcRes == fx8(a, b, c)) << L"program can run but return wrong value";
		}
	};
}