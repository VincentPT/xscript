/******************************************************************
* File:        ConditionalOperator.cpp
* Description: implement ConditionalOperatorFactory class. A factory
*              class that create expression unit builder for
*              conditional operator
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "ConditionalOperator.h"
#include "ScriptCompiler.h"

namespace ffscript {
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	ConditionalOperatorFactory::ConditionalOperatorFactory(ScriptCompiler* scriptCompiler) : FunctionFactory(POINTER_SIGN, scriptCompiler) {}
	ConditionalOperatorFactory::~ConditionalOperatorFactory() {}

	Function* ConditionalOperatorFactory::createFunction(const std::string& name, int id) {
		return new DynamicParamFunction(name, EXP_UNIT_ID_FUNC_CONDITIONAL, FUNCTION_PRIORITY_CONDITIONAL, 3);
	}

	void ConditionalOperatorFactory::addParamType(const ScriptType& paramType) {
		FunctionFactory::addParamType(paramType);
		this->setReturnType( paramType );
	}
}