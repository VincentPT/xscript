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