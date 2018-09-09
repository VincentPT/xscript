/******************************************************************
* File:        UserLibraryUT.cpp
* Description: Test cases focus on checking usage of some buit-in
*              custom library of C Lambda scripting language.
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
#include <MathLib.h>
#include <GeometryLib.h>

#include "Utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace ffscript;


namespace ffscriptUT
{	
	TEST_CLASS(UserLibraryUT)
	{
	public:
		TEST_METHOD(ComplexStringExpression)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(8);
			GlobalScopeRef rootScope = compiler.getGlobalScope();
			auto scriptCompiler = rootScope->getCompiler();

			const wchar_t* scriptCode =
				L"void test() {"
				L"	Point p = {0,0};"
				L"	Point u = {2,3};"
				L"	Point A = {4, -5};"
				L"  float val = 1.0f;"
				L"	String s = L\"value of A\" + String(A) + L\" base on the line = \" + val;"
				L"}"
				;


			includeRawStringToCompiler(scriptCompiler);
			includeMathToCompiler(scriptCompiler);
			includeGeoLibToCompiler(scriptCompiler);

			scriptCompiler->beginUserLib();
			Program* program = compiler.compileProgram(scriptCode, scriptCode + wcslen(scriptCode));

			Assert::IsNotNull(program, convertToWstring(scriptCompiler->getLastError()).c_str());

			int functionId = scriptCompiler->findFunction("test", "");
			Assert::IsTrue(functionId >= 0, L"cannot find function 'test'");

			ScriptTask scriptTask(program);
			scriptTask.runFunction(functionId, nullptr);
		}
	};
}