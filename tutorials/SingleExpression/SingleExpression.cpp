/******************************************************************
* File:        SingleExpression.cpp
* Description: Contains code for compiling, running single expression
*              of C Lambda scripting language.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
// script compiler suite header
#include <CompilerSuite.h>
// buit-in math lib
#include <MathLib.h>

// c++ standard headers
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

using namespace std::chrono;
using namespace std;
using namespace ffscript;

int main(int argc, char* argv[])
{
	CompilerSuite compiler;

	// initialize compiler, import system library and set the stack size of global context is 128
	// Since we only run a single expression, 128 bytes is enough
	compiler.initialize(128);
	auto globalScope = compiler.getGlobalScope();
	auto scriptCompiler = compiler.getCompiler().get();
	// import math functions
	includeMathToCompiler(scriptCompiler);
	// get buit-in float type
	auto floatTypeId = scriptCompiler->getTypeManager()->getBasicTypes().TYPE_FLOAT;
	ScriptType floatType(floatTypeId, scriptCompiler->getType(floatTypeId));

	// register variable 'x' then use it in expression 1
	auto pVariableX = globalScope->registVariable("x");
	if (pVariableX == nullptr) {
		cout << "variable x is already registered in somewhere" << endl;
		return -1;
	}
	// set x as a float
	pVariableX->setDataType(floatType);
	// compile expression 1
	auto executor1 = compiler.compileExpression(L"x*x*x + 3*x*x - 4");
	if (executor1 == nullptr) {
		cout << "compile expression failed: " << scriptCompiler->getLastError() << endl;
		return -1;
	}

	// register variable 'y' then use it in expression 2
	auto pVariableY = globalScope->registVariable("y");
	if (pVariableY == nullptr) {
		cout << "variable x is already registered in somewhere" << endl;
		return -1;
	}
	// set y as a float
	pVariableY->setDataType(floatType);
	// compile expression 2
	auto executor2 = compiler.compileExpression(L"x * sin(x) + y * sqrt(y)");
	if (executor2 == nullptr) {
		cout << "compile expression failed: " << scriptCompiler->getLastError() << endl;
		return -1;
	}

	// run expression 1
	// get reference of 'x' after compile in global scope
	float& x = *(float*)globalScope->getGlobalAddress(pVariableX->getOffset());
	// set 'x' to zero
	x = 0;
	constexpr int n = 10;
	cout << "set of y(x)    = {";
	for (int i = 0; i < n - 1; i++) {
		// run the compiled expression
		executor1->runCode();
		// get return data of the expression
		auto pY = (float*)executor1->getReturnData();
		// print the result
		cout << *pY << ", ";

		// change 'x' to compute expression at the next turn
		x += 0.2f;
	}
	executor1->runCode();
	auto pY = (float*)executor1->getReturnData();
	cout << *pY << "}" << endl;

	// run expression 2
	// get reference of 'y' after compile in global scope
	float& y = *(float*)globalScope->getGlobalAddress(pVariableY->getOffset());
	x = 0;
	y = 0;
	cout << "set of z(x, y) = {";
	for (int i = 0; i < n - 1; i++) {
		// run the compiled expression
		executor2->runCode();
		// get return data of the expression
		auto pZ = (float*)executor2->getReturnData();
		// print the result
		cout << *pZ << ", ";

		// change 'x' and 'y' to compute expression at the next turn
		x += 0.2f;
		y += 0.2f;
	}
	executor2->runCode();
	auto pZ = (float*)executor2->getReturnData();
	cout << *pZ << "}" << endl;

    return 0;
}

