/******************************************************************
* File:        InternalCompilerSuite.cpp
* Description: implement InternalCompilerSuite class. A compiler class
*              but used to compile constant expression only.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "InternalCompilerSuite.h"
#include "BasicFunction.h"
#include "ConditionalOperator.h"
#include "BasicOperators.hpp"
#include "FunctionRegisterHelper.h"

using namespace ffscript::basic::operators;
using namespace ffscript::basic;

namespace ffscript {
	InternalCompilerSuite::InternalCompilerSuite()
	{
	}

	InternalCompilerSuite::~InternalCompilerSuite()
	{
	}

	void InternalCompilerSuite::initialize(int globalMemSize) {
		_pCompiler = (ScriptCompilerRef)(new ScriptCompiler());
		_globalScopeRef = (GlobalScopeRef)(new GlobalScope(globalMemSize, _pCompiler.get()));

		FunctionRegisterHelper fb(_pCompiler.get());

		//register integer type only
		auto TYPE_INT = _pCompiler->registType("int");
		auto TYPE_BOOL = _pCompiler->registType("bool");

		ScriptType typeInt(TYPE_INT, "int");
		ScriptType typeBool(TYPE_BOOL, "bool");

		_pCompiler->setTypeSize(TYPE_INT, 4);
		_pCompiler->setTypeSize(TYPE_BOOL, 1);

		auto& basicType = _pCompiler->getTypeManager()->getBasicTypes();
		basicType.TYPE_INT = TYPE_INT;
		basicType.TYPE_BOOL = TYPE_BOOL;

		_pCompiler->registerTypeConversionAccurative(TYPE_INT, TYPE_BOOL, 800);
		_pCompiler->registerTypeConversionAccurative(TYPE_BOOL, TYPE_INT, 1000);
		auto scriptCompiler = _pCompiler.get();
		
		fb.registFunction("int", "bool", new ConversionFactoryBoolTo<int>(scriptCompiler, typeInt), true);
		fb.registFunction("bool", "int", new ConversionFactoryToBool<int>(scriptCompiler, typeBool), true);

#pragma region integer operators only
		fb.registPredefinedOperators("+", "int,int", "int", createFunctionCdecl<int, int, int>(add));
		fb.registPredefinedOperators("-", "int,int", "int", createFunctionCdecl<int, int, int>(sub));
		fb.registPredefinedOperators("*", "int,int", "int", createFunctionCdecl<int, int, int>(mul));
		fb.registPredefinedOperators("/", "int,int", "int", createFunctionCdecl<int, int, int>(div));
		fb.registPredefinedOperators("%", "int,int", "int", createFunctionCdecl<int, int, int>(mod));
		//bitwises
		fb.registPredefinedOperators("&", "int,int", "int", createFunctionCdecl<int, int, int>(and));
		fb.registPredefinedOperators("|", "int,int", "int", createFunctionCdecl<int, int, int>(or ));
		fb.registPredefinedOperators("^", "int,int", "int", createFunctionCdecl<int, int, int>(xor));
		fb.registPredefinedOperators("<<", "int,int", "int", createFunctionCdecl<int, int, int>(shiftLeft));
		fb.registPredefinedOperators(">>", "int,int", "int", createFunctionCdecl<int, int, int>(shiftRight));
		//pre-post fix operators
		fb.registPredefinedOperators("neg", "int", "int", createFunctionCdecl<int, int>(neg));
		fb.registPredefinedOperators("~", "int", "int", createFunctionCdecl<int, int>(not));
		//comparision operators
		fb.registPredefinedOperators("<", "int,int", "bool", createFunctionCdecl<bool, int, int>(operators::less));
		fb.registPredefinedOperators("<=", "int,int", "bool", createFunctionCdecl<bool, int, int>(operators::less_or_equal));
		fb.registPredefinedOperators(">", "int,int", "bool", createFunctionCdecl<bool, int, int>(operators::great));
		fb.registPredefinedOperators(">=", "int,int", "bool", createFunctionCdecl<bool, int, int>(operators::great_or_equal));
		fb.registPredefinedOperators("==", "int,int", "bool", createFunctionCdecl<bool, int, int>(operators::equal));
		fb.registPredefinedOperators("!=", "int,int", "bool", createFunctionCdecl<bool, int, int>(operators::not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "int,int", "bool", createFunctionCdecl<bool, int, int>(logic_and));
		fb.registPredefinedOperators("||", "int,int", "bool", createFunctionCdecl<bool, int, int>(logic_or));
		fb.registPredefinedOperators("!", "int", "bool", createFunctionCdecl<bool, int>(logic_not));		
#pragma endregion

#pragma region bool operators only
		//logic operators
		fb.registPredefinedOperators("&&", "bool,bool", "bool", createFunctionCdecl<bool, bool, bool>(logic_and));
		fb.registPredefinedOperators("||", "bool,bool", "bool", createFunctionCdecl<bool, bool, bool>(logic_or));
		fb.registPredefinedOperators("!", "bool", "bool", createFunctionCdecl<bool, bool>(logic_not));
#pragma endregion

#pragma region combine integer and bool
		//logic operators - integer and bool
		fb.registPredefinedOperators("&&", "int,bool", "bool", createFunctionCdecl<bool, int, bool>(logic_and));
		fb.registPredefinedOperators("||", "int,bool", "bool", createFunctionCdecl<bool, int, bool>(logic_or));
		//logic operators - bool and integer
		fb.registPredefinedOperators("&&", "bool,int", "bool", createFunctionCdecl<bool, bool, int>(logic_and));
		fb.registPredefinedOperators("||", "bool,int", "bool", createFunctionCdecl<bool, bool, int>(logic_or));
#pragma endregion

		//register choice operator of conditional operator( condition ? ifClause : elseClause)
		fb.registPredefinedOperators(":", "", "", nullptr);

		//register conditional opearator with param bool type as condition, void type for last two params
		//the void type for param usually don't work, these is only temporary and it will correct later
		//for this very special oprator
		auto conditionOperator = new ConditionalOperatorFactory(_pCompiler.get());
		_pCompiler->registConditionalFunction(conditionOperator);
		fb.addFactory(conditionOperator);

		_pCompiler->beginUserLib();
	}
}