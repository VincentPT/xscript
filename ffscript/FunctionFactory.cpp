#include "stdafx.h"
#include "expressionunit.h"
#include "FunctionFactory.h"
#include "ScriptCompiler.h"

namespace ffscript {
	FunctionFactory::FunctionFactory(const char* functionName, ScriptCompiler* scriptCompiler) :
		_paramSize(0), _scriptCompiler(scriptCompiler)
	{
		if (functionName) {
			_functionName = functionName;
		}
	}
	FunctionFactory::FunctionFactory() : FunctionFactory(nullptr, nullptr) {}
	FunctionFactory::~FunctionFactory() {
		//ScriptCompiler::getInstance()->unregisterFunction(_functionId);
	}

	Function* FunctionFactory::build(const std::string& name) {
		Function* function = this->createFunction(name, _functionId);
		function->setId(_functionId);
		function->setReturnType(_returnType);
		function->setParamsDataSize(this->getParamsDataSize());
		return function;
	}

	void FunctionFactory::setFunctionId(int functionId) {
		_functionId = functionId;
	}

	void FunctionFactory::addParamType(const ScriptType& paramType) {
		_paramSize += getCompiler()->getTypeSizeInStack(paramType.iType());
		_registeredParamTypes.push_back(paramType);
	}

	ScriptType& FunctionFactory::getParamType(int i) {
		return _registeredParamTypes.at(i);
	}

	int FunctionFactory::getParamCount() const {
		return (int)_registeredParamTypes.size();
	}

	const char* FunctionFactory::getName() const {
		return _functionName.c_str();
	}

	void FunctionFactory::setName(const char* functionName) {
		_functionName = functionName;
	}

	int FunctionFactory::getFunctionId() const {
		return _functionId;
	}

	void FunctionFactory::setReturnType(const ScriptType& returnType) {
		_returnType = returnType;
	}

	const ScriptType& FunctionFactory::getReturnType() const {
		return _returnType;
	}
	
	int FunctionFactory::getParamsDataSize() const {
		return _paramSize;
	}

	ScriptCompiler* FunctionFactory::getCompiler() const {
		return _scriptCompiler;
	}


	void FunctionFactory::setCompiler(ScriptCompiler* pCompiler) {
		_scriptCompiler = pCompiler;
	}
}