/******************************************************************
* File:        ConditionalOperator.h
* Description: declare ConditionalOperatorFactory class. A factory
*              class that create expression unit builder for
*              conditional operator
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "expressionunit.h"
#include "FunctionFactory.h"
#include "ScriptType.h"

namespace ffscript {
	class ScriptCompiler;

	class ConditionalOperatorFactory : public FunctionFactory {
	public:
		ConditionalOperatorFactory(ScriptCompiler* scriptCompiler);
		virtual ~ConditionalOperatorFactory();
		virtual Function* createFunction(const std::string& name, int id);
		virtual void addParamType(const ScriptType& paramType);
	};
}