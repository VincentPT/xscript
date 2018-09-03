/******************************************************************
* File:        ConditionalOperator.h
* Description: declare ConditionalOperatorFactory class. A factory
*              class that create expression unit builder for
*              conditional operator
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
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